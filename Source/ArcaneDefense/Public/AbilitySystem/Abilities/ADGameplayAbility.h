// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "ADGameplayAbility.generated.h"

class UTexture2D;

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
	
	/**
	 * Returns true when the ability avatar is a grounded Character.
	 */
	bool IsAvatarGrounded(const FGameplayAbilityActorInfo* ActorInfo) const;
};
