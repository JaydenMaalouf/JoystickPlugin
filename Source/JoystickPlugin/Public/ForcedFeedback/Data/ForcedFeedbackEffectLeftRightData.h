#pragma once

#include "ForcedFeedbackEffectBaseData.h"
#include "ForcedFeedback/Types/ForcedFeedbackDirectionType.h"
#include "ForcedFeedbackEffectLeftRightData.Generated.h"

USTRUCT(BlueprintType)
struct FForcedFeedbackEffectLeftRightData
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (ShortToolTip = "Duration", ToolTip = "The duration of the effect."))
		float Duration = 1.0f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (ShortToolTip = "Infinite Duration", ToolTip = "Whether the effect should run infinitely."))
		bool InfiniteDuration = false;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		float LargeMagnitude = 1.0f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		float SmallMagnitude = 1.0f;
};

