// JoystickPlugin is licensed under the MIT License.
// Copyright Jayden Maalouf. All Rights Reserved.

#pragma once

THIRD_PARTY_INCLUDES_START

#include "SDL_sensor.h"

THIRD_PARTY_INCLUDES_END

#include "MotionData.generated.h"

USTRUCT(BlueprintType)
struct JOYSTICKPLUGIN_API FMotionData
{
	GENERATED_BODY()
	
	static constexpr float GyroScale = 0.01f; // Gyroscope scaling factor

	void UpdateGyro(const FVector& InGyro, const int& InTimestamp)
	{
		if (InTimestamp != 0)
		{
			const float DeltaTime = FMath::Abs((InTimestamp - Timestamp) * 1000.0f);
		
			Tilt.X += InGyro.X * DeltaTime;
			Tilt.Y += InGyro.Y * DeltaTime;
			Tilt.Z += InGyro.Z * DeltaTime;

			// Calculate rotation rates using gyroscope data
			RotationRate.X = InGyro.X;
			RotationRate.Y = InGyro.Y;
			RotationRate.Z = InGyro.Z;	

			Gyro = InGyro;		
		}

		Timestamp = InTimestamp;
	}
	
	void UpdateAccelerometer(const int& InTimestamp)
	{
		//const float DeltaTime = FMath::Abs((InTimestamp - Timestamp) / 1000.0f);

		// Calculate gravity components using tilt angles
		Gravity.X = FMath::Sin(Tilt.Y * PI / 180.0f) * SDL_STANDARD_GRAVITY;
		Gravity.Y = -FMath::Sin(Tilt.X * PI / 180.0f) * SDL_STANDARD_GRAVITY;
		Gravity.Z = FMath::Sqrt(Accelerometer.X * Accelerometer.X + Accelerometer.Y * Accelerometer.Y + Accelerometer.Z * Accelerometer.Z) * SDL_STANDARD_GRAVITY;

		// Calculate linear acceleration components by subtracting gravity
		Acceleration.X = Accelerometer.X - Gravity.X;
		Acceleration.Y = Accelerometer.Y - Gravity.Y;
		Acceleration.Z = Accelerometer.Z - Gravity.Z;
	}

	UPROPERTY(VisibleAnywhere, BlueprintReadonly, Category = "Joystick|Data")
	FVector Tilt;

	UPROPERTY(VisibleAnywhere, BlueprintReadonly, Category = "Joystick|Data")
	FVector RotationRate;

	UPROPERTY(VisibleAnywhere, BlueprintReadonly, Category = "Joystick|Data")
	FVector Gravity;

	UPROPERTY(VisibleAnywhere, BlueprintReadonly, Category = "Joystick|Data")
	FVector Acceleration;

	UPROPERTY(VisibleAnywhere, BlueprintReadonly, Category = "Joystick|Data")
	FVector Gyro;	

	UPROPERTY(VisibleAnywhere, BlueprintReadonly, Category = "Joystick|Data")
	FVector Accelerometer;

	int Timestamp;
};
