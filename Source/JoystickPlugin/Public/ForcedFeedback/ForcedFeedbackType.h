#pragma once

#include "ForcedFeedbackType.Generated.h"

UENUM(BlueprintType)
enum class EForcedFeedbackType : uint8 
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