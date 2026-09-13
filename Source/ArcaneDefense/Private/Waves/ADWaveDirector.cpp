#include "Waves/ADWaveDirector.h"

#include "Game/ADGameMode.h"
#include "Characters/ADEnemyCharacter.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"
#include "Objective/ADDefenseObjective.h"
#include "TimerManager.h"
#include "Waves/ADSpawnPoint.h"
#include "Waves/ADWaveDataAsset.h"
#include "Waves/ADWaveTypes.h"

AADWaveDirector::AADWaveDirector()
{
	PrimaryActorTick.bCanEverTick = false;
}

void AADWaveDirector::BeginPlay()
{
	Super::BeginPlay();

	if (IsValid(DefenseObjective))
	{
		DefenseObjective->OnObjectiveDefeated.AddUniqueDynamic(
			this,
			&AADWaveDirector::HandleObjectiveDefeated);
	}

	if (bAutoStartFirstWave)
	{
		StartWave(0);
	}
}

void AADWaveDirector::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	GetWorldTimerManager().ClearTimer(SpawnTimerHandle);
	GetWorldTimerManager().ClearTimer(GroupDelayTimerHandle);
	GetWorldTimerManager().ClearTimer(NextWaveTimerHandle);

	Super::EndPlay(EndPlayReason);
}

void AADWaveDirector::StartWave(const int32 WaveIndex)
{
	if (bWaveSystemStopped)	{ return; }

	if (bWaveInProgress)
	{
		UE_LOG(
			LogTemp,
			Warning,
			TEXT(
				"Cannot start Wave %d because "
				"Wave %d is still in progress."
			),
			WaveIndex + 1,
			CurrentWaveIndex + 1
		);

		return;
	}

	if (!Waves.IsValidIndex(WaveIndex))
	{
		UE_LOG(
			LogTemp,
			Error,
			TEXT(
				"%s does not contain Wave %d."
			),
			*GetNameSafe(this),
			WaveIndex + 1
		);

		return;
	}

	UADWaveDataAsset* Wave = Waves[WaveIndex];
	if (!IsValid(Wave))
	{
		UE_LOG(
			LogTemp,
			Error,
			TEXT(
				"Wave %d is not configured."
			),
			WaveIndex + 1
		);

		return;
	}

	if (!IsValid(DefenseObjective))
	{
		UE_LOG(
			LogTemp,
			Error,
			TEXT(
				"%s has no Defense Objective."
			),
			*GetNameSafe(this)
		);

		return;
	}

	if (DefenseObjective->IsDefeated()) { return; }

	if (Wave->SpawnGroups.IsEmpty())
	{
		UE_LOG(
			LogTemp,
			Error,
			TEXT(
				"Wave %d contains no spawn groups."
			),
			WaveIndex + 1
		);

		return;
	}

	CurrentWaveIndex = WaveIndex;
	CurrentGroupIndex = 0;

	SpawnedEnemiesInCurrentGroup = 0;

	ActiveEnemies.Reset();

	bWaveInProgress = true;
	bIsSpawningWave = true;

	OnAliveEnemyCountChanged.Broadcast(0);

	OnWaveStarted.Broadcast(CurrentWaveIndex + 1);

	UE_LOG(
		LogTemp,
		Display,
		TEXT(
			"========== WAVE %d STARTED =========="
		),
		CurrentWaveIndex + 1
	);

	BeginCurrentGroup();
}

bool AADWaveDirector::IsSpawningWave() const
{
	return bIsSpawningWave;
}

void AADWaveDirector::BeginCurrentGroup()
{
	if (!Waves.IsValidIndex(CurrentWaveIndex))
	{
		FinishSpawningCurrentWave();
		return;
	}

	const UADWaveDataAsset* Wave =
		Waves[CurrentWaveIndex];

	if (!IsValid(Wave) || !Wave->SpawnGroups.IsValidIndex(CurrentGroupIndex))
	{
		FinishSpawningCurrentWave();
		return;
	}

	const FADWaveSpawnGroup& SpawnGroup = Wave->SpawnGroups[CurrentGroupIndex];

	if (!SpawnGroup.EnemyClass
		|| SpawnGroup.Quantity <= 0
		|| SpawnGroup.SpawnPointId.IsNone())
	{
		UE_LOG(
			LogTemp,
			Error,
			TEXT(
				"Wave %d Group %d contains invalid spawn data."
			),
			CurrentWaveIndex + 1,
			CurrentGroupIndex + 1
		);

		FinishCurrentGroup();
		return;
	}

	if (!IsValid(FindSpawnPoint(SpawnGroup.SpawnPointId)))
	{
		UE_LOG(
			LogTemp,
			Error,
			TEXT(
				"Spawn Point '%s' was not found."
			),
			*SpawnGroup.SpawnPointId.ToString()
		);

		FinishCurrentGroup();
		return;
	}

	SpawnedEnemiesInCurrentGroup = 0;

	// Spawn the first enemy immediately.
	SpawnNextEnemy();

	/*
	 * SpawnNextEnemy may already finish a group with Quantity == 1.
	 */
	if (SpawnedEnemiesInCurrentGroup >= SpawnGroup.Quantity) { return; }

	GetWorldTimerManager().SetTimer(
		SpawnTimerHandle,
		this,
		&AADWaveDirector::SpawnNextEnemy,
		SpawnGroup.SpawnInterval,
		true);
}

