#pragma once

#include "ForcedFeedbackEffectBaseData.h"
#include "ForcedFeedback/Types/ForcedFeedbackDirectionType.h"
#include "ForcedFeedbackEffectRampData.Generated.h"

USTRUCT(BlueprintType)
struct FForcedFeedbackEffectRampData : public FForcedFeedbackEffectBaseData
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (ClampMin = "-32768", ClampMax = "32767"))
		int32 RampStart = -1;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (ClampMin = "-32768", ClampMax = "32767"))
		int32 RampEnd = 1;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (ClampMin = "0", ClampMax = "65535"))
		int32 AttackLength = 0;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (ClampMin = "0", ClampMax = "65535"))
		int32 AttackLevel = 0;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (ClampMin = "0", ClampMax = "65535"))
		int32 FadeLength = 0;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (ClampMin = "0", ClampMax = "65535"))
		int32 FadeLevel = 0;
};

