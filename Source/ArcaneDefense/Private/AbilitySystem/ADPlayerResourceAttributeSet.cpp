#include "AbilitySystem/ADPlayerResourceAttributeSet.h"

#include "GameplayEffectExtension.h"
#include "GameplayEffectTypes.h"

void UADPlayerResourceAttributeSet::PreAttributeChange(
	const FGameplayAttribute& Attribute,
	float& NewValue)
{
	Super::PreAttributeChange(Attribute, NewValue);

	if (Attribute == GetCoinsAttribute())
	{
		NewValue = FMath::Max(0.0f, NewValue);
	}
}

void UADPlayerResourceAttributeSet::PostGameplayEffectExecute(
		const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);

	if (Data.EvaluatedData.Attribute == GetCoinsAttribute())
	{
		SetCoins(FMath::Max(0.0f, GetCoins()));
	}
}