// JoystickPlugin is licensed under the MIT License.
// Copyright Jayden Maalouf. All Rights Reserved.

#pragma once

#include "ForceFeedback/Data/Base/ForceFeedbackEffectDurationData.h"

#include "ForceFeedbackEffectLeftRightData.generated.h"

USTRUCT(BlueprintType)
struct JOYSTICKPLUGIN_API FForceFeedbackEffectLeftRightData
{
	GENERATED_BODY()

	FForceFeedbackEffectLeftRightData()
		: LargeMagnitude(1.0f)
		  , SmallMagnitude(1.0f)
	{
	}

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Joystick|Force Feedback|LeftRight")
	FForceFeedbackEffectDurationData DurationData;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Joystick|Force Feedback|LeftRight", meta=(UIMin="0", UIMax="1", ClampMin="0", ClampMax="1"))
	float LargeMagnitude;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Joystick|Force Feedback|LeftRight", meta=(UIMin="0", UIMax="1", ClampMin="0", ClampMax="1"))
	float SmallMagnitude;
};
