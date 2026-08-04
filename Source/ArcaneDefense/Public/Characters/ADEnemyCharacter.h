// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Characters/ADCharacterBase.h"
#include "ADEnemyCharacter.generated.h"

/**
 * Base class for enemy characters.
 *
 * Enemy-specific AI behavior will be added in a later milestone.
 */
UCLASS()
class ARCANEDEFENSE_API AADEnemyCharacter : public AADCharacterBase
{
	GENERATED_BODY()

public:
	/**
	 * Changes whether this enemy is currently selected by the player.
	 */
	UFUNCTION(BlueprintCallable, Category = "Targeting")
	void SetTargeted(bool  bNewTargeted);

	/**
	 * Returns whether this enemy is currently selected.
	 */
	UFUNCTION(BlueprintPure, Category = "Targeting")
	bool IsTargeted() const;

protected:
	/**
	 * Implemented by the Blueprint child to update the visual indicator.
	 */
	UFUNCTION(
		BlueprintImplementableEvent,
		Category = "Targeting",
		meta  = (DisplayName = "Targeted State  Changed"))
	void ReceiveTargetedStateChanged(bool bNewTargeted);

private:
	UPROPERTY(
		VisibleInstanceOnly,
		BlueprintReadOnly,
		Category = "Targeting",
		meta =  (AllowPrivateAccess = "true"))
	bool bIsTargeted = false;
};
