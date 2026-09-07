#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "ADGameMode.generated.h"

/**
 * Defines the high-level rules of an Arcane Defense match.
 */
UCLASS()
class ARCANEDEFENSE_API AADGameMode
	: public AGameModeBase
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "Game")
	void HandleGameDefeat();

	UFUNCTION(BlueprintPure, Category = "Game")
	bool IsGameDefeated() const;

protected:
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
	bool bGameDefeated = false;
};