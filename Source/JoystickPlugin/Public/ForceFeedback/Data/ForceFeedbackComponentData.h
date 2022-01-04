#pragma once

#include "ForceFeedbackComponentData.generated.h"

USTRUCT(BlueprintType)
struct JOYSTICKPLUGIN_API FForceFeedbackComponentData
{
	GENERATED_BODY()

	FForceFeedbackComponentData()
		: AutoInit(false)
		, AutoStartOnInit(false)
	{

	}

	FForceFeedbackComponentData(const bool bAutoInit, const bool bAutoStartOnInit)
		: AutoInit(bAutoInit)
		, AutoStartOnInit(bAutoStartOnInit)
	{

	}

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Force Feedback|Component|Data")
		bool AutoInit;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Force Feedback|Component|Data")
		bool AutoStartOnInit;
};