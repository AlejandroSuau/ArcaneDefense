#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ADGroundTargetingComponent.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(
	FADGroundTargetConfirmedSignature,
	const FVector&
);

DECLARE_MULTICAST_DELEGATE(
	FADGroundTargetCancelledSignature
);

/**
 * Handles local-player ground targeting and preview placement.
 *
 * This component does not activate abilities. It only resolves and stores
 * a valid world location selected by the player.
 */
UCLASS(
	ClassGroup = (ArcaneDefense),
	meta = (BlueprintSpawnableComponent)
)
class ARCANEDEFENSE_API UADGroundTargetingComponent
	: public UActorComponent
{
	GENERATED_BODY()

public:
	UADGroundTargetingComponent();

	virtual void TickComponent(
		float DeltaTime,
		ELevelTick TickType,
		FActorComponentTickFunction* ThisTickFunction
	) override;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	void StartTargeting(
		float InMaxRange,
		float InPreviewRadius,
		TSubclassOf<AActor> InPreviewActorClass
	);

	bool ConfirmTargeting();
	void CancelTargeting();
	bool IsTargeting() const;

	bool HasConfirmedLocation() const;
	bool ConsumeConfirmedLocation(FVector& OutLocation);
	void ClearConfirmedLocation();

	FADGroundTargetConfirmedSignature OnTargetConfirmed;
	FADGroundTargetCancelledSignature OnTargetCancelled;

private:
	void UpdateTargeting();
	void StopTargetingVisuals();
	void SetPreviewVisible(bool bVisible);
	bool IsLocationValid(const FHitResult& HitResult) const;

	UPROPERTY(
		EditDefaultsOnly,
		Category = "Ground Targeting",
		meta = (ClampMin = "0.0", ClampMax = "1.0"))
	float MinimumGroundNormalZ = 0.6f;

	float MaxRange = 0.0f;
	float PreviewRadius = 0.0f;

	bool bIsTargeting = false;

	bool bCurrentLocationValid = false;
	bool bHasConfirmedLocation = false;

	FVector CurrentLocation = FVector::ZeroVector;
	FVector ConfirmedLocation = FVector::ZeroVector;

	TSubclassOf<AActor> PreviewActorClass;
	TWeakObjectPtr<AActor> PreviewActor;
};