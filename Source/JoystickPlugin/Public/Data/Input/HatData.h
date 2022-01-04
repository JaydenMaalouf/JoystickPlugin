#pragma once

#include "Data/JoystickPOVDirection.h"
#include "HatData.generated.h"

USTRUCT(BlueprintType)
struct JOYSTICKPLUGIN_API FHatData
{	
	GENERATED_BODY()
	
	UPROPERTY(VisibleAnywhere, BlueprintReadonly, Category = JoystickInfo)
		EJoystickPOVDirection Direction;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadonly, Category = JoystickInfo)
		EJoystickPOVDirection PreviousDirection;

	FName KeyName;
};
