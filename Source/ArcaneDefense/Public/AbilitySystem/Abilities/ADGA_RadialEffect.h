#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/ADGameplayAbility.h"
#include "ADGA_RadialEffect.generated.h"

class AADEnemyCharacter;
class UGameplayEffect;

/**
 * Instant area ability that applies a configured Gameplay Effect
 * to every valid enemy inside a radius around the ability avatar.
 */
UCLASS(Abstract, Blueprintable)
class ARCANEDEFENSE_API UADGA_RadialEffect
	: public UADGameplayAbility
{
	GENERATED_BODY()

protected:
	virtual bool CanActivateAbility(
		const FGameplayAbilitySpecHandle Handle,
		const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayTagContainer* SourceTags,
		const FGameplayTagContainer* TargetTags,
		FGameplayTagContainer* OptionalRelevantTags
	) const override;

	virtual void ActivateAbility(
		const FGameplayAbilitySpecHandle Handle,
		const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo,
		const FGameplayEventData* TriggerEventData
	) override;

private:
	int32 ApplyEffectToEnemiesInRadius(const FGameplayAbilityActorInfo* ActorInfo);

	UPROPERTY(
		EditDefaultsOnly,
		BlueprintReadOnly,
		Category = "Ability|Area",
		meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UGameplayEffect> EffectClass;

	UPROPERTY(
		EditDefaultsOnly,
		BlueprintReadOnly,
		Category = "Ability|Area",
		meta = (
			AllowPrivateAccess = "true",
			ClampMin = "0.0",
			Units = "cm"
		))
	float Radius = 600.0f;
};