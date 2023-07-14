// JoystickPlugin is licensed under the MIT License.
// Copyright Jayden Maalouf. All Rights Reserved.

#pragma once

#include "AxisData.generated.h"

USTRUCT(BlueprintType)
struct JOYSTICKPLUGIN_API FAxisData
{
	GENERATED_BODY()

	FAxisData()
		: Value(0.f)
		  , PreviousValue(0.f)
		  , RemappingEnabled(false)
		  , InputOffset(0.f)
		  , InvertInput(false)
		  , InputRangeMin(0.f)
		  , InputRangeMax(1.f)
		  , OutputRangeMin(0.f)
		  , OutputRangeMax(1.f)
		  , InvertOutput(false)
		  , bGamepadStick(false)
	{
	}

	FAxisData(const float& InValue,
	          const float& InInputRangeMin, const float& InInputRangeMax, const float& InOutputRangeMin, const float& InOutputRangeMax,
	          const float& InOffset, const bool& bInInvertInput, const bool& bInInvertOutput, const bool& bInGamepadStick)
		: Value(InValue)
		  , PreviousValue(0.f)
		  , RemappingEnabled(false)
		  , InputOffset(InOffset)
		  , InvertInput(bInInvertInput)
		  , InputRangeMin(InInputRangeMin)
		  , InputRangeMax(InInputRangeMax)
		  , OutputRangeMin(InOutputRangeMin)
		  , OutputRangeMax(InOutputRangeMax)
		  , InvertOutput(bInInvertOutput)
		  , bGamepadStick(bInGamepadStick)
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
		return (InputRangeMin != -1.f || InputRangeMax != -1.f);
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
