// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilities/ADGameplayAbility.h"

UADGameplayAbility::UADGameplayAbility()
{
	// One ability instance is created and reused for each owning actor.
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}
