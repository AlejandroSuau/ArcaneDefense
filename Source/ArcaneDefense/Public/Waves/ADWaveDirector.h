#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ADWaveDirector.generated.h"

class AADDefenseObjective;
class AADEnemyCharacter;
class AADSpawnPoint;
class UADWaveDataAsset;

struct FADWaveSpawnGroup;

/**
 * Executes the configured enemy waves for the current match.
 */
UCLASS()
class ARCANEDEFENSE_API AADWaveDirector : public AActor
{
	GENERATED_BODY()

public:
	AADWaveDirector();

	/**
	 * Starts the requested wave.
	 */
	UFUNCTION(BlueprintCallable, Category = "Waves")
	void StartWave(int32 WaveIndex);

	UFUNCTION(BlueprintPure, Category = "Waves")
	bool IsSpawningWave() const;

protected:
	virtual void BeginPlay() override;

private:
	void BeginCurrentGroup();

	void SpawnNextEnemy();

	void FinishCurrentGroup();

	void AdvanceToNextGroup();

	void FinishSpawningCurrentWave();

	AADSpawnPoint* FindSpawnPoint(
		FName SpawnPointId
	) const;

	AADEnemyCharacter* SpawnEnemy(
		const FADWaveSpawnGroup& SpawnGroup,
		const AADSpawnPoint& SpawnPoint
	);

	/**
	 * Wave definitions available to this match.
	 */
	UPROPERTY(
		EditInstanceOnly,
		BlueprintReadOnly,
		Category = "Waves",
		meta = (AllowPrivateAccess = "true")
	)
	TArray<TObjectPtr<UADWaveDataAsset>> Waves;

	/**
	 * Level Spawn Points available to the director.
	 */
	UPROPERTY(
		EditInstanceOnly,
		BlueprintReadOnly,
		Category = "Waves",
		meta = (AllowPrivateAccess = "true")
	)
	TArray<TObjectPtr<AADSpawnPoint>> SpawnPoints;

	/**
	 * Actor every spawned enemy should navigate toward.
	 */
	UPROPERTY(
		EditInstanceOnly,
		BlueprintReadOnly,
		Category = "Waves",
		meta = (AllowPrivateAccess = "true")
	)
	TObjectPtr<AADDefenseObjective> DefenseObjective;

	/**
	 * Automatically starts Wave 0 when Play begins.
	 *
	 * Useful for the current prototype.
	 */
	UPROPERTY(
		EditAnywhere,
		BlueprintReadOnly,
		Category = "Waves",
		meta = (AllowPrivateAccess = "true")
	)
	bool bAutoStartFirstWave = true;

	int32 CurrentWaveIndex = INDEX_NONE;
	int32 CurrentGroupIndex = INDEX_NONE;

	int32 SpawnedEnemiesInCurrentGroup = 0;

	bool bIsSpawningWave = false;

	FTimerHandle SpawnTimerHandle;
	FTimerHandle GroupDelayTimerHandle;
};