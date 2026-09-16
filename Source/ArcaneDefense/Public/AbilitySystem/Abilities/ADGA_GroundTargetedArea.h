#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/ADGameplayAbility.h"
#include "ADGA_GroundTargetedArea.generated.h"

class AADEnemyCharacter;
class UADCastComponent;
class UGameplayEffect;

/**
 * Cast-time ability that applies a Gameplay Effect to enemies
 * around a previously confirmed world location.
 */
UCLASS(Abstract, Blueprintable)
class ARCANEDEFENSE_API UADGA_GroundTargetedArea
	: public UADGameplayAbility
{
	GENERATED_BODY()

public:
	UADGA_GroundTargetedArea();
	
	UFUNCTION(BlueprintPure, Category = "Ability|Ground Targeting")
	float GetTargetingRange() const;

	UFUNCTION(BlueprintPure, Category = "Ability|Ground Targeting")
	float GetEffectRadius() const;

	TSubclassOf<AActor>	GetTargetPreviewActorClass() const;

protected:
	virtual bool CanActivateAbility(
		const FGameplayAbilitySpecHandle Handle,
		const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayTagContainer* SourceTags,
		const FGameplayTagContainer* TargetTags,
		FGameplayTagContainer* OptionalRelevantTags
	) const override;

	virtual void ActivateAbility(
		const FGameplayAbilitySpecHandle Handle,
		const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo,
		const FGameplayEventData* TriggerEventData
	) override;

	virtual void EndAbility(
		const FGameplayAbilitySpecHandle Handle,
		const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo,
		bool bReplicateEndAbility,
		bool bWasCancelled
	) override;

	UFUNCTION(
		BlueprintImplementableEvent,
		Category = "Ability|Area",
		meta = (DisplayName = "Ground Effect Executed"))
	void ReceiveGroundEffectExecuted(FVector Location, float Radius, int32 AffectedEnemies);

private:
	UFUNCTION()
	void HandleCastFinished();

	bool IsTargetLocationInRange(
		const AActor* AvatarActor,
		const FVector& TargetLocation
	) const;

	int32 ApplyEffectAtTargetLocation();

	void EndCurrentAbility(bool bWasCancelled);

	UPROPERTY(
		EditDefaultsOnly,
		BlueprintReadOnly,
		Category = "Ability|Effect",
		meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UGameplayEffect> EffectClass;

	UPROPERTY(
		EditDefaultsOnly,
		BlueprintReadOnly,
		Category = "Ability|Ground Targeting",
		meta = (
			AllowPrivateAccess = "true",
			ClampMin = "0.0",
			Units = "cm"
		))
	float TargetingRange = 2500.0f;

	UPROPERTY(
		EditDefaultsOnly,
		BlueprintReadOnly,
		Category = "Ability|Ground Targeting",
		meta = (
			AllowPrivateAccess = "true",
			ClampMin = "0.0",
			Units = "cm"
		))
	float EffectRadius = 400.0f;

	UPROPERTY(
		EditDefaultsOnly,
		BlueprintReadOnly,
		Category = "Ability|Ground Targeting",
		meta = (AllowPrivateAccess = "true"))
	TSubclassOf<AActor> TargetPreviewActorClass;

	UPROPERTY(
		EditDefaultsOnly,
		BlueprintReadOnly,
		Category = "Ability|Casting",
		meta = (
			AllowPrivateAccess = "true",
			ClampMin = "0.0",
			Units = "s"
		))
	float CastTime = 2.0f;

	UPROPERTY(
		EditDefaultsOnly,
		BlueprintReadOnly,
		Category = "Ability|Casting",
		meta = (AllowPrivateAccess = "true"))
	FText CastDisplayName;

	FVector CachedTargetLocation = FVector::ZeroVector;

	bool bHasCachedTargetLocation = false;

	TWeakObjectPtr<UADCastComponent> CachedCastComponent;
};