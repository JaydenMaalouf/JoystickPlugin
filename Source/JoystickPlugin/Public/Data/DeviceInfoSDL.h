// JoystickPlugin is licensed under the MIT License.
// Copyright Jayden Maalouf. All Rights Reserved.

#pragma once

#include "Data/JoystickInformation.h"

THIRD_PARTY_INCLUDES_START

#include "SDL_haptic.h"
#include "SDL_joystick.h"

THIRD_PARTY_INCLUDES_END

struct FDeviceInfoSDL : FJoystickInformation
{
	FDeviceInfoSDL()
		: Connected(false)
		  , Haptic(nullptr)
		  , Joystick(nullptr)
		  , PlayerId(0)
	{
	}

	bool Connected;
	FString SafeDeviceName;

	SDL_Haptic* Haptic;
	SDL_Joystick* Joystick;

	int PlayerId;
};
