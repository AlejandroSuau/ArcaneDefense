#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ADTrapPlacementComponent.generated.h"

class AADTrapPlacementPreview;
class AADTrapPlacementSlot;
class UADTrapDataAsset;

UCLASS(
	ClassGroup = (ArcaneDefense),
	meta = (BlueprintSpawnableComponent))
class ARCANEDEFENSE_API UADTrapPlacementComponent
	: public UActorComponent
{
	GENERATED_BODY()

public:
	UADTrapPlacementComponent();

	virtual void TickComponent(
		float DeltaTime,
		ELevelTick TickType,
		FActorComponentTickFunction* ThisTickFunction) override;

	virtual void EndPlay(const EEndPlayReason::Type	EndPlayReason) override;
	
	bool StartPlacement(UADTrapDataAsset* InTrapData);
	bool ConfirmPlacement();
	void CancelPlacement();
	
	UFUNCTION(BlueprintPure, Category = "Trap|Placement")
	bool IsPlacing() const;

	UFUNCTION(BlueprintPure, Category = "Trap|Placement")
	bool IsPlacementValid() const;

private:
	void UpdatePlacement();

	bool GetPlacementSlotUnderCursor(FHitResult& OutHit, AADTrapPlacementSlot*& OutSlot) const;
	bool IsSlotPhysicallyBlocked(const AADTrapPlacementSlot* Slot) const;
	
	bool CanAffordCurrentTrap() const;
	bool SpawnPreview();
	void DestroyPreview();
	bool SpawnPlacedTrap();

	UPROPERTY(
		EditDefaultsOnly,
		Category = "Trap|Placement",
		meta = (
			ClampMin = "1000.0",
			Units = "cm"))
	float CursorTraceDistance =	100000.0f;

	UPROPERTY(
		EditDefaultsOnly,
		Category = "Trap|Placement",
		meta = (
			ClampMin = "0.0",
			Units = "cm"))
	float BlockingTestInset = 1.0f;

	UPROPERTY(Transient)
	TObjectPtr<UADTrapDataAsset> SelectedTrapData;

	TWeakObjectPtr<AADTrapPlacementPreview> PreviewActor;
	TWeakObjectPtr<AADTrapPlacementSlot> CurrentSlot;

	bool bIsPlacing = false;
	bool bPlacementValid = false;
};