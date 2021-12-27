#pragma once

#include "ForceFeedbackEffectReplayData.Generated.h"

USTRUCT(BlueprintType)
struct JOYSTICKPLUGIN_API FForceFeedbackEffectReplayData
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (ShortToolTip = "Delay before starting", ToolTip = "Delay before starting the effect."), Category = "Force Feedback|Replay|Data")
		float Delay = 0.0f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (ShortToolTip = "Retrigger Delay", ToolTip = "Delay before the effect can be retriggerd."), Category = "Force Feedback|Replay|Data")
		float RetriggerDelay = 0.0f;
};

