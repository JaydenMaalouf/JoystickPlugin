// JoystickPlugin is licensed under the MIT License.
// Copyright Jayden Maalouf 2026. All Rights Reserved.

#pragma once

#include "Data/Input/HatDirection.h"

#include "HatData.generated.h"

USTRUCT(BlueprintType)
struct JOYSTICKPLUGIN_API FHatData
{
	GENERATED_BODY()

	FHatData()
		: Direction(EHatDirection::None)
		  , PreviousDirection(EHatDirection::None)
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

	void Update(const EHatDirection& InValue)
	{
		PreviousDirection = Direction;
		Direction = InValue;
	}

	void Processed()
	{
		PreviousDirection = Direction;
	}

	UPROPERTY(VisibleAnywhere, BlueprintReadonly, Category="Joystick|Hat")
	EHatDirection Direction;

	UPROPERTY(VisibleAnywhere, BlueprintReadonly, Category="Joystick|Hat")
	EHatDirection PreviousDirection;
};
