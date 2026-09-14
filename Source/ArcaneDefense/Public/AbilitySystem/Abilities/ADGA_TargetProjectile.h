#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/ADGA_TargetedDamage.h"
#include "ADGA_TargetProjectile.generated.h"

class AADProjectile;

/**
 * Targeted cast ability that delivers its Gameplay Effect
 * through a homing projectile.
 */
UCLASS(Abstract, Blueprintable)
class ARCANEDEFENSE_API UADGA_TargetProjectile
	: public UADGA_TargetedDamage
{
	GENERATED_BODY()

protected:
	virtual bool CanActivateAbility(
		const FGameplayAbilitySpecHandle Handle,
		const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayTagContainer* SourceTags,
		const FGameplayTagContainer* TargetTags,
		FGameplayTagContainer* OptionalRelevantTags) const override;

	virtual bool ExecuteAbilityPayload(
		AADEnemyCharacter* Target,
		const FGameplayAbilityActorInfo* ActorInfo) override;

	/**
	 * Projectile Blueprint or native class spawned after casting.
	 */
	UPROPERTY(
		EditDefaultsOnly,
		BlueprintReadOnly,
		Category = "Ability|Projectile"
	)
	TSubclassOf<AADProjectile> ProjectileClass;

	/**
	 * Spawn offset expressed in the caster's local space.
	 */
	UPROPERTY(
		EditDefaultsOnly,
		BlueprintReadOnly,
		Category = "Ability|Projectile"
	)
	FVector ProjectileSpawnOffset =
		FVector(80.0f, 0.0f, 60.0f);
};