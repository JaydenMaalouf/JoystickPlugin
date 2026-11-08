// JoystickPlugin is licensed under the MIT License.
// Copyright Jayden Maalouf 2026. All Rights Reserved.

#pragma once

#include "JoystickGamepadType.generated.h"

UENUM(BlueprintType, meta=(Bitflags))
enum class EJoystickGamepadType : uint8
{
	Unknown = 0,
	Standard UMETA(ToolTip="All other gamepad types"),
	Xbox360,
	XboxOne,
	PlayStation3 UMETA(DisplayName="PlayStation 3"),
	PlayStation4 UMETA(DisplayName="PlayStation 4"),
	PlayStation5 UMETA(DisplayName="PlayStation 5"),
	NintendoSwitchPro,
	NintendoSwitchJoyConLeft UMETA(DisplayName="Nintendo Switch Joy-Con Left"),
	NintendoSwitchJoyConRight UMETA(DisplayName="Nintendo Switch Joy-Con Right"),
	NintendoSwitchJoyConPair UMETA(DisplayName="Nintendo Switch Joy-Con Pair"),
	GameCube UMETA(DisplayName="GameCube")
};
