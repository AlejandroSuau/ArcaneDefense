// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ADCastComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(
	FADCastStartedSignature,
	FText,
	CastName,
	float,
	CastDuration);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(
	FADCastEndedSignature,
	bool,
	bWasCancelled);

/**
 * Exposes the current cast state to presentation systems such as UI.
 *
 * This component does not control ability execution or run its own timer.
 */
UCLASS(
	ClassGroup=(Combat),
	meta=(BlueprintSpawnableComponent)
)
class ARCANEDEFENSE_API UADCastComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UADCastComponent();
	
	/**
	 * Records the beginning of a cast and notifies presentation systems.
	 */
	void StartCast(
		const FText& InCastName,
		float InCastDuration);

	/**
	 * Finishes the current cast.
	 */
	void EndCast(bool bWasCancelled);

	UFUNCTION(BlueprintPure, Category = "Casting")
	bool IsCasting() const;

	/**
	 * Returns a normalized value between zero and one.
	 */
	UFUNCTION(BlueprintPure, Category = "Casting")
	float GetCastProgress() const;

	UFUNCTION(BlueprintPure, Category = "Casting")
	float GetCastTimeRemaining() const;

	UFUNCTION(BlueprintPure, Category = "Casting")
	FText GetCastName() const;

	UPROPERTY(BlueprintAssignable, Category = "Casting")
	FADCastStartedSignature OnCastStarted;
	
	UPROPERTY(BlueprintAssignable, Category = "Casting")
	FADCastEndedSignature OnCastEnded;

private:
	UPROPERTY(Transient)
	bool bIsCasting = false;

	UPROPERTY(Transient)
	float CastStartTime = 0.f;

	UPROPERTY(Transient)
	float CastDuration = 0.f;

	UPROPERTY(Transient)
	FText CastName;
};
