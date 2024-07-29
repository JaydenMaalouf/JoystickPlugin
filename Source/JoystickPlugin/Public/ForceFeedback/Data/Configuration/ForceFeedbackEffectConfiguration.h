// JoystickPlugin is licensed under the MIT License.
// Copyright Jayden Maalouf. All Rights Reserved.

#pragma once

#include "ForceFeedbackConfigurationBase.h"

#include "ForceFeedbackEffectConfiguration.generated.h"

USTRUCT(BlueprintType)
struct JOYSTICKPLUGIN_API FForceFeedbackEffectConfiguration : public FForceFeedbackConfigurationBase
{
	GENERATED_BODY()

	FForceFeedbackEffectConfiguration()
		: AutoUpdatePostTick(true)
	{
	}

	FForceFeedbackEffectConfiguration(const FForceFeedbackConfigurationBase& BaseConfiguration, const bool bAutoUpdatePostTick)
		: Super(BaseConfiguration)
		  , AutoUpdatePostTick(bAutoUpdatePostTick)
	{
	}

	FForceFeedbackEffectConfiguration(const bool bAutoInit, const bool bAutoStartOnInit, const bool bAutoUpdatePostTick)
		: Super(bAutoInit, bAutoStartOnInit)
		  , AutoUpdatePostTick(bAutoUpdatePostTick)
	{
	}

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Force Feedback|Configuration", meta = (ExposeOnSpawn = true))
	bool AutoUpdatePostTick;
};
