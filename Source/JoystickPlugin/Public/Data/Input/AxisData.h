// JoystickPlugin is licensed under the MIT License.
// Copyright Jayden Maalouf. All Rights Reserved.

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
		  , bGamepadStick(false)
	{
	}

	float MapValue(const float Input) const
	{
		const float NormalizedValue = (InvertInput ? (Input * -1.0f) : Input);
		const float OffsetNormalizedValue = NormalizedValue + InputOffset;
		const float MappedValue = FMath::GetMappedRangeValueClamped(FVector2D(InputRangeMin, InputRangeMax), FVector2D(OutputRangeMin, OutputRangeMax), OffsetNormalizedValue);
		return InvertOutput ? (MappedValue * -1.0f) : MappedValue;
	}

	/* Whether the data represents a valid value */
	bool HasValue() const
	{
		return (InputRangeMin != -1.0f || InputRangeMax != -1.0f);
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
		return MapValue(PreviousValue);
	}

	void Update(const float& InValue)
	{
		PreviousValue = Value;
		Value = InValue;
	}

	/* Current analog value */
	UPROPERTY(VisibleAnywhere, BlueprintReadonly, Category = "Joystick|Data")
	float Value;

	/* Last analog value */
	UPROPERTY(VisibleAnywhere, BlueprintReadonly, Category = "Joystick|Data")
	float PreviousValue;

	/* Should remap ranges */
	UPROPERTY(VisibleAnywhere, BlueprintReadonly, Category = "Joystick|Data")
	bool RemappingEnabled;

	/* Offset to apply to normalized axis value */
	UPROPERTY(VisibleAnywhere, BlueprintReadonly, Category = "Joystick|Data")
	float InputOffset;

	UPROPERTY(VisibleAnywhere, BlueprintReadonly, Category = "Joystick|Data")
	bool InvertInput;

	/* Min Analog value */
	UPROPERTY(VisibleAnywhere, BlueprintReadonly, Category = "Joystick|Data")
	float InputRangeMin;

	/* Max analog value */
	UPROPERTY(VisibleAnywhere, BlueprintReadonly, Category = "Joystick|Data")
	float InputRangeMax;

	/* Min Analog value */
	UPROPERTY(VisibleAnywhere, BlueprintReadonly, Category = "Joystick|Data")
	float OutputRangeMin;

	/* Max analog value */
	UPROPERTY(VisibleAnywhere, BlueprintReadonly, Category = "Joystick|Data")
	float OutputRangeMax;

	/* Is this axis inverted */
	UPROPERTY(VisibleAnywhere, BlueprintReadonly, Category = "Joystick|Data")
	bool InvertOutput;

	/* Is this axis centered on 0 instead of 0.5 */
	UPROPERTY(VisibleAnywhere, BlueprintReadonly, Category = "Joystick|Data")
	bool bGamepadStick;
};
