#pragma once

#include "Data/DeviceId.h"
#include "Data/DeviceIndex.h"
#include "Data/InstanceId.h"

struct _SDL_Joystick;
typedef struct _SDL_Joystick SDL_Joystick;
struct _SDL_Haptic;
typedef struct _SDL_Haptic SDL_Haptic;

struct FDeviceInfoSDL
{
	FDeviceInfoSDL() {}

	FDeviceIndex DeviceIndex {0};
	FDeviceId DeviceId {0};
	FInstanceId InstanceId {0};

	FString Name = "unknown";

	SDL_Haptic* Haptic = nullptr;
	SDL_Joystick* Joystick = nullptr;
};