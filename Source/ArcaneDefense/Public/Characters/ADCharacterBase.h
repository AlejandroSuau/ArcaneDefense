#pragma once

#include "CoreMinimal.h"
#include  "AbilitySystemInterface.h"
#include "GameFramework/Character.h"
#include "ADCharacterBase.generated.h"

class UADAttributeSet;
class UAbilitySystemComponent;
class UGameplayEffect;

/**
 * Base class for all living characters in Arcane Defense.
 *
 * Player-specific input and AI-specific behavior must not be implemented here.
 */
UCLASS(Abstract)
class ARCANEDEFENSE_API AADCharacterBase
	: public ACharacter
	, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	AADCharacterBase();

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	/**
	 * Enters the character death state.
	 *
	 * Called by the Attribute Set when Health reaches zero.
	 */
	virtual void HandleDeath();

	UFUNCTION(BlueprintPure, Category = "Attributes")
	float GetHealth() const;

	UFUNCTION(BlueprintPure, Category = "Attributes")
	float GetMaxHealth() const;

	UFUNCTION(BlueprintPure, Category = "Attributes")
	float GetMana() const;

	UFUNCTION(BlueprintPure, Category = "Attributes")
	float GetMaxMana() const;

	/**
	 * Returns whether this character has entered its death state.
	 */
	UFUNCTION(BlueprintPure, Category = "Combat")
	bool IsDead() const;

protected:
	virtual void BeginPlay() override;

	/**
	 * Allows Blueprint children to provide death presentation.
	 */
	UFUNCTION(
		BlueprintImplementableEvent,
		Category = "Combat",
		meta = (DisplayName = "Death")
	)
	void ReceiveDeath();

private:
	void InitializeAbilitySystem();
	void ApplyInitialAttributes();

	/** Coordinates abilities, effects, attributes, and gameplay tags. */
	UPROPERTY(
		VisibleAnywhere,
		BlueprintReadOnly,
		Category = "Abilities",
		meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;

	/** Stores this character's shared combat attributes. */
	UPROPERTY(
		EditDefaultsOnly,
		BlueprintReadOnly,
		Category = "Attributes",
		meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UADAttributeSet> AttributeSet;

	/** Instant effect used to initialize this character's attributes. */
	UPROPERTY(
		EditDefaultsOnly,
		BlueprintReadOnly,
		Category = "Attributes",
		meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UGameplayEffect> InitialAttributesEffect;

	UPROPERTY(
		VisibleInstanceOnly,
		BlueprintReadOnly,
		Category = "Combat",
		meta = (AllowPrivateAccess = "true"))
	bool bIsDead = false;
};
