#include "Characters/ADEnemyCharacter.h"

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

