#include "JoystickHapticDeviceManager.h"

#include "Engine/Engine.h"
#include "JoystickSubsystem.h"

SDL_Haptic* UJoystickHapticDeviceManager::GetHapticDevice(const int32 DeviceId) const
{
	UJoystickSubsystem* JoystickSubsystem = GEngine->GetEngineSubsystem<UJoystickSubsystem>();
	if (JoystickSubsystem == nullptr)
	{
		return nullptr;
	}

	const FDeviceInfoSDL* DeviceInfo = JoystickSubsystem->GetDeviceInfo(DeviceId);
	if (DeviceInfo == nullptr)
	{
		return nullptr;
	}

	return DeviceInfo->Haptic;
}

bool UJoystickHapticDeviceManager::SetAutoCenter(const int32 DeviceId, const int32 Center) 
{
	SDL_Haptic* HapticDevice = GetHapticDevice(DeviceId);
	if (HapticDevice == nullptr)
	{
		return false;
	}

	int32 result = SDL_HapticSetAutocenter(HapticDevice, Center);
	if (result == -1) {
		TCHAR* errorMessage = ANSI_TO_TCHAR(SDL_GetError());
		UE_LOG(LogJoystickPlugin, Log,TEXT("Autocenter error: %s") , errorMessage);
		return false;
	}

	return true;
}

bool UJoystickHapticDeviceManager::SetGain(const int32 DeviceId, const int32 Gain) 
{
	SDL_Haptic* HapticDevice = GetHapticDevice(DeviceId);
	if (HapticDevice == nullptr)
	{
		return false;
	}

	int32 result = SDL_HapticSetGain(HapticDevice, Gain);
	if (result == -1) {
		TCHAR* errorMessage = ANSI_TO_TCHAR(SDL_GetError());
		UE_LOG(LogJoystickPlugin, Log, TEXT("Gain error: %s"), errorMessage);
		return false;
	}

	return true;
}

int32 UJoystickHapticDeviceManager::GetEffectStatus(const int32 DeviceId, const int32 EffectId) 
{
	SDL_Haptic* HapticDevice = GetHapticDevice(DeviceId);
	if (HapticDevice == nullptr)
	{
		return -1;
	}

	int32 result = SDL_HapticGetEffectStatus(HapticDevice,EffectId);
	if (result == -1) {
		TCHAR* errorMessage = ANSI_TO_TCHAR(SDL_GetError());
		UE_LOG(LogJoystickPlugin, Log, TEXT("GetEffectStatus error: %s"), errorMessage);
		return -1;
	}

	return result;
}

void UJoystickHapticDeviceManager::PauseDevice(const int32 DeviceId) const
{
	SDL_Haptic* HapticDevice = GetHapticDevice(DeviceId);
	if (HapticDevice == nullptr)
	{
		return;
	}

	SDL_HapticPause(HapticDevice);
}

void UJoystickHapticDeviceManager::UnpauseDevice(const int32 DeviceId) const
{
	SDL_Haptic* HapticDevice = GetHapticDevice(DeviceId);
	if (HapticDevice == nullptr)
	{
		return;
	}

	SDL_HapticUnpause(HapticDevice);
}

void UJoystickHapticDeviceManager::StopAllEffects(const int32 DeviceId) const
{
	SDL_Haptic* HapticDevice = GetHapticDevice(DeviceId);
	if (HapticDevice == nullptr)
	{
		return;
	}

	SDL_HapticStopAll(HapticDevice);
}

UForceFeedbackEffectBase* UJoystickHapticDeviceManager::CreateEffect(const TSubclassOf<UForceFeedbackEffectBase> ClassType, const bool AutoStart) const
{
	UForceFeedbackEffectBase* effect = NewObject<UForceFeedbackEffectBase>(ClassType);
	effect->AutoStartOnInit = AutoStart;

	if (AutoStart) 
	{
		effect->InitialiseEffect();
	}

	return effect;
}

int32 UJoystickHapticDeviceManager::GetNumEffects(const int32 DeviceId) const
{
	SDL_Haptic* HapticDevice = GetHapticDevice(DeviceId);
	if (HapticDevice == nullptr)
	{
		return -1;
	}

	return SDL_HapticNumEffects(HapticDevice);
}

int32 UJoystickHapticDeviceManager::GetNumEffectsPlaying(const int32 DeviceId) const
{
	SDL_Haptic* HapticDevice = GetHapticDevice(DeviceId);
	if (HapticDevice == nullptr)
	{
		return -1;
	}

	return SDL_HapticNumEffectsPlaying(HapticDevice);
}
