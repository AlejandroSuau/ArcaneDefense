#include "AbilitySystem/Abilities/ADGA_TargetProjectile.h"

#include "Characters/ADEnemyCharacter.h"
#include "AbilitySystemComponent.h"
#include "Combat/ADProjectile.h"
#include "Engine/World.h"
#include "GameFramework/Pawn.h"
#include "Kismet/GameplayStatics.h"

bool UADGA_TargetProjectile::CanActivateAbility(
	const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayTagContainer* SourceTags,
	const FGameplayTagContainer* TargetTags,
	FGameplayTagContainer* OptionalRelevantTags) const
{
	if (!Super::CanActivateAbility(
		Handle,
		ActorInfo,
		SourceTags,
		TargetTags,
		OptionalRelevantTags
	))
	{
		return false;
	}

	return (ProjectileClass && DamageEffectClass);
}

bool UADGA_TargetProjectile::ExecuteAbilityPayload(
	AADEnemyCharacter* Target,
	const FGameplayAbilityActorInfo* ActorInfo)
{
	if (!IsValid(Target) || !ActorInfo || !ProjectileClass || !DamageEffectClass)
	{
		return false;
	}

	AActor* AvatarActor = ActorInfo->AvatarActor.Get();
	if (!IsValid(AvatarActor)) { return false; }

	UAbilitySystemComponent* SourceASC = GetAbilitySystemComponentFromActorInfo();
	if (!IsValid(SourceASC)) { return false; }

	const FGameplayEffectSpecHandle DamageSpec = MakeOutgoingGameplayEffectSpec(
		DamageEffectClass,
		GetAbilityLevel());

	if (!DamageSpec.IsValid()) { return false; }

	const FTransform AvatarTransform = AvatarActor->GetActorTransform();
	const FVector SpawnLocation =
		AvatarActor->GetActorLocation()
		+ AvatarTransform.TransformVectorNoScale(ProjectileSpawnOffset);

	const FVector TargetDirection =
		(Target->GetActorLocation()	- SpawnLocation).GetSafeNormal();

	const FRotator SpawnRotation = TargetDirection.Rotation();
	const FTransform SpawnTransform(SpawnRotation,SpawnLocation);

	UWorld* World = GetWorld();
	if (!IsValid(World)) { return false; }

	AADProjectile* Projectile = World->SpawnActorDeferred<AADProjectile>(
		ProjectileClass,
		SpawnTransform,
		AvatarActor,
		Cast<APawn>(AvatarActor),
		ESpawnActorCollisionHandlingMethod::AlwaysSpawn);

	if (!IsValid(Projectile)) {	return false; }

	Projectile->InitializeProjectile(
		SourceASC,
		Target,
		DamageSpec
	);

	UGameplayStatics::FinishSpawningActor(
		Projectile,
		SpawnTransform
	);

	return true;
}