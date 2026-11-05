// JoystickPlugin is licensed under the MIT License.
// Copyright Jayden Maalouf 2026. All Rights Reserved.

#pragma once

#include "JoystickGamepadType.generated.h"

/**
 * Mirrors SDL_GamepadType. Which family of gamepad a device is, resolved from its USB vendor and
 * product ids rather than from its mapping - SDL_GetRealGamepadTypeForID.
 *
 * Standard is the answer for every mapped device SDL does not recognise as a console pad, which is
 * what makes it useful: a flight stick or a throttle that happens to carry a mapping lands there,
 * and a real pad does not.
 */
UENUM(BlueprintType)
enum class EJoystickGamepadType : uint8
{
	Unknown = 0,
	Standard,
	Xbox360,
	XboxOne,
	PS3,
	PS4,
	PS5,
	NintendoSwitchPro,
	NintendoSwitchJoyConLeft,
	NintendoSwitchJoyConRight,
	NintendoSwitchJoyConPair,
	GameCube
};
