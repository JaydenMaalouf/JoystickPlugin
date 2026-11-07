// JoystickPlugin is licensed under the MIT License.
// Copyright Jayden Maalouf 2026. All Rights Reserved.

#pragma once

#include "JoystickGamepadType.generated.h"

// Mirrors SDL_GamepadType, resolved from the device's vendor and product ids rather than from its
// mapping (SDL_GetRealGamepadTypeForID)
UENUM(BlueprintType)
enum class EJoystickGamepadType : uint8
{
	Unknown = 0,
	Standard,
	Xbox360,
	XboxOne,
	PlayStation3,
	PlayStation4,
	PlayStation5,
	NintendoSwitchPro,
	NintendoSwitchJoyConLeft,
	NintendoSwitchJoyConRight,
	NintendoSwitchJoyConPair,
	GameCube
};
