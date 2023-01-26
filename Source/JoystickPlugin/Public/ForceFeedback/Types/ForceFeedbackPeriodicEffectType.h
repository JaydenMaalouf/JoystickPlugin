#pragma once

#include "ForceFeedbackPeriodicEffectType.Generated.h"

UENUM(BlueprintType)
enum class EForceFeedbackPeriodicEffectType : uint8
{
	Sine,
	Triangle,
	SawtoothUp,
	SawtoothDown,
	LeftRight
};
