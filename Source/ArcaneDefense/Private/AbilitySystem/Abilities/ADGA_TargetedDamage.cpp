#include "AbilitySystem/Abilities/ADGA_TargetedDamage.h"

#include "Abilities/Tasks/AbilityTask_WaitDelay.h"

#include "AbilitySystemComponent.h"
#include "Characters/ADCharacterBase.h"
#include "Characters/ADEnemyCharacter.h"
#include "Combat/ADTargetingComponent.h"
#include "Combat/ADCastComponent.h"
#include "GameplayEffect.h"

#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

UADGA_TargetedDamage::UADGA_TargetedDamage()
{
	CastDisplayName = NSLOCTEXT(
		"ArcaneDefense",
		"DefaultTargetedSpellName",
		"Spell");
}

bool UADGA_TargetedDamage::CanActivateAbility(
	const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayTagContainer* SourceTags,
	const FGameplayTagContainer* TargetTags,
	FGameplayTagContainer* OptionalRelevantTags) const
{
	if  (!Super::CanActivateAbility(
		Handle,
		ActorInfo,
		SourceTags,
		TargetTags,
		OptionalRelevantTags))
	{
		return false;
	}

	const ACharacter* Character = Cast<ACharacter>(ActorInfo->AvatarActor.Get());
	if (!IsValid(Character))
	{
		return false;
	}

	const UCharacterMovementComponent* MovementComponent = Character->GetCharacterMovement();
	if (IsValid(MovementComponent) && MovementComponent->IsFalling())
	{
		return false;
	}
	
	return IsValid(GetValidTarget(ActorInfo));
}

void UADGA_TargetedDamage::ActivateAbility(
	const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	AADEnemyCharacter* Target = GetValidTarget(ActorInfo);
	if (!IsValid(Target))
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}

	AActor* AvatarActor = (ActorInfo == nullptr) ? nullptr : ActorInfo->AvatarActor.Get();
	if (!IsValid(AvatarActor))
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}

	UADCastComponent* CastComponent = AvatarActor->FindComponentByClass<UADCastComponent>();
	if (!ensureMsgf(
		IsValid(CastComponent),
		TEXT("%s requires an AD Cast Component"),
		*GetNameSafe(AvatarActor)))
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}

	CachedTarget = Target;
	CachedCastComponent = CastComponent;

	if (CastTime <= 0.f)
	{
		HandleCastFinished();
		return;
	}

	CastComponent->StartCast(CastDisplayName, CastTime);

	UAbilityTask_WaitDelay* WaitTask = UAbilityTask_WaitDelay::WaitDelay(this, CastTime);
	if (!ensureMsgf(IsValid(WaitTask),
		TEXT("%s failed to create its cast wait task."),
		*GetNameSafe(this)))
	{
		EndCurrentAbility(true);
		return;
	}

	WaitTask->OnFinish.AddDynamic(this, &UADGA_TargetedDamage::HandleCastFinished);

	WaitTask->ReadyForActivation();
	
	UE_LOG(
		LogTemp,
		Display,
		TEXT("%s started casting %s. on %s for %.2f seconds."),
		*GetNameSafe(AvatarActor),
		*CastDisplayName.ToString(),
		*GetNameSafe(Target),
		CastTime);
}

