// JoystickPlugin is licensed under the MIT License.
// Copyright Jayden Maalouf 2026. All Rights Reserved.

#pragma once

#include "ForceFeedback/Effects/ForceFeedbackEffectBase.h"
#include "ForceFeedback/Data/ForceFeedbackEffectCustomData.h"

#include "ForceFeedbackEffectCustom.generated.h"

UCLASS(Blueprintable)
class JOYSTICKPLUGIN_API UForceFeedbackEffectCustom : public UForceFeedbackEffectBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Force Feedback")
	FForceFeedbackEffectCustomData EffectData;

	virtual void DescribeEffect(FForceFeedbackEffectDebugInfo& OutInfo) const override;
	virtual uint32 GetEffectDuration() const override;
	virtual uint32 GetEffectDelay() const override;

protected:
	virtual void UpdateEffectData() override;

private:
	// Backing storage for Effect.custom.data, always holds channels * samples entries.
	TArray<uint16> SampleData;
};
