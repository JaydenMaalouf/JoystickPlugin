// JoystickPlugin is licensed under the MIT License.
// Copyright Jayden Maalouf 2026. All Rights Reserved.

#pragma once

#include "JoystickPowerState.generated.h"

UENUM(BlueprintType)
enum class EJoystickPowerState : uint8
{
	Error = 0,
	Unknown,
	OnBattery,
	OffBattery,
	Charging,
	Charged
};
