// JoystickPlugin is licensed under the MIT License.
// Copyright Jayden Maalouf. All Rights Reserved.

#pragma once

#include "ForceFeedbackConfigurationBase.generated.h"

USTRUCT(BlueprintType)
struct JOYSTICKPLUGIN_API FForceFeedbackConfigurationBase
{
	GENERATED_BODY()

	FForceFeedbackConfigurationBase()
		: AutoInitialise(true)
		  , AutoStartOnInitialisation(true)
	{
	}

	FForceFeedbackConfigurationBase(const bool bAutoInit, const bool bAutoStartOnInit)
		: AutoInitialise(bAutoInit)
		  , AutoStartOnInitialisation(bAutoStartOnInit)
	{
	}

	void Update(const FForceFeedbackConfigurationBase& Configuration)
	{
		AutoInitialise = Configuration.AutoInitialise;
		AutoStartOnInitialisation = Configuration.AutoStartOnInitialisation;
	}

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Force Feedback|Component|Data")
	bool AutoInitialise;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Force Feedback|Component|Data")
	bool AutoStartOnInitialisation;
};
