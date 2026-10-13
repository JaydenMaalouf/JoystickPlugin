// JoystickPlugin is licensed under the MIT License.
// Copyright Jayden Maalouf 2026. All Rights Reserved.

#pragma once

#include "ForceFeedback/ForceFeedbackConversion.h"

THIRD_PARTY_INCLUDES_START

#include "JoystickSDL.h"

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

	uint32 GetEffectDuration() const
	{
		return InfiniteDuration ? SDL_HAPTIC_INFINITY : FForceFeedbackConversion::SecondsToDuration(Duration);
	}

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Joystick|Force Feedback|Duration",
		meta=(Units="Seconds", ShortToolTip="Duration", ToolTip="The duration of the effect.", UIMin="0", UIMax="4294.967", ClampMin="0", ClampMax="4294.967", EditCondition="!InfiniteDuration"))
	float Duration;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Joystick|Force Feedback|Duration", meta=(ShortToolTip="Infinite Duration", ToolTip="Whether the effect should run infinitely."))
	bool InfiniteDuration;
};
