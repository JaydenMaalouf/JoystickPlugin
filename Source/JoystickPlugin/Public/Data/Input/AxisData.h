// JoystickPlugin is licensed under the MIT License.
// Copyright Jayden Maalouf 2026. All Rights Reserved.

#pragma once
#include "Data/Settings/JoystickAxisRemapping.h"

#include "AxisData.generated.h"

// An analog axis' state, with the remapping its configuration applies on read.
USTRUCT(BlueprintType)
struct JOYSTICKPLUGIN_API FAxisData : public FJoystickAxisRemapping
{
	GENERATED_BODY()

	FAxisData()
		: Value(0.0f)
		  , PreviousValue(0.0f)
		  , Timestamp(0)
	{
	}

	float GetValue() const
	{
		return MapValue(Value);
	}

	// The current value as the given remapping would map it, for previewing a configuration before it is applied.
	float GetMockValue(const FJoystickAxisRemapping& Remapping) const
	{
		return Remapping.MapValue(Value);
	}

	float GetPreviousValue() const
	{
		return MapValue(PreviousValue);
	}

	void Update(const float& InValue, const int64 InTimestamp)
	{
		PreviousValue = Value;
		Value = InValue;
		Timestamp = InTimestamp;
	}

	// Marks the current value as handled, so it no longer reads as changed from the previous one.
	void Processed()
	{
		PreviousValue = Value;
	}

	// The current raw value, in -1..1.
	UPROPERTY(VisibleAnywhere, BlueprintReadonly, Category="Joystick|Axis")
	float Value;

	// The raw value before the last update, in -1..1.
	UPROPERTY(VisibleAnywhere, BlueprintReadonly, Category="Joystick|Axis")
	float PreviousValue;

	// When SDL read the current value, in nanoseconds.
	UPROPERTY(VisibleAnywhere, BlueprintReadonly, Category="Joystick|Axis")
	int64 Timestamp;
};
