#include "AbilitySystem/Abilities/ADGA_GroundTargetedArea.h"

#include "Abilities/Tasks/AbilityTask_WaitDelay.h"
#include "AbilitySystemComponent.h"
#include "Characters/ADEnemyCharacter.h"
#include "Combat/ADCastComponent.h"
#include "Combat/ADGroundTargetingComponent.h"
#include "Engine/EngineTypes.h"
#include "Kismet/KismetSystemLibrary.h"

UADGA_GroundTargetedArea::UADGA_GroundTargetedArea()
{
	CastDisplayName = NSLOCTEXT(
		"ArcaneDefense",
		"DefaultGroundAbilityName",
		"Ground Spell"
		);
}

bool UADGA_GroundTargetedArea::CanActivateAbility(
	const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayTagContainer* SourceTags,
	const FGameplayTagContainer* TargetTags,
	FGameplayTagContainer* OptionalRelevantTags) const
{
	if (!Super::CanActivateAbility(
		Handle,
		ActorInfo,
		SourceTags,
		TargetTags,
		OptionalRelevantTags
	))
	{
		return false;
	}

	if (!IsAvatarGrounded(ActorInfo)
		|| !EffectClass
		|| TargetingRange <= 0.0f
		|| EffectRadius <= 0.0f)
	{
		return false;
	}

	AActor* AvatarActor = ActorInfo->AvatarActor.Get();
	const auto* GroundTargeting = IsValid(AvatarActor)
		? AvatarActor->FindComponentByClass<UADGroundTargetingComponent>()
		: nullptr;
	
	return (IsValid(GroundTargeting) && GroundTargeting->HasConfirmedLocation());
}

void UADGA_GroundTargetedArea::ActivateAbility(
	const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(
		Handle,
		ActorInfo,
		ActivationInfo,
		TriggerEventData
	);

	AActor* AvatarActor = ActorInfo->AvatarActor.Get();

	auto* GroundTargeting = IsValid(AvatarActor)
				? AvatarActor->FindComponentByClass<UADGroundTargetingComponent>()
				: nullptr;

	if (!IsValid(GroundTargeting)
		|| !GroundTargeting->ConsumeConfirmedLocation(CachedTargetLocation))
	{
		EndCurrentAbility(true);
		return;
	}

	if (!IsTargetLocationInRange(AvatarActor, CachedTargetLocation))
	{
		EndCurrentAbility(true);
		return;
	}

	bHasCachedTargetLocation = true;

	auto* CastComponent = AvatarActor->FindComponentByClass<UADCastComponent>();
	if (!IsValid(CastComponent))
	{
		EndCurrentAbility(true);
		return;
	}

	CachedCastComponent = CastComponent;

	if (CastTime <= 0.0f)
	{
		HandleCastFinished();
		return;
	}

	CastComponent->StartCast(CastDisplayName, CastTime);

	auto* WaitTask = UAbilityTask_WaitDelay::WaitDelay(this, CastTime);
	WaitTask->OnFinish.AddDynamic(this, &UADGA_GroundTargetedArea::HandleCastFinished);
	WaitTask->ReadyForActivation();
}

void UADGA_GroundTargetedArea::HandleCastFinished()
{
	if (!IsActive()	|| !bHasCachedTargetLocation) { return;	}

	const FGameplayAbilityActorInfo* ActorInfo = GetCurrentActorInfo();

	AActor* AvatarActor = ActorInfo ? ActorInfo->AvatarActor.Get() : nullptr;
	if (!IsValid(AvatarActor)
		|| !IsAvatarGrounded(ActorInfo)
		|| !IsTargetLocationInRange(
			AvatarActor,
			CachedTargetLocation))
	{
		EndCurrentAbility(true);
		return;
	}

	if (!CommitAbility(GetCurrentAbilitySpecHandle(), ActorInfo, GetCurrentActivationInfo()))
	{
		EndCurrentAbility(true);
		return;
	}

	const int32 AffectedEnemies = ApplyEffectAtTargetLocation();

	ReceiveGroundEffectExecuted(CachedTargetLocation, EffectRadius,	AffectedEnemies);

	UE_LOG(
		LogTemp,
		Display,
		TEXT("%s cast %s at %s and affected %d enemies."),
		*GetNameSafe(AvatarActor),
		*CastDisplayName.ToString(),
		*CachedTargetLocation.ToString(),
		AffectedEnemies
	);

	EndCurrentAbility(false);
}

int32 UADGA_GroundTargetedArea::ApplyEffectAtTargetLocation()
{
	if (!bHasCachedTargetLocation || !EffectClass) { return 0; }

	UAbilitySystemComponent* SourceASC = GetAbilitySystemComponentFromActorInfo();
	AActor* AvatarActor = GetAvatarActorFromActorInfo();
	if (!IsValid(SourceASC) || !IsValid(AvatarActor)) { return 0; }

	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;

	ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECC_Pawn));

	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(AvatarActor);

	TArray<AActor*> OverlappedActors;

	UKismetSystemLibrary::SphereOverlapActors(
		AvatarActor,
		CachedTargetLocation,
		EffectRadius,
		ObjectTypes,
		AADEnemyCharacter::StaticClass(),
		ActorsToIgnore,
		OverlappedActors
	);

	int32 AffectedEnemies = 0;
	for (AActor* Actor : OverlappedActors)
	{
		auto* Enemy = Cast<AADEnemyCharacter>(Actor);
		if (!IsValid(Enemy) || Enemy->IsDead()) { continue; }

		auto* TargetASC= Enemy->GetAbilitySystemComponent();
		if (!IsValid(TargetASC)) { continue; }

		const auto Spec = MakeOutgoingGameplayEffectSpec(EffectClass, GetAbilityLevel());
		if (!Spec.IsValid()) { continue; }

		SourceASC->ApplyGameplayEffectSpecToTarget(*Spec.Data.Get(), TargetASC);

		++AffectedEnemies;
	}

	return AffectedEnemies;
}

bool UADGA_GroundTargetedArea::IsTargetLocationInRange(
		const AActor* AvatarActor,
		const FVector& TargetLocation) const
{
	if (!IsValid(AvatarActor))
	{
		return false;
	}

	return FVector::DistSquared(
		AvatarActor->GetActorLocation(),
		TargetLocation
	)
	<= FMath::Square(TargetingRange);
}

void UADGA_GroundTargetedArea::EndCurrentAbility(const bool bWasCancelled)
{
	EndAbility(
		GetCurrentAbilitySpecHandle(),
		GetCurrentActorInfo(),
		GetCurrentActivationInfo(),
		true,
		bWasCancelled
	);
}

void UADGA_GroundTargetedArea::EndAbility(
	const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo,
	const bool bReplicateEndAbility,
	const bool bWasCancelled)
{
	if (UADCastComponent* CastComponent = CachedCastComponent.Get())
	{
		CastComponent->EndCast(bWasCancelled);
	}

	CachedCastComponent.Reset();

	CachedTargetLocation = FVector::ZeroVector;

	bHasCachedTargetLocation = false;

	Super::EndAbility(
		Handle,
		ActorInfo,
		ActivationInfo,
		bReplicateEndAbility,
		bWasCancelled
	);
}

float UADGA_GroundTargetedArea::GetTargetingRange() const
{
	return TargetingRange;
}

float UADGA_GroundTargetedArea::GetEffectRadius() const
{
	return EffectRadius;
}

TSubclassOf<AActor> UADGA_GroundTargetedArea::GetTargetPreviewActorClass() const
{
	return TargetPreviewActorClass;
}