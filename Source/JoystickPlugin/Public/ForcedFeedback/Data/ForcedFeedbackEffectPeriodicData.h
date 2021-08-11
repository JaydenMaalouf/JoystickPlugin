#pragma once

#include "ForcedFeedbackEffectBaseData.h"
#include "ForcedFeedback/Types/ForcedFeedbackPeriodicEffectType.h"
#include "ForcedFeedbackEffectPeriodicData.Generated.h"

USTRUCT(BlueprintType)
struct FForcedFeedbackEffectPeriodicData : public FForcedFeedbackEffectBaseData
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		EForcedFeedbackPeriodicEffectType EffectType;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (ClampMin = "0", ClampMax = "65535"))
		int32 Period = 0;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (ClampMin = "-32768", ClampMax = "32767"))
		int32 Magnitude = 0;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (ClampMin = "-32768", ClampMax = "32767"))
		int32 Offset = 0;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (ClampMin = "0", ClampMax = "65535"))
		int32 Phase = 0;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (ClampMin = "0", ClampMax = "65535"))
		int32 AttackLength = 0;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (ClampMin = "0", ClampMax = "65535"))
		int32 AttackLevel = 0;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (ClampMin = "0", ClampMax = "65535"))
		int32 FadeLength = 0;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (ClampMin = "0", ClampMax = "65535"))
		int32 FadeLevel = 0;
};

