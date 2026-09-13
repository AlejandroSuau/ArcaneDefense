#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "ADGameMode.generated.h"

UENUM(BlueprintType)
enum class EADGameResult : uint8
{
	InProgress,
	Victory,
	Defeat
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(
	FADGameResultChangedSignature,
	EADGameResult,
	NewResult
);

/**
 * Defines the high-level rules and final result of an
 * Arcane Defense match.
 */
UCLASS()
class ARCANEDEFENSE_API AADGameMode
	: public AGameModeBase
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "Game")
	void HandleGameVictory();

	UFUNCTION(BlueprintCallable, Category = "Game")
	void HandleGameDefeat();

	UFUNCTION(BlueprintPure, Category = "Game")
	EADGameResult GetGameResult() const;

	UFUNCTION(BlueprintPure, Category = "Game")
	bool IsGameOver() const;

	UFUNCTION(BlueprintPure, Category = "Game")
	bool IsGameDefeated() const;

	UFUNCTION(BlueprintPure, Category = "Game")
	bool IsGameVictorious() const;

	UPROPERTY(BlueprintAssignable, Category = "Game")
	FADGameResultChangedSignature OnGameResultChanged;

protected:
	UFUNCTION(
		BlueprintImplementableEvent,
		Category = "Game",
		meta = (DisplayName = "Game Victory")
	)
	void ReceiveGameVictory();

	UFUNCTION(
		BlueprintImplementableEvent,
		Category = "Game",
		meta = (DisplayName = "Game Defeat")
	)
	void ReceiveGameDefeat();

private:
	UPROPERTY(
		VisibleInstanceOnly,
		BlueprintReadOnly,
		Category = "Game",
		meta = (AllowPrivateAccess = "true")
	)
	EADGameResult GameResult =
		EADGameResult::InProgress;
};