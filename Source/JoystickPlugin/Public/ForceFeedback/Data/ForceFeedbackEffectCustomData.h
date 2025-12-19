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

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Joystick|Force Feedback|Custom")
	FForceFeedbackEffectDirectionData DirectionData;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Joystick|Force Feedback|Custom")
	FForceFeedbackEffectDurationData DurationData;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Joystick|Force Feedback|Custom")
	FForceFeedbackEffectReplayData ReplayData;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Joystick|Force Feedback|Custom")
	FForceFeedbackEffectEnvelopeData EnvelopeData;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Joystick|Force Feedback|Custom")
	uint8 Channels;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Joystick|Force Feedback|Custom", meta=(UIMin="0", UIMax="1", ClampMin="0", ClampMax="1"))
	float Period;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Joystick|Force Feedback|Custom", meta=(ClampMin="0", ClampMax="65535"))
	int Samples;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Joystick|Force Feedback|Custom", meta=(ClampMin="0", ClampMax="65535"))
	TArray<int> Data;
};
