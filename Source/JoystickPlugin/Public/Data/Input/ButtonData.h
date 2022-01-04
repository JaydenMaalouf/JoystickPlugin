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
	
	UPROPERTY(VisibleAnywhere, BlueprintReadonly, Category = JoystickInfo)
		bool ButtonState;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadonly, Category = JoystickInfo)
		bool PreviousButtonState;

	FName KeyName;
};
