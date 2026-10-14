// JoystickPlugin is licensed under the MIT License.
// Copyright Jayden Maalouf 2026. All Rights Reserved.

#pragma once

#include "BallData.generated.h"

// A trackball's state.
USTRUCT(BlueprintType)
struct JOYSTICKPLUGIN_API FBallData
{
	GENERATED_BODY()

	FBallData()
		: Direction(FVector2D::ZeroVector)
		  , PreviousDirection(FVector2D::ZeroVector)
		  , Timestamp(0)
	{
	}

	FVector2D GetValue() const
	{
		return Direction;
	}

	FVector2D GetPreviousValue() const
	{
		return PreviousDirection;
	}

	void Update(const FVector2D& InValue, const int64 InTimestamp)
	{
		PreviousDirection = Direction;
		Direction = InValue;
		Timestamp = InTimestamp;
	}

	// Marks the current value as handled, so it no longer reads as changed from the previous one.
	void Processed()
	{
		PreviousDirection = Direction;
	}

	// How far the ball moved in its last report, in the device's own units.
	UPROPERTY(VisibleAnywhere, BlueprintReadonly, Category="Joystick|Ball")
	FVector2D Direction;

	// The movement before the last update.
	UPROPERTY(VisibleAnywhere, BlueprintReadonly, Category="Joystick|Ball")
	FVector2D PreviousDirection;

	// When SDL read the current value, in nanoseconds on UJoystickFunctionLibrary::GetTicksNS()'s clock.
	UPROPERTY(VisibleAnywhere, BlueprintReadonly, Category="Joystick|Ball")
	int64 Timestamp;
};
