#include "AI/ADEnemyAIController.h"

#include "Characters/ADEnemyCharacter.h"
#include "Navigation/PathFollowingComponent.h"
#include "TimerManager.h"

void AADEnemyAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	AADEnemyCharacter* EnemyCharacter = Cast<AADEnemyCharacter>(InPawn);
	if (!ensureMsgf(
		IsValid(EnemyCharacter),
		TEXT("%s can only possess an ADEnemyCharacter."),
		*GetNameSafe(this)))
	{
		return;
	}
	
	MoveTarget = EnemyCharacter->GetMoveTarget();

	// TODO: Don't rely on frame timing, wait for navigation initialization
	// and use OnNavigationInitDone events, etc.
	GetWorldTimerManager().SetTimerForNextTick(
		this,
		&AADEnemyAIController::MoveToCurrentTarget
	);
}

void AADEnemyAIController::SetMoveTarget(AActor* NewMoveTarget)
{
	MoveTarget = NewMoveTarget;
	if (!MoveTarget.IsValid())
	{
		StopMovement();

		UE_LOG(
			LogTemp,
			Warning,
			TEXT("%s does not have a valid movement target."),
			*GetNameSafe(this));
		
		return;
	}

	MoveToCurrentTarget();
}

void AADEnemyAIController::MoveToCurrentTarget()
{
	AActor* Target = MoveTarget.Get();
	if (!IsValid(Target)) { return; }

	const EPathFollowingRequestResult::Type RequestResult = MoveToActor(
		Target,
		AcceptanceRadius,
		true,
		true,
		false,
		nullptr,
		true);

	switch (RequestResult)
	{
	case EPathFollowingRequestResult::RequestSuccessful:
		UE_LOG(
			LogTemp,
			Display,
			TEXT("%s started moving toward %s."),
			*GetNameSafe(GetPawn()),
			*GetNameSafe(Target)
		);
		break;
	case EPathFollowingRequestResult::AlreadyAtGoal:
		{
			UE_LOG(
				LogTemp,
				Display,
				TEXT("%s is already at %s."),
				*GetNameSafe(GetPawn()),
				*GetNameSafe(Target)
			);

			AADEnemyCharacter* Enemy = Cast<AADEnemyCharacter>(GetPawn());
			if (IsValid(Enemy)) { Enemy->HandleReachedMoveTarget(); }
			break;
		}
	case EPathFollowingRequestResult::Failed:
	default:
		UE_LOG(
			LogTemp,
			Warning,
			TEXT("%s failed to request movement toward %s."),
			*GetNameSafe(GetPawn()),
			*GetNameSafe(Target)
		);
		break;
	}
}

void AADEnemyAIController::OnMoveCompleted(
	FAIRequestID RequestID,
	const FPathFollowingResult& Result)
{
	Super::OnMoveCompleted(RequestID, Result);

	UE_LOG(
		LogTemp,
		Display,
		TEXT("%s finished its movement request. Result: %s."),
		*GetNameSafe(GetPawn()),
		*Result.ToString()
	);

	if (!Result.IsSuccess()) { return; }

	AADEnemyCharacter* Enemy = Cast<AADEnemyCharacter>(GetPawn());
	if (IsValid(Enemy))
	{
		Enemy->HandleReachedMoveTarget();
	}
}
