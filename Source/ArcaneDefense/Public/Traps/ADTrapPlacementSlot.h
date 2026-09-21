#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ADTrapPlacementSlot.generated.h"

class AADTrapBase;
class UADTrapDataAsset;
class UBoxComponent;
class USceneComponent;

UENUM(BlueprintType)
enum class EADTrapPlacementSlotType : uint8
{
	Floor,
	Wall
};

/**
 * Explicit placement position authored by the level designer.
 *
 * Local +Z points away from the supporting surface.
 */
UCLASS(Blueprintable)
class ARCANEDEFENSE_API AADTrapPlacementSlot
	: public AActor
{
	GENERATED_BODY()

public:
	AADTrapPlacementSlot();

	virtual void OnConstruction(const FTransform& Transform) override;
	bool SupportsTrap(const UADTrapDataAsset* TrapData) const;
	bool IsOccupied() const;
	bool TryOccupy(AADTrapBase* Trap);
	void Release(AADTrapBase* Trap);

	FTransform GetPlacementTransform() const;
	FVector GetPlacementBlockingHalfExtent() const;

	UFUNCTION(BlueprintPure, Category = "Trap|Placement")
	EADTrapPlacementSlotType GetSlotType() const;

protected:
	UPROPERTY(
		VisibleAnywhere,
		BlueprintReadOnly,
		Category = "Components")
	TObjectPtr<USceneComponent> SceneRoot;

	UPROPERTY(
		VisibleAnywhere,
		BlueprintReadOnly,
		Category = "Components")
	TObjectPtr<UBoxComponent> SelectionVolume;

private:
	void ApplySlotSettings();

	UPROPERTY(
		EditInstanceOnly,
		BlueprintReadOnly,
		Category = "Trap|Placement",
		meta = (AllowPrivateAccess = "true"))
	EADTrapPlacementSlotType SlotType = EADTrapPlacementSlotType::Floor;

	/**
	 * Half size of the manually authored placement square.
	 */
	UPROPERTY(
		EditInstanceOnly,
		BlueprintReadOnly,
		Category = "Trap|Placement",
		meta = (
			AllowPrivateAccess = "true",
			Units = "cm"))
	FVector2D SlotHalfSize = FVector2D(100.0f, 100.0f);

	/**
	 * How much free space must exist away from the supporting surface.
	 */
	UPROPERTY(
		EditInstanceOnly,
		BlueprintReadOnly,
		Category = "Trap|Placement",
		meta = (
			AllowPrivateAccess = "true",
			ClampMin = "1.0",
			Units = "cm"))
	float PlacementClearance = 60.0f;

	UPROPERTY(Transient)
	TWeakObjectPtr<AADTrapBase> OccupyingTrap;
};