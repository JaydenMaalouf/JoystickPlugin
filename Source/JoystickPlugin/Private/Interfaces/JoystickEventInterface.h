#pragma once

#include "Data/DeviceId.h"
#include "Data/JoystickPOVDirection.h"
#include "Data/DeviceInfoSDL.h"

class IJoystickEventInterface
{
public:
	virtual ~IJoystickEventInterface()
	{
	}

	virtual void JoystickPluggedIn(const FDeviceInfoSDL &Device) = 0;
	virtual void JoystickUnplugged(FDeviceId DeviceId) = 0;
	virtual void JoystickButton(FDeviceId DeviceId, int32 Button, bool Pressed) = 0;
	virtual void JoystickAxis(FDeviceId DeviceId, int32 Axis, float Value) = 0;
	virtual void JoystickHat(FDeviceId DeviceId, int32 Hat, EJoystickPOVDirection Value) = 0;
	virtual void JoystickBall(FDeviceId DeviceId, int32 Ball, FVector2D Delta) = 0;
};