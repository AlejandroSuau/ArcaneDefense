#include "AbilitySystem/ADGameplayTags.h"

namespace ADGameplayTags
{
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(
		State_Casting,
		"State.Casting",
		"The character is currently casting an ability");

	UE_DEFINE_GAMEPLAY_TAG_COMMENT(
		State_Rooted,
		"State.Rooted",
		"The actor is unable to move.");
	
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(
		Ability_CancelOnMovement,
		"Ability.CancelOnMovement",
		"Active abilities with this tag are cancelled by movement.");
	
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(
		Effect_Root,
		"Effect.Root",
		"Identifies a gameplay effect that applies root.");

	UE_DEFINE_GAMEPLAY_TAG_COMMENT(
		Cooldown_FrostNova,
		"Cooldown.Ability.FrostNova",
		"Cooldown state for Frost Nova.");

	UE_DEFINE_GAMEPLAY_TAG_COMMENT(
		Cooldown_Meteor,
		"Cooldown.Ability.Meteor",
		"Cooldown state for Meteor.");

	UE_DEFINE_GAMEPLAY_TAG_COMMENT(
		Data_Resource_Coins,
		"Data.Resource.Coins",
		"SetByCaller magnitude used for coin transactions.");
}