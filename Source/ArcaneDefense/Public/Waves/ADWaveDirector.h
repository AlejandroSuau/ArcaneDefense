#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ADWaveDirector.generated.h"

class AADDefenseObjective;
class AADEnemyCharacter;
class AADSpawnPoint;
class UADWaveDataAsset;

struct FADWaveSpawnGroup;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(
	FADWaveStartedSignature,
	int32,
	WaveNumber
);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(
	FADWaveCompletedSignature,
	int32,
	WaveNumber
);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(
	FADAliveEnemyCountChangedSignature,
	int32,
	AliveEnemyCount
);

/**
 * Executes and tracks the configured enemy waves for the current match.
 */
UCLASS()
class ARCANEDEFENSE_API AADWaveDirector : public AActor
{
	GENERATED_BODY()

public:
	AADWaveDirector();

	/**
	 * Starts the requested zero-based wave index.
	 */
	UFUNCTION(BlueprintCallable, Category = "Waves")
	void StartWave(int32 WaveIndex);

	UFUNCTION(BlueprintPure, Category = "Waves")
	bool IsSpawningWave() const;

	UFUNCTION(BlueprintPure, Category = "Waves")
	bool IsWaveInProgress() const;

	UFUNCTION(BlueprintPure, Category = "Waves")
	int32 GetAliveEnemyCount() const;

	UFUNCTION(BlueprintPure, Category = "Waves")
	int32 GetCurrentWaveNumber() const;

	UPROPERTY(BlueprintAssignable, Category = "Waves")
	FADWaveStartedSignature OnWaveStarted;

	UPROPERTY(BlueprintAssignable, Category = "Waves")
	FADWaveCompletedSignature OnWaveCompleted;

	UPROPERTY(BlueprintAssignable, Category = "Waves")
	FADAliveEnemyCountChangedSignature
		OnAliveEnemyCountChanged;

protected:
	virtual void BeginPlay() override;

	virtual void EndPlay(
		const EEndPlayReason::Type EndPlayReason
	) override;

private:
	void BeginCurrentGroup();

	void SpawnNextEnemy();

	void FinishCurrentGroup();

	void AdvanceToNextGroup();

	void FinishSpawningCurrentWave();

	void RegisterSpawnedEnemy(AADEnemyCharacter* Enemy);

	void HandleEnemyDefeated(
		AADEnemyCharacter* Enemy
	);

	void TryCompleteCurrentWave();

	void ScheduleNextWave();

	void StartNextWave();

	void CompleteAllWaves();

	void StopWaveSystem();

	UFUNCTION()
	void HandleObjectiveDefeated();

	AADSpawnPoint* FindSpawnPoint(
		FName SpawnPointId
	) const;

	AADEnemyCharacter* SpawnEnemy(
		const FADWaveSpawnGroup& SpawnGroup,
		const AADSpawnPoint& SpawnPoint
	);

	UPROPERTY(
		EditInstanceOnly,
		BlueprintReadOnly,
		Category = "Waves",
		meta = (AllowPrivateAccess = "true")
	)
	TArray<TObjectPtr<UADWaveDataAsset>> Waves;

	UPROPERTY(
		EditInstanceOnly,
		BlueprintReadOnly,
		Category = "Waves",
		meta = (AllowPrivateAccess = "true")
	)
	TArray<TObjectPtr<AADSpawnPoint>> SpawnPoints;

	UPROPERTY(
		EditInstanceOnly,
		BlueprintReadOnly,
		Category = "Waves",
		meta = (AllowPrivateAccess = "true")
	)
	TObjectPtr<AADDefenseObjective> DefenseObjective;

	UPROPERTY(
		EditAnywhere,
		BlueprintReadOnly,
		Category = "Waves",
		meta = (AllowPrivateAccess = "true")
	)
	bool bAutoStartFirstWave = true;

	/**
	 * Preparation delay after completing one wave.
	 */
	UPROPERTY(
		EditAnywhere,
		BlueprintReadOnly,
		Category = "Waves",
		meta = (
			AllowPrivateAccess = "true",
			ClampMin = "0.0",
			Units = "s"
		)
	)
	float DelayBetweenWaves = 3.0f;

	int32 CurrentWaveIndex = INDEX_NONE;

	int32 CurrentGroupIndex = INDEX_NONE;

	int32 SpawnedEnemiesInCurrentGroup = 0;

	bool bIsSpawningWave = false;

	bool bWaveInProgress = false;

	bool bWaveSystemStopped = false;

	/**
	 * Non-owning references to living enemies belonging to the
	 * currently active wave.
	 */
	TSet<TWeakObjectPtr<AADEnemyCharacter>> ActiveEnemies;

	FTimerHandle SpawnTimerHandle;

	FTimerHandle GroupDelayTimerHandle;

	FTimerHandle NextWaveTimerHandle;
};