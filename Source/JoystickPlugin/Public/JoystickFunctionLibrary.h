#pragma once

#include "Data/JoystickPOVDirection.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "JoystickFunctionLibrary.generated.h"

UCLASS()
class JOYSTICKPLUGIN_API UJoystickFunctionLibrary final : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintPure, Category = "SDL2 Input|Joystick input")
	static FVector2D POVAxis(const EJoystickPOVDirection Direction);

	static EJoystickPOVDirection HatValueToDirection(int8 Value);
};
