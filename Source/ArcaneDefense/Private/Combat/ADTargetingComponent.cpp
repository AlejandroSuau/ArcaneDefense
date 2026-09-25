#include "Combat/ADTargetingComponent.h"

#include "Characters/ADEnemyCharacter.h"
#include "Engine/EngineTypes.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/PlayerController.h"

UADTargetingComponent::UADTargetingComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

bool UADTargetingComponent::TrySelectTargetUnderCursor()
{
	const APawn* PawnOwner = Cast<APawn>(GetOwner());
	if (!ensureMsgf(
		IsValid(PawnOwner),
		TEXT("%s must be owned by a Pawn."),
		*GetNameSafe(this)))
	{
		return false;
	}

	APlayerController* PlayerController =
		Cast<APlayerController>(PawnOwner->GetController());
	if (!ensureMsgf(
		IsValid(PlayerController),
		TEXT("%s requires a Player Controller"),
		*GetNameSafe(this)))
	{
		return false;
	}

	FHitResult HitResult;
	const ETraceTypeQuery TraceChannel = UEngineTypes::ConvertToTraceType(ECC_Visibility);
	const bool bHitSomething = PlayerController->GetHitResultUnderCursorByChannel(
		TraceChannel,
		true,
		HitResult);
	if (!bHitSomething)
	{
		ClearTarget();
		return false;
	}

	AADEnemyCharacter* HitEnemy = Cast<AADEnemyCharacter>(HitResult.GetActor());
	if (!IsValidTargetCandidate(HitEnemy))
	{
		ClearTarget();
		return false;
	}

	SetCurrentTarget(HitEnemy);
	return true;
}

void UADTargetingComponent::ClearTarget()
{
	SetCurrentTarget(nullptr);
}

AADEnemyCharacter* UADTargetingComponent::GetCurrentTarget() const
{
	return CurrentTarget.Get();
}

bool UADTargetingComponent::HasValidTarget() const
{
	return CurrentTarget.IsValid();
}

void UADTargetingComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if (AADEnemyCharacter* Target = CurrentTarget.Get())
	{
		Target->OnDestroyed.RemoveDynamic(
			this,
			&UADTargetingComponent::HandleCurrentTargetDestroyed);
		Target->SetTargeted(false);
	}

	CurrentTarget.Reset();

	Super::EndPlay(EndPlayReason);
}

bool UADTargetingComponent::IsValidTargetCandidate(const AADEnemyCharacter* Candidate) const
{
	if (!IsValid(Candidate)) { return false; }

	const AActor* OwnerActor = GetOwner();
	if (!IsValid(OwnerActor)) { return false; }

	const float DistanceSquared = FVector::DistSquared(
		OwnerActor->GetActorLocation(),
		Candidate->GetActorLocation());
	return DistanceSquared <= FMath::Square(MaxSelectionDistance);
}

void UADTargetingComponent::SetCurrentTarget(AADEnemyCharacter* NewTarget)
{
	AADEnemyCharacter* PreviousTarget = CurrentTarget.Get();
	if (PreviousTarget == NewTarget) { return; }

	if (IsValid(PreviousTarget))
	{
		PreviousTarget->OnDestroyed.RemoveDynamic(
			this,
			&UADTargetingComponent::HandleCurrentTargetDestroyed);
		PreviousTarget->SetTargeted(false);
	}

	CurrentTarget = NewTarget;
	
	if (IsValid(NewTarget))
	{
		NewTarget->OnDestroyed.AddUniqueDynamic(
			this,
			&UADTargetingComponent::HandleCurrentTargetDestroyed);
		NewTarget->SetTargeted(true);
	}

	UE_LOG(
		LogTemp,
		Display,
		TEXT("Target changed from %s to %s."),
		*GetNameSafe(PreviousTarget),
		*GetNameSafe(NewTarget));
	
	OnTargetChanged.Broadcast(PreviousTarget, NewTarget);
}

void UADTargetingComponent::HandleCurrentTargetDestroyed(AActor* DestroyedActor)
{
	AActor* PreviousTarget = DestroyedActor;
	CurrentTarget.Reset();

	UE_LOG(
		LogTemp,
		Display,
		TEXT("Selected target %s was destroyed."),
		*GetNameSafe(DestroyedActor));
	OnTargetChanged.Broadcast(PreviousTarget, nullptr);
}


