// JoystickPlugin is licensed under the MIT License.
// Copyright Jayden Maalouf. All Rights Reserved.

#pragma once

#include "JoystickPOVDirection.generated.h"

UENUM(BlueprintType)
enum class EJoystickPOVDirection : uint8
{
	Direction_None,
	Direction_Up,
	Direction_Up_Right,
	Direction_Right,
	Direction_Down_Right,
	Direction_Down,
	Direction_Down_Left,
	Direction_Left,
	Direction_Up_Left
};
