// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "ADPlayerController.generated.h"

class  UUserWidget;

/**
 * Player controller used by the local Arcane Defense player.
 */
UCLASS()
class ARCANEDEFENSE_API AADPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	AADPlayerController();

protected:
	virtual void BeginPlay() override;

private:
	/** Root widget created for the local player. */
	UPROPERTY(
		EditDefaultsOnly,
		BlueprintReadOnly,
		Category = "UI",
		meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UUserWidget> PlayerHUDClass;

	UPROPERTY(Transient)
	TObjectPtr<UUserWidget> PlayerHUDWidget;
};
