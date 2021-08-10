#pragma once

#include "JoystickDeviceManager.h"
#include "Interfaces/JoystickPluginInterface.h"
#include "Interfaces/JoystickInterface.h"
#include "JoystickPlugin.h"
#include "DeviceSDL.h"

THIRD_PARTY_INCLUDES_START

#include "SDL.h"

THIRD_PARTY_INCLUDES_END

class SDLFunctions
{

public:
	static SDL_Haptic* GetSDLHapticFromDeviceId(int32 DeviceId);
};