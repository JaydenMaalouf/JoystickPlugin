// JoystickPlugin is licensed under the MIT License.
// Copyright Jayden Maalouf. All Rights Reserved.

#pragma once

#include "JoystickIdentifierType.generated.h"

UENUM(BlueprintType)
enum class EJoystickIdentifierType : uint8
{
	Hashed = 0,
	Legacy
};
