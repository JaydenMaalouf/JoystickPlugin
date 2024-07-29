// JoystickPlugin is licensed under the MIT License.
// Copyright Jayden Maalouf. All Rights Reserved.

#pragma once

#include "Data/Input/AxisData.h"
#include "Data/Input/BallData.h"
#include "Data/Input/ButtonData.h"
#include "Data/Input/HatData.h"
#include "Data/Input/MotionData.h"

#include "JoystickDeviceState.generated.h"

USTRUCT(BlueprintType)
struct JOYSTICKPLUGIN_API FJoystickDeviceState
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadonly, Category = "JoystickState")
	TArray<FAxisData> Axes;

	UPROPERTY(VisibleAnywhere, BlueprintReadonly, Category = "JoystickState")
	TArray<FButtonData> Buttons;

	UPROPERTY(VisibleAnywhere, BlueprintReadonly, Category = "JoystickState")
	TArray<FHatData> Hats;

	UPROPERTY(VisibleAnywhere, BlueprintReadonly, Category = "JoystickState")
	TArray<FBallData> Balls;

	UPROPERTY(VisibleAnywhere, BlueprintReadonly, Category = "JoystickState")
	FMotionData Motion;
};
