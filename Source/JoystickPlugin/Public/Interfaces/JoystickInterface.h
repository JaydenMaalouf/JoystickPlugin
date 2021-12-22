#pragma once

#include "UObject/Interface.h"

#include "JoystickInterface.generated.h"

UINTERFACE(BlueprintType)
class UJoystickInterface : public UInterface
{
	GENERATED_BODY()
};

class IJoystickInterface
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintImplementableEvent, Category = "SDL2 Joystick")
		void JoystickButtonPressed(int32 Button);

	UFUNCTION(BlueprintImplementableEvent, Category = "SDL2 Joystick")
		void JoystickButtonReleased(int32 Button);

	UFUNCTION(BlueprintImplementableEvent, Category = "SDL2 Joystick")
		void JoystickAxisChanged(int32 Axis, float value, float valuePrev);

	UFUNCTION(BlueprintImplementableEvent, Category = "SDL2 Joystick")
		void JoystickHatChanged(int32 Hat, EJoystickPOVDirection Value);

	UFUNCTION(BlueprintImplementableEvent, Category = "SDL2 Joystick")
		void JoystickBallMoved(int32 Ball, FVector2D Delta);

	UFUNCTION(BlueprintImplementableEvent, Category = "SDL2 Joystick")
		void JoystickPluggedIn(int32 DeviceId);

	UFUNCTION(BlueprintImplementableEvent, Category = "SDL2 Joystick")
		void JoystickUnplugged(int32 DeviceId);
};
