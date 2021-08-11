#pragma once

#include "ForcedFeedbackDirectionType.Generated.h"

UENUM(BlueprintType)
enum class EForcedFeedbackDirectionType : uint8 
{
	CARTESIAN,
	POLAR,
	SPHERICAL
};