#pragma once

#include "Data/JoystickPOVDirection.h"
#include "HatData.generated.h"

USTRUCT(BlueprintType)
struct JOYSTICKPLUGIN_API FHatData
{	
	GENERATED_BODY()
	
	EJoystickPOVDirection Direction;
	EJoystickPOVDirection PreviousDirection;

	FName KeyName;
};
