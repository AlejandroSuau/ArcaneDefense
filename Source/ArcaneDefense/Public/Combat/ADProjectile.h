#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameplayEffectTypes.h"
#include "ADProjectile.generated.h"

class UAbilitySystemComponent;
class UProjectileMovementComponent;
class USphereComponent;

/**
 * Reusable homing projectile capable of carrying a Gameplay Effect Spec
 * from a source Ability System Component to a target Actor.
 *
 * The projectile does not know which spell created it or which attribute
 * the carried Gameplay Effect modifies.
 */
UCLASS()
class ARCANEDEFENSE_API AADProjectile : public AActor
{
	GENERATED_BODY()

public:
	AADProjectile();

	/**
	 * Configures the projectile before it enters gameplay.
	 */
	void InitializeProjectile(
		UAbilitySystemComponent* InSourceAbilitySystem,
		AActor* InTargetActor,
		const FGameplayEffectSpecHandle& InEffectSpec
	);

protected:
	virtual void BeginPlay() override;

	UFUNCTION(
		BlueprintImplementableEvent,
		Category = "Projectile",
		meta = (DisplayName = "Projectile Impact")
	)
	void ReceiveProjectileImpact(AActor* ImpactedActor);

private:
	UFUNCTION()
	void HandleBeginOverlap(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComponent,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult
	);

	UFUNCTION()
	void HandleBlockingHit(
		UPrimitiveComponent* HitComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComponent,
		FVector NormalImpulse,
		const FHitResult& Hit
	);

	UFUNCTION()
	void HandleTargetDestroyed(AActor* DestroyedActor);

	bool ApplyEffectToTarget(AActor* TargetActor);

	void FinishProjectile(AActor* ImpactedActor);

	UPROPERTY(
		VisibleAnywhere,
		BlueprintReadOnly,
		Category = "Components",
		meta = (AllowPrivateAccess = "true")
	)
	TObjectPtr<USphereComponent> CollisionSphere;

	UPROPERTY(
		VisibleAnywhere,
		BlueprintReadOnly,
		Category = "Components",
		meta = (AllowPrivateAccess = "true")
	)
	TObjectPtr<UProjectileMovementComponent> ProjectileMovement;

	UPROPERTY(
		EditDefaultsOnly,
		BlueprintReadOnly,
		Category = "Projectile",
		meta = (
			AllowPrivateAccess = "true",
			ClampMin = "0.1",
			Units = "s"
		)
	)
	float MaximumLifetime = 8.0f;

	TWeakObjectPtr<UAbilitySystemComponent>
		SourceAbilitySystem;

	TWeakObjectPtr<AActor> TargetActor;

	FGameplayEffectSpecHandle EffectSpec;

	bool bImpactProcessed = false;
};