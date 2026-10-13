// JoystickPlugin is licensed under the MIT License.
// Copyright Jayden Maalouf 2026. All Rights Reserved.

#pragma once

#include "ForceFeedbackEffectEnvelopeData.generated.h"

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

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Joystick|Force Feedback|Envelope", meta=(Units="Seconds", UIMin="0", UIMax="32.767", ClampMin="0", ClampMax="32.767"))
	float AttackDuration;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Joystick|Force Feedback|Envelope", meta=(UIMin="0", UIMax="1", ClampMin="0", ClampMax="1"))
	float AttackLevel;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Joystick|Force Feedback|Envelope", meta=(Units="Seconds", UIMin="0", UIMax="32.767", ClampMin="0", ClampMax="32.767"))
	float FadeDuration;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Joystick|Force Feedback|Envelope", meta=(UIMin="0", UIMax="1", ClampMin="0", ClampMax="1"))
	float FadeLevel;
};
