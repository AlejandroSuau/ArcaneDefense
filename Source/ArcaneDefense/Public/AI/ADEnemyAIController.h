// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "ADEnemyAIController.generated.h"

/**
 * Basic AI controller for Arcane Defense enemies.
 *
 * For now, its only responsibility is navigating the controlled enemy
 * toward a configured goal actor.
 */
UCLASS()
class ARCANEDEFENSE_API AADEnemyAIController : public AAIController
{
	GENERATED_BODY()

public:
	/**
	 * Changes the actor this AI should navigate toward.
	 */
	void SetMoveTarget(AActor* NewMoveTarget);

protected:
	virtual void OnPossess(APawn* InPawn) override;
	virtual void OnMoveCompleted(
		FAIRequestID RequestID,
		const FPathFollowingResult& Result) override;

private:
	void MoveToCurrentTarget();

	/** Distance from the destination at which movement is considered complete. */
	UPROPERTY(
		EditDefaultsOnly,
		Category = "AI|Movement",
		meta = (ClampMin = "0.0", Units = "cm"))
	float AcceptanceRadius = 100.f;

	/** Non-owning reference to the actor being approached. */
	TWeakObjectPtr<AActor> MoveTarget;
};
