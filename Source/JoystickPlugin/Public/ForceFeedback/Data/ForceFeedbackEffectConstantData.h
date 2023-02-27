// JoystickPlugin is licensed under the MIT License.
// Copyright Jayden Maalouf. All Rights Reserved.

#pragma once

#include "ForceFeedback/Data/Base/ForceFeedbackEffectDirectionData.h"
#include "ForceFeedback/Data/Base/ForceFeedbackEffectDurationData.h"
#include "ForceFeedback/Data/Base/ForceFeedbackEffectReplayData.h"
#include "ForceFeedback/Data/Base/ForceFeedbackEffectEnvelopeData.h"

#include "ForceFeedbackEffectConstantData.generated.h"

USTRUCT(BlueprintType)
struct JOYSTICKPLUGIN_API FForceFeedbackEffectConstantData
{
	GENERATED_BODY()

	FForceFeedbackEffectConstantData()
		: EffectLevel(1.0f)
	{
	}

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Force Feedback|Constant|Data")
	FForceFeedbackEffectDirectionData DirectionData;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Force Feedback|Constant|Data")
	FForceFeedbackEffectDurationData DurationData;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Force Feedback|Constant|Data")
	FForceFeedbackEffectReplayData ReplayData;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Force Feedback|Constant|Data")
	FForceFeedbackEffectEnvelopeData EnvelopeData;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Force Feedback|Constant|Data", meta=(UIMin="-1", UIMax="1", ClampMin="-1", ClampMax="1"))
	float EffectLevel;
};
