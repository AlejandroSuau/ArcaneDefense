#pragma once

#include "CoreMinimal.h"
#include "ActiveGameplayEffectHandle.h"
#include "Traps/ADTrapBase.h"
#include "ADSlowTrap.generated.h"

class AADEnemyCharacter;
class UGameplayEffect;

UCLASS(Blueprintable)
class ARCANEDEFENSE_API AADSlowTrap
	: public AADTrapBase
{
	GENERATED_BODY()

protected:
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	virtual void HandleEnemyEnteredTrigger(AADEnemyCharacter* Enemy) override;
	virtual void HandleEnemyExitedTrigger(AADEnemyCharacter* Enemy) override;

private:
	bool ApplySlowToEnemy(AADEnemyCharacter* Enemy);
	void RemoveSlowFromEnemy(AADEnemyCharacter* Enemy);

	void CleanupInvalidEffects();

	UPROPERTY(
		EditDefaultsOnly,
		BlueprintReadOnly,
		Category = "Trap|Slow",
		meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UGameplayEffect> SlowEffectClass;

	TMap<TWeakObjectPtr<AADEnemyCharacter>,	FActiveGameplayEffectHandle> ActiveSlowEffects;
};