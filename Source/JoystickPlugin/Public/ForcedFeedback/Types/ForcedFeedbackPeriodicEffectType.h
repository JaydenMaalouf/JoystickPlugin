#pragma once

#include "ForcedFeedbackPeriodicEffectType.Generated.h"

UENUM(BlueprintType)
enum class EForcedFeedbackPeriodicEffectType : uint8 
{
	SINE,
	TRIANGLE,
	SAWTOOTHUP,
	SAWTOOTHDOWN,
	LEFTRIGHT
};