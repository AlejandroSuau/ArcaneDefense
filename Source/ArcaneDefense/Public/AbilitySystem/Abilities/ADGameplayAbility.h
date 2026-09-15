// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "ADGameplayAbility.generated.h"

/**
 * Base class for Arcane Defense gameplay abilities.
 */
UCLASS(Abstract, Blueprintable)
class ARCANEDEFENSE_API UADGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()

public:
	UADGameplayAbility();

protected:
	/**
	 * Returns true when the ability avatar is a grounded Character.
	 */
	bool IsAvatarGrounded(const FGameplayAbilityActorInfo* ActorInfo) const;
};
