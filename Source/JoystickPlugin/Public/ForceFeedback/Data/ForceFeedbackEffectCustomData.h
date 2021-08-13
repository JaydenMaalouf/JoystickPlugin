#pragma once

#include "ForceFeedbackEffectBaseData.h"
#include "ForceFeedback/Types/ForceFeedbackDirectionType.h"
#include "ForceFeedbackEffectCustomData.Generated.h"

USTRUCT(BlueprintType)
struct FForceFeedbackEffectCustomData : public FForceFeedbackEffectBaseData
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		uint8 Channels = 0;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (ClampMin = "0", ClampMax = "65535"))
		int32 Period = 0;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (ClampMin = "0", ClampMax = "65535"))
		int32 Samples = 0;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (ClampMin = "0", ClampMax = "65535"))
		TArray<int32> Data;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		float AttackDuration = 0.0f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		float AttackLevel = 0.0f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		float FadeDuration = 0.0f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		float FadeLevel = 0.0f;
};

