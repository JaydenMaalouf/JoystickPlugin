#pragma once

#include "Data/JoystickPOVDirection.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "JoystickFunctionLibrary.generated.h"

UCLASS()
class UJoystickFunctionLibrary : public UBlueprintFunctionLibrary
{
	
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintPure, Category = "SDL2 Input|Joystick input")
		static FVector2D POVAxis(EJoystickPOVDirection Direction);
	static EJoystickPOVDirection HatValueToDirection(int8 Value);
};
