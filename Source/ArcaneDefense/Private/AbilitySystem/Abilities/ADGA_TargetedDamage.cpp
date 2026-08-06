#include "AbilitySystem/Abilities/ADGA_TargetedDamage.h"

#include "AbilitySystemComponent.h"
#include "Characters/ADCharacterBase.h"
#include "Characters/ADEnemyCharacter.h"
#include "Combat/ADTargetingComponent.h"
#include "GameplayEffect.h"

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
	
	return IsValid(GetValidTarget(ActorInfo));
}

void UADGA_TargetedDamage::ActivateAbility(
	const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	AADEnemyCharacter* Target  =  GetValidTarget(ActorInfo);
	if (!IsValid(Target))
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}

	UAbilitySystemComponent* TargetAbilitySystem = Target->GetAbilitySystemComponent();
	if (!ensureMsgf(
		IsValid(TargetAbilitySystem),
		TEXT("%s does not have a valid Ability System Component."),
		*GetNameSafe(Target)))
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}

	if (!ensureMsgf(
		DamageEffectClass != nullptr,
		TEXT("%s does not have a DamageEffect class."),
		*GetNameSafe(this)))
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}

	const FGameplayEffectSpecHandle DamageSpec
		= MakeOutgoingGameplayEffectSpec(
			DamageEffectClass,
			GetAbilityLevel());
	if (!ensureMsgf(
		DamageSpec.IsValid(),
		TEXT("%s failed to create its dammage effect spec."),
		*GetNameSafe(this)))
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}

	// Commit applies the configured cost and coldown.
	if (!CommitAbility(
		Handle, ActorInfo, ActivationInfo))
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}

	TargetAbilitySystem->ApplyGameplayEffectSpecToSelf(
		*DamageSpec.Data.Get());
	const AADCharacterBase* SourceCharacter =
		ActorInfo != nullptr
			? Cast<AADCharacterBase>(ActorInfo->AvatarActor.Get())
			: nullptr;
	
	UE_LOG(
		LogTemp,
		Display,
		TEXT(
			"%s hit %s. Target Health: %.0f/%.0f. Caster Mana: %.0f/%.0f."
		),
		*GetNameSafe(SourceCharacter),
		*GetNameSafe(Target),
		Target->GetHealth(),
		Target->GetMaxHealth(),
		IsValid(SourceCharacter)
			? SourceCharacter->GetMana()
			: 0.0f,
		IsValid(SourceCharacter)
			? SourceCharacter->GetMaxMana()
			: 0.0f
	);

	EndAbility(
		Handle,
		ActorInfo,
		ActivationInfo,
		true,
		false
	);
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
	if (!IsValid(Target) || Target->IsDead())
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
