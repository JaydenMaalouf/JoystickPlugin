// JoystickPlugin is licensed under the MIT License.
// Copyright Jayden Maalouf. All Rights Reserved.

#pragma once

#include "ForceFeedbackConditionEffectType.Generated.h"

UENUM(BlueprintType)
enum class EForceFeedbackConditionEffectType : uint8
{
	Spring,
	Damper,
	Inertia,
	Friction
};
