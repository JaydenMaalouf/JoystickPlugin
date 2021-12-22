#pragma once

#include "Data/JoystickPOVDirection.h"
#include "Data/DeviceInfoSDL.h"

class IJoystickEventInterface
{
public:
	virtual ~IJoystickEventInterface()
	{
	}

	virtual void JoystickPluggedIn(const FDeviceInfoSDL &Device) = 0;
	virtual void JoystickUnplugged(int32 DeviceId) = 0;
	virtual void JoystickButton(int32 DeviceId, int32 Button, bool Pressed) = 0;
	virtual void JoystickAxis(int32 DeviceId, int32 Axis, float Value) = 0;
	virtual void JoystickHat(int32 DeviceId, int32 Hat, EJoystickPOVDirection Value) = 0;
	virtual void JoystickBall(int32 DeviceId, int32 Ball, FVector2D Value) = 0;
};