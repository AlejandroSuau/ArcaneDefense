#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ADSpawnPoint.generated.h"

class UArrowComponent;
class USceneComponent;

/**
 * Defines a named location from which enemies can be spawned.
 */
UCLASS()
class ARCANEDEFENSE_API AADSpawnPoint : public AActor
{
	GENERATED_BODY()

public:
	AADSpawnPoint();

	UFUNCTION(BlueprintPure, Category = "Wave")
	FName GetSpawnPointId() const;

private:
	UPROPERTY(
		VisibleAnywhere,
		Category = "Components"
	)
	TObjectPtr<USceneComponent> SceneRoot;

	UPROPERTY(
		VisibleAnywhere,
		Category = "Components"
	)
	TObjectPtr<UArrowComponent> DirectionArrow;

	/**
	 * Identifier used by Wave Data Assets.
	 */
	UPROPERTY(
		EditInstanceOnly,
		BlueprintReadOnly,
		Category = "Wave",
		meta = (AllowPrivateAccess = "true")
	)
	FName SpawnPointId = NAME_None;
};