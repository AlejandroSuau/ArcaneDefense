#include "Waves/ADWaveDirector.h"

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

	if (bAutoStartFirstWave)
	{
		StartWave(0);
	}
}

void AADWaveDirector::StartWave(
	const int32 WaveIndex
)
{
	if (bIsSpawningWave)
	{
		UE_LOG(
			LogTemp,
			Warning,
			TEXT(
				"%s cannot start Wave %d because another wave "
				"is currently spawning."
			),
			*GetNameSafe(this),
			WaveIndex
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
			WaveIndex
		);

		return;
	}

	if (!IsValid(Waves[WaveIndex]))
	{
		UE_LOG(
			LogTemp,
			Error,
			TEXT(
				"Wave %d is not configured on %s."
			),
			WaveIndex,
			*GetNameSafe(this)
		);

		return;
	}

	if (!IsValid(DefenseObjective))
	{
		UE_LOG(
			LogTemp,
			Error,
			TEXT(
				"%s does not have a Defense Objective."
			),
			*GetNameSafe(this)
		);

		return;
	}

	if (Waves[WaveIndex]->SpawnGroups.IsEmpty())
	{
		UE_LOG(
			LogTemp,
			Warning,
			TEXT(
				"Wave %d does not contain any spawn groups."
			),
			WaveIndex
		);

		return;
	}

	CurrentWaveIndex = WaveIndex;
	CurrentGroupIndex = 0;
	SpawnedEnemiesInCurrentGroup = 0;
	bIsSpawningWave = true;

	UE_LOG(
		LogTemp,
		Display,
		TEXT("Starting Wave %d."),
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
		GetWorldTimerManager().ClearTimer(
			SpawnTimerHandle
		);

		FinishCurrentGroup();
		return;
	}

	AADEnemyCharacter* Enemy = SpawnEnemy(SpawnGroup, *SpawnPoint);

	if (IsValid(Enemy))
	{
		++SpawnedEnemiesInCurrentGroup;

		UE_LOG(
			LogTemp,
			Display,
			TEXT(
				"Wave %d Group %d spawned %s (%d/%d)."
			),
			CurrentWaveIndex + 1,
			CurrentGroupIndex + 1,
			*GetNameSafe(Enemy),
			SpawnedEnemiesInCurrentGroup,
			SpawnGroup.Quantity
		);
	}

	if (SpawnedEnemiesInCurrentGroup >= SpawnGroup.Quantity)
	{
		GetWorldTimerManager().ClearTimer(
			SpawnTimerHandle
		);

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
	GetWorldTimerManager().ClearTimer(
		SpawnTimerHandle
	);

	GetWorldTimerManager().ClearTimer(
		GroupDelayTimerHandle
	);

	UE_LOG(
		LogTemp,
		Display,
		TEXT(
			"Wave %d has finished spawning."
		),
		CurrentWaveIndex + 1
	);

	bIsSpawningWave = false;
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

	UGameplayStatics::FinishSpawningActor(Enemy, SpawnTransform);

	return Enemy;
}