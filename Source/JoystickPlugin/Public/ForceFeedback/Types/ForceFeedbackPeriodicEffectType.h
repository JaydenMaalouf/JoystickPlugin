// JoystickPlugin is licensed under the MIT License.
// Copyright Jayden Maalouf 2026. All Rights Reserved.

#pragma once

#include "Misc/EnumRange.h"

#include "ForceFeedbackPeriodicEffectType.generated.h"

UENUM(BlueprintType)
enum class EForceFeedbackPeriodicEffectType : uint8
{
	Sine,
	Triangle,
	SawtoothUp,
	SawtoothDown,
	Square
};

ENUM_RANGE_BY_FIRST_AND_LAST(EForceFeedbackPeriodicEffectType, EForceFeedbackPeriodicEffectType::Sine, EForceFeedbackPeriodicEffectType::Square);
