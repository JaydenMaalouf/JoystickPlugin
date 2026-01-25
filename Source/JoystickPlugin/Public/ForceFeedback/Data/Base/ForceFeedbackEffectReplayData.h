// JoystickPlugin is licensed under the MIT License.
// Copyright Jayden Maalouf 2026. All Rights Reserved.

#pragma once

#include "ForceFeedbackEffectReplayData.generated.h"

USTRUCT(BlueprintType)
struct JOYSTICKPLUGIN_API FForceFeedbackEffectReplayData
{
	GENERATED_BODY()

	FForceFeedbackEffectReplayData()
		: Delay(0.0f)
		  , RetriggerDelay(0.0f)
	{
	}

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Joystick|Force Feedback|Replay",
		meta=(Units="Seconds", ShortToolTip="Delay before starting", ToolTip="Delay before starting the effect.", UIMin="0", UIMax="65.535", ClampMin="0", ClampMax="65.535"))
	float Delay;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Joystick|Force Feedback|Replay",
		meta=(Units="Seconds", ShortToolTip="Retrigger Delay", ToolTip="Delay before the effect can be retriggerd.", UIMin="0", UIMax="65.535", ClampMin="0", ClampMax="65.535"))
	float RetriggerDelay;
};
