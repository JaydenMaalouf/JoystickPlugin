#pragma once

#include "ForceFeedbackEffectEnvelopeData.Generated.h"

USTRUCT(BlueprintType)
struct JOYSTICKPLUGIN_API FForceFeedbackEffectEnvelopeData
{
	GENERATED_BODY()
	
	FForceFeedbackEffectEnvelopeData()
		: AttackDuration(0.0f)
		, AttackLevel(0.0f)
		, FadeDuration(0.0f)
		, FadeLevel(0.0f)
	{
		
	}

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Force Feedback|Envelope|Data")
		float AttackDuration;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Force Feedback|Envelope|Data")
		float AttackLevel;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Force Feedback|Envelope|Data")
		float FadeDuration;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Force Feedback|Envelope|Data")
		float FadeLevel;

};

