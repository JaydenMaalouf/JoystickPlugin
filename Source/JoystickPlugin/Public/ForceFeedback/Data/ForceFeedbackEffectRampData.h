#pragma once

#include "ForceFeedbackEffectBaseData.h"
#include "ForceFeedback/Types/ForceFeedbackDirectionType.h"
#include "ForceFeedbackEffectRampData.Generated.h"

USTRUCT(BlueprintType)
struct FForceFeedbackEffectRampData : public FForceFeedbackEffectBaseData
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		float RampStart = -1.0f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		float RampEnd = 1.0f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		float AttackDuration = 0.0f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		float AttackLevel = 0.0f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		float FadeDuration = 0.0f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		float FadeLevel = 0.0f;
};

