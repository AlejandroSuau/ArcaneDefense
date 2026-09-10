#include "Waves/ADSpawnPoint.h"

#include "Components/ArrowComponent.h"
#include "Components/SceneComponent.h"

AADSpawnPoint::AADSpawnPoint()
{
	PrimaryActorTick.bCanEverTick = false;

	SceneRoot =
		CreateDefaultSubobject<USceneComponent>(
			TEXT("SceneRoot")
		);

	SetRootComponent(SceneRoot);

	DirectionArrow =
		CreateDefaultSubobject<UArrowComponent>(
			TEXT("DirectionArrow")
		);

	DirectionArrow->SetupAttachment(SceneRoot);
}

FName AADSpawnPoint::GetSpawnPointId() const
{
	return SpawnPointId;
}