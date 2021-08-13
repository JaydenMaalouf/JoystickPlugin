#pragma once

#include "ForceFeedbackEffectBaseData.h"
#include "ForceFeedback/Types/ForceFeedbackPeriodicEffectType.h"
#include "ForceFeedbackEffectPeriodicData.Generated.h"

USTRUCT(BlueprintType)
struct FForceFeedbackEffectPeriodicData : public FForceFeedbackEffectBaseData
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		EForceFeedbackPeriodicEffectType EffectType;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		float Period = 0.2f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		float Magnitude = 1.0f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		float Offset = 0.0f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		float Phase = 0.0f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		float AttackDuration = 0.0f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		float AttackLevel = 0.0f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		float FadeDuration = 0.0f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		float FadeLevel = 0.0f;
};

