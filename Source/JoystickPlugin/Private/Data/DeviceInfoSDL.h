#pragma once

THIRD_PARTY_INCLUDES_START

#include "SDL_haptic.h"
#include "SDL_joystick.h"

THIRD_PARTY_INCLUDES_END

struct _SDL_Joystick;
typedef struct _SDL_Joystick SDL_Joystick;
struct _SDL_Haptic;
typedef struct _SDL_Haptic SDL_Haptic;

struct FDeviceInfoSDL
{
	FDeviceInfoSDL()
		: DeviceIndex(0)
		, DeviceId(0)
		, InstanceId(0)
		, DeviceName("Unknown Device")
		, Haptic(nullptr)
		, Joystick(nullptr)
	{
	}
	
	int32 DeviceIndex;

	int32 DeviceId;
	int32 InstanceId;

	FString DeviceName;

	SDL_Haptic* Haptic;
	SDL_Joystick* Joystick;
};