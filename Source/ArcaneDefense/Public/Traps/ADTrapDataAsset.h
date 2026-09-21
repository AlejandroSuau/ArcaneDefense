#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "ADTrapDataAsset.generated.h"

class AADTrapBase;
class AADTrapPlacementPreview;
class UTexture2D;

/**
 * Data-driven definition of a placeable trap.
 */
UCLASS(BlueprintType)
class ARCANEDEFENSE_API UADTrapDataAsset
	: public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(
		EditAnywhere,
		BlueprintReadOnly,
		Category = "Trap|Identity")
	FText DisplayName;

	UPROPERTY(
		EditAnywhere,
		BlueprintReadOnly,
		Category = "Trap|Identity")
	TObjectPtr<UTexture2D> Icon;

	UPROPERTY(
		EditAnywhere,
		BlueprintReadOnly,
		Category = "Trap|Identity")
	TSubclassOf<AADTrapBase> TrapActorClass;

	UPROPERTY(
		EditAnywhere,
		BlueprintReadOnly,
		Category = "Trap|Placement")
	TSubclassOf<AADTrapPlacementPreview> PlacementPreviewClass;

	UPROPERTY(
		EditAnywhere,
		BlueprintReadOnly,
		Category = "Trap|Economy",
		meta = (ClampMin = "0"))
	int32 ConstructionCost = 25;

	UPROPERTY(
		EditAnywhere,
		BlueprintReadOnly,
		Category = "Trap|Economy",
		meta = (
			ClampMin = "0.0",
			ClampMax = "1.0"))
	float SellRefundRatio = 1.0f;
	
	UPROPERTY(
		EditAnywhere,
		BlueprintReadOnly,
		Category = "Trap|Placement")
	bool bCanPlaceOnFloor = true;

	UPROPERTY(
		EditAnywhere,
		BlueprintReadOnly,
		Category = "Trap|Placement")
	bool bCanPlaceOnWall = false;

	UPROPERTY(
		EditAnywhere,
		BlueprintReadOnly,
		Category = "Trap|Range",
		meta = (
			ClampMin = "1.0",
			Units = "cm"))
	float ActivationRadius = 100.0f;

	UPROPERTY(
		EditAnywhere,
		BlueprintReadOnly,
		Category = "Trap|Range",
		meta = (
			ClampMin = "1.0",
			Units = "cm"))
	float EffectRadius = 150.0f;
};