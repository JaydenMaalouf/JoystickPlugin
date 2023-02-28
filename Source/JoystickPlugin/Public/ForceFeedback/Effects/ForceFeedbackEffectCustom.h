// JoystickPlugin is licensed under the MIT License.
// Copyright Jayden Maalouf. All Rights Reserved.

#pragma once

#include "ForceFeedback/Effects/ForceFeedbackEffectBase.h"
#include "ForceFeedback/Data/ForceFeedbackEffectCustomData.h"

#include "ForceFeedbackEffectCustom.generated.h"

UCLASS(Blueprintable)
class JOYSTICKPLUGIN_API UForceFeedbackEffectCustom : public UForceFeedbackEffectBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Force Feedback")
	FForceFeedbackEffectCustomData EffectData;

protected:
	virtual void UpdateEffectData() override;
};
