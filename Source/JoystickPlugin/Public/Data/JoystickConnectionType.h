// JoystickPlugin is licensed under the MIT License.
// Copyright Jayden Maalouf 2026. All Rights Reserved.

#pragma once

#include "JoystickConnectionType.generated.h"

UENUM(BlueprintType)
enum class EJoystickConnectionType : uint8
{
	Invalid = 0,
	Unknown,
	Wired,
	Wireless
};
