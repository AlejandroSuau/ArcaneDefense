#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"
#include "ADPlayerResourceAttributeSet.generated.h"

struct FGameplayEffectModCallbackData;

/**
 * Player-only resources that participate in the Gameplay Ability System.
 *
 * Kept separate from the shared combat AttributeSet because enemies and
 * objectives do not own construction resources.
 */
UCLASS()
class ARCANEDEFENSE_API UADPlayerResourceAttributeSet
	: public UAttributeSet
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadOnly, Category = "Resources")
	FGameplayAttributeData Coins;

	GAMEPLAYATTRIBUTE_PROPERTY_GETTER(UADPlayerResourceAttributeSet, Coins)
	GAMEPLAYATTRIBUTE_VALUE_GETTER(Coins)
	GAMEPLAYATTRIBUTE_VALUE_SETTER(Coins)
	GAMEPLAYATTRIBUTE_VALUE_INITTER(Coins)

	virtual void PreAttributeChange(
		const FGameplayAttribute& Attribute,
		float& NewValue) override;

	virtual void PostGameplayEffectExecute(
		const FGameplayEffectModCallbackData& Data) override;
};