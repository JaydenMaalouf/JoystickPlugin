#pragma once

#include "ForcedFeedbackEffectType.Generated.h"

UENUM(BlueprintType)
enum class EForcedFeedbackEffectType : uint8 
{
	CONSTANT,
	SPRING,
	DAMPER,
	INERTIA,
	FRICTION,
	SINE,
	TRIANGLE,
	SAWTOOTHUP,
	SAWTOOTHDOWN,
	RAMP
	//LEFTRIGHT,
	//CUSTOM
};