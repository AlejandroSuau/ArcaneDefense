#include "AbilitySystem/Abilities/ADGameplayAbility.h"

#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

UADGameplayAbility::UADGameplayAbility()
{
	// One ability instance is created and reused for each owning actor.
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

bool UADGameplayAbility::IsAvatarGrounded(const FGameplayAbilityActorInfo* ActorInfo) const
{
	if (!ActorInfo)	{ return false;	}

	const ACharacter* Character = Cast<ACharacter>(ActorInfo->AvatarActor.Get());
	if (!IsValid(Character)) { return false; }

	const UCharacterMovementComponent* MovementComponent = Character->GetCharacterMovement();
	return (IsValid(MovementComponent) && !MovementComponent->IsFalling());
}