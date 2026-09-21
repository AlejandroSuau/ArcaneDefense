#pragma once

#include "Engine/EngineTypes.h"

namespace ADCollisionChannels
{
	/*
	 * Check Project Settings > Collision if the project uses
	 * different GameTraceChannel indexes.
	 */
	inline constexpr ECollisionChannel TrapPlacementSlot = ECC_GameTraceChannel1;
	inline constexpr ECollisionChannel TrapPlacementBlocker = ECC_GameTraceChannel2;
}