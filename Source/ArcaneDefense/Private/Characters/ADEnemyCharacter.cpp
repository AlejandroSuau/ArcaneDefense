#include "Characters/ADEnemyCharacter.h"

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

