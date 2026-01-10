// JoystickPlugin is licensed under the MIT License.
// Copyright Jayden Maalouf 2026. All Rights Reserved.

#pragma once

#include "PlatformTypes.h"
#include "Data/JoystickInformation.h"
#include "Runtime/Launch/Resources/Version.h"

THIRD_PARTY_INCLUDES_START

#include "SDL_haptic.h"
#include "SDL_joystick.h"
#include "SDL_gamepad.h"

THIRD_PARTY_INCLUDES_END

struct FDeviceInfoSDL : FJoystickInformation
{
	FDeviceInfoSDL()
		: Connected(false)
		  , SDLHaptic(nullptr)
		  , SDLJoystick(nullptr)
		  , SDLGamepad(nullptr)
	{
		InputDeviceId = FInputDeviceId::CreateFromInternalId(0);
		PlatformUserId = FPlatformUserId::CreateFromInternalId(0);
	}

	void SetInputDeviceId(const FInputDeviceId& Id)
	{
		InputDeviceId = Id;
	}

	const FInputDeviceId& GetInputDeviceId() const
	{
		return InputDeviceId;
	}

	void SetPlatformUserId(const FPlatformUserId& UserId)
	{
		PlatformUserId = UserId;
	}

	const FPlatformUserId& GetPlatformUserId() const
	{
		return PlatformUserId;
	}

	bool Connected;
	FString SafeDeviceName;
	SDL_Haptic* SDLHaptic;
	SDL_Joystick* SDLJoystick;
	SDL_Gamepad* SDLGamepad;

private:
	FPlatformUserId PlatformUserId;
	FInputDeviceId InputDeviceId;
};
