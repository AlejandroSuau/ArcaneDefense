#include "Game/ADGameMode.h"

void AADGameMode::HandleGameDefeat()
{
	if (bGameDefeated) { return; }

	bGameDefeated = true;

	UE_LOG(
		LogTemp,
		Warning,
		TEXT("GAME DEFEAT: The defense objective was destroyed.")
	);

	ReceiveGameDefeat();
}

bool AADGameMode::IsGameDefeated() const
{
	return bGameDefeated;
}