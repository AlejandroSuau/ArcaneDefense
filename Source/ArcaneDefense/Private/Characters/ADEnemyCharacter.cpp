#include "Characters/ADEnemyCharacter.h"

#include "AI/ADEnemyAIController.h"

AADEnemyCharacter::AADEnemyCharacter()
{
	AIControllerClass = AADEnemyAIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
}

void AADEnemyCharacter::HandleDeath()
{
	if (IsDead())
	{
		return;
	}

	SetTargeted(false);
	
	Super::HandleDeath();

	// Give Blueprint presentation a brief moment before destruction.
	SetLifeSpan(0.75f);
}

void AADEnemyCharacter::SetTargeted(const bool bNewTargeted)
{
	if (bIsTargeted == bNewTargeted)
	{
		return;
	}

	bIsTargeted = bNewTargeted;
	ReceiveTargetedStateChanged(bIsTargeted);
}

bool AADEnemyCharacter::IsTargeted() const
{
	return  bIsTargeted;
}

void AADEnemyCharacter::SetMoveTarget(AActor* NewMoveTarget)
{
	MoveTarget = NewMoveTarget;
	AADEnemyAIController* EnemyController = Cast<AADEnemyAIController>(GetController());
	if (IsValid(EnemyController))
	{
		EnemyController->SetMoveTarget(MoveTarget);
	}
}

AActor* AADEnemyCharacter::GetMoveTarget() const
{
	return MoveTarget;
}
