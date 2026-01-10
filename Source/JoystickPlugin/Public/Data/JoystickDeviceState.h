// JoystickPlugin is licensed under the MIT License.
// Copyright Jayden Maalouf 2026. All Rights Reserved.

#pragma once

#include "Data/Input/AxisData.h"
#include "Data/Input/BallData.h"
#include "Data/Input/ButtonData.h"
#include "Data/Input/HatData.h"
#include "Data/Input/MotionData.h"

#include "JoystickDeviceState.generated.h"

USTRUCT(BlueprintType)
struct FJoystickDeviceState
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadonly, Category="Joystick|State")
	TArray<FAxisData> Axes;

	UPROPERTY(VisibleAnywhere, BlueprintReadonly, Category="Joystick|State")
	TArray<FButtonData> Buttons;

	UPROPERTY(VisibleAnywhere, BlueprintReadonly, Category="Joystick|State")
	TArray<FHatData> Hats;

	UPROPERTY(VisibleAnywhere, BlueprintReadonly, Category="Joystick|State")
	TArray<FBallData> Balls;

	UPROPERTY(VisibleAnywhere, BlueprintReadonly, Category="Joystick|State")
	FMotionData Motion;
};