void AADWaveDirector::SpawnNextEnemy()
{
	if (!Waves.IsValidIndex(CurrentWaveIndex)) { return; }

	const UADWaveDataAsset* Wave = Waves[CurrentWaveIndex];
	if (!IsValid(Wave) || !Wave->SpawnGroups.IsValidIndex(CurrentGroupIndex)) {	return;	}

	const FADWaveSpawnGroup& SpawnGroup = Wave->SpawnGroups[CurrentGroupIndex];

	AADSpawnPoint* SpawnPoint = FindSpawnPoint(SpawnGroup.SpawnPointId);
	if (!IsValid(SpawnPoint))
	{
		GetWorldTimerManager().ClearTimer(SpawnTimerHandle);

		FinishCurrentGroup();
		return;
	}

	AADEnemyCharacter* Enemy = SpawnEnemy(SpawnGroup, *SpawnPoint);
	if (!IsValid(Enemy))
	{
		UE_LOG(
			LogTemp,
			Error,
			TEXT(
				"Wave %d Group %d failed to spawn an enemy. "
				"Aborting this spawn group."
			),
			CurrentWaveIndex + 1,
			CurrentGroupIndex + 1
		);

		GetWorldTimerManager().ClearTimer(SpawnTimerHandle);

		FinishCurrentGroup();

		return;
	}
	
	++SpawnedEnemiesInCurrentGroup;

	if (SpawnedEnemiesInCurrentGroup >= SpawnGroup.Quantity)
	{
		GetWorldTimerManager().ClearTimer(SpawnTimerHandle);

		FinishCurrentGroup();
	}
}

void AADWaveDirector::FinishCurrentGroup()
{
	if (!Waves.IsValidIndex(CurrentWaveIndex))
	{
		FinishSpawningCurrentWave();
		return;
	}

	const UADWaveDataAsset* Wave =
		Waves[CurrentWaveIndex];

	if (!IsValid(Wave) || !Wave->SpawnGroups.IsValidIndex(CurrentGroupIndex))
	{
		FinishSpawningCurrentWave();
		return;
	}

	const FADWaveSpawnGroup& FinishedGroup = Wave->SpawnGroups[CurrentGroupIndex];

	const bool bHasAnotherGroup = Wave->SpawnGroups.IsValidIndex(CurrentGroupIndex + 1);

	if (!bHasAnotherGroup)
	{
		FinishSpawningCurrentWave();
		return;
	}

	if (FinishedGroup.DelayAfterGroup <= 0.0f)
	{
		AdvanceToNextGroup();
		return;
	}

	GetWorldTimerManager().SetTimer(
		GroupDelayTimerHandle,
		this,
		&AADWaveDirector::AdvanceToNextGroup,
		FinishedGroup.DelayAfterGroup,
		false
	);
}

void AADWaveDirector::AdvanceToNextGroup()
{
	++CurrentGroupIndex;

	BeginCurrentGroup();
}

void AADWaveDirector::FinishSpawningCurrentWave()
{
	GetWorldTimerManager().ClearTimer(SpawnTimerHandle);
	GetWorldTimerManager().ClearTimer(GroupDelayTimerHandle);

	bIsSpawningWave = false;

	UE_LOG(
		LogTemp,
		Display,
		TEXT(
			"Wave %d has finished spawning. "
			"Alive enemies: %d."
		),
		CurrentWaveIndex + 1,
		ActiveEnemies.Num()
	);

	TryCompleteCurrentWave();
}

AADSpawnPoint* AADWaveDirector::FindSpawnPoint(const FName SpawnPointId) const
{
	for (AADSpawnPoint* SpawnPoint : SpawnPoints)
	{
		if (IsValid(SpawnPoint) && SpawnPoint->GetSpawnPointId() == SpawnPointId)
		{
			return SpawnPoint;
		}
	}

	return nullptr;
}

AADEnemyCharacter* AADWaveDirector::SpawnEnemy(
	const FADWaveSpawnGroup& SpawnGroup,
	const AADSpawnPoint& SpawnPoint)
{
	UWorld* World = GetWorld();

	if (!IsValid(World)
		|| !SpawnGroup.EnemyClass
		|| !IsValid(DefenseObjective))
	{
		return nullptr;
	}

	const FTransform SpawnTransform = SpawnPoint.GetActorTransform();

	AADEnemyCharacter* Enemy = World->SpawnActorDeferred<AADEnemyCharacter>(
		SpawnGroup.EnemyClass,
		SpawnTransform,
		this,
		nullptr,
		ESpawnActorCollisionHandlingMethod::
			AdjustIfPossibleButAlwaysSpawn
	);

	if (!IsValid(Enemy))
	{
		UE_LOG(
			LogTemp,
			Error,
			TEXT(
				"%s failed to spawn enemy class %s."
			),
			*GetNameSafe(this),
			*GetNameSafe(SpawnGroup.EnemyClass)
		);

		return nullptr;
	}

	/*
	 * Configure gameplay data BEFORE BeginPlay and AI possession.
	 */
	Enemy->SetMoveTarget(DefenseObjective);

	RegisterSpawnedEnemy(Enemy);
	
	UGameplayStatics::FinishSpawningActor(Enemy, SpawnTransform);

	return Enemy;
}

