// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/ADGameplayAbility.h"
#include "ADGA_TargetedDamage.generated.h"

class AADEnemyCharacter;
class UADCastComponent;
class UGameplayEffect;

/**
 * Targeted ability that waits for a configurable cast time before
 * applying a damage Gameplay Effect to its captured target.
 */
UCLASS(Abstract, Blueprintable)
class ARCANEDEFENSE_API UADGA_TargetedDamage : public UADGameplayAbility
{
	GENERATED_BODY()

public:
	UADGA_TargetedDamage();
	
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

	virtual void EndAbility(
		const FGameplayAbilitySpecHandle Handle,
		const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo,
		bool bReplicateEndAbility,
		bool bWasCancelled
	) override;

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

	/** Time required to complete the ability. */
	UPROPERTY(
		EditDefaultsOnly,
		BlueprintReadOnly,
		Category = "Ability|Casting",
		meta = (ClampMin = "0.0", Units = "s"))
	float  CastTime = 1.5f;

	/** Name presented by the cast bar. */
	UPROPERTY(
		EditDefaultsOnly,
		BlueprintReadOnly,
		Category = "Ability|Casting")
	FText CastDisplayName;

private:
	UFUNCTION()
	void HandleCastFinished();
	
	AADEnemyCharacter* GetValidTarget(
		const FGameplayAbilityActorInfo* ActorInfo) const;

	bool IsTargetInRange(
		const AActor* SourceActor,
		const AActor* TargetActor) const;

	void EndCurrentAbility(bool bWasCancelled);

	TWeakObjectPtr<AADEnemyCharacter> CachedTarget;
	TWeakObjectPtr<UADCastComponent> CachedCastComponent;
};
