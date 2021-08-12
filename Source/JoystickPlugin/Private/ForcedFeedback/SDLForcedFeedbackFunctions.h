#pragma once

THIRD_PARTY_INCLUDES_START

#include "SDL_haptic.h"

THIRD_PARTY_INCLUDES_END

class SDLForcedFeedbackFunctions
{

public:
	static SDL_Haptic* GetSDLHapticFromDeviceId(int32 DeviceId);
};