// JoystickPlugin is licensed under the MIT License.
// Copyright Jayden Maalouf. All Rights Reserved.

#pragma once

#include "ForceFeedbackComponentData.generated.h"

USTRUCT(BlueprintType)
struct JOYSTICKPLUGIN_API FForceFeedbackComponentData
{
	GENERATED_BODY()

	FForceFeedbackComponentData()
		: AutoInit(true)
		  , AutoStartOnInit(true)
		  , OverrideEffectTick(true)
	{
	}

	FForceFeedbackComponentData(const bool bAutoInit, const bool bAutoStartOnInit, const bool bOverrideEffectTick)
		: AutoInit(bAutoInit)
		  , AutoStartOnInit(bAutoStartOnInit)
		  , OverrideEffectTick(bOverrideEffectTick)
	{
	}

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Force Feedback|Component|Data")
	bool AutoInit;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Force Feedback|Component|Data")
	bool AutoStartOnInit;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Force Feedback|Component", meta=(ToolTip="Useful if you want to control the Tick Interval and Tick Group of the effects."))
	bool OverrideEffectTick;
};
