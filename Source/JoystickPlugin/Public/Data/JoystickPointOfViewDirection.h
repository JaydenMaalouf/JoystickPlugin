// JoystickPlugin is licensed under the MIT License.
// Copyright Jayden Maalouf. All Rights Reserved.

#pragma once

#include "JoystickPointOfViewDirection.generated.h"

UENUM(BlueprintType)
enum class EJoystickPointOfViewDirection : uint8
{
	None,
	Up,
	Up_Right,
	Right,
	Down_Right,
	Down,
	Down_Left,
	Left,
	Up_Left
};
