// JoystickPlugin is licensed under the MIT License.
// Copyright Jayden Maalouf 2026. All Rights Reserved.

#pragma once

#include "Data/Input/AxisData.h"
#include "Data/Input/BallData.h"
#include "Data/Input/ButtonData.h"
#include "Data/Input/HatData.h"
#include "Data/Input/MotionData.h"

#include "JoystickDeviceState.generated.h"

// A device's full input state, each input in the order SDL indexes them.
USTRUCT(BlueprintType)
struct JOYSTICKPLUGIN_API FJoystickDeviceState
{
	GENERATED_BODY()

	// The analog axes.
	UPROPERTY(VisibleAnywhere, BlueprintReadonly, Category="Joystick|State")
	TArray<FAxisData> Axes;

	// The buttons.
	UPROPERTY(VisibleAnywhere, BlueprintReadonly, Category="Joystick|State")
	TArray<FButtonData> Buttons;

	// The hat switches.
	UPROPERTY(VisibleAnywhere, BlueprintReadonly, Category="Joystick|State")
	TArray<FHatData> Hats;

	// The trackballs.
	UPROPERTY(VisibleAnywhere, BlueprintReadonly, Category="Joystick|State")
	TArray<FBallData> Balls;

	// The gyro and accelerometer, on devices that have them.
	UPROPERTY(VisibleAnywhere, BlueprintReadonly, Category="Joystick|State")
	FMotionData Motion;
};
