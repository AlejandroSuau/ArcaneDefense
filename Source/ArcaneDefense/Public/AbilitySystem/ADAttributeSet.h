// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "AttributeSet.h"
#include "ADAttributeSet.generated.h"

/**
 *   Generates the standard accessors used by Gameplay Attributes.
 */
#define AD_ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

/**
 * Contains the shared combat attributes used by Arcane Defense characters.
 */
UCLASS()
class ARCANEDEFENSE_API UADAttributeSet : public UAttributeSet
{
	GENERATED_BODY()

public:
	virtual void PreAttributeChange(
		const FGameplayAttribute& Attribute,
		float& NewValue) override;

	virtual void PostGameplayEffectExecute(
		const FGameplayEffectModCallbackData& Data
	) override;

protected:
	UPROPERTY(
		BlueprintReadOnly,
		Category = "Attributes|Health")
	FGameplayAttributeData Health;

	UPROPERTY(
		BlueprintReadOnly,
		Category="Attributes|Health")
	FGameplayAttributeData MaxHealth;

	UPROPERTY(
		BlueprintReadOnly,
		Category = "Attributes|Mana")
	FGameplayAttributeData Mana;

	UPROPERTY(
		BlueprintReadOnly,
		Category = "Attributes|Mana")
	FGameplayAttributeData MaxMana;	

public:
	AD_ATTRIBUTE_ACCESSORS(UADAttributeSet, Health)
	AD_ATTRIBUTE_ACCESSORS(UADAttributeSet, MaxHealth)
	AD_ATTRIBUTE_ACCESSORS(UADAttributeSet, Mana)
	AD_ATTRIBUTE_ACCESSORS(UADAttributeSet, MaxMana)
};
