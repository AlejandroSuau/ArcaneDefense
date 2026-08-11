#include "AbilitySystem/ADGameplayTags.h"

namespace ADGameplayTags
{
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(
		State_Casting,
		"State.Casting",
		"The character is currently casting an ability");
	 
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(
		Ability_CancelOnMovement,
		"Ability.CancelOnMovement",
		"Active abilities with this tag are cancelled by movement.");
}