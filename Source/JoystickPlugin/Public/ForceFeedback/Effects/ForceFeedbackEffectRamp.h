// JoystickPlugin is licensed under the MIT License.
// Copyright Jayden Maalouf. All Rights Reserved.

#pragma once

#include "ForceFeedback/Effects/ForceFeedbackEffectBase.h"
#include "ForceFeedback/Data/ForceFeedbackEffectRampData.h"

#include "ForceFeedbackEffectRamp.generated.h"

UCLASS(Blueprintable)
class JOYSTICKPLUGIN_API UForceFeedbackEffectRamp : public UForceFeedbackEffectBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Force Feedback")
	FForceFeedbackEffectRampData EffectData;

protected:
	virtual void UpdateEffectData() override;
};
