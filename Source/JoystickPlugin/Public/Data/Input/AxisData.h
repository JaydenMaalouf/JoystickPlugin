#pragma once

#include "InputCoreTypes.h"

#include "AxisData.generated.h"

USTRUCT(BlueprintType)
struct JOYSTICKPLUGIN_API FAxisData
{
	GENERATED_BODY()

	FAxisData()
		: Index(INDEX_NONE)
		  , Value(0.f)
		  , PreviousValue(0.f)
		  , bRemapRanges(false)
		  , InputOffset(0.f)
		  , bInvertInput(false)
		  , InputRangeMin(0.f)
		  , InputRangeMax(1.f)
		  , OutputRangeMin(0.f)
		  , OutputRangeMax(1.f)
		  , bInvertOutput(false)
		  , bGamepadStick(false)
	{
	}

	FAxisData(const int32 InIndex, const float InValue,
	          const float InInputRangeMin, const float InInputRangeMax, const float InOutputRangeMin, const float InOutputRangeMax,
	          const float InOffset, const bool bInInvertInput, const bool bInInvertOutput, const bool bInGamepadStick)
		: Index(InIndex)
		  , Value(InValue)
		  , PreviousValue(0.f)
		  , bRemapRanges(false)
		  , InputOffset(InOffset)
		  , bInvertInput(bInInvertInput)
		  , InputRangeMin(InInputRangeMin)
		  , InputRangeMax(InInputRangeMax)
		  , OutputRangeMin(InOutputRangeMin)
		  , OutputRangeMax(InOutputRangeMax)
		  , bInvertOutput(bInInvertOutput)
		  , bGamepadStick(bInGamepadStick)
	{
	}

	float GetValue() const
	{
		if (bRemapRanges)
		{
			return MapValue(Value);
		}
		return Value;
	}

	float GetPreviousValue() const
	{
		return MapValue(PreviousValue);
	}

	float MapValue(const float Input) const
	{
		const float NormalizedValue = (bInvertInput ? (Input * -1.0f) : Input);
		const float OffsetNormalizedValue = NormalizedValue + InputOffset;
		const float MappedValue = FMath::GetMappedRangeValueClamped(FVector2D(InputRangeMin, InputRangeMax), FVector2D(OutputRangeMin, OutputRangeMax), OffsetNormalizedValue);
		return bInvertOutput ? (MappedValue * -1.0f) : MappedValue;
	}

	/* Whether the data represents a valid value */
	bool HasValue() const
	{
		return ((Index != INDEX_NONE) && ((InputRangeMin != -1.f) || (InputRangeMax != -1.f)));
	}

	/* Index in the value data*/
	UPROPERTY(VisibleAnywhere, BlueprintReadonly, Category = "Joystick|Data")
	int32 Index;

	/* Current analog value */
	UPROPERTY(VisibleAnywhere, BlueprintReadonly, Category = "Joystick|Data")
	float Value;

	/* Last analog value */
	UPROPERTY(VisibleAnywhere, BlueprintReadonly, Category = "Joystick|Data")
	float PreviousValue;

	/* Should remap ranges */
	UPROPERTY(VisibleAnywhere, BlueprintReadonly, Category = "Joystick|Data")
	bool bRemapRanges;

	/* Offset to apply to normalized axis value */
	UPROPERTY(VisibleAnywhere, BlueprintReadonly, Category = "Joystick|Data")
	float InputOffset;

	UPROPERTY(VisibleAnywhere, BlueprintReadonly, Category = "Joystick|Data")
	bool bInvertInput;

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
	bool bInvertOutput;

	/* Is this axis centered on 0 instead of 0.5 */
	UPROPERTY(VisibleAnywhere, BlueprintReadonly, Category = "Joystick|Data")
	bool bGamepadStick;

	UPROPERTY(VisibleAnywhere, BlueprintReadonly, Category = "Joystick|Data")
	FKey Key;
};
