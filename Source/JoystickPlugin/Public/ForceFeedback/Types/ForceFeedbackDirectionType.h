#pragma once

#include "Runtime/Launch/Resources/Version.h"

#include "ForceFeedbackDirectionType.Generated.h"

UENUM(BlueprintType)
enum class EForceFeedbackDirectionType : uint8 
{
	POLAR = 0,
	CARTESIAN = 1,
	SPHERICAL = 2,
#if (ENGINE_MAJOR_VERSION == 4 && ENGINE_MINOR_VERSION >= 26 || ENGINE_MAJOR_VERSION > 4)
	STEERING_AXIS = 3
#endif
};