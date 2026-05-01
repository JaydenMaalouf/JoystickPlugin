// JoystickPlugin is licensed under the MIT License.
// Copyright Jayden Maalouf 2026. All Rights Reserved.

#pragma once

#include "JoystickPrioritisationType.generated.h"

UENUM(BlueprintType)
enum class EJoystickPrioritisationType : uint8
{
	Lowest = 0,
	Highest
};
