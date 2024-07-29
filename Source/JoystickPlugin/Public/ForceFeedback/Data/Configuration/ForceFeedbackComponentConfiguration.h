// JoystickPlugin is licensed under the MIT License.
// Copyright Jayden Maalouf. All Rights Reserved.

#pragma once

#include "ForceFeedbackConfigurationBase.h"

#include "ForceFeedbackComponentConfiguration.generated.h"

USTRUCT(BlueprintType)
struct JOYSTICKPLUGIN_API FForceFeedbackComponentConfiguration : public FForceFeedbackConfigurationBase
{
	GENERATED_BODY()

	FForceFeedbackComponentConfiguration()
		: OverrideEffectTick(true)
	{
	}

	FForceFeedbackComponentConfiguration(const FForceFeedbackConfigurationBase& BaseConfiguration, const bool bOverrideEffectTick)
		: Super(BaseConfiguration)
		  , OverrideEffectTick(bOverrideEffectTick)
	{
	}

	FForceFeedbackComponentConfiguration(const bool bAutoInit, const bool bAutoStartOnInit, const bool bOverrideEffectTick)
		: Super(bAutoInit, bAutoStartOnInit)
		  , OverrideEffectTick(bOverrideEffectTick)
	{
	}

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Force Feedback|Component|Configuration", meta=(ToolTip="Useful if you want to control the Tick Interval and Tick Group of the effects."))
	bool OverrideEffectTick;
};
