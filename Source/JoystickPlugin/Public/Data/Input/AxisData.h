// JoystickPlugin is licensed under the MIT License.
// Copyright Jayden Maalouf 2026. All Rights Reserved.

#pragma once

#include "AxisData.generated.h"

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
	{
	}

	float MapValue(const float Input) const
	{
		const float InvertedInput = InvertInput ? -Input : Input;
		const float OffsetNormalizedValue = InvertedInput + InputOffset;
		const float MappedValue = FMath::GetMappedRangeValueClamped(FVector2D(InputRangeMin, InputRangeMax), FVector2D(OutputRangeMin, OutputRangeMax), OffsetNormalizedValue);
		return InvertOutput ? -MappedValue : MappedValue;
	}

	float GetValue() const
	{
		if (RemappingEnabled)
		{
			return MapValue(Value);
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

	void Update(const float& InValue)
	{
		PreviousValue = Value;
		Value = InValue;
	}

	void Processed()
	{
		PreviousValue = Value;
	}

	/* Current analog value */
	UPROPERTY(VisibleAnywhere, BlueprintReadonly, Category="Joystick|Axis")
	float Value;

	/* Last analog value */
	UPROPERTY(VisibleAnywhere, BlueprintReadonly, Category="Joystick|Axis")
	float PreviousValue;

	/* Should remap ranges */
	UPROPERTY(VisibleAnywhere, BlueprintReadonly, Category="Joystick|Axis")
	bool RemappingEnabled;

	/* Offset to apply to analog value */
	UPROPERTY(VisibleAnywhere, BlueprintReadonly, Category="Joystick|Axis")
	float InputOffset;

	/* Invert input */
	UPROPERTY(VisibleAnywhere, BlueprintReadonly, Category="Joystick|Axis")
	bool InvertInput;

	/* Input min analog value */
	UPROPERTY(VisibleAnywhere, BlueprintReadonly, Category="Joystick|Axis")
	float InputRangeMin;

	/* Input max analog value */
	UPROPERTY(VisibleAnywhere, BlueprintReadonly, Category="Joystick|Axis")
	float InputRangeMax;

	/* Output min analog value */
	UPROPERTY(VisibleAnywhere, BlueprintReadonly, Category="Joystick|Axis")
	float OutputRangeMin;

	/* Output max analog value */
	UPROPERTY(VisibleAnywhere, BlueprintReadonly, Category="Joystick|Axis")
	float OutputRangeMax;

	/* Invert the processed output */
	UPROPERTY(VisibleAnywhere, BlueprintReadonly, Category="Joystick|Axis")
	bool InvertOutput;
};
