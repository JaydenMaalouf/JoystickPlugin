// JoystickPlugin is licensed under the MIT License.
// Copyright Jayden Maalouf 2026. All Rights Reserved.

#pragma once

#include "MotionData.generated.h"

// A device's motion sensors. Each sensor reports on its own, so each has its own timestamp.
USTRUCT(BlueprintType)
struct JOYSTICKPLUGIN_API FMotionData
{
	GENERATED_BODY()

	FMotionData()
		: Gyro(FVector::ZeroVector)
		  , GyroTimestamp(0)
		  , Accelerometer(FVector::ZeroVector)
		  , AccelerometerTimestamp(0)
	{
	}

	void UpdateGyro(const FVector& InGyro, const int64 InTimestamp)
	{
		Gyro = InGyro;
		GyroTimestamp = InTimestamp;
	}

	void UpdateAccelerometer(const FVector& InAccelerometer, const int64 InTimestamp)
	{
		Accelerometer = InAccelerometer;
		AccelerometerTimestamp = InTimestamp;
	}

	// The gyro's rotation rate, in radians per second, as SDL reports it.
	UPROPERTY(VisibleAnywhere, BlueprintReadonly, Category="Joystick|Motion")
	FVector Gyro;

	// When SDL read the current gyro value, in nanoseconds.
	UPROPERTY(VisibleAnywhere, BlueprintReadonly, Category="Joystick|Motion")
	int64 GyroTimestamp;

	// The accelerometer's reading, in metres per second squared, as SDL reports it.
	UPROPERTY(VisibleAnywhere, BlueprintReadonly, Category="Joystick|Motion")
	FVector Accelerometer;

	// When SDL read the current accelerometer value, in nanoseconds.
	UPROPERTY(VisibleAnywhere, BlueprintReadonly, Category="Joystick|Motion")
	int64 AccelerometerTimestamp;
};
