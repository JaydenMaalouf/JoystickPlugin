// JoystickPlugin is licensed under the MIT License.
// Copyright Jayden Maalouf 2026. All Rights Reserved.

#pragma once

#include "MotionData.generated.h"

USTRUCT(BlueprintType)
struct JOYSTICKPLUGIN_API FMotionData
{
	GENERATED_BODY()

	FMotionData()
		: Gyro(FVector::ZeroVector)
		  , Accelerometer(FVector::ZeroVector)
	{
	}

	void UpdateGyro(const FVector& InGyro)
	{
		Gyro = InGyro;
	}

	void UpdateAccelerometer(const FVector& InAccelerometer)
	{
		Accelerometer = InAccelerometer;
	}

	UPROPERTY(VisibleAnywhere, BlueprintReadonly, Category="Joystick|Motion")
	FVector Gyro;

	UPROPERTY(VisibleAnywhere, BlueprintReadonly, Category="Joystick|Motion")
	FVector Accelerometer;
};
