#include "Traps/ADSpikeTrap.h"

#include "AbilitySystemComponent.h"
#include "Characters/ADEnemyCharacter.h"
#include "Engine/World.h"
#include "GameplayEffect.h"
#include "TimerManager.h"

AADSpikeTrap::AADSpikeTrap()
{
	PrimaryActorTick.bCanEverTick = false;
}

void AADSpikeTrap::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if (UWorld* World = GetWorld())
	{
		World->GetTimerManager().ClearTimer(RearmTimerHandle);
	}

	Super::EndPlay(EndPlayReason);
}

bool AADSpikeTrap::IsArmed() const
{
	return bIsArmed;
}

void AADSpikeTrap::HandleEnemyEnteredTrigger(AADEnemyCharacter* Enemy)
{
	Super::HandleEnemyEnteredTrigger(Enemy);

	GetWorldTimerManager().SetTimerForNextTick(
	FTimerDelegate::CreateWeakLambda(this, [this, Enemy]()
	{
		if (IsValid(Enemy))
		{
			ReceiveEnemyEnteredTrigger(Enemy);
		}
	}));

	if (!HasAuthority() || !bIsArmed || !IsValid(Enemy) || Enemy->IsDead()) { return; }

	ActivateSpikeTrap(Enemy);
}

void AADSpikeTrap::ActivateSpikeTrap(AADEnemyCharacter* TriggeringEnemy)
{
	if (!bIsArmed) { return; }

	bIsArmed = false;

	/*
	 * The BeginOverlap event should already make the triggering
	 * enemy part of ActivationVolume. TriggeringEnemy is kept as
	 * an explicit parameter so the activation reason remains clear.
	 */
	const int32 AffectedEnemyCount = ApplyDamageToEnemies();

	ReceiveSpikeTrapActivated(AffectedEnemyCount);

	StartRearm();
}

int32 AADSpikeTrap::ApplyDamageToEnemies()
{
	TArray<AADEnemyCharacter*> Enemies;

	GetValidEnemiesInActivationRange(Enemies);

	int32 AffectedEnemyCount = 0;
	for (AADEnemyCharacter* Enemy : Enemies)
	{
		if (ApplyDamageToEnemy(Enemy))
		{
			++AffectedEnemyCount;
		}
	}

	return AffectedEnemyCount;
}

bool AADSpikeTrap::ApplyDamageToEnemy(AADEnemyCharacter* Enemy)
{
	if (!IsValid(Enemy) || Enemy->IsDead() || !DamageEffectClass) { return false; }

	UAbilitySystemComponent* SourceASC = GetSourceAbilitySystem();
	UAbilitySystemComponent* TargetASC = Enemy->GetAbilitySystemComponent();
	if (!IsValid(SourceASC) || !IsValid(TargetASC)) { return false; }

	FGameplayEffectContextHandle EffectContext = SourceASC->MakeEffectContext();

	/*
	 * The Player ASC remains the gameplay source,
	 * while the concrete trap is recorded as the SourceObject.
	 */
	EffectContext.AddSourceObject(this);

	FGameplayEffectSpecHandle EffectSpec = SourceASC->MakeOutgoingSpec(
		DamageEffectClass,
		1.0f,
		EffectContext);

	if (!EffectSpec.IsValid()) { return false; }

	SourceASC->ApplyGameplayEffectSpecToTarget(*EffectSpec.Data.Get(), TargetASC);

	return true;
}

void AADSpikeTrap::StartRearm()
{
	UWorld* World = GetWorld();
	if (!IsValid(World)) { return; }

	if (RearmDelay <= 0.0f)
	{
		bIsArmed = true;
		ReceiveSpikeTrapRearmed();
		return;
	}

	World->GetTimerManager().SetTimer(
		RearmTimerHandle,
		this,
		&AADSpikeTrap::HandleRearmFinished,
		RearmDelay,
		false);
}

void AADSpikeTrap::HandleRearmFinished()
{
	bIsArmed = true;

	ReceiveSpikeTrapRearmed();

	/*
	 * If enemies are still standing inside the active area,
	 * trigger another spike cycle immediately.
	 */
	TArray<AADEnemyCharacter*> Enemies;
	GetValidEnemiesInActivationRange(Enemies);

	if (!Enemies.IsEmpty())
	{
		ActivateSpikeTrap(Enemies[0]);
	}
}