// JoystickPlugin is licensed under the MIT License.
// Copyright Jayden Maalouf. All Rights Reserved.

#pragma once

#include "JoystickSensorInformation.generated.h"

USTRUCT(BlueprintType)
struct JOYSTICKPLUGIN_API FJoystickSensorInformation
{
	GENERATED_BODY()

	FJoystickSensorInformation()
		: GyroSupport(false)
		  , AccelerometerSupport(false)
	{
	}

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Device Config/Supported Features")
	bool GyroSupport;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Device Config/Supported Features")
	bool AccelerometerSupport;
};
