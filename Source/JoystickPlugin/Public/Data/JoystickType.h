// JoystickPlugin is licensed under the MIT License.
// Copyright Jayden Maalouf. All Rights Reserved.

#pragma once

#include "JoystickType.generated.h"

UENUM(BlueprintType)
enum class EJoystickType : uint8
{
	Unknown = 0,
	GameController,
	Wheel,
	ArcadeStick,
	FlightStick,
	DancePad,
	Guitar,
	DrumKit,
	ArcadePad,
	Throttle
};
