// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/ADGameplayAbility.h"
#include "ADGA_TargetedDamage.generated.h"

class AADEnemyCharacter;
class UGameplayEffect;

/**
 * Instant targeted ability that applies a damage Gameplay Effect
 * to the player's currently selected enemy.
 */
UCLASS(Abstract, Blueprintable)
class ARCANEDEFENSE_API UADGA_TargetedDamage : public UADGameplayAbility
{
	GENERATED_BODY()

protected:
	virtual bool CanActivateAbility(
		const FGameplayAbilitySpecHandle Handle,
		const FGameplayAbilityActorInfo*  ActorInfo,
		const FGameplayTagContainer* SourceTags,
		const FGameplayTagContainer* TargetTags,
		FGameplayTagContainer*  OptionalRelevantTags) const override;

	virtual void ActivateAbility(
		const FGameplayAbilitySpecHandle Handle,
		const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo,
		const FGameplayEventData* TriggerEventData) override;

	/** Effect applied to the selected target after committing the ability. */
	UPROPERTY(
		EditDefaultsOnly,
		BlueprintReadonly,
		Category = "Ability|Damage")
	TSubclassOf<UGameplayEffect> DamageEffectClass;

	/** Maximum distance between the caster and the target. */
	UPROPERTY(
		EditDefaultsOnly,
		BlueprintReadOnly,
		Category = "Ability|Targeting",
		meta = (ClampMin = "0.0", Units = "cm")
	)
	float MaxRange = 2000.0f;

private:
	AADEnemyCharacter* GetValidTarget(
		const FGameplayAbilityActorInfo* ActorInfo) const;

	bool IsTargetInRange(
		const AActor* SourceActor,
		const AActor* TargetActor) const;
};
