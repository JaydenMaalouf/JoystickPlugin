#pragma once

#include "ForceFeedbackPeriodicEffectType.Generated.h"

UENUM(BlueprintType)
enum class EForceFeedbackPeriodicEffectType : uint8 
{
	SINE,
	TRIANGLE,
	SAWTOOTHUP,
	SAWTOOTHDOWN,
	LEFTRIGHT
};