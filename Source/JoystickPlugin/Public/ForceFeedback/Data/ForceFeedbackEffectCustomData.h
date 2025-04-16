// JoystickPlugin is licensed under the MIT License.
// Copyright Jayden Maalouf. All Rights Reserved.

#pragma once

#include "ForceFeedback/Data/Base/ForceFeedbackEffectDirectionData.h"
#include "ForceFeedback/Data/Base/ForceFeedbackEffectDurationData.h"
#include "ForceFeedback/Data/Base/ForceFeedbackEffectReplayData.h"
#include "ForceFeedback/Data/Base/ForceFeedbackEffectEnvelopeData.h"

#include "Containers/Array.h"

#include "ForceFeedbackEffectCustomData.generated.h"

USTRUCT(BlueprintType)
struct JOYSTICKPLUGIN_API FForceFeedbackEffectCustomData
{
	GENERATED_BODY()

	FForceFeedbackEffectCustomData()
		: Channels(0)
		  , Period(0)
		  , Samples(0)
	{
	}

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Force Feedback|Data")
	FForceFeedbackEffectDirectionData DirectionData;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Force Feedback|Data")
	FForceFeedbackEffectDurationData DurationData;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Force Feedback|Data")
	FForceFeedbackEffectReplayData ReplayData;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Force Feedback|Data")
	FForceFeedbackEffectEnvelopeData EnvelopeData;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Force Feedback|Data")
	uint8 Channels;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Force Feedback|Data", meta = (UIMin="0", UIMax="1", ClampMin="0", ClampMax="1"))
	float Period;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Force Feedback|Data", meta = (ClampMin = "0", ClampMax = "65535"))
	int Samples;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Force Feedback|Data", meta = (ClampMin = "0", ClampMax = "65535"))
	TArray<int> Data;
};
