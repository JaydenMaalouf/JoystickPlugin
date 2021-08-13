#pragma once

#include "ForceFeedbackDirection.h"
#include "ForceFeedback/Types/ForceFeedbackDirectionType.h"
#include "ForceFeedbackEffectBaseData.Generated.h"

USTRUCT(BlueprintType)
struct FForceFeedbackEffectBaseData
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		FForceFeedbackDirection Direction;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (ShortToolTip = "Duration", ToolTip = "The duration of the effect."))
		float Duration = 1.0f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (ShortToolTip = "Infinite Duration", ToolTip = "Whether the effect should run infinitely."))
		bool InfiniteDuration = false;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (ShortToolTip = "Delay before starting", ToolTip = "Delay before starting the effect."))
		float Delay = 0.0f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (ShortToolTip = "Retrigger Delay", ToolTip = "Delay before the effect can be retriggerd."))
		float RetriggerDelay = 0.0f;
};

