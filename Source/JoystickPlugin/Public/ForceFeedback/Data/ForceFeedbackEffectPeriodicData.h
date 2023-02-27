// JoystickPlugin is licensed under the MIT License.
// Copyright Jayden Maalouf. All Rights Reserved.

#pragma once

#include "ForceFeedback/Data/Base/ForceFeedbackEffectDirectionData.h"
#include "ForceFeedback/Data/Base/ForceFeedbackEffectDurationData.h"
#include "ForceFeedback/Data/Base/ForceFeedbackEffectReplayData.h"
#include "ForceFeedback/Data/Base/ForceFeedbackEffectEnvelopeData.h"
#include "ForceFeedback/Types/ForceFeedbackPeriodicEffectType.h"

#include "ForceFeedbackEffectPeriodicData.generated.h"

USTRUCT(BlueprintType)
struct JOYSTICKPLUGIN_API FForceFeedbackEffectPeriodicData
{
	GENERATED_BODY()

	FForceFeedbackEffectPeriodicData()
		: EffectType(EForceFeedbackPeriodicEffectType::Sine)
		  , Period(0.2f)
		  , Magnitude(1.0f)
		  , Offset(0.0f)
		  , Phase(0.0f)
	{
	}

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Force Feedback|Periodic|Data")
	FForceFeedbackEffectDirectionData DirectionData;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Force Feedback|Periodic|Data")
	FForceFeedbackEffectDurationData DurationData;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Force Feedback|Periodic|Data")
	FForceFeedbackEffectReplayData ReplayData;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Force Feedback|Periodic|Data")
	FForceFeedbackEffectEnvelopeData EnvelopeData;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Force Feedback|Periodic|Data")
	EForceFeedbackPeriodicEffectType EffectType;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Force Feedback|Periodic|Data", meta=(UIMin="0", ClampMin="0"))
	float Period;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Force Feedback|Periodic|Data", meta=(UIMin="-1", UIMax="1", ClampMin="-1", ClampMax="1"))
	float Magnitude;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Force Feedback|Periodic|Data", meta=(UIMin="-1", UIMax="1", ClampMin="-1", ClampMax="1"))
	float Offset;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Force Feedback|Periodic|Data", meta=(UIMin="0", ClampMin="0"))
	float Phase;
};
