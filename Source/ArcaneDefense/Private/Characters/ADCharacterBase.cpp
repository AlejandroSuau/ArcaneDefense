#include "Characters/ADCharacterBase.h"

#include "AbilitySystem/ADAttributeSet.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

AADCharacterBase::AADCharacterBase()
{
	PrimaryActorTick.bCanEverTick = true;

	AbilitySystemComponent = CreateDefaultSubobject<UAbilitySystemComponent>(
		TEXT("AbilitySystemComponent"));

	AttributeSet = CreateDefaultSubobject<UADAttributeSet>(
		TEXT("AttributeSet"));
}

void AADCharacterBase::BeginPlay()
{
	Super::BeginPlay();

	InitializeAbilitySystem();
	ApplyInitialAttributes();

	UE_LOG(
		LogTemp,
		Display,
		TEXT("%s initialized with Health %.0f/%.0f and Mana  %.0f/%.0f"),
		*GetNameSafe(this),
		GetHealth(),
		GetMaxHealth(),
		GetMana(),
		GetMaxMana());
}

UAbilitySystemComponent* AADCharacterBase::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

void AADCharacterBase::HandleDeath()
{
	if (bIsDead) { return; }

	bIsDead = true;
	if (UCharacterMovementComponent* MovementComponent = GetCharacterMovement())
	{
		MovementComponent->DisableMovement();
	}

	if (UCapsuleComponent* Capsule = GetCapsuleComponent())
	{
		Capsule->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}

	UE_LOG(
		LogTemp,
		Display,
		TEXT("%s entered the death state."),
		*GetNameSafe(this));

	ReceiveDeath();
}

float AADCharacterBase::GetHealth() const
{
	return IsValid(AttributeSet) ? AttributeSet->GetHealth() : 0.f;
}

float AADCharacterBase::GetMaxHealth() const
{
	return IsValid(AttributeSet) ? AttributeSet->GetMaxHealth() : 0.f;
}

float AADCharacterBase::GetMana() const
{
	return IsValid(AttributeSet) ? AttributeSet->GetMana() : 0.f;
}

float AADCharacterBase::GetMaxMana() const
{
	return IsValid(AttributeSet) ? AttributeSet->GetMaxMana() : 0.f;
}

bool AADCharacterBase::IsDead() const
{
	return bIsDead;
}

void AADCharacterBase::InitializeAbilitySystem()
{
	if (!ensureMsgf(
		IsValid(AbilitySystemComponent),
		TEXT("%s does not have a valid Ability System Component."),
		*GetNameSafe(this)))
	{
		return;
	}

	if (!ensureMsgf(
		IsValid(AttributeSet),
		TEXT("%s does not have a valid Attribute Set."),
		*GetNameSafe(this)))
	{
		return;
	}

	AbilitySystemComponent->InitAbilityActorInfo(this, this);
}

void AADCharacterBase::ApplyInitialAttributes()
{
	if (!IsValid(AttributeSet))
	{
		return;
	}

	if (!InitialAttributesEffect)
	{
		UE_LOG(
			LogTemp,
			Warning,
			TEXT("%s does not have an Initial Attributes Effect"),
			*GetNameSafe(this));
		return;
	}

	FGameplayEffectContextHandle EffectContext = AbilitySystemComponent->MakeEffectContext();
	EffectContext.AddSourceObject(this);

	const FGameplayEffectSpecHandle EffectSpec = AbilitySystemComponent->MakeOutgoingSpec(
		InitialAttributesEffect,
		1.f,
		EffectContext);

	if (!EffectSpec.IsValid())
	{
		UE_LOG(
			LogTemp,
			Error,
			TEXT("Failed to create the initial attributes effect for %s."),
			*GetNameSafe(this));
		return;
	}

	AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*EffectSpec.Data.Get());
}