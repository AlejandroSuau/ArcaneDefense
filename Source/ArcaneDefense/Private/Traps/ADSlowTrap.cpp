#include "Traps/ADSlowTrap.h"

#include "AbilitySystemComponent.h"
#include "Characters/ADEnemyCharacter.h"
#include "GameplayEffect.h"

void AADSlowTrap::HandleEnemyEnteredTrigger(AADEnemyCharacter* Enemy)
{
	Super::HandleEnemyEnteredTrigger(Enemy);
	
	if (!HasAuthority() || !IsValid(Enemy) || Enemy->IsDead()) { return; }

	CleanupInvalidEffects();
	ApplySlowToEnemy(Enemy);
}

void AADSlowTrap::HandleEnemyExitedTrigger(AADEnemyCharacter* Enemy)
{
	Super::HandleEnemyExitedTrigger(Enemy);
	
	if (!HasAuthority() || !IsValid(Enemy)) { return; }

	RemoveSlowFromEnemy(Enemy);
}

bool AADSlowTrap::ApplySlowToEnemy(AADEnemyCharacter* Enemy)
{
	if (!IsValid(Enemy) || !SlowEffectClass) { return false; }
	if (ActiveSlowEffects.Contains(Enemy)) { return true; }

	UAbilitySystemComponent* SourceASC = GetSourceAbilitySystem();
	UAbilitySystemComponent* TargetASC = Enemy->GetAbilitySystemComponent();
	if (!IsValid(SourceASC) || !IsValid(TargetASC)) { return false; }

	FGameplayEffectContextHandle Context = SourceASC->MakeEffectContext();
	Context.AddSourceObject(this);

	auto Spec = SourceASC->MakeOutgoingSpec(SlowEffectClass,1.0f,Context);
	if (!Spec.IsValid()) { return false; }

	const auto Handle = SourceASC->ApplyGameplayEffectSpecToTarget(*Spec.Data.Get(), TargetASC);
	if (!Handle.WasSuccessfullyApplied()) { return false; }

	ActiveSlowEffects.Add(Enemy, Handle);

	return true;
}

void AADSlowTrap::RemoveSlowFromEnemy(AADEnemyCharacter* Enemy)
{
	auto* Handle = ActiveSlowEffects.Find(Enemy);
	if (!Handle) { return; }

	if (UAbilitySystemComponent* TargetASC = Handle->GetOwningAbilitySystemComponent())
	{
		TargetASC->RemoveActiveGameplayEffect(*Handle);
	}

	ActiveSlowEffects.Remove(Enemy);
}

void AADSlowTrap::CleanupInvalidEffects()
{
	for (auto It = ActiveSlowEffects.CreateIterator(); It; ++It)
	{
		const bool bEnemyInvalid = !It.Key().IsValid();
		const bool bEffectInvalid = !It.Value().GetOwningAbilitySystemComponent();
		if (bEnemyInvalid || bEffectInvalid)
		{
			It.RemoveCurrent();
		}
	}
}

void AADSlowTrap::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	for (const TPair<TWeakObjectPtr<AADEnemyCharacter>, FActiveGameplayEffectHandle>& Pair : ActiveSlowEffects)
	{
		if (auto* TargetASC = Pair.Value.GetOwningAbilitySystemComponent())
		{
			TargetASC->RemoveActiveGameplayEffect(Pair.Value);
		}
	}

	ActiveSlowEffects.Empty();

	Super::EndPlay(EndPlayReason);
}