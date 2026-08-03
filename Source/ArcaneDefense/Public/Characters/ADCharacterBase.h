#pragma once

#include "CoreMinimal.h"
#include  "AbilitySystemInterface.h"
#include "GameFramework/Character.h"
#include "ADCharacterBase.generated.h"

class UADAttributeSet;
class UAbilitySystemComponent;
class UGameplayEffect;

UCLASS(Abstract)
class ARCANEDEFENSE_API AADCharacterBase
	: public ACharacter
	, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	AADCharacterBase();

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	UFUNCTION(BlueprintPure, Category = "Attributes")
	float GetHealth() const;

	UFUNCTION(BlueprintPure, Category = "Attributes")
	float GetMaxHealth() const;

	UFUNCTION(BlueprintPure, Category = "Attributes")
	float GetMana() const;

	UFUNCTION(BlueprintPure, Category = "Attributes")
	float GetMaxMana() const;

protected:
	virtual void BeginPlay() override;

private:
	void InitializeAbilitySystem();
	void ApplyInitialAttributes();

	UPROPERTY(
		VisibleAnywhere,
		BlueprintReadOnly,
		Category = "Abilities",
		meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY(
		EditDefaultsOnly,
		BlueprintReadOnly,
		Category = "Attributes",
		meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UADAttributeSet> AttributeSet;

	UPROPERTY(
		EditDefaultsOnly,
		BlueprintReadOnly,
		Category = "Attributes",
		meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UGameplayEffect> InitialAttributesEffect;
};
