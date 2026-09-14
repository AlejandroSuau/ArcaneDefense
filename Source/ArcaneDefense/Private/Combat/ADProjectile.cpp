#include "Combat/ADProjectile.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystemGlobals.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"

AADProjectile::AADProjectile()
{
	PrimaryActorTick.bCanEverTick = false;

	CollisionSphere = CreateDefaultSubobject<USphereComponent>(	TEXT("CollisionSphere"));

	SetRootComponent(CollisionSphere);

	CollisionSphere->InitSphereRadius(16.0f);
	CollisionSphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	CollisionSphere->SetCollisionObjectType(ECC_WorldDynamic);
	CollisionSphere->SetCollisionResponseToAllChannels(ECR_Ignore);

	// Intended target characters use Pawn collision.
	CollisionSphere->SetCollisionResponseToChannel(ECC_Pawn,ECR_Overlap);

	// The projectile should disappear if it hits the level.
	CollisionSphere->SetCollisionResponseToChannel(	ECC_WorldStatic,ECR_Block);
	CollisionSphere->SetGenerateOverlapEvents(true);

	CollisionSphere->OnComponentBeginOverlap.AddDynamic(
		this,
		&AADProjectile::HandleBeginOverlap);

	CollisionSphere->OnComponentHit.AddDynamic(
		this,
		&AADProjectile::HandleBlockingHit);

	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(
		TEXT("ProjectileMovement"));

	ProjectileMovement->SetUpdatedComponent(CollisionSphere);

	ProjectileMovement->InitialSpeed = 1200.0f;
	ProjectileMovement->MaxSpeed = 1200.0f;
	ProjectileMovement->ProjectileGravityScale = 0.0f;
	ProjectileMovement->bRotationFollowsVelocity = true;
	ProjectileMovement->bIsHomingProjectile = true;
	ProjectileMovement->HomingAccelerationMagnitude = 5000.0f;
}

void AADProjectile::BeginPlay()
{
	Super::BeginPlay();

	SetLifeSpan(MaximumLifetime);
}

void AADProjectile::InitializeProjectile(
	UAbilitySystemComponent* InSourceAbilitySystem,
	AActor* InTargetActor,
	const FGameplayEffectSpecHandle& InEffectSpec)
{
	if (!IsValid(InSourceAbilitySystem)
		|| !IsValid(InTargetActor)
		|| !InEffectSpec.IsValid())
	{
		UE_LOG(
			LogTemp,
			Error,
			TEXT(
				"%s received invalid initialization data."
			),
			*GetNameSafe(this)
		);

		Destroy();
		return;
	}

	SourceAbilitySystem = InSourceAbilitySystem;
	TargetActor = InTargetActor;
	EffectSpec = InEffectSpec;

	USceneComponent* TargetComponent =	InTargetActor->GetRootComponent();
	if (!IsValid(TargetComponent) || !IsValid(ProjectileMovement))
	{
		Destroy();
		return;
	}

	ProjectileMovement->HomingTargetComponent = TargetComponent;

	ProjectileMovement->bIsHomingProjectile = true;

	const FVector InitialDirection = (
		InTargetActor->GetActorLocation()- GetActorLocation()).GetSafeNormal();

	ProjectileMovement->Velocity = InitialDirection	* ProjectileMovement->InitialSpeed;

	AActor* SourceActor = InSourceAbilitySystem->GetAvatarActor();
	if (IsValid(SourceActor))
	{
		CollisionSphere->IgnoreActorWhenMoving(
			SourceActor,
			true
		);
	}

	InTargetActor->OnDestroyed.AddUniqueDynamic(
		this,
		&AADProjectile::HandleTargetDestroyed);
}

void AADProjectile::HandleBeginOverlap(
	UPrimitiveComponent* /*OverlappedComponent*/,
	AActor* OtherActor,
	UPrimitiveComponent* /*OtherComponent*/,
	int32 /*OtherBodyIndex*/,
	bool /*bFromSweep*/,
	const FHitResult& /*SweepResult*/
)
{
	if (bImpactProcessed) {	return;	}

	AActor* CurrentTarget =	TargetActor.Get();
	if (!IsValid(CurrentTarget))
	{
		Destroy();
		return;
	}

	/*
	 * A targeted projectile only damages the actor it was created for.
	 * Crossing another enemy does not redirect or consume it.
	 */
	if (OtherActor != CurrentTarget) { return; }

	if (!ApplyEffectToTarget(CurrentTarget))
	{
		Destroy();
		return;
	}

	FinishProjectile(CurrentTarget);
}

bool AADProjectile::ApplyEffectToTarget(AActor* InTargetActor)
{
	if (!IsValid(InTargetActor)	|| !EffectSpec.IsValid()) {	return false; }

	UAbilitySystemComponent* SourceASC = SourceAbilitySystem.Get();
	if (!IsValid(SourceASC)) { return false; }

	UAbilitySystemComponent* TargetASC =
		UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(
			InTargetActor,
			true);

	if (!IsValid(TargetASC)) { return false; }

	SourceASC->ApplyGameplayEffectSpecToTarget(*EffectSpec.Data.Get(), TargetASC);

	UE_LOG(
		LogTemp,
		Display,
		TEXT(
			"%s impacted %s and applied %s."
		),
		*GetNameSafe(this),
		*GetNameSafe(InTargetActor),
		*GetNameSafe(EffectSpec.Data->Def)
	);

	return true;
}

void AADProjectile::HandleBlockingHit(
	UPrimitiveComponent* /*HitComponent*/,
	AActor* OtherActor,
	UPrimitiveComponent* /*OtherComponent*/,
	FVector /*NormalImpulse*/,
	const FHitResult& /*Hit*/
)
{
	if (bImpactProcessed) {	return;	}

	/*
	 * Pawn targets are handled by overlap.
	 * A blocking hit normally means we hit world geometry.
	 */
	FinishProjectile(OtherActor);
}

void AADProjectile::FinishProjectile(AActor* ImpactedActor)
{
	if (bImpactProcessed) {	return;	}

	bImpactProcessed = true;

	if (AActor* CurrentTarget = TargetActor.Get())
	{
		CurrentTarget->OnDestroyed.RemoveDynamic(
			this,
			&AADProjectile::HandleTargetDestroyed);
	}

	ReceiveProjectileImpact(ImpactedActor);

	Destroy();
}

void AADProjectile::HandleTargetDestroyed(AActor* /*DestroyedActor*/)
{
	if (bImpactProcessed) {	return;	}

	Destroy();
}