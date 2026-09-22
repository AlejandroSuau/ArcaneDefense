// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "ADGameplayAbility.generated.h"

class UTexture2D;
class UAnimMontage;
class UAbilityTask_PlayMontageAndWait;

/**
 * Base class for Arcane Defense gameplay abilities.
 */
UCLASS(Abstract, Blueprintable)
class ARCANEDEFENSE_API UADGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()

public:
	UADGameplayAbility();

	UFUNCTION(BlueprintPure, Category = "Ability|UI")
	FText GetAbilityDisplayName() const;

	UFUNCTION(BlueprintPure, Category = "Ability|UI")
	UTexture2D* GetAbilityIcon() const;

protected:
	/**
	 * Requests the casting montage to leave its Channel loop
	 * and continue into the End section.
	 */
	void FinishActivationMontage();
	
	/**
	 * Starts the presentation montage associated with this ability.
	 *
	 * The montage is not responsible for gameplay timing.
	 */
	void PlayActivationMontage();
	
	UPROPERTY(
		EditDefaultsOnly,
		BlueprintReadOnly,
		Category = "Ability|UI")
	FText AbilityDisplayName;

	UPROPERTY(
		EditDefaultsOnly,
		BlueprintReadOnly,
		Category = "Ability|UI")
	TObjectPtr<UTexture2D> AbilityIcon;
	
	UPROPERTY(
		EditDefaultsOnly,
		BlueprintReadOnly,
		Category = "Ability|Animation")
	TObjectPtr<UAnimMontage> ActivationMontage;

	/**
	 * Visual playback speed only.
	 * Does not modify CastTime.
	 */
	UPROPERTY(
		EditDefaultsOnly,
		BlueprintReadOnly,
		Category = "Ability|Animation",
		meta = (ClampMin = "0.01"))
	float ActivationMontagePlayRate = 1.0f;

	/**
	 * If true, the montage stops when the ability ends normally.
	 *
	 * Explicit ability cancellation always stops PlayMontageAndWait.
	 */
	UPROPERTY(
		EditDefaultsOnly,
		BlueprintReadOnly,
		Category = "Ability|Animation")
	bool bStopMontageWhenAbilityEnds = true;
	
	/**
	 * Returns true when the ability avatar is a grounded Character.
	 */
	bool IsAvatarGrounded(const FGameplayAbilityActorInfo* ActorInfo) const;
};
