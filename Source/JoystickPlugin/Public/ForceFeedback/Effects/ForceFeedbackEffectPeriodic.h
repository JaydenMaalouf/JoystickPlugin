// JoystickPlugin is licensed under the MIT License.
// Copyright Jayden Maalouf 2026. All Rights Reserved.

#pragma once

#include "ForceFeedback/Effects/ForceFeedbackEffectBase.h"
#include "ForceFeedback/Data/ForceFeedbackEffectPeriodicData.h"

#include "ForceFeedbackEffectPeriodic.generated.h"

UCLASS(Blueprintable)
class JOYSTICKPLUGIN_API UForceFeedbackEffectPeriodic : public UForceFeedbackEffectBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Force Feedback")
	FForceFeedbackEffectPeriodicData EffectData;

	virtual void DescribeEffect(FForceFeedbackEffectDebugInfo& OutInfo) const override;
	virtual uint32 GetEffectDuration() const override;
	virtual uint32 GetEffectDelay() const override;

protected:
	virtual void UpdateEffectData() override;
};
