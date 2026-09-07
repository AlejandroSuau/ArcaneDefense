// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameFramework/Actor.h"
#include "ADDefenseObjective.generated.h"

class UADAttributeSet;
class UAbilitySystemComponent;
class UGameplayEffect;
class USceneComponent;

struct FOnAttributeChangeData;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(
	FADObjectiveHealthChangedSignature,
	float,
	CurrentHealth,
	float,
	MaxHealth
);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(
	FADObjectiveDefeatedSignature
);

/**
 * Central objective the player must defend.
 *
 * Enemies navigate toward this actor and damage its Health attribute.
 */
UCLASS()
class ARCANEDEFENSE_API AADDefenseObjective
	: public AActor
	, public IAbilitySystemInterface
{
	GENERATED_BODY()
	
public:	
	AADDefenseObjective();

	virtual UAbilitySystemComponent*
	GetAbilitySystemComponent() const override;

	UFUNCTION(BlueprintPure, Category = "Objective")
	float GetHealth() const;

	UFUNCTION(BlueprintPure, Category = "Objective")
	float GetMaxHealth() const;

	UFUNCTION(BlueprintPure, Category = "Objective")
	bool IsDefeated() const;

	UPROPERTY(
		BlueprintAssignable,
		Category = "Objective"
	)
	FADObjectiveHealthChangedSignature OnHealthChanged;

	UPROPERTY(
		BlueprintAssignable,
		Category = "Objective"
	)
	FADObjectiveDefeatedSignature OnObjectiveDefeated;
	
protected:
	virtual void BeginPlay() override;

	UFUNCTION(
		BlueprintImplementableEvent,
		Category = "Objective",
		meta = (DisplayName = "Objective Defeated")
	)
	void ReceiveObjectiveDefeated();

private:
	void InitializeAbilitySystem();
	void ApplyInitialAttributes();

	void HandleHealthChanged(
		const FOnAttributeChangeData& Data
	);

	void HandleOutOfHealth();

	UPROPERTY(
		VisibleAnywhere,
		BlueprintReadOnly,
		Category = "Components",
		meta = (AllowPrivateAccess = "true")
	)
	TObjectPtr<USceneComponent> SceneRoot;

	UPROPERTY(
		VisibleAnywhere,
		BlueprintReadOnly,
		Category = "Abilities",
		meta = (AllowPrivateAccess = "true")
	)
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY(
		VisibleAnywhere,
		BlueprintReadOnly,
		Category = "Attributes",
		meta = (AllowPrivateAccess = "true")
	)
	TObjectPtr<UADAttributeSet> AttributeSet;

	UPROPERTY(
		EditDefaultsOnly,
		BlueprintReadOnly,
		Category = "Attributes",
		meta = (AllowPrivateAccess = "true")
	)
	TSubclassOf<UGameplayEffect> InitialAttributesEffect;

	UPROPERTY(
		VisibleInstanceOnly,
		BlueprintReadOnly,
		Category = "Objective",
		meta = (AllowPrivateAccess = "true")
	)
	bool bIsDefeated = false;

};
