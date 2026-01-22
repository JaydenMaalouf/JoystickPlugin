// JoystickPlugin is licensed under the MIT License.
// Copyright Jayden Maalouf 2026. All Rights Reserved.

#pragma once

#include "ForceFeedbackConfigurationBase.h"

#include "ForceFeedbackEffectConfiguration.generated.h"

USTRUCT(BlueprintType)
struct JOYSTICKPLUGIN_API FForceFeedbackEffectConfiguration : public FForceFeedbackConfigurationBase
{
	GENERATED_BODY()

	FForceFeedbackEffectConfiguration()
		: AutoUpdatePostTick(true)
		  , SubstepTicks(false)
		  , EffectHz(60)
		  , MaxSubticks(12)
	{
	}

	FForceFeedbackEffectConfiguration(const FForceFeedbackConfigurationBase& BaseConfiguration, const bool bAutoUpdatePostTick)
		: Super(BaseConfiguration)
		  , AutoUpdatePostTick(bAutoUpdatePostTick)
		  , SubstepTicks(false)
		  , EffectHz(60)
		  , MaxSubticks(12)
	{
	}

	FForceFeedbackEffectConfiguration(const bool bAutoInit, const bool bAutoStartOnInit, const bool bAutoUpdatePostTick)
		: Super(bAutoInit, bAutoStartOnInit)
		  , AutoUpdatePostTick(bAutoUpdatePostTick)
		  , SubstepTicks(false)
		  , EffectHz(60)
		  , MaxSubticks(12)
	{
	}

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Joystick|Force Feedback", meta=(ExposeOnSpawn=true))
	bool AutoUpdatePostTick;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Joystick|Force Feedback", meta=(ExposeOnSpawn=true))
	bool SubstepTicks;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Joystick|Force Feedback", meta=(ExposeOnSpawn=true, EditCondition="SubstepTicks", EditConditionHides, ClampMin="1", ClampMax="1000", UIMin="1", UIMax="1200"))
	int EffectHz;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Joystick|Force Feedback", meta=(ExposeOnSpawn=true, EditCondition="SubstepTicks", EditConditionHides, ClampMin="0", ClampMax="12", UIMin="0", UIMax="12"))
	int MaxSubticks;
};
