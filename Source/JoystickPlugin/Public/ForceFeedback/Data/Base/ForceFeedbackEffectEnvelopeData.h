#pragma once

#include "ForceFeedbackEffectEnvelopeData.Generated.h"

USTRUCT(BlueprintType)
struct JOYSTICKPLUGIN_API FForceFeedbackEffectEnvelopeData
{
	GENERATED_BODY()

public:	

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		float AttackDuration = 0.0f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		float AttackLevel = 0.0f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		float FadeDuration = 0.0f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		float FadeLevel = 0.0f;

};

