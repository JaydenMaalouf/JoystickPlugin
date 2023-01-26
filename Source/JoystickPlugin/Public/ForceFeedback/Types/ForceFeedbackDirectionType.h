#pragma once

#include "Runtime/Launch/Resources/Version.h"

#include "ForceFeedbackDirectionType.Generated.h"

UENUM(BlueprintType)
enum class EForceFeedbackDirectionType : uint8
{
	Polar,
	Cartesian,
	Spherical
};
