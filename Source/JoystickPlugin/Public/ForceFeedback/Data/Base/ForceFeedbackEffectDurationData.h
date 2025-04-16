// JoystickPlugin is licensed under the MIT License.
// Copyright Jayden Maalouf. All Rights Reserved.

#pragma once


THIRD_PARTY_INCLUDES_START

#include "SDL_haptic.h"

THIRD_PARTY_INCLUDES_END

#include "ForceFeedbackEffectDurationData.generated.h"

USTRUCT(BlueprintType)
struct JOYSTICKPLUGIN_API FForceFeedbackEffectDurationData
{
	GENERATED_BODY()

	FForceFeedbackEffectDurationData()
		: Duration(1.0f)
		  , InfiniteDuration(false)
	{
	}

	float GetEffectDuration() const
	{
		return InfiniteDuration ? SDL_HAPTIC_INFINITY : FMath::Clamp<Uint32>(Duration * 1000.0f, 0, UINT32_MAX);
	}

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (ShortToolTip = "Duration", ToolTip = "The duration of the effect.", UIMin="0", ClampMin="0", EditCondition="!InfiniteDuration"), Category = "Force Feedback|Duration|Data")
	float Duration;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (ShortToolTip = "Infinite Duration", ToolTip = "Whether the effect should run infinitely."), Category = "Force Feedback|Duration|Data")
	bool InfiniteDuration;
};
