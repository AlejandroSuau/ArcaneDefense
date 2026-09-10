#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Waves/ADWaveTypes.h"
#include "ADWaveDataAsset.generated.h"

/**
 * Data-driven definition of a single enemy wave.
 */
UCLASS(BlueprintType)
class ARCANEDEFENSE_API UADWaveDataAsset
	: public UDataAsset
{
	GENERATED_BODY()

public:
	/**
	 * Groups are executed sequentially in array order.
	 */
	UPROPERTY(
		EditAnywhere,
		BlueprintReadOnly,
		Category = "Wave"
	)
	TArray<FADWaveSpawnGroup> SpawnGroups;
};