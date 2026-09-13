// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Characters/ADCharacterBase.h"
#include "ADEnemyCharacter.generated.h"

class AADDefenseObjective;
class UGameplayEffect;

class AADEnemyCharacter;

DECLARE_MULTICAST_DELEGATE_OneParam(
	FADEnemyDefeatedSignature,
	AADEnemyCharacter*
);

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
	AADEnemyCharacter();

	virtual void HandleDeath() override;
	
	/**
	 * Called by the AI controller when navigation reaches its goal.
	 */
	void HandleReachedMoveTarget();
	
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

	/**
	 * Changes the actor this enemy should navigate toward.
	 */
	UFUNCTION(BlueprintCallable, Category = "AI")
	void SetMoveTarget(AActor*  NewMoveTarget);

	/**
	 * Returns the actor this enemy should navigate toward.
	 */
	UFUNCTION(BlueprintPure, Category = "AI")
	AActor* GetMoveTarget() const;

	/**
	 * Broadcast exactly once when this enemy enters its death state.
	 *
	 * Used by systems such as the Wave Director to track living enemies.
	 */
	FADEnemyDefeatedSignature OnEnemyDefeated;
	
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
	void StartAttackingObjective(AADDefenseObjective* Objective);
	void StopAttackingObjective();
	void AttackObjective();

	UPROPERTY(
		EditDefaultsOnly,
		BlueprintReadOnly,
		Category = "Combat|Objective",
		meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UGameplayEffect> ObjectiveDamageEffect;

	FTimerHandle ObjectiveAttackTimerHandle;

	TWeakObjectPtr<AADDefenseObjective> ObjectiveBeingAttacked;
	
	UPROPERTY(
		EditDefaultsOnly,
		BlueprintReadOnly,
		Category = "Combat|Objective",
		meta = (
			AllowPrivateAccess = "true",
			ClampMin = "0.1",
			Units = "s"
		))
	float ObjectiveAttackInterval = 1.0f;
	
	UPROPERTY(
		VisibleInstanceOnly,
		BlueprintReadOnly,
		Category = "Targeting",
		meta =  (AllowPrivateAccess = "true"))
	bool bIsTargeted = false;

	/**
	 * Destination actor used by the enemy AI.
	 *
	 * EditInstanceOnly allows us to assign a level actor to enemies
	 * placed manually in the prototype map.
	 */
	UPROPERTY(
		EditInstanceOnly,
		BlueprintReadOnly,
		Category = "AI",
		meta = (AllowPrivateAccess = "true"))
	TObjectPtr<AActor> MoveTarget;
};
