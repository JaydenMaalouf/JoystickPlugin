// JoystickPlugin is licensed under the MIT License.
// Copyright Jayden Maalouf. All Rights Reserved.

#pragma once

#include "MotionData.generated.h"

USTRUCT(BlueprintType)
struct JOYSTICKPLUGIN_API FMotionData
{
	GENERATED_BODY()

	FMotionData()
	{
	}

	FMotionData(const FVector& InValue)
		: Value(InValue)
	{
	}

	FVector GetValue() const
	{
		return Value;
	}

	FVector GetPreviousValue() const
	{
		return PreviousValue;
	}

	UPROPERTY(VisibleAnywhere, BlueprintReadonly, Category = "Joystick|Data")
	FVector Value;

	UPROPERTY(VisibleAnywhere, BlueprintReadonly, Category = "Joystick|Data")
	FVector PreviousValue;
};
