// JoystickPlugin is licensed under the MIT License.
// Copyright Jayden Maalouf. All Rights Reserved.

#pragma once

#include "ForceFeedback/Effects/ForceFeedbackEffectBase.h"
#include "ForceFeedback/Data/ForceFeedbackEffectConstantData.h"

#include "ForceFeedbackEffectConstant.generated.h"

UCLASS(Blueprintable)
class JOYSTICKPLUGIN_API UForceFeedbackEffectConstant : public UForceFeedbackEffectBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Force Feedback")
	FForceFeedbackEffectConstantData EffectData;

protected:
	virtual void UpdateEffectData() override;
};
