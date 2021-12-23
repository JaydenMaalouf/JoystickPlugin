#pragma once

#include "ForceFeedback/Data/Base/ForceFeedbackEffectDirectionData.h"
#include "ForceFeedback/Data/Base/ForceFeedbackEffectDurationData.h"
#include "ForceFeedback/Data/Base/ForceFeedbackEffectReplayData.h"
#include "ForceFeedback/Data/Base/ForceFeedbackEffectEnvelopeData.h"

#include "ForceFeedback/Types/ForceFeedbackPeriodicEffectType.h"
#include "ForceFeedbackEffectPeriodicData.Generated.h"

USTRUCT(BlueprintType)
struct JOYSTICKPLUGIN_API FForceFeedbackEffectPeriodicData
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Force Feedback|Periodic|Data")
		FForceFeedbackEffectDirectionData DirectionData;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Force Feedback|Periodic|Data")
		FForceFeedbackEffectDurationData DurationData;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Force Feedback|Periodic|Data")
		FForceFeedbackEffectReplayData ReplayData;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Force Feedback|Periodic|Data")
		FForceFeedbackEffectEnvelopeData EnvelopeData;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Force Feedback|Periodic|Data")
		EForceFeedbackPeriodicEffectType EffectType;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Force Feedback|Periodic|Data")
		float Period = 0.2f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Force Feedback|Periodic|Data")
		float Magnitude = 1.0f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Force Feedback|Periodic|Data")
		float Offset = 0.0f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Force Feedback|Periodic|Data")
		float Phase = 0.0f;
};

