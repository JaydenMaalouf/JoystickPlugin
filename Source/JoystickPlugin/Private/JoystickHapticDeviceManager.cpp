// JoystickPlugin is licensed under the MIT License.
// Copyright Jayden Maalouf. All Rights Reserved.

#include "JoystickHapticDeviceManager.h"
#include "JoystickLogManager.h"
#include "Engine/Engine.h"
#include "JoystickSubsystem.h"
#include "Data/DeviceInfoSDL.h"
#include "Runtime/Launch/Resources/Version.h"

THIRD_PARTY_INCLUDES_START

#include "SDL_haptic.h"

THIRD_PARTY_INCLUDES_END

FDeviceInfoSDL* UJoystickHapticDeviceManager::GetDeviceInfo(const int DeviceId) const
{
	UJoystickSubsystem* JoystickSubsystem = GEngine->GetEngineSubsystem<UJoystickSubsystem>();
	if (!IsValid(JoystickSubsystem))
	{
		return nullptr;
	}

	return JoystickSubsystem->GetDeviceInfo(DeviceId);
}

SDL_Haptic* UJoystickHapticDeviceManager::GetHapticDevice(const int DeviceId) const
{
	const FDeviceInfoSDL* DeviceInfo = GetDeviceInfo(DeviceId);
	if (DeviceInfo == nullptr)
	{
		return nullptr;
	}

	return DeviceInfo->Haptic;
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
		FJoystickLogManager::Get()->LogError(TEXT("Autocenter Error: %s"), *ErrorMessage);
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
		FJoystickLogManager::Get()->LogError(TEXT("Gain Error: %s"), *ErrorMessage);
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
		FJoystickLogManager::Get()->LogError(TEXT("GetEffectStatus Error: %s"), *ErrorMessage);
		return -1;
	}

	return Result;
}

void UJoystickHapticDeviceManager::PlayRumble(const int DeviceId, const float LowFrequencyRumble, const float HighFrequencyRumble, const float Duration) const
{
#if (ENGINE_MAJOR_VERSION == 4 && ENGINE_MINOR_VERSION >= 27 || ENGINE_MAJOR_VERSION == 5)
	const FDeviceInfoSDL* DeviceInfo = GetDeviceInfo(DeviceId);
	if (DeviceInfo == nullptr)
	{
		return;
	}

	const Uint16 LowFrequency = FMath::Clamp<Uint16>(LowFrequencyRumble * UINT16_MAX, 0, UINT16_MAX);
	const Uint16 HighFrequency = FMath::Clamp<Uint16>(HighFrequencyRumble * UINT16_MAX, 0, UINT16_MAX);
	const Uint32 ClampedDuration = Duration == -1 ? SDL_HAPTIC_INFINITY : FMath::Clamp<Uint32>(Duration * 1000.0f, 0, UINT32_MAX);
	SDL_JoystickRumble(DeviceInfo->Joystick, LowFrequency, HighFrequency, ClampedDuration);
#else
	FJoystickLogManager::Get()->LogError(TEXT("PlayRumble not supported on this engine version."));
#endif
}

void UJoystickHapticDeviceManager::StopRumble(const int DeviceId)
{
	SDL_Haptic* HapticDevice = GetHapticDevice(DeviceId);
	if (HapticDevice == nullptr)
	{
		return;
	}

	SDL_HapticRumbleStop(HapticDevice);
}

int UJoystickHapticDeviceManager::CreateEffect(const int DeviceId, SDL_HapticEffect& Effect) const
{
	SDL_Haptic* HapticDevice = GetHapticDevice(DeviceId);
	if (HapticDevice == nullptr)
	{
		return -1;
	}

	const int EffectId = SDL_HapticNewEffect(HapticDevice, &Effect);
	if (EffectId == -1)
	{
		const FString ErrorMessage = FString(SDL_GetError());
		FJoystickLogManager::Get()->LogError(TEXT("Haptic CreateEffect Error: %s"), *ErrorMessage);
	}

	return EffectId;
}

bool UJoystickHapticDeviceManager::UpdateEffect(const int DeviceId, const int EffectId, SDL_HapticEffect& Effect) const
{
	SDL_Haptic* HapticDevice = GetHapticDevice(DeviceId);
	if (HapticDevice == nullptr)
	{
		return false;
	}

	const int Result = SDL_HapticUpdateEffect(HapticDevice, EffectId, &Effect);
	if (Result != 0)
	{
		const FString ErrorMessage = FString(SDL_GetError());
		FJoystickLogManager::Get()->LogError(TEXT("Haptic UpdateEffect Error: %s"), *ErrorMessage);
		return false;
	}

	return true;
}

bool UJoystickHapticDeviceManager::RunEffect(const int DeviceId, const int EffectId, const int Iterations) const
{
	SDL_Haptic* HapticDevice = GetHapticDevice(DeviceId);
	if (HapticDevice == nullptr)
	{
		return false;
	}

	const int Result = SDL_HapticRunEffect(HapticDevice, EffectId, Iterations);
	if (Result != 0)
	{
		const FString ErrorMessage = FString(SDL_GetError());
		FJoystickLogManager::Get()->LogError(TEXT("Haptic RunEffect Error: %s"), *ErrorMessage);
		return false;
	}

	return true;
}

bool UJoystickHapticDeviceManager::StopEffect(const int DeviceId, const int EffectId) const
{
	SDL_Haptic* HapticDevice = GetHapticDevice(DeviceId);
	if (HapticDevice == nullptr)
	{
		return false;
	}

	const int Result = SDL_HapticStopEffect(HapticDevice, EffectId);
	if (Result != 0)
	{
		const FString ErrorMessage = FString(SDL_GetError());
		FJoystickLogManager::Get()->LogError(TEXT("Haptic StopEffect Error: %s"), *ErrorMessage);
		return false;
	}

	return true;
}

void UJoystickHapticDeviceManager::DestroyEffect(const int DeviceId, const int EffectId) const
{
	SDL_Haptic* HapticDevice = GetHapticDevice(DeviceId);
	if (HapticDevice == nullptr)
	{
		return;
	}

	SDL_HapticDestroyEffect(HapticDevice, EffectId);
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
