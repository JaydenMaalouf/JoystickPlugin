// JoystickPlugin is licensed under the MIT License.
// Copyright Jayden Maalouf. All Rights Reserved.

#pragma once

#include "ForceFeedbackDirectionType.generated.h"

UENUM(BlueprintType)
enum class EForceFeedbackDirectionType : uint8
{
	Polar = 0,
	Cartesian,
	Spherical,
	SteeringAxis
};
