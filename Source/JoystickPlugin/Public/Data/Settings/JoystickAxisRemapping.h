// JoystickPlugin is licensed under the MIT License.
// Copyright Jayden Maalouf 2026. All Rights Reserved.

#pragma once

#include "JoystickAxisRemapping.generated.h"

// The remapping applied to an analog axis' raw value, shared by its configuration and its runtime state.
USTRUCT(BlueprintType)
struct JOYSTICKPLUGIN_API FJoystickAxisRemapping
{
	GENERATED_BODY()

	FJoystickAxisRemapping()
		: RemappingEnabled(false)
		  , InputOffset(0.0f)
		  , InvertInput(false)
		  , RerangeInput(false)
		  , InputRangeMin(-1.0f)
		  , InputRangeMax(1.0f)
		  , RerangeOutput(false)
		  , OutputRangeMin(-1.0f)
		  , OutputRangeMax(1.0f)
		  , InvertOutput(false)
	{
	}

	// Whether the remapping below is applied.
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Axis Properties")
	bool RemappingEnabled;

	// Added to the value after the input inversion.
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Axis Properties", meta=(EditCondition="RemappingEnabled", EditConditionHides))
	float InputOffset;

	// Inverts the value before it is offset and remapped.
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Axis Properties", meta=(EditCondition="RemappingEnabled", EditConditionHides))
	bool InvertInput;

	// Whether the input range participates in remapping.
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Axis Properties", meta=(EditCondition="RemappingEnabled", EditConditionHides))
	bool RerangeInput;

	// The input range's minimum, mapped to OutputRangeMin.
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Axis Properties", meta=(EditCondition="RemappingEnabled && RerangeInput", EditConditionHides))
	float InputRangeMin;

	// The input range's maximum, mapped to OutputRangeMax.
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Axis Properties", meta=(EditCondition="RemappingEnabled && RerangeInput", EditConditionHides))
	float InputRangeMax;

	// Whether the output range participates in remapping.
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Axis Properties", meta=(EditCondition="RemappingEnabled", EditConditionHides))
	bool RerangeOutput;

	// The output range's minimum.
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Axis Properties", meta=(EditCondition="RemappingEnabled && RerangeOutput", EditConditionHides))
	float OutputRangeMin;

	// The output range's maximum.
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Axis Properties", meta=(EditCondition="RemappingEnabled && RerangeOutput", EditConditionHides))
	float OutputRangeMax;

	// Inverts the value after it is remapped.
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Axis Properties", meta=(EditCondition="RemappingEnabled", EditConditionHides))
	bool InvertOutput;

	void CopyRemapping(const FJoystickAxisRemapping& Source)
	{
		*this = Source;
	}

	float MapValue(const float Input) const
	{
		if (RemappingEnabled == false)
		{
			return Input;
		}

		const float OffsetValue = (InvertInput ? -Input : Input) + InputOffset;
		const FVector2D InputRange = GetInputRange();
		const FVector2D OutputRange = GetOutputRange();
		const float MappedValue = FMath::GetMappedRangeValueClamped(InputRange, OutputRange, OffsetValue);
		return InvertOutput ? OutputRange.X + OutputRange.Y - MappedValue : MappedValue;
	}

	FVector2D GetInputRange() const
	{
		return GetRange(RerangeInput, InputRangeMin, InputRangeMax);
	}

	// A disabled output range keeps the input range.
	FVector2D GetOutputRange() const
	{
		return GetRange(RerangeOutput, OutputRangeMin, OutputRangeMax, GetInputRange());
	}

	static FVector2D GetRange(const bool Rerange, const float RangeMin, const float RangeMax, const FVector2D& Fallback = FVector2D(-1.0f, 1.0f))
	{
		if (Rerange == false)
		{
			return Fallback;
		}

		return FVector2D(RangeMin, RangeMax);
	}
};
