// JoystickPlugin is licensed under the MIT License.
// Copyright Jayden Maalouf. All Rights Reserved.

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

	void UpdateGyro(const FVector& InGyro, const int& InTimestamp)
	{
		Gyro = InGyro;
	}

	void UpdateAccelerometer(const FVector& InAccelerometer, const int& InTimestamp)
	{
		Accelerometer = InAccelerometer;
	}

	UPROPERTY(VisibleAnywhere, BlueprintReadonly, Category = "Joystick|Data")
	FVector Gyro;

	UPROPERTY(VisibleAnywhere, BlueprintReadonly, Category = "Joystick|Data")
	FVector Accelerometer;
};
