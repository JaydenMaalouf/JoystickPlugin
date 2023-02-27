// JoystickPlugin is licensed under the MIT License.
// Copyright Jayden Maalouf. All Rights Reserved.

#pragma once

#include "InputType.generated.h"

UENUM(BlueprintType)
enum class EInputType : uint8
{
	InputType_Unknown,
	InputType_Joystick,
	InputType_GameController,
};
