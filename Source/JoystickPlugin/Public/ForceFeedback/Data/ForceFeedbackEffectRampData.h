// JoystickPlugin is licensed under the MIT License.
// Copyright Jayden Maalouf. All Rights Reserved.

#pragma once

#include "ForceFeedback/Data/Base/ForceFeedbackEffectDirectionData.h"
#include "ForceFeedback/Data/Base/ForceFeedbackEffectDurationData.h"
#include "ForceFeedback/Data/Base/ForceFeedbackEffectReplayData.h"
#include "ForceFeedback/Data/Base/ForceFeedbackEffectEnvelopeData.h"

#include "ForceFeedbackEffectRampData.generated.h"

USTRUCT(BlueprintType)
struct JOYSTICKPLUGIN_API FForceFeedbackEffectRampData
{
	GENERATED_BODY()

	FForceFeedbackEffectRampData()
		: RampStart(-1.0f)
		  , RampEnd(1.0f)
	{
	}

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Force Feedback|Ramp|Data")
	FForceFeedbackEffectDirectionData DirectionData;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Force Feedback|Ramp|Data")
	FForceFeedbackEffectDurationData DurationData;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Force Feedback|Ramp|Data")
	FForceFeedbackEffectReplayData ReplayData;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Force Feedback|Ramp|Data")
	FForceFeedbackEffectEnvelopeData EnvelopeData;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Force Feedback|Ramp|Data", meta=(UIMin="-1", UIMax="1", ClampMin="-1", ClampMax="1"))
	float RampStart;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Force Feedback|Ramp|Data", meta=(UIMin="-1", UIMax="1", ClampMin="-1", ClampMax="1"))
	float RampEnd;
};
