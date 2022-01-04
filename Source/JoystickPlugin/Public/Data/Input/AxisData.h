#pragma once

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
		, RangeMin(0.f)
		, RangeMax(1.f)
		, Offset(0.f)
		, bInverted(false)
		, bGamepadStick(false)
	{
	}

	FAxisData(const int32 InIndex, const float InValue, const float InRangeMin, const float InRangeMax, const float InOffset, const bool bInInverted, const bool bInGamepadStick)
		: Index(InIndex)
		, Value(InValue)
		, PreviousValue(0.f)
		, bRemapRanges(false)
		, RangeMin(InRangeMin)
		, RangeMax(InRangeMax)
		, Offset(InOffset)
		, bInverted(bInInverted)
		, bGamepadStick(bInGamepadStick)
	{
	}

	/* Helper function to get the offset and normalized value */
	float GetValue() const
	{
		if (bRemapRanges)
		{
			return MapValue(Value);
		}
		else
		{
			return Value;
		}		
	}

	float GetPreviousValue() const
	{
		return MapValue(PreviousValue);
	}

	float MapValue(float Input) const
	{		
		const float Factor = 1.f / (RangeMax - RangeMin);
		if (bGamepadStick)
		{
			// Need to do the re-centering before inverting
			const float NormalizedValue = ((Input * Factor) - 0.5f) * 2 * (bInverted ? -1.0f : 1.0f);
			return NormalizedValue + Offset;
		}

		const float NormalizedValue = (bInverted ? (Input * Factor * -1.f) : (Input * Factor));
		return NormalizedValue + Offset;
	}

	/* Whether the data represents a valid value */
	bool HasValue() const
	{
		return ((Index != INDEX_NONE) && ((RangeMin != -1.f) || (RangeMax != -1.f)));
	}

	/* Index in the value data*/
	int32 Index;

	/* Current analog value */
	float Value;
	
	/* Last analog value */
	float PreviousValue;

	/* Should remap ranges */
	bool bRemapRanges;

	/* Min Analog value */
	float RangeMin;
	
	/* Max analog value */
	float RangeMax;
	
	/* Offset to apply to normalized axis value */
	float Offset;

	/* Is this axis inverted */
	bool bInverted;

	/* Is this axis centered on 0 instead of 0.5 */
	bool bGamepadStick;
};
