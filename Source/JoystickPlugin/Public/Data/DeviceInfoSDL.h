// JoystickPlugin is licensed under the MIT License.
// Copyright Jayden Maalouf 2026. All Rights Reserved.

#pragma once

#include "PlatformTypes.h"
#include "Data/JoystickInformation.h"
#include "Runtime/Launch/Resources/Version.h"

THIRD_PARTY_INCLUDES_START

#include "JoystickSDL.h"

THIRD_PARTY_INCLUDES_END

struct FDeviceInfoSDL : FJoystickInformation
{
	FDeviceInfoSDL()
		: Connected(false)
		  , SDLHaptic(nullptr)
		  , SDLJoystick(nullptr)
		  , SDLGamepad(nullptr)
		  , EngineRumbleEffectId(-1)
		  , EngineRumbleLevel(0)
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
	SDL_Haptic* SDLHaptic;
	SDL_Joystick* SDLJoystick;
	SDL_Gamepad* SDLGamepad;
	int EngineRumbleEffectId;
	int16 EngineRumbleLevel;

private:
	FPlatformUserId PlatformUserId;
	FInputDeviceId InputDeviceId;
};
