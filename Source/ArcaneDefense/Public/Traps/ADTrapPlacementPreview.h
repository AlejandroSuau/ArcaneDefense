#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ADTrapPlacementPreview.generated.h"

class USceneComponent;

UCLASS(Blueprintable)
class ARCANEDEFENSE_API AADTrapPlacementPreview
	: public AActor
{
	GENERATED_BODY()

public:
	AADTrapPlacementPreview();

	void ConfigurePreview(
		float ActivationRange,
		const FVector& SlotHalfExtent);
	void SetPlacementValid(bool bInPlacementValid);

protected:
	UFUNCTION(
		BlueprintImplementableEvent,
		Category = "Trap|Placement",
		meta = (DisplayName = "Preview Configured"))
	void ReceivePreviewConfigured(float ActivationRange, FVector SlotHalfExtent);

	UFUNCTION(
		BlueprintImplementableEvent,
		Category = "Trap|Placement",
		meta = (DisplayName = "Placement Validity Changed"))
	void ReceivePlacementValidityChanged(bool bIsValid);

private:
	UPROPERTY(
		VisibleAnywhere,
		Category = "Components")
	TObjectPtr<USceneComponent> SceneRoot;

	float CachedActivationRange = -1.0f;
	FVector CachedSlotHalfExtent  = FVector::ZeroVector;
	bool bHasValidity = false;
	bool bPlacementValid = false;
};