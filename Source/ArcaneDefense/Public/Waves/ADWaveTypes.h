#pragma once

#include "CoreMinimal.h"
#include "ADWaveTypes.generated.h"

class AADEnemyCharacter;

/**
 * Describes one sequential group of enemies inside a wave.
 */
USTRUCT(BlueprintType)
struct ARCANEDEFENSE_API FADWaveSpawnGroup
{
	GENERATED_BODY()

	/**
	 * Enemy Blueprint or C++ class spawned by this group.
	 */
	UPROPERTY(
		EditAnywhere,
		BlueprintReadOnly,
		Category = "Wave")
	TSubclassOf<AADEnemyCharacter> EnemyClass;

	/**
	 * Number of enemies spawned by this group.
	 */
	UPROPERTY(
		EditAnywhere,
		BlueprintReadOnly,
		Category = "Wave",
		meta = (ClampMin = "1"))
	int32 Quantity = 1;

	/**
	 * Delay between individual enemy spawns.
	 */
	UPROPERTY(
		EditAnywhere,
		BlueprintReadOnly,
		Category = "Wave",
		meta = (ClampMin = "0.05", Units = "s"))
	float SpawnInterval = 1.0f;

	/**
	 * Logical identifier of the level Spawn Point to use.
	 */
	UPROPERTY(
		EditAnywhere,
		BlueprintReadOnly,
		Category = "Wave")
	FName SpawnPointId = NAME_None;

	/**
	 * Delay after this group has finished spawning.
	 */
	UPROPERTY(
		EditAnywhere,
		BlueprintReadOnly,
		Category = "Wave",
		meta = (ClampMin = "0.0", Units = "s"))
	float DelayAfterGroup = 0.0f;
};