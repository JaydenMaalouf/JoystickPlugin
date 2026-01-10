// JoystickPlugin is licensed under the MIT License.
// Copyright Jayden Maalouf 2026. All Rights Reserved.

#pragma once

#include "ForceFeedbackPeriodicEffectType.generated.h"

UENUM(BlueprintType)
enum class EForceFeedbackPeriodicEffectType : uint8
{
	Sine,
	Triangle,
	SawtoothUp,
	SawtoothDown,
	LeftRight
};