void UADGA_TargetedDamage::HandleCastFinished()
{
	if (!IsActive()) { return; }

	const FGameplayAbilityActorInfo* ActorInfo = GetCurrentActorInfo();
	if (!ActorInfo)
	{
		EndCurrentAbility(true);
		return;
	}

	AActor* AvatarActor = ActorInfo->AvatarActor.Get();
	AADEnemyCharacter* Target = CachedTarget.Get();
	if (!IsValid(AvatarActor) ||
		!IsValid(Target) ||
		Target->IsDead() ||
		!IsTargetInRange(AvatarActor, Target))
	{
		EndCurrentAbility(true);
		return;
	}

	UAbilitySystemComponent* TargetAbilitySystem = Target->GetAbilitySystemComponent();
	if (!IsValid(TargetAbilitySystem) || !DamageEffectClass)
	{
		EndCurrentAbility(true);
		return;
	}

	const FGameplayEffectSpecHandle DamageSpec = MakeOutgoingGameplayEffectSpec(
		DamageEffectClass,
		GetAbilityLevel());
	if (!DamageSpec.IsValid())
	{
		EndCurrentAbility(true);
		return;
	}

	/*
	 * The final resource check and mana cost occur after the cast.
	 * Interrupted casts therefore do not consume mana.
	 */
	if (!CommitAbility(
		GetCurrentAbilitySpecHandle(),
		ActorInfo,
		GetCurrentActivationInfo()))
	{
		EndCurrentAbility(true);
		return;
	}

	if (!ExecuteAbilityPayload(Target,ActorInfo))
	{
		UE_LOG(
			LogTemp,
			Error,
			TEXT(
				"%s failed to execute its ability payload."
			),
			*GetNameSafe(this)
		);

		EndCurrentAbility(false);
		return;
	}

	const AADCharacterBase* SourceCharacter = Cast<AADCharacterBase>(
	ActorInfo->AvatarActor.Get());

	UE_LOG(
		LogTemp,
		Display,
		TEXT("%s completed casting %s on %s. Mana: %.0f/%.0f."),
		*GetNameSafe(SourceCharacter),
		*CastDisplayName.ToString(),
		*GetNameSafe(Target),
		IsValid(SourceCharacter) ? SourceCharacter->GetMana(): 0.0f,
		IsValid(SourceCharacter) ? SourceCharacter->GetMaxMana() : 0.0f);

	EndCurrentAbility(false);
}

void UADGA_TargetedDamage::EndAbility(
	const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo,
	bool bReplicateEndAbility,
	bool bWasCancelled)
{
	if (UADCastComponent* CastComponent = CachedCastComponent.Get())
	{
		CastComponent->EndCast(bWasCancelled);
	}

	CachedTarget.Reset();
	CachedCastComponent.Reset();

	Super::EndAbility(
		Handle,
		ActorInfo,
		ActivationInfo,
		bReplicateEndAbility,
		bWasCancelled);
}


AADEnemyCharacter* UADGA_TargetedDamage::GetValidTarget(
	const FGameplayAbilityActorInfo* ActorInfo) const
{
	if (ActorInfo == nullptr)
	{
		return nullptr;
	}

	AActor* AvatarActor = ActorInfo->AvatarActor.Get();
	if (!IsValid(AvatarActor))
	{
		return nullptr;
	}

	UADTargetingComponent* TargetingComponent =
		AvatarActor->FindComponentByClass<UADTargetingComponent>();
	if (!IsValid(TargetingComponent))
	{
		return nullptr;
	}

	AADEnemyCharacter* Target = TargetingComponent->GetCurrentTarget();
	if (!IsValid(Target) ||
		Target->IsDead() ||
		!IsTargetInRange(AvatarActor, Target))
	{
		return nullptr;
	}

	return Target;
}

bool UADGA_TargetedDamage::IsTargetInRange(const AActor* SourceActor, const AActor* TargetActor) const
{
	if (!IsValid(SourceActor) || !IsValid(TargetActor))
	{
		return false;
	}

	const float DistanceSquared = FVector::DistSquared(
		SourceActor->GetActorLocation(),
		TargetActor->GetActorLocation());
	return DistanceSquared <= FMath::Square(MaxRange);
}

void UADGA_TargetedDamage::EndCurrentAbility(const bool bWasCancelled)
{
	const FGameplayAbilityActorInfo* ActorInfo = GetCurrentActorInfo();
	if (!ActorInfo)	{ return; }

	EndAbility(
		GetCurrentAbilitySpecHandle(),
		ActorInfo,
		GetCurrentActivationInfo(),
		true,
		bWasCancelled);
}

bool UADGA_TargetedDamage::ExecuteAbilityPayload(
	AADEnemyCharacter* Target,
	const FGameplayAbilityActorInfo* /*ActorInfo*/)
{
	if (!IsValid(Target) || !DamageEffectClass)	{ return false;	}

	UAbilitySystemComponent* TargetAbilitySystem = Target->GetAbilitySystemComponent();
	if (!IsValid(TargetAbilitySystem)) { return false; }

	const FGameplayEffectSpecHandle DamageSpec = MakeOutgoingGameplayEffectSpec(
		DamageEffectClass,
		GetAbilityLevel());

	if (!DamageSpec.IsValid()) { return false; }

	TargetAbilitySystem->ApplyGameplayEffectSpecToSelf(*DamageSpec.Data.Get());

	return true;
}
