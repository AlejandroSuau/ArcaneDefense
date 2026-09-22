#pragma once

#include "CoreMinimal.h"
#include "Traps/ADTrapBase.h"
#include "ADSpikeTrap.generated.h"

class AADEnemyCharacter;
class UGameplayEffect;

/**
 * Concrete floor trap that damages every valid enemy currently
 * inside the trap activation volume.
 *
 * Rearming is specific to this trap type and is not part of
 * the generic trap base class.
 */
UCLASS(Blueprintable)
class ARCANEDEFENSE_API AADSpikeTrap
	: public AADTrapBase
{
	GENERATED_BODY()

public:
	AADSpikeTrap();

	UFUNCTION(
		BlueprintPure,
		Category = "Trap|Spike")
	bool IsArmed() const;

protected:
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	virtual void HandleEnemyEnteredTrigger(AADEnemyCharacter* Enemy) override;

	/**
	 * Presentation hook.
	 *
	 * Blueprint may animate the spikes, play VFX or audio.
	 */
	UFUNCTION(
		BlueprintImplementableEvent,
		Category = "Trap|Spike|Presentation",
		meta = (DisplayName = "Spike Trap Activated"))
	void ReceiveSpikeTrapActivated(int32 AffectedEnemyCount);

	UFUNCTION(
		BlueprintImplementableEvent,
		Category = "Trap|Spike|Presentation",
		meta = (DisplayName = "Spike Trap Rearmed"))
	void ReceiveSpikeTrapRearmed();

private:
	void ActivateSpikeTrap(AADEnemyCharacter* TriggeringEnemy);
	int32 ApplyDamageToEnemies();
	bool ApplyDamageToEnemy(AADEnemyCharacter* Enemy);
	void StartRearm();
	void HandleRearmFinished();

	UPROPERTY(
		EditDefaultsOnly,
		BlueprintReadOnly,
		Category = "Trap|Spike",
		meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UGameplayEffect> DamageEffectClass;

	/**
	 * Mechanical reset time after the spikes activate.
	 *
	 * This is not a generic trap cooldown.
	 */
	UPROPERTY(
		EditDefaultsOnly,
		BlueprintReadOnly,
		Category = "Trap|Spike",
		meta = (
			AllowPrivateAccess = "true",
			ClampMin = "0.1",
			Units = "s"))
	float RearmDelay = 2.0f;
	
	bool bIsArmed = true;
	FTimerHandle RearmTimerHandle;
};