#include "Combat/ADGroundTargetingComponent.h"

#include "Engine/EngineTypes.h"
#include "Engine/World.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/PlayerController.h"

UADGroundTargetingComponent::UADGroundTargetingComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.bStartWithTickEnabled = false;
}

void UADGroundTargetingComponent::TickComponent(
	const float DeltaTime,
	const ELevelTick TickType,
	FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
	if (!bIsTargeting) { return; }

	UpdateTargeting();
}

void UADGroundTargetingComponent::StartTargeting(
	const float InMaxRange,
	const float InPreviewRadius,
	TSubclassOf<AActor> InPreviewActorClass)
{
	CancelTargeting();

	MaxRange = FMath::Max(0.0f, InMaxRange);
	PreviewRadius =	FMath::Max(0.0f, InPreviewRadius);
	PreviewActorClass =	InPreviewActorClass;

	bHasConfirmedLocation = false;
	bCurrentLocationValid = false;

	if (MaxRange <= 0.0f || !PreviewActorClass) { return; }

	UWorld* World = GetWorld();
	AActor* Owner = GetOwner();
	if (!IsValid(World) || !IsValid(Owner))	{ return; }

	FActorSpawnParameters SpawnParameters;

	SpawnParameters.Owner = Owner;
	SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	AActor* SpawnedPreview = World->SpawnActor<AActor>(
		PreviewActorClass,
		Owner->GetActorLocation(),
		FRotator::ZeroRotator,
		SpawnParameters);
	if (!IsValid(SpawnedPreview)) { return; }

	PreviewActor = SpawnedPreview;

	/*
	 * BP_ADGroundTargetPreview is authored with a 100 cm visual radius.
	 */
	const float PreviewScale = PreviewRadius / 100.0f;

	SpawnedPreview->SetActorScale3D(FVector(PreviewScale));
	SpawnedPreview->SetActorHiddenInGame(true);

	bIsTargeting = true;

	SetComponentTickEnabled(true);

	// Update immediately so the player does not wait one frame.
	UpdateTargeting();
}

bool UADGroundTargetingComponent::ConfirmTargeting()
{
	if (!bIsTargeting || !bCurrentLocationValid) { return false; }

	ConfirmedLocation = CurrentLocation;

	bHasConfirmedLocation = true;

	const FVector ConfirmedCopy = ConfirmedLocation;

	StopTargetingVisuals();

	OnTargetConfirmed.Broadcast(ConfirmedCopy);

	return true;
}

void UADGroundTargetingComponent::CancelTargeting()
{
	const bool bWasTargeting = bIsTargeting;

	bHasConfirmedLocation = false;

	StopTargetingVisuals();

	if (bWasTargeting)
	{
		OnTargetCancelled.Broadcast();
	}
}

bool UADGroundTargetingComponent::IsTargeting() const
{
	return bIsTargeting;
}

bool UADGroundTargetingComponent::HasConfirmedLocation() const
{
	return bHasConfirmedLocation;
}

bool UADGroundTargetingComponent::ConsumeConfirmedLocation(FVector& OutLocation)
{
	if (!bHasConfirmedLocation)	{ return false;	}

	OutLocation = ConfirmedLocation;

	bHasConfirmedLocation = false;

	ConfirmedLocation = FVector::ZeroVector;

	return true;
}

void UADGroundTargetingComponent::ClearConfirmedLocation()
{
	bHasConfirmedLocation = false;

	ConfirmedLocation =	FVector::ZeroVector;
}

void UADGroundTargetingComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	StopTargetingVisuals();

	Super::EndPlay(EndPlayReason);
}

void UADGroundTargetingComponent::UpdateTargeting()
{
	APawn* OwnerPawn =
		Cast<APawn>(
			GetOwner()
		);

	APlayerController* PlayerController =
		IsValid(OwnerPawn)
			? Cast<APlayerController>(
				OwnerPawn->GetController()
			)
			: nullptr;

	if (!IsValid(PlayerController))
	{
		bCurrentLocationValid = false;

		SetPreviewVisible(false);

		return;
	}

	TArray<TEnumAsByte<EObjectTypeQuery>>
		GroundObjectTypes;

	GroundObjectTypes.Add(
		UEngineTypes::ConvertToObjectType(
			ECC_WorldStatic
		)
	);

	FHitResult HitResult;

	const bool bHit =
		PlayerController->
			GetHitResultUnderCursorForObjects(
				GroundObjectTypes,
				false,
				HitResult
			);

	if (!bHit)
	{
		bCurrentLocationValid = false;

		SetPreviewVisible(false);

		return;
	}

	CurrentLocation =
		HitResult.ImpactPoint;

	bCurrentLocationValid =
		IsLocationValid(
			HitResult
		);

	AActor* CurrentPreview =
		PreviewActor.Get();

	if (!IsValid(CurrentPreview))
	{
		return;
	}

	CurrentPreview->SetActorLocation(
		HitResult.ImpactPoint
		+
		HitResult.ImpactNormal * 2.0f
	);

	SetPreviewVisible(
		bCurrentLocationValid
	);
}

bool UADGroundTargetingComponent::IsLocationValid(
	const FHitResult& HitResult
) const
{
	const AActor* Owner =
		GetOwner();

	if (!IsValid(Owner))
	{
		return false;
	}

	/*
	 * Reject steep or vertical surfaces such as walls.
	 */
	if (HitResult.ImpactNormal.Z
		< MinimumGroundNormalZ)
	{
		return false;
	}

	const float DistanceSquared =
		FVector::DistSquared(
			Owner->GetActorLocation(),
			HitResult.ImpactPoint
		);

	return DistanceSquared
		<= FMath::Square(MaxRange);
}

void UADGroundTargetingComponent::StopTargetingVisuals()
{
	bIsTargeting = false;

	bCurrentLocationValid = false;

	SetComponentTickEnabled(false);

	if (AActor* CurrentPreview = PreviewActor.Get())
	{
		CurrentPreview->Destroy();
	}

	PreviewActor.Reset();

	PreviewActorClass = nullptr;
}

void UADGroundTargetingComponent::SetPreviewVisible(const bool bVisible)
{
	if (AActor* CurrentPreview = PreviewActor.Get())
	{
		CurrentPreview->SetActorHiddenInGame(!bVisible);
	}
}