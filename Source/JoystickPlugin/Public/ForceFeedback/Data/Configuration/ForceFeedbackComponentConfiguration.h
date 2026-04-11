// JoystickPlugin is licensed under the MIT License.
// Copyright Jayden Maalouf 2026. All Rights Reserved.

#pragma once

#include "ForceFeedbackConfigurationBase.h"

#include "ForceFeedbackComponentConfiguration.generated.h"

USTRUCT(BlueprintType)
struct JOYSTICKPLUGIN_API FForceFeedbackComponentConfiguration : public FForceFeedbackConfigurationBase
{
	GENERATED_BODY()

	FForceFeedbackComponentConfiguration()
		: UseAsyncPhysicsTick(false)
		  , UsePhysicsSubstepTick(false)
		  , OverrideEffectTick(true)
	{
	}

	FForceFeedbackComponentConfiguration(const FForceFeedbackConfigurationBase& BaseConfiguration, const bool bOverrideEffectTick)
		: Super(BaseConfiguration)
		  , UseAsyncPhysicsTick(false)
		  , UsePhysicsSubstepTick(false)
		  , OverrideEffectTick(bOverrideEffectTick)
	{
	}

	FForceFeedbackComponentConfiguration(const bool bAutoInit, const bool bAutoStartOnInit, const bool bOverrideEffectTick)
		: Super(bAutoInit, bAutoStartOnInit)
		  , UseAsyncPhysicsTick(false)
		  , UsePhysicsSubstepTick(false)
		  , OverrideEffectTick(bOverrideEffectTick)
	{
	}

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Joystick|Force Feedback|Tick")
	bool UseAsyncPhysicsTick;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Joystick|Force Feedback|Tick", meta=(ToolTip="Ticks the component from Chaos physics substeps using a PreIntegrate sim callback. This runs on the physics thread, so ReceivedTick implementations must not touch game-thread-only state."))
	bool UsePhysicsSubstepTick;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Joystick|Force Feedback", meta=(ToolTip="Useful if you want to control the Tick Interval and Tick Group of the effects."))
	bool OverrideEffectTick;
};
