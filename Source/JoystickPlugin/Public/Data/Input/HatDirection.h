// JoystickPlugin is licensed under the MIT License.
// Copyright Jayden Maalouf. All Rights Reserved.

#pragma once

#include "HatDirection.generated.h"

UENUM(BlueprintType, meta=(Bitflags, UseEnumValuesAsMaskValuesInEditor="true"))
enum class EHatDirection : uint8
{
	None = 0,
	Up = 1 << 0,
	Right_Up = 1 << 1,
	Right = 1 << 2,
	Right_Down = 1 << 3,
	Down = 1 << 4,
	Left_Down = 1 << 5,
	Left = 1 << 6,
	Left_Up = 1 << 7
};

ENUM_CLASS_FLAGS(EHatDirection);
