// JoystickPlugin is licensed under the MIT License.
// Copyright Jayden Maalouf 2026. All Rights Reserved.

#pragma once

#include "Data/Input/HatDirection.h"

#include "HatData.generated.h"

// A hat switch's state.
USTRUCT(BlueprintType)
struct JOYSTICKPLUGIN_API FHatData
{
	GENERATED_BODY()

	FHatData()
		: Direction(EHatDirection::None)
		  , PreviousDirection(EHatDirection::None)
		  , Timestamp(0)
	{
	}

	EHatDirection GetValue() const
	{
		return Direction;
	}

	EHatDirection GetPreviousValue() const
	{
		return PreviousDirection;
	}

	void Update(const EHatDirection& InValue, const int64 InTimestamp)
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

	// The direction the hat is pressed.
	UPROPERTY(VisibleAnywhere, BlueprintReadonly, Category="Joystick|Hat")
	EHatDirection Direction;

	// The direction before the last update.
	UPROPERTY(VisibleAnywhere, BlueprintReadonly, Category="Joystick|Hat")
	EHatDirection PreviousDirection;

	// When SDL read the current value, in nanoseconds on UJoystickFunctionLibrary::GetTicksNS()'s clock.
	UPROPERTY(VisibleAnywhere, BlueprintReadonly, Category="Joystick|Hat")
	int64 Timestamp;
};
