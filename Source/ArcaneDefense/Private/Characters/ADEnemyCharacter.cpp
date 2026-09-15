#include "Characters/ADEnemyCharacter.h"

#include "AI/ADEnemyAIController.h"

#include "Objective/ADDefenseObjective.h"

#include "AbilitySystemComponent.h"
#include "GameplayEffect.h"
#include "TimerManager.h"

#include "AbilitySystem/ADGameplayTags.h"
#include "GameFramework/CharacterMovementComponent.h"

AADEnemyCharacter::AADEnemyCharacter()
{
	AIControllerClass = AADEnemyAIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
}

void AADEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();

	UAbilitySystemComponent* ASC = GetAbilitySystemComponent();
	if (!IsValid(ASC)) { return; }

	ASC->RegisterGameplayTagEvent(
		ADGameplayTags::State_Rooted,
		EGameplayTagEventType::NewOrRemoved
	)
	.AddUObject(this, &AADEnemyCharacter::HandleRootedTagChanged);
}

void AADEnemyCharacter::HandleDeath()
{
	if (IsDead()) { return; }

	SetTargeted(false);

	StopAttackingObjective();
	
	Super::HandleDeath();

	// Notify gameplay systems immediately.
	OnEnemyDefeated.Broadcast(this);
	
	// Give Blueprint presentation a brief moment before destruction.
	SetLifeSpan(0.75f);
}

void AADEnemyCharacter::SetTargeted(const bool bNewTargeted)
{
	if (bIsTargeted == bNewTargeted) { return;	}

	bIsTargeted = bNewTargeted;
	ReceiveTargetedStateChanged(bIsTargeted);
}

bool AADEnemyCharacter::IsTargeted() const
{
	return bIsTargeted;
}

void AADEnemyCharacter::SetMoveTarget(AActor* NewMoveTarget)
{
	MoveTarget = NewMoveTarget;
	AADEnemyAIController* EnemyController = Cast<AADEnemyAIController>(GetController());
	if (IsValid(EnemyController))
	{
		EnemyController->SetMoveTarget(MoveTarget);
	}
}

AActor* AADEnemyCharacter::GetMoveTarget() const
{
	return MoveTarget;
}

void AADEnemyCharacter::HandleReachedMoveTarget()
{
	if (IsDead()) { return;	}

	AADDefenseObjective* Objective = Cast<AADDefenseObjective>(MoveTarget);

	if (!IsValid(Objective) || Objective->IsDefeated()) { return; }

	StartAttackingObjective(Objective);
}

void AADEnemyCharacter::StartAttackingObjective(AADDefenseObjective* Objective)
{
	if (!IsValid(Objective) || IsDead()) { return; }

	StopAttackingObjective();

	ObjectiveBeingAttacked = Objective;

	// First attack happens immediately after reaching the objective.
	AttackObjective();

	if (!ObjectiveBeingAttacked.IsValid()) { return; }

	GetWorldTimerManager().SetTimer(
		ObjectiveAttackTimerHandle,
		this,
		&AADEnemyCharacter::AttackObjective,
		ObjectiveAttackInterval,
		true,
		ObjectiveAttackInterval
	);
}

void AADEnemyCharacter::StopAttackingObjective()
{
	GetWorldTimerManager().ClearTimer(ObjectiveAttackTimerHandle);

	ObjectiveBeingAttacked.Reset();
}

void AADEnemyCharacter::AttackObjective()
{
	if (IsDead())
	{
		StopAttackingObjective();
		return;
	}

	AADDefenseObjective* Objective = ObjectiveBeingAttacked.Get();

	if (!IsValid(Objective) || Objective->IsDefeated())
	{
		StopAttackingObjective();
		return;
	}

	if (!ObjectiveDamageEffect)
	{
		UE_LOG(
			LogTemp,
			Warning,
			TEXT("%s has no Objective Damage Effect."),
			*GetNameSafe(this)
		);

		StopAttackingObjective();
		return;
	}

	UAbilitySystemComponent* SourceAbilitySystem = GetAbilitySystemComponent();
	UAbilitySystemComponent* TargetAbilitySystem = Objective->GetAbilitySystemComponent();

	if (!IsValid(SourceAbilitySystem) || !IsValid(TargetAbilitySystem))
	{
		StopAttackingObjective();
		return;
	}

	FGameplayEffectContextHandle EffectContext = SourceAbilitySystem->MakeEffectContext();

	EffectContext.AddSourceObject(this);

	const FGameplayEffectSpecHandle DamageSpec = SourceAbilitySystem->MakeOutgoingSpec(
		ObjectiveDamageEffect,
		1.0f,
		EffectContext
	);

	if (!DamageSpec.IsValid()) { return; }

	SourceAbilitySystem->ApplyGameplayEffectSpecToTarget(
		*DamageSpec.Data.Get(),
		TargetAbilitySystem
	);

	UE_LOG(
		LogTemp,
		Display,
		TEXT("%s attacked %s. Objective Health: %.0f/%.0f."),
		*GetNameSafe(this),
		*GetNameSafe(Objective),
		Objective->GetHealth(),
		Objective->GetMaxHealth()
	);
}
void AADEnemyCharacter::HandleRootedTagChanged(
	const FGameplayTag /*Tag*/,
	const int32 NewCount)
{
	if (IsDead()) { return;	}

	UCharacterMovementComponent* MovementComponent = GetCharacterMovement();
	if (!IsValid(MovementComponent)) { return; }

	const bool bIsRooted = (NewCount > 0);
	if (bIsRooted)
	{
		if (auto* EnemyController = Cast<AAIController>(GetController()))
		{
			EnemyController->StopMovement();
		}

		MovementComponent->StopMovementImmediately();
		MovementComponent->DisableMovement();

		UE_LOG(
			LogTemp,
			Display,
			TEXT("%s is rooted."),
			*GetNameSafe(this)
		);
	}
	else
	{
		MovementComponent->SetMovementMode(MOVE_Walking);

		/*
		 * An enemy already attacking the objective does not need
		 * to restart navigation. Root prevents movement, not attacks.
		 */
		if (!ObjectiveBeingAttacked.IsValid())
		{
			if (auto* EnemyController = Cast<AADEnemyAIController>(GetController()))
			{
				EnemyController->SetMoveTarget(MoveTarget);
			}
		}

		UE_LOG(
			LogTemp,
			Display,
			TEXT("%s is no longer rooted."),
			*GetNameSafe(this)
		);
	}

	ReceiveRootedStateChanged(bIsRooted);
}