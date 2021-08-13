#pragma once

#include "ForceFeedbackDirectionType.Generated.h"

UENUM(BlueprintType)
enum class EForceFeedbackDirectionType : uint8 
{
	POLAR = 0,
	CARTESIAN = 1,
	SPHERICAL = 2,
	STEERING_AXIS = 3
};