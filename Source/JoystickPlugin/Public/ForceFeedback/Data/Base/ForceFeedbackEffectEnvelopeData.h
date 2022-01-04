#pragma once

#include "ForceFeedbackEffectEnvelopeData.Generated.h"

USTRUCT(BlueprintType)
struct JOYSTICKPLUGIN_API FForceFeedbackEffectEnvelopeData
{
	GENERATED_BODY()

public:	

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Force Feedback|Envelope|Data")
		float AttackDuration = 0.0f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Force Feedback|Envelope|Data")
		float AttackLevel = 0.0f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Force Feedback|Envelope|Data")
		float FadeDuration = 0.0f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Force Feedback|Envelope|Data")
		float FadeLevel = 0.0f;

};

