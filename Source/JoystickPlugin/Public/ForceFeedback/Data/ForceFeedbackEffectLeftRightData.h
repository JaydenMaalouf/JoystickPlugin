#pragma once

#include "ForceFeedback/Data/Base/ForceFeedbackEffectDurationData.h"

#include "ForceFeedbackEffectLeftRightData.Generated.h"

USTRUCT(BlueprintType)
struct JOYSTICKPLUGIN_API FForceFeedbackEffectLeftRightData
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Force Feedback|Left/Right|Data")
		FForceFeedbackEffectDurationData DurationData;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Force Feedback|Left/Right|Data")
		float LargeMagnitude = 1.0f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Force Feedback|Left/Right|Data")
		float SmallMagnitude = 1.0f;
};

