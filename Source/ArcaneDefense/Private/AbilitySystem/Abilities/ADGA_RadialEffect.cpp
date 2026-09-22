#include "AbilitySystem/Abilities/ADGA_RadialEffect.h"

#include "AbilitySystemComponent.h"
#include "Characters/ADEnemyCharacter.h"
#include "Engine/EngineTypes.h"
#include "Kismet/KismetSystemLibrary.h"

bool UADGA_RadialEffect::CanActivateAbility(
	const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayTagContainer* SourceTags,
	const FGameplayTagContainer* TargetTags,
	FGameplayTagContainer* OptionalRelevantTags
) const
{
	if (!Super::CanActivateAbility(
		Handle,
		ActorInfo,
		SourceTags,
		TargetTags,
		OptionalRelevantTags))
	{
		return false;
	}
	
	return (IsAvatarGrounded(ActorInfo) && EffectClass && Radius > 0.0f);
}

void UADGA_RadialEffect::ActivateAbility(
	const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	if (!CommitAbility( Handle, ActorInfo, ActivationInfo))
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}

	const int32 AffectedEnemyCount = ApplyEffectToEnemiesInRadius(ActorInfo);

	UE_LOG(
		LogTemp,
		Display,
		TEXT("%s affected %d enemies with radial ability."),
		*GetNameSafe(ActorInfo->AvatarActor.Get()),
		AffectedEnemyCount
	);

	PlayActivationMontage();
	
	EndAbility(Handle, ActorInfo, ActivationInfo,true,false);
}

int32 UADGA_RadialEffect::ApplyEffectToEnemiesInRadius(
	const FGameplayAbilityActorInfo* ActorInfo)
{
	if (!ActorInfo || !EffectClass)	{ return 0;	}

	AActor* AvatarActor = ActorInfo->AvatarActor.Get();
	if (!IsValid(AvatarActor)) { return 0; }

	UAbilitySystemComponent* SourceASC = GetAbilitySystemComponentFromActorInfo();
	if (!IsValid(SourceASC)) { return 0; }

	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
	ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECC_Pawn));

	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(AvatarActor);

	TArray<AActor*> OverlappedActors;

	UKismetSystemLibrary::SphereOverlapActors(
		AvatarActor,
		AvatarActor->GetActorLocation(),
		Radius,
		ObjectTypes,
		AADEnemyCharacter::StaticClass(),
		ActorsToIgnore,
		OverlappedActors
	);

	int32 AffectedEnemyCount = 0;
	for (AActor* OverlappedActor : OverlappedActors)
	{
		auto* Enemy = Cast<AADEnemyCharacter>(OverlappedActor);
		if (!IsValid(Enemy) || Enemy->IsDead()) { continue; }

		auto* TargetASC = Enemy->GetAbilitySystemComponent();
		if (!IsValid(TargetASC)) { continue; }

		const auto EffectSpec = MakeOutgoingGameplayEffectSpec(EffectClass, GetAbilityLevel());
		if (!EffectSpec.IsValid()) { continue; }

		SourceASC->ApplyGameplayEffectSpecToTarget(*EffectSpec.Data.Get(), TargetASC);

		++AffectedEnemyCount;
	}

	return AffectedEnemyCount;
}