void AADWaveDirector::RegisterSpawnedEnemy(AADEnemyCharacter* Enemy)
{
	if (!IsValid(Enemy)) { return; }

	ActiveEnemies.Add(Enemy);

	Enemy->OnEnemyDefeated.AddUObject(this,	&AADWaveDirector::HandleEnemyDefeated);

	OnAliveEnemyCountChanged.Broadcast(ActiveEnemies.Num());

	UE_LOG(
		LogTemp,
		Display,
		TEXT("Enemy registered. Alive enemies: %d."),
		ActiveEnemies.Num()
	);
}

void AADWaveDirector::HandleEnemyDefeated(AADEnemyCharacter* Enemy)
{
	if (!Enemy) { return; }

	const TWeakObjectPtr<AADEnemyCharacter>
		EnemyReference(Enemy);

	const int32 RemovedCount = ActiveEnemies.Remove(EnemyReference);
	if (RemovedCount == 0) { return; }

	Enemy->OnEnemyDefeated.RemoveAll(this);

	OnAliveEnemyCountChanged.Broadcast(ActiveEnemies.Num());

	UE_LOG(
		LogTemp,
		Display,
		TEXT(
			"%s defeated. Alive enemies: %d."
		),
		*GetNameSafe(Enemy),
		ActiveEnemies.Num()
	);

	TryCompleteCurrentWave();
}

void AADWaveDirector::TryCompleteCurrentWave()
{
	if (bWaveSystemStopped
		|| !bWaveInProgress
		|| bIsSpawningWave
		|| !ActiveEnemies.IsEmpty())
	{
		return;
	}
	
	const int32 CompletedWaveNumber =CurrentWaveIndex + 1;

	bWaveInProgress = false;

	UE_LOG(
		LogTemp,
		Display,
		TEXT(
			"========== WAVE %d COMPLETED =========="
		),
		CompletedWaveNumber
	);

	OnWaveCompleted.Broadcast(CompletedWaveNumber);

	if (Waves.IsValidIndex(CurrentWaveIndex + 1))
	{
		ScheduleNextWave();
		return;
	}

	CompleteAllWaves();
}

void AADWaveDirector::ScheduleNextWave()
{
	if (DelayBetweenWaves <= 0.0f)
	{
		StartNextWave();
		return;
	}

	UE_LOG(
		LogTemp,
		Display,
		TEXT(
			"Next wave starts in %.1f seconds."
		),
		DelayBetweenWaves
	);

	GetWorldTimerManager().SetTimer(
		NextWaveTimerHandle,
		this,
		&AADWaveDirector::StartNextWave,
		DelayBetweenWaves,
		false
	);
}

void AADWaveDirector::StartNextWave()
{
	if (bWaveSystemStopped) { return; }

	StartWave(
		CurrentWaveIndex + 1
	);
}

void AADWaveDirector::CompleteAllWaves()
{
	if (bWaveSystemStopped
		|| !IsValid(DefenseObjective)
		|| DefenseObjective->IsDefeated())
	{
		return;
	}

	bWaveSystemStopped = true;

	UE_LOG(
		LogTemp,
		Display,
		TEXT(
			"========== ALL WAVES COMPLETED =========="
		)
	);

	UWorld* World = GetWorld();

	if (!IsValid(World))
	{
		return;
	}

	AADGameMode* GameMode =
		World->GetAuthGameMode<AADGameMode>();

	if (IsValid(GameMode))
	{
		GameMode->HandleGameVictory();
	}
}

void AADWaveDirector::HandleObjectiveDefeated()
{
	StopWaveSystem();
}

void AADWaveDirector::StopWaveSystem()
{
	if (bWaveSystemStopped) { return; }

	bWaveSystemStopped = true;

	bIsSpawningWave = false;
	bWaveInProgress = false;

	GetWorldTimerManager().ClearTimer(SpawnTimerHandle);
	GetWorldTimerManager().ClearTimer(GroupDelayTimerHandle);
	GetWorldTimerManager().ClearTimer(NextWaveTimerHandle);

	UE_LOG(LogTemp, Display, TEXT("Wave system stopped."));
}

bool AADWaveDirector::IsWaveInProgress() const
{
	return bWaveInProgress;
}

int32 AADWaveDirector::GetAliveEnemyCount() const
{
	return ActiveEnemies.Num();
}

int32 AADWaveDirector::GetCurrentWaveNumber() const
{
	return (CurrentWaveIndex == INDEX_NONE) ? 0 : CurrentWaveIndex + 1;
}