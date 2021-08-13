#include "ForceFeedback/ForceFeedbackFunctionLibrary.h"

#include "JoystickDeviceManager.h"
#include "Interfaces/JoystickPluginInterface.h"
#include "Interfaces/JoystickInterface.h"
#include "JoystickPlugin.h"
#include "DeviceSDL.h"

#include "SDLForceFeedbackFunctions.h"

bool UForceFeedbackFunctionLibrary::SetAutocenter(int32 DeviceId, int32 Center) 
{
	SDL_Haptic* Haptic = SDLForceFeedbackFunctions::GetSDLHapticFromDeviceId(DeviceId);
	if (Haptic == nullptr)
	{
		return false;
	}

	int32 result = SDL_HapticSetAutocenter(Haptic, Center);
	if (result == -1) {
		TCHAR* errorMessage = ANSI_TO_TCHAR(SDL_GetError());
		UE_LOG(JoystickPluginLog, Log,TEXT("Autocenter error: %s") , errorMessage);
		return false;
	}

	return true;
}

bool UForceFeedbackFunctionLibrary::SetGain(int32 DeviceId, int32 Gain) 
{
	SDL_Haptic* Haptic = SDLForceFeedbackFunctions::GetSDLHapticFromDeviceId(DeviceId);
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

int32 UForceFeedbackFunctionLibrary::GetEffectStatus(int32 DeviceId, int32 EffectId) 
{
	SDL_Haptic* Haptic = SDLForceFeedbackFunctions::GetSDLHapticFromDeviceId(DeviceId);
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

void UForceFeedbackFunctionLibrary::PauseDevice(int32 DeviceId) 
{
	SDL_Haptic* Haptic = SDLForceFeedbackFunctions::GetSDLHapticFromDeviceId(DeviceId);
	if (Haptic == nullptr)
	{
		return;
	}

	SDL_HapticPause(Haptic);
}

void UForceFeedbackFunctionLibrary::UnpauseDevice(int32 DeviceId) 
{
	SDL_Haptic* Haptic = SDLForceFeedbackFunctions::GetSDLHapticFromDeviceId(DeviceId);
	if (Haptic == nullptr)
	{
		return;
	}

	SDL_HapticUnpause(Haptic);
}

void UForceFeedbackFunctionLibrary::StopAllEffects(int32 DeviceId) 
{
	SDL_Haptic* Haptic = SDLForceFeedbackFunctions::GetSDLHapticFromDeviceId(DeviceId);
	if (Haptic == nullptr)
	{
		return;
	}

	SDL_HapticStopAll(Haptic);
}

UForceFeedbackEffectBase* UForceFeedbackFunctionLibrary::CreateEffect(TSubclassOf<UForceFeedbackEffectBase> classType, bool AutoStart)
{
	UForceFeedbackEffectBase* effect = NewObject<UForceFeedbackEffectBase>(classType);
	effect->AutoStartOnInit = AutoStart;

	if (AutoStart) 
	{
		effect->InitialiseEffect();
	}

	return effect;
}

int32 UForceFeedbackFunctionLibrary::GetNumEffects(int32 DeviceId) 
{
	SDL_Haptic* Haptic = SDLForceFeedbackFunctions::GetSDLHapticFromDeviceId(DeviceId);
	if (Haptic == nullptr)
	{
		return -1;
	}

	return SDL_HapticNumEffects(Haptic);
}


int32 UForceFeedbackFunctionLibrary::GetNumEffectsPlaying(int32 DeviceId) 
{
	SDL_Haptic* Haptic = SDLForceFeedbackFunctions::GetSDLHapticFromDeviceId(DeviceId);
	if (Haptic == nullptr)
	{
		return -1;
	}

	return SDL_HapticNumEffectsPlaying(Haptic);
}