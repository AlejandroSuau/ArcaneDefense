#include "Traps/ADTrapPlacementSlot.h"

#include "Components/BoxComponent.h"
#include "Components/SceneComponent.h"
#include "Traps/ADTrapBase.h"
#include "Traps/ADTrapCollision.h"
#include "Traps/ADTrapDataAsset.h"

AADTrapPlacementSlot::AADTrapPlacementSlot()
{
	PrimaryActorTick.bCanEverTick = false;

	SceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("SceneRoot"));

	SetRootComponent(SceneRoot);

	SelectionVolume = CreateDefaultSubobject<UBoxComponent>(TEXT("SelectionVolume"));
	SelectionVolume->SetupAttachment(SceneRoot);
	SelectionVolume->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	SelectionVolume->SetCollisionObjectType(ECC_WorldDynamic);
	SelectionVolume->SetCollisionResponseToAllChannels(ECR_Ignore);
	SelectionVolume->SetCollisionResponseToChannel(
		ADCollisionChannels::TrapPlacementSlot,ECR_Block);
	SelectionVolume->SetGenerateOverlapEvents(false);

	ApplySlotSettings();
}

void AADTrapPlacementSlot::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	ApplySlotSettings();

	if (!GetActorScale3D().Equals(FVector::OneVector,0.001f))
	{
		UE_LOG(
			LogTemp,
			Warning,
			TEXT("%s: Placement Slots should use scale 1,1,1."),
			*GetNameSafe(this)
		);
	}
}

void AADTrapPlacementSlot::ApplySlotSettings()
{
	if (!IsValid(SelectionVolume)) { return; }

	constexpr float SelectionDepth = 5.0f;

	SelectionVolume->SetBoxExtent(
		FVector(
			FMath::Max(1.0f, SlotHalfSize.X),
			FMath::Max(1.0f, SlotHalfSize.Y),
			SelectionDepth * 0.5f),
		false);

	/*
	 * Actor origin represents the actual placement plane.
	 * The selection box sits slightly behind that plane.
	 */
	SelectionVolume->SetRelativeLocation(
		FVector(0.0f,0.0f,-SelectionDepth * 0.5f));
}

bool AADTrapPlacementSlot::SupportsTrap(const UADTrapDataAsset* TrapData) const
{
	if (!IsValid(TrapData)) { return false; }

	switch (SlotType)
	{
	case EADTrapPlacementSlotType::Floor:
		return TrapData->bCanPlaceOnFloor;
	case EADTrapPlacementSlotType::Wall:
		return TrapData->bCanPlaceOnWall;
	default:
		return false;
	}
}

bool AADTrapPlacementSlot::IsOccupied() const
{
	return OccupyingTrap.IsValid();
}

bool AADTrapPlacementSlot::TryOccupy(AADTrapBase* Trap)
{
	if (!IsValid(Trap) || IsOccupied()) { return false; }

	OccupyingTrap = Trap;
	return true;
}

void AADTrapPlacementSlot::Release(AADTrapBase* Trap)
{
	if (OccupyingTrap.Get() != Trap) { return; }

	OccupyingTrap.Reset();
}

FTransform AADTrapPlacementSlot::GetPlacementTransform() const
{
	return GetActorTransform();
}

FVector AADTrapPlacementSlot::GetPlacementBlockingHalfExtent() const
{
	return FVector(
		FMath::Max(1.0f, SlotHalfSize.X),
		FMath::Max(1.0f, SlotHalfSize.Y),
		FMath::Max(1.0f,PlacementClearance * 0.5f));
}

EADTrapPlacementSlotType AADTrapPlacementSlot::GetSlotType() const
{
	return SlotType;
}