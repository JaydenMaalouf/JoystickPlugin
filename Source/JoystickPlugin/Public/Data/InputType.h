#pragma once

#include "InputType.generated.h"

UENUM(BlueprintType)
enum class EInputType : uint8
{
	INPUTTYPE_UNKNOWN,
	INPUTTYPE_JOYSTICK,
	INPUTTYPE_GAMECONTROLLER,	
};