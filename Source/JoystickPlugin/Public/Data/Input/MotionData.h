// JoystickPlugin is licensed under the MIT License.
// Copyright Jayden Maalouf. All Rights Reserved.

#pragma once

#include "MotionData.generated.h"

USTRUCT(BlueprintType)
struct JOYSTICKPLUGIN_API FMotionData
{
	GENERATED_BODY()

	FMotionData()
		: Timestamp(0)
	{
	}

	FMotionData(const FVector& InValue, const int& InTimestamp)
		: Value(InValue)
		  , Timestamp(InTimestamp)
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

	void Update(const FVector& InValue, const int InTimestamp)
	{
		PreviousValue = Value;
		Value = InValue;
		Timestamp = InTimestamp;
	}

	UPROPERTY(VisibleAnywhere, BlueprintReadonly, Category = "Joystick|Data")
	FVector Value;

	UPROPERTY(VisibleAnywhere, BlueprintReadonly, Category = "Joystick|Data")
	FVector PreviousValue;

	UPROPERTY(VisibleAnywhere, BlueprintReadonly, Category = "Joystick|Data")
	int Timestamp;
};
