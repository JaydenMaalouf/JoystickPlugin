// JoystickPlugin is licensed under the MIT License.
// Copyright Jayden Maalouf. All Rights Reserved.

#pragma once

#include "PlatformTypes.h"
#include "Data/JoystickInformation.h"
#include "Runtime/Launch/Resources/Version.h"

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
		InputDeviceId = FInputDeviceId::CreateFromInternalId(0);
		PlatformUserId = FPlatformUserId::CreateFromInternalId(0);
	}

	const FPlatformUserId& GetPlatformUserId() const
	{
		return PlatformUserId;
	}

	const FInputDeviceId& GetInputDeviceId() const
	{
		return InputDeviceId;
	}

	bool Connected;
	FString SafeDeviceName;

	SDL_Haptic* SDLHaptic;
	SDL_Joystick* SDLJoystick;
	SDL_GameController* SDLGameController;

	FPlatformUserId PlatformUserId;
	FInputDeviceId InputDeviceId;
};
