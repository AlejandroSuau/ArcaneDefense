#include "Traps/ADTrapPlacementComponent.h"

#include "Characters/ADPlayerCharacter.h"
#include "Engine/World.h"
#include "Game/ADGameMode.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/PlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "Traps/ADTrapBase.h"
#include "Traps/ADTrapCollision.h"
#include "Traps/ADTrapDataAsset.h"
#include "Traps/ADTrapPlacementPreview.h"
#include "Traps/ADTrapPlacementSlot.h"

UADTrapPlacementComponent::UADTrapPlacementComponent()
{
	PrimaryComponentTick.bCanEverTick =	true;
	PrimaryComponentTick.bStartWithTickEnabled = false;
}

void UADTrapPlacementComponent::TickComponent(
	const float DeltaTime,
	const ELevelTick TickType,
	FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (bIsPlacing)
	{
		UpdatePlacement();
	}
}

bool UADTrapPlacementComponent::StartPlacement(UADTrapDataAsset* InTrapData)
{
	if (!IsValid(InTrapData)
		|| !InTrapData->TrapActorClass
		|| !InTrapData->PlacementPreviewClass)
	{
		return false;
	}
	
	CancelPlacement();

	SelectedTrapData = InTrapData;

	if (!SpawnPreview())
	{
		SelectedTrapData = nullptr;
		return false;
	}

	bIsPlacing = true;
	bPlacementValid = false;

	SetComponentTickEnabled(true);

	UpdatePlacement();

	return true;
}

bool UADTrapPlacementComponent::SpawnPreview()
{
	if (!IsValid(SelectedTrapData)) { return false; }

	UWorld* World = GetWorld();
	if (!IsValid(World)) { return false; }

	FActorSpawnParameters SpawnParameters;
	SpawnParameters.Owner = GetOwner();
	SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	AADTrapPlacementPreview* Preview = World->SpawnActor<AADTrapPlacementPreview>(
		SelectedTrapData->PlacementPreviewClass, FTransform::Identity, SpawnParameters);
	if (!IsValid(Preview)) { return false; }

	PreviewActor = Preview;
	Preview->SetActorHiddenInGame(true);

	return true;
}

bool UADTrapPlacementComponent::IsSlotPhysicallyBlocked(const AADTrapPlacementSlot* Slot) const
{
	if (!IsValid(Slot)) { return true; }

	UWorld* World = GetWorld();
	if (!IsValid(World)) { return true; }

	FVector Extent = Slot->GetPlacementBlockingHalfExtent();
	/*
	* Avoid considering simple face contact with neighboring
	 * blocking volumes as a placement overlap.
	 */
	Extent.X = FMath::Max(1.0f,Extent.X - 1.0f);
	Extent.Y = FMath::Max(1.0f, Extent.Y - 1.0f);
	
	const FTransform Transform = Slot->GetPlacementTransform();
	const FVector Center = Transform.TransformPosition({0.0f,0.0f, Extent.Z});
	const FCollisionShape Shape = FCollisionShape::MakeBox(Extent);

	FCollisionQueryParams Params(
		SCENE_QUERY_STAT(TrapPlacementSlotBlocking),
		false,
		GetOwner());
	if (AADTrapPlacementPreview* Preview = PreviewActor.Get())
	{
		Params.AddIgnoredActor(Preview);
	}

	return World->OverlapBlockingTestByChannel(
		Center,
		Transform.GetRotation(),
		ADCollisionChannels::TrapPlacementBlocker,
		Shape,
		Params);
}

void UADTrapPlacementComponent::UpdatePlacement()
{
	AADTrapPlacementPreview* Preview = PreviewActor.Get();
	if (!IsValid(Preview) || !IsValid(SelectedTrapData)) { return; }

	FHitResult Hit;
	AADTrapPlacementSlot* Slot = nullptr;
	if (!GetPlacementSlotUnderCursor(Hit, Slot))
	{
		CurrentSlot.Reset();
		bPlacementValid = false;
		Preview->SetActorHiddenInGame(true);

		return;
	}

	CurrentSlot = Slot;
	const FTransform Transform = Slot->GetPlacementTransform();
	Preview->SetActorTransform(Transform);
	Preview->SetActorHiddenInGame(false);

	Preview->ConfigurePreview(
		SelectedTrapData->ActivationRange,
		Slot->GetPlacementBlockingHalfExtent());

	const bool bSupportsTrap = Slot->SupportsTrap(SelectedTrapData);
	const bool bSlotFree = !Slot->IsOccupied();
	const bool bPhysicallyBlocked = IsSlotPhysicallyBlocked(Slot);
	const bool bAffordable = CanAffordCurrentTrap();

	bPlacementValid = (
		bSupportsTrap
		&& bSlotFree
		&& !bPhysicallyBlocked
		&& bAffordable);

	Preview->SetPlacementValid(bPlacementValid);
}

