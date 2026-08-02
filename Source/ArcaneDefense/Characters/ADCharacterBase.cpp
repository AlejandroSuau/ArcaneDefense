#include "ADCharacterBase.h"

AADCharacterBase::AADCharacterBase()
{
	// Character movement is handled by its components.
	// The Actor itself does not require a custom Tick at this stage.
	PrimaryActorTick.bCanEverTick = true;
}
