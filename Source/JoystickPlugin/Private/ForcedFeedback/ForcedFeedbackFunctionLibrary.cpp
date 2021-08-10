#include "ForcedFeedback/ForcedFeedbackFunctionLibrary.h"

#include "JoystickDeviceManager.h"
#include "Interfaces/JoystickPluginInterface.h"
#include "Interfaces/JoystickInterface.h"
#include "JoystickPlugin.h"
#include "DeviceSDL.h"

#include "ForcedFeedbackSDLFunctions.h"

bool UForcedFeedbackFunctionLibrary::SetAutocenter(int32 DeviceId, int32 Center) 
{
	SDL_Haptic* Haptic = SDLFunctions::GetSDLHapticFromDeviceId(DeviceId);
	if (Haptic == nullptr)
	{
		return false;
	}

	int32 result=SDL_HapticSetAutocenter(Haptic, Center);
	if (result == -1) {
		TCHAR* errorMessage = ANSI_TO_TCHAR(SDL_GetError());
		UE_LOG(JoystickPluginLog, Log,TEXT("Autocenter error: %s") , errorMessage);
		return false;
	}

	return true;
}

bool UForcedFeedbackFunctionLibrary::SetGain(int32 DeviceId, int32 Gain) 
{
	SDL_Haptic* Haptic = SDLFunctions::GetSDLHapticFromDeviceId(DeviceId);
	if (Haptic == nullptr)
	{
		return false;
	}

	int32 result = SDL_HapticSetGain(Haptic, Gain);
	if (result == -1) {
		TCHAR* errorMessage = ANSI_TO_TCHAR(SDL_GetError());
		UE_LOG(JoystickPluginLog, Log, TEXT("Gain error: %s"), errorMessage);
		return false;
	}

	return true;
}

int32 UForcedFeedbackFunctionLibrary::GetEffectStatus(int32 DeviceId, int32 EffectId) 
{
	SDL_Haptic* Haptic = SDLFunctions::GetSDLHapticFromDeviceId(DeviceId);
	if (Haptic == nullptr)
	{
		return -1;
	}

	int32 result = SDL_HapticGetEffectStatus(Haptic,EffectId);
	if (result == -1) {
		TCHAR* errorMessage = ANSI_TO_TCHAR(SDL_GetError());
		UE_LOG(JoystickPluginLog, Log, TEXT("GetEffectStatus error: %s"), errorMessage);
		return -1;
	}

	return result;
}

void UForcedFeedbackFunctionLibrary::PauseDevice(int32 DeviceId) 
{
	SDL_Haptic* Haptic = SDLFunctions::GetSDLHapticFromDeviceId(DeviceId);
	if (Haptic == nullptr)
	{
		return;
	}

	SDL_HapticPause(Haptic);
}

void UForcedFeedbackFunctionLibrary::UnpauseDevice(int32 DeviceId) 
{
	SDL_Haptic* Haptic = SDLFunctions::GetSDLHapticFromDeviceId(DeviceId);
	if (Haptic == nullptr)
	{
		return;
	}

	SDL_HapticUnpause(Haptic);
}

void UForcedFeedbackFunctionLibrary::StopAllEffects(int32 DeviceId) 
{
	SDL_Haptic* Haptic = SDLFunctions::GetSDLHapticFromDeviceId(DeviceId);
	if (Haptic == nullptr)
	{
		return;
	}

	SDL_HapticStopAll(Haptic);
}

int32 UForcedFeedbackFunctionLibrary::GetNumEffects(int32 DeviceId) 
{
	SDL_Haptic* Haptic = SDLFunctions::GetSDLHapticFromDeviceId(DeviceId);
	if (Haptic == nullptr)
	{
		return -1;
	}

	return SDL_HapticNumEffects(Haptic);
}


int32 UForcedFeedbackFunctionLibrary::GetNumEffectsPlaying(int32 DeviceId) 
{
	SDL_Haptic* Haptic = SDLFunctions::GetSDLHapticFromDeviceId(DeviceId);
	if (Haptic == nullptr)
	{
		return -1;
	}

	return SDL_HapticNumEffectsPlaying(Haptic);
}