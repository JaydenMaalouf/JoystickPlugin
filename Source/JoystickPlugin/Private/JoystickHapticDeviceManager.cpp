// JoystickPlugin is licensed under the MIT License.
// Copyright Jayden Maalouf. All Rights Reserved.

#include "JoystickHapticDeviceManager.h"

#include "Engine/Engine.h"
#include "JoystickSubsystem.h"
#include "Data/DeviceInfoSDL.h"

SDL_Haptic* UJoystickHapticDeviceManager::GetHapticDevice(const int DeviceId) const
{
	UJoystickSubsystem* JoystickSubsystem = GEngine->GetEngineSubsystem<UJoystickSubsystem>();
	if (!IsValid(JoystickSubsystem))
	{
		return nullptr;
	}

	FDeviceInfoSDL DeviceInfo;
	const bool Result = JoystickSubsystem->GetDeviceInfo(DeviceId, DeviceInfo);
	if (!Result)
	{
		return nullptr;
	}

	return DeviceInfo.Haptic;
}

bool UJoystickHapticDeviceManager::SetAutoCenter(const int DeviceId, const int Center)
{
	SDL_Haptic* HapticDevice = GetHapticDevice(DeviceId);
	if (HapticDevice == nullptr)
	{
		return false;
	}

	const int Result = SDL_HapticSetAutocenter(HapticDevice, Center);
	if (Result == -1)
	{
		const FString ErrorMessage = FString(SDL_GetError());
		UE_LOG(LogJoystickPlugin, Log, TEXT("Autocenter error: %s"), *ErrorMessage);
		return false;
	}

	return true;
}

bool UJoystickHapticDeviceManager::SetGain(const int DeviceId, const int Gain)
{
	SDL_Haptic* HapticDevice = GetHapticDevice(DeviceId);
	if (HapticDevice == nullptr)
	{
		return false;
	}

	const int Result = SDL_HapticSetGain(HapticDevice, Gain);
	if (Result == -1)
	{
		const FString ErrorMessage = FString(SDL_GetError());
		UE_LOG(LogJoystickPlugin, Log, TEXT("Gain error: %s"), *ErrorMessage);
		return false;
	}

	return true;
}

int UJoystickHapticDeviceManager::GetEffectStatus(const int DeviceId, const int EffectId)
{
	SDL_Haptic* HapticDevice = GetHapticDevice(DeviceId);
	if (HapticDevice == nullptr)
	{
		return -1;
	}

	const int Result = SDL_HapticGetEffectStatus(HapticDevice, EffectId);
	if (Result == -1)
	{
		const FString ErrorMessage = FString(SDL_GetError());
		UE_LOG(LogJoystickPlugin, Log, TEXT("GetEffectStatus error: %s"), *ErrorMessage);
		return -1;
	}

	return Result;
}

void UJoystickHapticDeviceManager::PauseDevice(const int DeviceId) const
{
	SDL_Haptic* HapticDevice = GetHapticDevice(DeviceId);
	if (HapticDevice == nullptr)
	{
		return;
	}

	SDL_HapticPause(HapticDevice);
}

void UJoystickHapticDeviceManager::UnpauseDevice(const int DeviceId) const
{
	SDL_Haptic* HapticDevice = GetHapticDevice(DeviceId);
	if (HapticDevice == nullptr)
	{
		return;
	}

	SDL_HapticUnpause(HapticDevice);
}

void UJoystickHapticDeviceManager::StopAllEffects(const int DeviceId) const
{
	SDL_Haptic* HapticDevice = GetHapticDevice(DeviceId);
	if (HapticDevice == nullptr)
	{
		return;
	}

	SDL_HapticStopAll(HapticDevice);
}

int UJoystickHapticDeviceManager::GetNumEffects(const int DeviceId) const
{
	SDL_Haptic* HapticDevice = GetHapticDevice(DeviceId);
	if (HapticDevice == nullptr)
	{
		return -1;
	}

	return SDL_HapticNumEffects(HapticDevice);
}

int UJoystickHapticDeviceManager::GetNumEffectsPlaying(const int DeviceId) const
{
	SDL_Haptic* HapticDevice = GetHapticDevice(DeviceId);
	if (HapticDevice == nullptr)
	{
		return -1;
	}

	return SDL_HapticNumEffectsPlaying(HapticDevice);
}
