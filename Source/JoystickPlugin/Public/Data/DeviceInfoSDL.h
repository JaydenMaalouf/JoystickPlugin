// JoystickPlugin is licensed under the MIT License.
// Copyright Jayden Maalouf. All Rights Reserved.

#pragma once

#include "Data/JoystickInformation.h"
#include "Misc/CoreMiscDefines.h"

THIRD_PARTY_INCLUDES_START

#include "SDL_haptic.h"
#include "SDL_joystick.h"
#include "SDL_gamecontroller.h"

THIRD_PARTY_INCLUDES_END

struct FDeviceInfoSDL : FJoystickInformation
{
	FDeviceInfoSDL()
		: Connected(false)
		  , SDLHaptic(nullptr)
		  , SDLJoystick(nullptr)
		  , SDLGameController(nullptr)
	{
#if ENGINE_MAJOR_VERSION == 5 && ENGINE_MINOR_VERSION >= 1
		PlatformUserId = FPlatformUserId::CreateFromInternalId(0);
		InputDeviceId = FInputDeviceId::CreateFromInternalId(0);
#else
		PlayerId = 0;
		DeviceId = 0;
#endif
	}

	bool Connected;
	FString SafeDeviceName;

	SDL_Haptic* SDLHaptic;
	SDL_Joystick* SDLJoystick;
	SDL_GameController* SDLGameController;

#if ENGINE_MAJOR_VERSION == 5 && ENGINE_MINOR_VERSION >= 1
	FPlatformUserId PlatformUserId;
	FInputDeviceId InputDeviceId;
#else
	int PlayerId;
	int DeviceId;
#endif
};
