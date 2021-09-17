#pragma once

#include "ForceFeedback/Data/Base/ForceFeedbackEffectDurationData.h"

#include "ForceFeedbackEffectLeftRightData.Generated.h"

USTRUCT(BlueprintType)
struct JOYSTICKPLUGIN_API FForceFeedbackEffectLeftRightData
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		FForceFeedbackEffectDurationData DurationData;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		float LargeMagnitude = 1.0f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		float SmallMagnitude = 1.0f;
};

