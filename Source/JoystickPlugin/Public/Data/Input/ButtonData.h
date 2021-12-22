#pragma once

#include "ButtonData.generated.h"

USTRUCT(BlueprintType)
struct JOYSTICKPLUGIN_API FButtonData
{
	GENERATED_BODY()
	
	FButtonData()
		: ButtonState(false)
		, PreviousButtonState(false)
	{
	}
	
	bool ButtonState;
	bool PreviousButtonState;

	FName KeyName;
};
