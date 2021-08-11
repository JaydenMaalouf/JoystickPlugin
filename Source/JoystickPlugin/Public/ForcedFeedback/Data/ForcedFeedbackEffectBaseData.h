#pragma once

#include "ForcedFeedbackDirection.h"
#include "ForcedFeedback/Types/ForcedFeedbackDirectionType.h"
#include "ForcedFeedbackEffectBaseData.Generated.h"

USTRUCT(BlueprintType)
struct FForcedFeedbackEffectBaseData
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		FForcedFeedbackDirection Direction;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (ClampMin = "0", ClampMax = "4294967295", ShortToolTip = "Duration", ToolTip = "The duration of the effect."))
		int64 Length = 1;

	UPROPERTY(BlueprintReadWrite, EditAnywhere) 
		bool InfiniteLength = false;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (ClampMin = "0", ClampMax = "65535", ShortToolTip = "Delay before starting", ToolTip = "Delay before starting the effect"))
		int32 Delay = 0;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (ClampMin = "0", ClampMax = "65535", ShortToolTip = "Retrigger Delay", ToolTip = "Delay before the effect can be retriggerd"))
		int32 Interval = 0;
};

