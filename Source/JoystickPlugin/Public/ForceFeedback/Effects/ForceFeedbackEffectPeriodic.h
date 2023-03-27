// JoystickPlugin is licensed under the MIT License.
// Copyright Jayden Maalouf. All Rights Reserved.

#pragma once

#include "ForceFeedback/Effects/ForceFeedbackEffectBase.h"
#include "ForceFeedback/Data/ForceFeedbackEffectPeriodicData.h"

#include "ForceFeedbackEffectPeriodic.generated.h"

UCLASS(Blueprintable)
class JOYSTICKPLUGIN_API UForceFeedbackEffectPeriodic : public UForceFeedbackEffectBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Force Feedback")
	FForceFeedbackEffectPeriodicData EffectData;

protected:
	virtual void UpdateEffectData() override;
};
