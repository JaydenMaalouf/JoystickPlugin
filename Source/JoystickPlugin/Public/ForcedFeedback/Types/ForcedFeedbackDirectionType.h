#pragma once

#include "ForcedFeedbackDirectionType.Generated.h"

UENUM(BlueprintType)
enum class EForcedFeedbackDirectionType : uint8 
{
	POLAR = 0,
	CARTESIAN = 1,
	SPHERICAL = 2,
	STEERING_AXIS = 3
};