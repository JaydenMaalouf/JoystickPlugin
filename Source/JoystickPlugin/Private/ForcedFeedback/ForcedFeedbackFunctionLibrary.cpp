#include "ForcedFeedback/ForcedFeedbackFunctionLibrary.h"

#include "JoystickDevice.h"
#include "Interfaces/JoystickPluginInterface.h"
#include "Interfaces/JoystickInterface.h"
#include "JoystickPlugin.h"
#include "DeviceSDL.h"

#include "SDLFunctions.h"

bool UForcedFeedbackFunctionLibrary::SetAutocenter(int32 DeviceId, int Center) 
{
	SDL_Haptic* Haptic = GetSDLHapticFromDeviceId(DeviceId);
	if (Haptic == nullptr)
	{
		return false;
	}

	int Success=SDL_HapticSetAutocenter(Haptic, Center);

	if (Success == -1) {
		TCHAR* Error = ANSI_TO_TCHAR(SDL_GetError());
		UE_LOG(JoystickPluginLog, Log,TEXT("Autocenter error: %s") , Error);
		return false;
	}

	return true;
}

bool UForcedFeedbackFunctionLibrary::SetGain(int32 DeviceId, int Gain) 
{
	SDL_Haptic* Haptic = GetSDLHapticFromDeviceId(DeviceId);
	if (Haptic == nullptr)
	{
		return false;
	}

	int Success = SDL_HapticSetGain(Haptic, Gain);

	if (Success == -1) {
		TCHAR* Error = ANSI_TO_TCHAR(SDL_GetError());
		UE_LOG(JoystickPluginLog, Log, TEXT("Gain error: %s"), Error);
		return false;
	}

	return true;
}

void UForcedFeedbackFunctionLibrary::DestroyEffect(int32 DeviceId, int32 EffectId) 
{
	SDL_Haptic* Haptic = GetSDLHapticFromDeviceId(DeviceId);
	if (Haptic == nullptr)
	{
		return;
	}

	SDL_HapticDestroyEffect(Haptic, EffectId);
}

int UForcedFeedbackFunctionLibrary::GetEffectStatus(int32 DeviceId, int32 EffectId) 
{
	SDL_Haptic* Haptic = GetSDLHapticFromDeviceId(DeviceId);
	if (Haptic == nullptr)
	{
		return -1;
	}

	int Status = SDL_HapticGetEffectStatus(Haptic,EffectId);
	if (Status == -1) {
		TCHAR* Error = ANSI_TO_TCHAR(SDL_GetError());
		UE_LOG(JoystickPluginLog, Log, TEXT("GetEffectStatus error: %s"), Error);
		return -1;
	}
	else {
		return Status;
	}
}

void UForcedFeedbackFunctionLibrary::StopEffect(int32 DeviceId, int32 EffectId) 
{
	SDL_Haptic* Haptic = GetSDLHapticFromDeviceId(DeviceId);
	if (Haptic == nullptr)
	{
		return;
	}

	SDL_HapticStopEffect(Haptic, EffectId);
}

void UForcedFeedbackFunctionLibrary::PauseDevice(int32 DeviceId) 
{
	SDL_Haptic* Haptic = GetSDLHapticFromDeviceId(DeviceId);
	if (Haptic == nullptr)
	{
		return;
	}

	SDL_HapticPause(Haptic);
}

void UForcedFeedbackFunctionLibrary::UnpauseDevice(int32 DeviceId) 
{
	SDL_Haptic* Haptic = GetSDLHapticFromDeviceId(DeviceId);
	if (Haptic == nullptr)
	{
		return;
	}

	SDL_HapticUnpause(Haptic);
}

void UForcedFeedbackFunctionLibrary::StopAllEffects(int32 DeviceId) 
{
	SDL_Haptic* Haptic = GetSDLHapticFromDeviceId(DeviceId);
	if (Haptic == nullptr)
	{
		return;
	}

	SDL_HapticStopAll(Haptic);
}

int UForcedFeedbackFunctionLibrary::GetNumEffects(int32 DeviceId) 
{
	SDL_Haptic* Haptic = GetSDLHapticFromDeviceId(DeviceId);
	if (Haptic == nullptr)
	{
		return -1;
	}

	return SDL_HapticNumEffects(Haptic);
}


int UForcedFeedbackFunctionLibrary::GetNumEffectsPlaying(int32 DeviceId) 
{
	SDL_Haptic* Haptic = GetSDLHapticFromDeviceId(DeviceId);
	if (Haptic == nullptr)
	{
		return -1;
	}

	return SDL_HapticNumEffectsPlaying(Haptic);
}

int32 UForcedFeedbackFunctionLibrary::CreateEffect(int32 DeviceId, FForcedFeedbackData EffectData, int NumIterations, bool Infinity) 
{
	SDL_Haptic* Haptic = GetSDLHapticFromDeviceId(DeviceId);
	if (Haptic == nullptr)
	{
		return -1;
	}

	SDL_HapticEffect Effect = FeedbackDataToSDLHapticEffect(EffectData);

	int EffectId = SDL_HapticNewEffect(Haptic, &Effect);

	if (EffectId == -1) {
		TCHAR* Error = ANSI_TO_TCHAR(SDL_GetError());
		UE_LOG(JoystickPluginLog, Log, TEXT("HapticNewEffect error: %s"), Error);
		return -1;
	}

	if (Infinity) { NumIterations = SDL_HAPTIC_INFINITY; }

	int Success = SDL_HapticRunEffect(Haptic, EffectId, NumIterations);

	if (Success == -1) {
		TCHAR* Error = ANSI_TO_TCHAR(SDL_GetError());
		UE_LOG(JoystickPluginLog, Log, TEXT("HapticRunEffect error: %s"), Error);
		return -1;
	}

	return EffectId;
}

void UForcedFeedbackFunctionLibrary::UpdateEffect(int32 DeviceId, int32 EffectId, FForcedFeedbackData EffectData) {
	SDL_Haptic* Haptic = GetSDLHapticFromDeviceId(DeviceId);
	if (!Haptic) return;
	SDL_HapticEffect Effect = FeedbackDataToSDLHapticEffect(EffectData);

	int Success = SDL_HapticUpdateEffect(Haptic, EffectId, &Effect);

	if (Success == -1) {
		TCHAR* Error = ANSI_TO_TCHAR(SDL_GetError());
		UE_LOG(JoystickPluginLog, Log, TEXT("HapticUpdateEffect error: %s"), Error);
	}
}