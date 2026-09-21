#include "Traps/ADTrapPlacementPreview.h"

#include "Components/SceneComponent.h"

AADTrapPlacementPreview::AADTrapPlacementPreview()
{
	PrimaryActorTick.bCanEverTick = false;

	SceneRoot =	CreateDefaultSubobject<USceneComponent>(TEXT("SceneRoot"));

	SetRootComponent(SceneRoot);
	SetActorEnableCollision(false);
}

void AADTrapPlacementPreview::ConfigurePreview(
		const float ActivationRange, const FVector& SlotHalfExtent)
{
	const bool bSameRange = FMath::IsNearlyEqual(CachedActivationRange,ActivationRange);
	const bool bSameSlot = CachedSlotHalfExtent.Equals(SlotHalfExtent,0.01f);
	if (bSameRange && bSameSlot) { return; }

	CachedActivationRange = ActivationRange;
	CachedSlotHalfExtent = SlotHalfExtent;

	ReceivePreviewConfigured(ActivationRange, SlotHalfExtent);
}

void AADTrapPlacementPreview::SetPlacementValid(const bool bInPlacementValid)
{
	if (bHasValidity && bPlacementValid	== bInPlacementValid) { return;	}

	bHasValidity = true;
	bPlacementValid = bInPlacementValid;
	ReceivePlacementValidityChanged(bPlacementValid);
}