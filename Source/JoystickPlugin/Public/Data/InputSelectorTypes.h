// JoystickPlugin is licensed under the MIT License.
// Copyright Jayden Maalouf 2026. All Rights Reserved.

#pragma once

#include "InputSelectorTypes.generated.h"

UENUM(BlueprintType, meta = (Bitflags, UseEnumValuesAsMaskValuesInEditor = "true"))
enum class EInputSelectorTypes : uint8
{
	None = 0 UMETA(Hidden),
	Keyboard = 1 << 0,
	Mouse = 1 << 1,
	Gamepad = 1 << 2,
	Joystick = 1 << 3,
};

ENUM_CLASS_FLAGS(EInputSelectorTypes)
