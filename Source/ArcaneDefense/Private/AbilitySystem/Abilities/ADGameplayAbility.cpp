#include "AbilitySystem/Abilities/ADGameplayAbility.h"

#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Animation/AnimMontage.h"

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

void UADGameplayAbility::FinishActivationMontage()
{
	if (!IsValid(ActivationMontage)) { return; }

	static const FName ChannelSectionName(TEXT("Channel"));
	static const FName EndSectionName(TEXT("Release"));
	/*const bool bHasChannelSection =	(
		ActivationMontage->GetSectionIndex(ChannelSectionName) != INDEX_NONE);
	const bool bHasEndSection = (
		ActivationMontage->GetSectionIndex(EndSectionName) != INDEX_NONE);
	if (!bHasChannelSection || !bHasEndSection) { return; }*/
	
	MontageJumpToSection(EndSectionName);
}

void UADGameplayAbility::PlayActivationMontage()
{
	if (!IsValid(ActivationMontage)) { return; }

	auto* MontageTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(
		this,
		NAME_None,
		ActivationMontage,
		ActivationMontagePlayRate,
		NAME_None,
		bStopMontageWhenAbilityEnds);

	if (!IsValid(MontageTask)) { return; }

	MontageTask->ReadyForActivation();
}

FText UADGameplayAbility::GetAbilityDisplayName() const
{
	return AbilityDisplayName;
}

UTexture2D* UADGameplayAbility::GetAbilityIcon() const
{
	return AbilityIcon;
}