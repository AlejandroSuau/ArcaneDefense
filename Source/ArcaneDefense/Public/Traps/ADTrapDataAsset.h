#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "ADTrapDataAsset.generated.h"

class AADTrapBase;
class AActor;
class UTexture2D;

/**
 * Data-driven definition of a placeable trap.
 *
 * Contains configuration only. Runtime behavior belongs to the trap actor
 * and placement rules belong to the placement system.
 */
UCLASS(BlueprintType)
class ARCANEDEFENSE_API UADTrapDataAsset
	: public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(
		EditDefaultsOnly,
		BlueprintReadOnly,
		Category = "Trap|Identity")
	FText DisplayName;

	UPROPERTY(
		EditDefaultsOnly,
		BlueprintReadOnly,
		Category = "Trap|Identity")
	TObjectPtr<UTexture2D> Icon;

	/**
	 * Runtime actor spawned after successful placement.
	 */
	UPROPERTY(
		EditDefaultsOnly,
		BlueprintReadOnly,
		Category = "Trap|Identity")
	TSubclassOf<AADTrapBase> TrapActorClass;

	/**
	 * Presentation-only actor used during placement.
	 */
	UPROPERTY(
		EditDefaultsOnly,
		BlueprintReadOnly,
		Category = "Trap|Placement")
	TSubclassOf<AActor> PlacementPreviewClass;

	/**
	 * Base construction price.
	 *
	 * The placed actor will remember the price that was actually paid,
	 * because talents may change this value in the future.
	 */
	UPROPERTY(
		EditDefaultsOnly,
		BlueprintReadOnly,
		Category = "Trap|Economy",
		meta = (ClampMin = "0"))
	int32 ConstructionCost = 25;

	/**
	 * Fraction of the actual purchase price returned when selling.
	 */
	UPROPERTY(
		EditDefaultsOnly,
		BlueprintReadOnly,
		Category = "Trap|Economy",
		meta = (
			ClampMin = "0.0",
			ClampMax = "1.0"))
	float SellRefundRatio = 1.0f;

	/**
	 * Area used by the runtime trap to detect enemies.
	 */
	UPROPERTY(
		EditDefaultsOnly,
		BlueprintReadOnly,
		Category = "Trap|Runtime")
	FVector TriggerBoxExtent = FVector(100.0f, 100.0f, 75.0f);

	/**
	 * Physical footprint required for placement.
	 */
	UPROPERTY(
		EditDefaultsOnly,
		BlueprintReadOnly,
		Category = "Trap|Placement")
	FVector PlacementBoxExtent = FVector(75.0f, 75.0f, 30.0f);

	UPROPERTY(
		EditDefaultsOnly,
		BlueprintReadOnly,
		Category = "Trap|Placement")
	bool bCanPlaceOnFloor = true;

	UPROPERTY(
		EditDefaultsOnly,
		BlueprintReadOnly,
		Category = "Trap|Placement")
	bool bCanPlaceOnWall = false;

	/**
	 * Maximum accepted floor slope.
	 */
	UPROPERTY(
		EditDefaultsOnly,
		BlueprintReadOnly,
		Category = "Trap|Placement",
		meta = (
			ClampMin = "0.0",
			ClampMax = "89.0",
			Units = "deg"))
	float MaximumFloorSlopeDegrees = 10.0f;

	/**
	 * Maximum allowed height difference across sampled placement points.
	 */
	UPROPERTY(
		EditDefaultsOnly,
		BlueprintReadOnly,
		Category = "Trap|Placement",
		meta = (
			ClampMin = "0.0",
			Units = "cm"))
	float SurfaceHeightTolerance = 5.0f;
};