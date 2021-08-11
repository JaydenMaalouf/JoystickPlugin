#pragma once

#include "ForcedFeedbackConditionEffectType.Generated.h"

UENUM(BlueprintType)
enum class EForcedFeedbackConditionEffectType : uint8 
{
	SPRING,
	DAMPER,
	INERTIA,
	FRICTION
};