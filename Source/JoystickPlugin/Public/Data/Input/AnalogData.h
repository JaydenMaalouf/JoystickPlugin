#pragma once

#include "AnalogData.generated.h"

USTRUCT(BlueprintType)
struct JOYSTICKPLUGIN_API FAnalogData
{
	GENERATED_BODY()
	
	FAnalogData()
		: Index(INDEX_NONE)
		, Value(0.f)
		, PreviousValue(0.f)
		, RangeMin(-1.f)
		, RangeMax(-1.f)
		, Offset(0.f)
		, bInverted(false)
		, bGamepadStick(false)
	{
	}

	FAnalogData(const int32 InIndex, const float InValue, const float InRangeMin, const float InRangeMax, const float InOffset, const bool bInInverted, const bool bInGamepadStick, const FName InKeyName)
		: Index(InIndex)
		, Value(InValue)
		, PreviousValue(0.f)
		, RangeMin(InRangeMin)
		, RangeMax(InRangeMax)
		, Offset(0.f)
		, bInverted(bInInverted)
		, bGamepadStick(bInGamepadStick)
		, KeyName(InKeyName)
	{
	}

	/* Helper function to get the offset and normalized value */
	float GetValue() const
	{
		return MapValue(Value);
	}

	float GetPreviousValue() const
	{
		return MapValue(PreviousValue);
	}

	float MapValue(float input) const
	{		
		const float Factor = 1.f / (RangeMax - RangeMin);
		if (bGamepadStick)
		{
			// Need to do the re-centering before inverting
			const float NormalizedValue = ((input * Factor) - 0.5f) * 2 * (bInverted ? -1.0f : 1.0f);
			return NormalizedValue + Offset;
		}

		const float NormalizedValue = (bInverted ? (input * Factor * -1.f) : (input * Factor));
		return NormalizedValue + Offset;
	}

	/* Whether the data represents a valid value */
	bool HasValue() const
	{
		return ((Index != INDEX_NONE) && !KeyName.IsNone() && ((RangeMin != -1.f) || (RangeMax != -1.f)));
	}

	/* Index in the value data*/
	int32 Index;

	/* Current analog value */
	float Value;
	
	/* Last analog value */
	float PreviousValue;

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
	
	/* Key name */
	FName KeyName;	
};
