// JoystickPlugin is licensed under the MIT License.
// Copyright Jayden Maalouf. All Rights Reserved.

#pragma once

#include "Data/JoystickInformation.h"

THIRD_PARTY_INCLUDES_START

#include "SDL_haptic.h"
#include "SDL_joystick.h"
#include "SDL_gamecontroller.h"

THIRD_PARTY_INCLUDES_END

struct JOYSTICKPLUGIN_API FDeviceInfoSDL : FJoystickInformation
{
	FDeviceInfoSDL()
		: Connected(false)
		  , Haptic(nullptr)
		  , Joystick(nullptr)
		  , GameController(nullptr)
		  , PlayerId(0)
		  , InternalDeviceIndex(-1)
	{
	}

	bool Connected;
	FString SafeDeviceName;

	SDL_Haptic* Haptic;
	SDL_Joystick* Joystick;
	SDL_GameController* GameController;

	int PlayerId;
	int InternalDeviceIndex;
};
