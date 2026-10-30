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
		  , AutoUpdateOnStart(true)
		  , UseEffectRate(false)
		  , EffectHz(60)
		  , MaxSubticks(12)
	{
	}

	FForceFeedbackEffectConfiguration(const FForceFeedbackConfigurationBase& BaseConfiguration, const bool bAutoUpdatePostTick)
		: Super(BaseConfiguration)
		  , AutoUpdatePostTick(bAutoUpdatePostTick)
		  , AutoUpdateOnStart(true)
		  , UseEffectRate(false)
		  , EffectHz(60)
		  , MaxSubticks(12)
	{
	}

	FForceFeedbackEffectConfiguration(const bool bAutoInit, const bool bAutoStartOnInit, const bool bAutoUpdatePostTick)
		: Super(bAutoInit, bAutoStartOnInit)
		  , AutoUpdatePostTick(bAutoUpdatePostTick)
		  , AutoUpdateOnStart(true)
		  , UseEffectRate(false)
		  , EffectHz(60)
		  , MaxSubticks(12)
	{
	}

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Joystick|Force Feedback",
		meta=(ExposeOnSpawn=true, ToolTip="Automatically updates the force feedback effect after each ReceivedTick call."))
	bool AutoUpdatePostTick;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Joystick|Force Feedback",
		meta=(ExposeOnSpawn=true, ToolTip="Sends the current effect data to the device before the effect starts. Starting only replays what the device last received, so without this any change made since the last update is not played."))
	bool AutoUpdateOnStart;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Joystick|Force Feedback",
		meta=(ExposeOnSpawn=true, ToolTip="Ticks this effect at a fixed rate instead of using the current frame's delta time."))
	bool UseEffectRate;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Joystick|Force Feedback",
		meta=(ExposeOnSpawn=true, EditCondition="UseEffectRate", EditConditionHides, ToolTip="The fixed tick rate, in hertz, used when UseEffectRate is enabled.", ClampMin="1", ClampMax="1000", UIMin="1", UIMax="1000"))
	int EffectHz;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Joystick|Force Feedback",
		meta=(ExposeOnSpawn=true, EditCondition="UseEffectRate", EditConditionHides, ToolTip="The maximum number of fixed-rate ticks processed in a single frame before the accumulated time is clamped.", ClampMin="1", ClampMax="16", UIMin=
			"1", UIMax="16"))
	int MaxSubticks;
};
