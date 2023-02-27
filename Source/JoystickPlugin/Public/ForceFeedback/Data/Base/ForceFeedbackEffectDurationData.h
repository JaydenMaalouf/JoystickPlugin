// JoystickPlugin is licensed under the MIT License.
// Copyright Jayden Maalouf. All Rights Reserved.

#pragma once

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

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (ShortToolTip = "Duration", ToolTip = "The duration of the effect.", UIMin="0", ClampMin="0", EditCondition="!InfiniteDuration"), Category = "Force Feedback|Duration|Data")
	float Duration;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (ShortToolTip = "Infinite Duration", ToolTip = "Whether the effect should run infinitely."), Category = "Force Feedback|Duration|Data")
	bool InfiniteDuration;
};