bool UADTrapPlacementComponent::GetPlacementSlotUnderCursor(
	FHitResult& OutHit,
	AADTrapPlacementSlot*& OutSlot) const
{
	OutSlot = nullptr;

	const auto* OwnerPawn = Cast<APawn>(GetOwner());
	APlayerController* PC = IsValid(OwnerPawn)
		? Cast<APlayerController>(OwnerPawn->GetController())
		: nullptr;

	UWorld* World = GetWorld();

	if (!IsValid(PC) || !IsValid(World)) { return false; }

	float MouseX = 0.0f;
	float MouseY = 0.0f;
	if (!PC->GetMousePosition(MouseX, MouseY)) { return false; }

	FVector WorldOrigin;
	FVector WorldDirection;
	if (!PC->DeprojectScreenPositionToWorld(
		MouseX, MouseY,WorldOrigin,WorldDirection))
	{
		return false;
	}

	const FVector End = WorldOrigin + WorldDirection * CursorTraceDistance;
	FCollisionQueryParams Params(
		SCENE_QUERY_STAT(TrapPlacementSlotTrace),
		false,
		GetOwner());

	if (!World->LineTraceSingleByChannel(
		OutHit,
		WorldOrigin,
		End,
		ADCollisionChannels::TrapPlacementSlot,
		Params))
	{
		return false;
	}

	OutSlot = Cast<AADTrapPlacementSlot>(OutHit.GetActor());

	return IsValid(OutSlot);
}

bool UADTrapPlacementComponent::CanAffordCurrentTrap() const
{
	const auto* Player = Cast<AADPlayerCharacter>(GetOwner());
	return IsValid(Player)
		&& IsValid(SelectedTrapData)
		&& Player->CanAffordCoins(SelectedTrapData->ConstructionCost);
}

bool UADTrapPlacementComponent::ConfirmPlacement()
{
	if (!bIsPlacing) { return false; }

	/*
	 * Never trust the previous frame.
	 * Revalidate immediately before the transaction.
	 */
	UpdatePlacement();

	if (!bPlacementValid || !SpawnPlacedTrap()) { return false; }
	
	/*
	 * Stay in placement mode for fast repeated placement.
	 */
	UpdatePlacement();

	return true;
}

bool UADTrapPlacementComponent::SpawnPlacedTrap()
{
	auto* Player = Cast<AADPlayerCharacter>(GetOwner());
	auto* Slot = CurrentSlot.Get();
	if (!IsValid(Player)
		|| !IsValid(Slot)
		|| !IsValid(SelectedTrapData)
		|| !SelectedTrapData->TrapActorClass
		|| Slot->IsOccupied()
		|| !Slot->SupportsTrap(SelectedTrapData)
		|| (IsSlotPhysicallyBlocked(Slot)))
	{
		return false;
	}

	const int32 PurchasePrice = SelectedTrapData->ConstructionCost;
	if (!Player->CanAffordCoins(PurchasePrice)) { return false; }

	UWorld* World = GetWorld();
	if (!IsValid(World)) { return false; }

	const FTransform SpawnTransform = Slot->GetPlacementTransform();
	auto* Trap = World->SpawnActorDeferred<AADTrapBase>(
		SelectedTrapData->TrapActorClass,
		SpawnTransform,
		Player,
		Player,
		ESpawnActorCollisionHandlingMethod::AlwaysSpawn);

	if (!IsValid(Trap)) { return false; }

	if (!Slot->TryOccupy(Trap))
	{
		Trap->Destroy();
		return false;
	}

	if (!Player->SpendCoins(PurchasePrice))
	{
		Slot->Release(Trap);
		Trap->Destroy();
		return false;
	}

	Trap->InitializeTrap(
		SelectedTrapData,
		Player->GetAbilitySystemComponent(),
		PurchasePrice,
		Slot);

	auto* FinishedActor = UGameplayStatics::FinishSpawningActor(Trap, SpawnTransform);
	if (!IsValid(FinishedActor))
	{
		Slot->Release(Trap);
		Player->AddCoins(PurchasePrice);
		Trap->Destroy();

		return false;
	}

	return true;
}

void UADTrapPlacementComponent::CancelPlacement()
{
	bIsPlacing = false;
	bPlacementValid = false;
	CurrentSlot.Reset();
	
	SetComponentTickEnabled(false);

	DestroyPreview();

	SelectedTrapData = nullptr;
}

void UADTrapPlacementComponent::DestroyPreview()
{
	if (AADTrapPlacementPreview* Preview = PreviewActor.Get())
	{
		Preview->Destroy();
	}

	PreviewActor.Reset();
}

void UADTrapPlacementComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	CancelPlacement();

	Super::EndPlay(EndPlayReason);
}

bool UADTrapPlacementComponent::IsPlacing() const
{
	return bIsPlacing;
}

bool UADTrapPlacementComponent::IsPlacementValid() const
{
	return bPlacementValid;
}