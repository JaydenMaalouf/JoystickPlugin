#pragma once

#include "JoystickDeviceManager.h"
#include "JoystickHapticDeviceManager.h"
#include "Data/JoystickInfo.h"
#include "Data/JoystickPOVDirection.h"

#include "Kismet/BlueprintFunctionLibrary.h"

#include "JoystickFunctionLibrary.generated.h"

UCLASS()
class UJoystickFunctionLibrary : public UBlueprintFunctionLibrary
{
	
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, BlueprintPure)
		static UJoystickDeviceManager* GetJoystickManager();
	
	UFUNCTION(BlueprintCallable, BlueprintPure)
		static UJoystickHapticDeviceManager* GetJoystickHapticManager();

	UFUNCTION(BlueprintPure, Category = "SDL2 Input|Joystick input")
		static FVector2D POVAxis(EJoystickPOVDirection Direction);
};
