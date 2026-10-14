// JoystickPlugin is licensed under the MIT License.
// Copyright Jayden Maalouf 2026. All Rights Reserved.

#pragma once
#include "Data/Settings/JoystickInputDeviceAxisProperties.h"

#include "AxisData.generated.h"

// An analog axis' state, with the remapping its configuration applies on read.
USTRUCT(BlueprintType)
struct JOYSTICKPLUGIN_API FAxisData
{
	GENERATED_BODY()

	FAxisData()
		: Value(0.0f)
		  , PreviousValue(0.0f)
		  , RemappingEnabled(false)
		  , InputOffset(0.0f)
		  , InvertInput(false)
		  , InputRangeMin(0.0f)
		  , InputRangeMax(1.0f)
		  , OutputRangeMin(0.0f)
		  , OutputRangeMax(1.0f)
		  , InvertOutput(false)
		  , Timestamp(0)
	{
	}

	float MapValue(const float Input) const
	{
		const float InvertedInput = InvertInput ? -Input : Input;
		const float OffsetNormalizedValue = InvertedInput + InputOffset;
		const float MappedValue = FMath::GetMappedRangeValueClamped(FVector2D(InputRangeMin, InputRangeMax), FVector2D(OutputRangeMin, OutputRangeMax), OffsetNormalizedValue);
		return InvertOutput ? -MappedValue : MappedValue;
	}

	// Maps with the given properties rather than the ones stored here, for previewing a configuration before it is applied.
	float MapValue(const FJoystickInputDeviceAxisProperties& AxisProperties, const float Input) const
	{
		const float InvertedInput = AxisProperties.InvertInput ? -Input : Input;
		const float OffsetNormalizedValue = InvertedInput + AxisProperties.InputOffset;
		if (AxisProperties.RerangeInput || AxisProperties.RerangeOutput)
		{
			const float MappedValue = FMath::GetMappedRangeValueClamped(FVector2D(AxisProperties.InputRangeMin, AxisProperties.InputRangeMax), FVector2D(AxisProperties.OutputRangeMin, AxisProperties.OutputRangeMax), OffsetNormalizedValue);
			return AxisProperties.InvertOutput ? -MappedValue : MappedValue;
		}
		return AxisProperties.InvertOutput ? -OffsetNormalizedValue : OffsetNormalizedValue;
	}

	float GetValue() const
	{
		if (RemappingEnabled)
		{
			return MapValue(Value);
		}
		return Value;
	}

	// The current value as the given properties would remap it. See MapValue.
	float GetMockValue(const FJoystickInputDeviceAxisProperties& AxisProperties) const
	{
		if (AxisProperties.RemappingEnabled)
		{
			return MapValue(AxisProperties, Value);
		}

		return Value;
	}

	float GetPreviousValue() const
	{
		if (RemappingEnabled)
		{
			return MapValue(PreviousValue);
		}
		return PreviousValue;
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

	// Whether GetValue() and GetPreviousValue() apply the remapping below.
	UPROPERTY(VisibleAnywhere, BlueprintReadonly, Category="Joystick|Axis")
	bool RemappingEnabled;

	// Added to the value after the input inversion.
	UPROPERTY(VisibleAnywhere, BlueprintReadonly, Category="Joystick|Axis")
	float InputOffset;

	// Inverts the value before it is offset and remapped.
	UPROPERTY(VisibleAnywhere, BlueprintReadonly, Category="Joystick|Axis")
	bool InvertInput;

	// The input range's minimum, mapped to OutputRangeMin.
	UPROPERTY(VisibleAnywhere, BlueprintReadonly, Category="Joystick|Axis")
	float InputRangeMin;

	// The input range's maximum, mapped to OutputRangeMax.
	UPROPERTY(VisibleAnywhere, BlueprintReadonly, Category="Joystick|Axis")
	float InputRangeMax;

	// The output range's minimum.
	UPROPERTY(VisibleAnywhere, BlueprintReadonly, Category="Joystick|Axis")
	float OutputRangeMin;

	// The output range's maximum.
	UPROPERTY(VisibleAnywhere, BlueprintReadonly, Category="Joystick|Axis")
	float OutputRangeMax;

	// Inverts the value after it is remapped.
	UPROPERTY(VisibleAnywhere, BlueprintReadonly, Category="Joystick|Axis")
	bool InvertOutput;

	// When SDL read the current value, in nanoseconds on UJoystickFunctionLibrary::GetTicksNS()'s clock.
	UPROPERTY(VisibleAnywhere, BlueprintReadonly, Category="Joystick|Axis")
	int64 Timestamp;
};
