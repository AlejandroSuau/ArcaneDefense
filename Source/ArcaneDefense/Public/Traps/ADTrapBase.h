#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ADTrapBase.generated.h"

class AADEnemyCharacter;
class AADTrapPlacementSlot;
class UAbilitySystemComponent;
class UADTrapDataAsset;
class UPrimitiveComponent;
class USceneComponent;
class UBoxComponent;

/**
 * Common runtime base for placed traps.
 *
 * Owns trap identity, source attribution, placement ownership
 * and activation-range enemy detection.
 */
UCLASS(Blueprintable)
class ARCANEDEFENSE_API AADTrapBase
	: public AActor
{
	GENERATED_BODY()

public:
	AADTrapBase();

	/**
	 * Initializes a runtime trap after placement.
	 *
	 * InPurchasePrice is the real price paid by the player, not
	 * necessarily the Data Asset base price.
	 */
	void InitializeTrap(
		UADTrapDataAsset* InTrapData,
		UAbilitySystemComponent* InSourceAbilitySystem,
		int32 InPurchasePrice,
		AADTrapPlacementSlot* InPlacementSlot);
	
	UFUNCTION(BlueprintPure, Category = "Trap")
	UADTrapDataAsset* GetTrapData() const;

	UFUNCTION(BlueprintPure, Category = "Trap")
	int32 GetPurchasePrice() const;

	UFUNCTION(BlueprintPure, Category = "Trap")
	float GetSellRefundRatio() const;

	UFUNCTION(BlueprintPure, Category = "Trap")
	int32 GetExpectedSellValue() const;

	UFUNCTION(BlueprintPure, Category = "Trap")
	float GetActivationRange() const;
	
	UFUNCTION(BlueprintPure, Category = "Trap")
	int32 GetValidEnemyCount() const;

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	/**
	 * Extension point for discrete or persistent trap behavior.
	 */
	virtual void HandleEnemyEnteredTrigger(AADEnemyCharacter* Enemy);
	virtual void HandleEnemyExitedTrigger(AADEnemyCharacter* Enemy);
	void GetValidEnemiesInActivationRange(TArray<AADEnemyCharacter*>& OutEnemies) const;

	UAbilitySystemComponent* GetSourceAbilitySystem() const;

	UFUNCTION(
		BlueprintImplementableEvent,
		Category = "Trap|Presentation",
		meta = (DisplayName = "Enemy Entered Trap"))
	void ReceiveEnemyEnteredTrigger(AADEnemyCharacter* Enemy);

	UFUNCTION(
		BlueprintImplementableEvent,
		Category = "Trap|Presentation",
		meta = (DisplayName = "Enemy Exited Trap"))
	void ReceiveEnemyExitedTrigger(AADEnemyCharacter* Enemy);

private:
	UFUNCTION()
	void HandleActivationBeginOverlap(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComponent,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult);

	UFUNCTION()
	void HandleActivationEndOverlap(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComponent,
		int32 OtherBodyIndex);

	void ApplyTrapData();

	UPROPERTY(
		VisibleAnywhere,
		BlueprintReadOnly,
		Category = "Components",
		meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USceneComponent> SceneRoot;

	UPROPERTY(
		VisibleAnywhere,
		BlueprintReadOnly,
		Category = "Components",
		meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UBoxComponent> ActivationVolume;

	/**
	 * EditInstanceOnly allows manual level testing during development.
	 * Runtime placement will initialize this automatically.
	 */
	UPROPERTY(
		EditInstanceOnly,
		BlueprintReadOnly,
		Category = "Trap",
		meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UADTrapDataAsset> TrapData;

	TWeakObjectPtr<UAbilitySystemComponent>	SourceAbilitySystem;

	UPROPERTY(
		VisibleInstanceOnly,
		BlueprintReadOnly,
		Category = "Trap|Economy",
		meta = (AllowPrivateAccess = "true"))
	int32 PurchasePrice = 0;

	TWeakObjectPtr<AADTrapPlacementSlot> PlacementSlot;
};