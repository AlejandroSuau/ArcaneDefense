#include "Objective/ADDefenseObjective.h"

#include "AbilitySystem/ADAttributeSet.h"
#include "AbilitySystemComponent.h"
#include "Components/SceneComponent.h"
#include "Game/ADGameMode.h"
#include "GameplayEffect.h"

AADDefenseObjective::AADDefenseObjective()
{
	PrimaryActorTick.bCanEverTick = false;

	SceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("SceneRoot"));

	SetRootComponent(SceneRoot);

	AbilitySystemComponent = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	AttributeSet = CreateDefaultSubobject<UADAttributeSet>(TEXT("AttributeSet"));
}

void AADDefenseObjective::BeginPlay()
{
	Super::BeginPlay();

	InitializeAbilitySystem();

	if (IsValid(AttributeSet))
	{
		AttributeSet->OnOutOfHealth.AddUObject(
			this,
			&AADDefenseObjective::HandleOutOfHealth
		);
	}

	if (IsValid(AbilitySystemComponent))
	{
		AbilitySystemComponent
			->GetGameplayAttributeValueChangeDelegate(
				UADAttributeSet::GetHealthAttribute()
			)
			.AddUObject(
				this,
				&AADDefenseObjective::HandleHealthChanged
			);
	}

	ApplyInitialAttributes();

	UE_LOG(
		LogTemp,
		Display,
		TEXT(
			"%s initialized with Health %.0f/%.0f."
		),
		*GetNameSafe(this),
		GetHealth(),
		GetMaxHealth()
	);
}

UAbilitySystemComponent* AADDefenseObjective::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

float AADDefenseObjective::GetHealth() const
{
	return IsValid(AttributeSet) ? AttributeSet->GetHealth() : 0.0f;
}

float AADDefenseObjective::GetMaxHealth() const
{
	return IsValid(AttributeSet) ? AttributeSet->GetMaxHealth() : 0.0f;
}

bool AADDefenseObjective::IsDefeated() const
{
	return bIsDefeated;
}

void AADDefenseObjective::InitializeAbilitySystem()
{
	if (!ensureMsgf(
		IsValid(AbilitySystemComponent),
		TEXT("%s requires an Ability System Component."),
		*GetNameSafe(this)))
	{
		return;
	}

	AbilitySystemComponent->InitAbilityActorInfo(this, this);
}

void AADDefenseObjective::ApplyInitialAttributes()
{
	if (!IsValid(AbilitySystemComponent) || !InitialAttributesEffect) { return; }

	FGameplayEffectContextHandle EffectContext =
		AbilitySystemComponent->MakeEffectContext();

	EffectContext.AddSourceObject(this);

	const FGameplayEffectSpecHandle EffectSpec = AbilitySystemComponent->MakeOutgoingSpec(
		InitialAttributesEffect,
		1.0f,
		EffectContext
	);

	if (!EffectSpec.IsValid()) { return; }

	AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*EffectSpec.Data.Get());
}

void AADDefenseObjective::HandleHealthChanged(const FOnAttributeChangeData& Data)
{
	OnHealthChanged.Broadcast(Data.NewValue, GetMaxHealth());

	UE_LOG(
		LogTemp,
		Display,
		TEXT(
			"%s Health: %.0f/%.0f."
		),
		*GetNameSafe(this),
		Data.NewValue,
		GetMaxHealth()
	);
}

void AADDefenseObjective::HandleOutOfHealth()
{
	if (bIsDefeated) { return; }

	bIsDefeated = true;

	UE_LOG(
		LogTemp,
		Warning,
		TEXT("%s has been defeated."),
		*GetNameSafe(this)
	);

	OnObjectiveDefeated.Broadcast();

	ReceiveObjectiveDefeated();

	if (UWorld* World = GetWorld())
	{
		if (AADGameMode* GameMode = World->GetAuthGameMode<AADGameMode>())
		{
			GameMode->HandleGameDefeat();
		}
	}
}