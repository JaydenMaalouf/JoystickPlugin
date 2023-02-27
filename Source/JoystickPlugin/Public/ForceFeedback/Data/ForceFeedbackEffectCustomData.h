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

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Force Feedback|Custom|Data")
	FForceFeedbackEffectDirectionData DirectionData;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Force Feedback|Custom|Data")
	FForceFeedbackEffectDurationData DurationData;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Force Feedback|Custom|Data")
	FForceFeedbackEffectReplayData ReplayData;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Force Feedback|Custom|Data")
	FForceFeedbackEffectEnvelopeData EnvelopeData;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Force Feedback|Custom|Data")
	uint8 Channels;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (UIMin="0", UIMax="1", ClampMin="0", ClampMax="1"), Category = "Force Feedback|Custom|Data")
	float Period;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (ClampMin = "0", ClampMax = "65535"), Category = "Force Feedback|Custom|Data")
	int Samples;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (ClampMin = "0", ClampMax = "65535"), Category = "Force Feedback|Custom|Data")
	TArray<int> Data;
};
