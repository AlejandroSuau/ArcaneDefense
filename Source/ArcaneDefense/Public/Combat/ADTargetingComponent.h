// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ADTargetingComponent.generated.h"

class AADEnemyCharacter;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(
	FADTargetChangedSignature,
	AActor*,
	PreviousTarget,
	AActor*,
	NewTarget);

/**
 * Handles player target selection.
 *
 * This component does not own the selected target and must remain valid
 * when that target is destroyed.
 */
UCLASS(
	ClassGroup=(Combat),
	meta=(BlueprintSpawnableComponent))
class ARCANEDEFENSE_API UADTargetingComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UADTargetingComponent();

	/**
	 * Performs a trace under the mouse cursor and selects a valid enemy.
	 *
	 * Clicking somewhere without a valid enemy clears the current target.
	 */
	UFUNCTION(BlueprintCallable, Category = "Targeting")
	bool TrySelectTargetUnderCursor();

	/**
	 * Removes the current selection.
	 */
	UFUNCTION(BlueprintCallable, Category = "Targeting")
	void ClearTarget();

	/**
	 * Returns the currently selected enemy.
	 */
	UFUNCTION(BlueprintPure, Category = "Targeting")
	AADEnemyCharacter* GetCurrentTarget()  const;

	/**
	 * Returns true when the component has a valid selected target.
	 */
	UFUNCTION(BlueprintPure, Category = "Targeting")
	bool HasValidTarget() const;

	/**
	 * Broadcast whenever the current target changes.
	 *
	 * The UI will subscribe to this delegate later.
	 */
	UPROPERTY(BlueprintAssignable, Category = "Targeting")
	FADTargetChangedSignature OnTargetChanged;

protected:
	virtual void EndPlay(
		const EEndPlayReason::Type EndPlayReason) override;

private:
	bool IsValidTargetCandidate(const AADEnemyCharacter* Candidate) const;
	void SetCurrentTarget(AADEnemyCharacter* NewTarget);

	UFUNCTION()
	void HandleCurrentTargetDestroyed(AActor* DestroyedActor);

	/**
	 * Maximum distance at which an enemy can be selected.
	 */
	UPROPERTY(
		EditDefaultsOnly,
		Category = "Targeting",
		meta = (ClampMin = "0.0", Units = "cm"))
	float MaxSelectionDistance = 3000.f;
	
	/**
	 * Non-owning reference to the current target.
	 */
	UPROPERTY(Transient)
	TWeakObjectPtr<AADEnemyCharacter> CurrentTarget;
	
};
