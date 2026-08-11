#include "Combat/ADCastComponent.h"

#include  "Engine/World.h"

UADCastComponent::UADCastComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UADCastComponent::StartCast(const FText& InCastName, float InCastDuration)
{
	if (InCastDuration <= 0.f) { return; }
	if (bIsCasting) { EndCast(true); }

	const UWorld* World = GetWorld();
	if (!IsValid(World)) { return; }

	bIsCasting = true;
	CastName = InCastName;
	CastDuration = InCastDuration;
	CastStartTime = World->GetTimeSeconds();

	OnCastStarted.Broadcast(CastName, CastDuration);
}

void UADCastComponent::EndCast(bool bWasCancelled)
{
	if (!bIsCasting) { return; }

	bIsCasting = false;

	OnCastEnded.Broadcast(bWasCancelled);

	CastStartTime = 0.f;
	CastDuration = 0.f;
	CastName = FText::GetEmpty();
}

bool UADCastComponent::IsCasting() const
{
	return bIsCasting;
}

float UADCastComponent::GetCastProgress() const
{
	if (!bIsCasting || CastDuration <= 0.f) { return 0.f; }

	const UWorld* World = GetWorld();
	if (!IsValid(World)) { return 0.f; }

	const float ElapsedTime =  World->GetTimeSeconds() - CastStartTime;

	return FMath::Clamp(
		ElapsedTime / CastDuration,
		0.f,
		1.f);
}

float UADCastComponent::GetCastTimeRemaining() const
{
	if (!bIsCasting) { return 0.f; }

	const UWorld* World = GetWorld();
	if (!IsValid(World)) { return 0.f; }

	const float ElapsedTime = World->GetTimeSeconds() - CastStartTime;

	return FMath::Max(
		CastDuration - ElapsedTime,
		0.f);
}

FText UADCastComponent::GetCastName() const
{
	return CastName;
}
