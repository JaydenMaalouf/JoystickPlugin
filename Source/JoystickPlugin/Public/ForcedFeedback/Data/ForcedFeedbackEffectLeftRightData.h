#pragma once

#include "ForcedFeedbackEffectBaseData.h"
#include "ForcedFeedback/Types/ForcedFeedbackDirectionType.h"
#include "ForcedFeedbackEffectLeftRightData.Generated.h"

USTRUCT(BlueprintType)
struct FForcedFeedbackEffectLeftRightData
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (ClampMin = "0", ClampMax = "65535"))
		int32 LargeMagnitude = 1;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (ClampMin = "0", ClampMax = "65535"))
		int32 SmallMagnitude = 1;
};

