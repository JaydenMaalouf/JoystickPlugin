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

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool AutoInit = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool AutoStartOnInit = false;
};