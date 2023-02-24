// JoystickPlugin is licensed under the MIT License.
// Copyright Jayden Maalouf. All Rights Reserved.

#pragma once

THIRD_PARTY_INCLUDES_START

#include "SDL_haptic.h"
#include "SDL_joystick.h"

THIRD_PARTY_INCLUDES_END

struct FDeviceInfoSDL
{
	FDeviceInfoSDL()
		: DeviceIndex(0)
		  , DeviceId(0)
		  , InstanceId(0)
		  , IsGamepad(false)
		  , HasRumble(false)
		  , DeviceName("Unknown Device")
		  , Haptic(nullptr)
		  , Joystick(nullptr)
	{
	}

	int DeviceIndex;

	int DeviceId;
	int InstanceId;

	bool IsGamepad;
	bool HasRumble;

	FString DeviceName;
	FGuid ProductId;

	SDL_Haptic* Haptic;
	SDL_Joystick* Joystick;
};
