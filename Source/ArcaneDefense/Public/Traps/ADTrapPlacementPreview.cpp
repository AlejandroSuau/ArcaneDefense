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
	float ActivationRadius,
	float EffectRadius,
	const FVector& SlotHalfExtent)
{
	const bool bSameActivation = FMath::IsNearlyEqual(
		CachedActivationRadius, ActivationRadius);
	const bool bSameEffect = FMath::IsNearlyEqual(
		CachedEffectRadius, EffectRadius);
	const bool bSameSlot = CachedSlotHalfExtent.Equals(SlotHalfExtent,0.01f);

	if (bSameActivation	&& bSameEffect && bSameSlot) { return; }

	CachedActivationRadius = ActivationRadius;
	CachedEffectRadius = EffectRadius;
	CachedSlotHalfExtent = SlotHalfExtent;

	ReceivePreviewConfigured(ActivationRadius, EffectRadius, SlotHalfExtent);
}

void AADTrapPlacementPreview::SetPlacementValid(const bool bInPlacementValid)
{
	if (bHasValidity && bPlacementValid	== bInPlacementValid) { return;	}

	bHasValidity = true;
	bPlacementValid = bInPlacementValid;
	ReceivePlacementValidityChanged(bPlacementValid);
}