#pragma once

#include "Data/JoystickInfo.h"
#include "Data/JoystickState.h"
#include "Data/JoystickPOVDirection.h"

#include "JoystickFunctionLibrary.generated.h"

UCLASS()
class UJoystickFunctionLibrary : public UBlueprintFunctionLibrary
{
	
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintPure, Category = "SDL2 Input|Joystick input")
		static int32 GetDefaultDeviceId();

	UFUNCTION(BlueprintPure, Category = "SDL2 Input|Joystick input")
		static FVector2D POVAxis(EJoystickPOVDirection Direction);

	UFUNCTION(BlueprintPure, Category = "SDL2 Input|Joystick input")
		static FJoystickInfo GetJoystick(int32 DeviceId);

	UFUNCTION(BlueprintPure, Category = "SDL2 Input|Joystick input")
		static FJoystickState GetJoystickState(int32 DeviceId);

	UFUNCTION(BlueprintPure, Category = "SDL2 Input|Joystick input")
		static FJoystickState GetPreviousJoystickState(int32 DeviceId);

	UFUNCTION(BlueprintPure, Category = "SDL2 Input|Joystick input")
		static int32 JoystickCount();

	UFUNCTION(BlueprintCallable, Category = "SDL2 Input|Joystick input")
		static void RegisterForJoystickEvents(UObject* Listener);

	UFUNCTION(BlueprintCallable, Category = "SDL2 Input|Joystick input")
		static void MapJoystickDeviceToPlayer(int32 DeviceId, int32 Player);

	UFUNCTION(BlueprintCallable, Category = "SDL2 Input|Joystick input")
		static void IgnoreGameControllers(bool bIgnore);
};
