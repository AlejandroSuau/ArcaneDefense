#include "Game/ADGameMode.h"

void AADGameMode::HandleGameVictory()
{
	if (GameResult != EADGameResult::InProgress) { return; }

	GameResult = EADGameResult::Victory;

	UE_LOG(
		LogTemp,
		Display,
		TEXT(
			"GAME VICTORY: All configured waves were completed."
		)
	);

	OnGameResultChanged.Broadcast(GameResult);

	ReceiveGameVictory();
}

void AADGameMode::HandleGameDefeat()
{
	if (GameResult != EADGameResult::InProgress) { return;	}

	GameResult = EADGameResult::Defeat;

	UE_LOG(
		LogTemp,
		Warning,
		TEXT("GAME DEFEAT: The defense objective was destroyed.")
	);

	OnGameResultChanged.Broadcast(GameResult);

	ReceiveGameDefeat();
}

EADGameResult AADGameMode::GetGameResult() const
{
	return GameResult;
}

bool AADGameMode::IsGameOver() const
{
	return GameResult != EADGameResult::InProgress;
}

bool AADGameMode::IsGameDefeated() const
{
	return GameResult == EADGameResult::Defeat;
}

bool AADGameMode::IsGameVictorious() const
{
	return GameResult == EADGameResult::Victory;
}