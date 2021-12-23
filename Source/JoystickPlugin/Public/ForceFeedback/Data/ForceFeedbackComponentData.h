#pragma once

#include "ForceFeedbackComponentData.generated.h"

USTRUCT(BlueprintType)
struct JOYSTICKPLUGIN_API FForceFeedbackComponentData
{
	GENERATED_BODY()
public:

	FForceFeedbackComponentData() 
	{

	}

	FForceFeedbackComponentData(bool autoInit, bool autoStartOnInit) : AutoInit(autoInit), AutoStartOnInit(autoStartOnInit)
	{

	}

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Force Feedback|Component|Data")
		bool AutoInit = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Force Feedback|Component|Data")
		bool AutoStartOnInit = false;
};