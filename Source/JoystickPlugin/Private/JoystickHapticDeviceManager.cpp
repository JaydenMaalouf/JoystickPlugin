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

FDeviceInfoSDL* UJoystickHapticDeviceManager::GetDeviceInfo(const FJoystickInstanceId& InstanceId) const
{
	if (!IsValid(GEngine))
	{
		return nullptr;
	}

	UJoystickSubsystem* JoystickSubsystem = GEngine->GetEngineSubsystem<UJoystickSubsystem>();
	if (!IsValid(JoystickSubsystem))
	{
		return nullptr;
	}

	return JoystickSubsystem->GetDeviceInfo(InstanceId);
}

SDL_Haptic* UJoystickHapticDeviceManager::GetHapticDevice(const FJoystickInstanceId& InstanceId) const
{
	const FDeviceInfoSDL* DeviceInfo = GetDeviceInfo(InstanceId);
	if (DeviceInfo == nullptr)
	{
		return nullptr;
	}

	return DeviceInfo->SDLHaptic;
}

bool UJoystickHapticDeviceManager::SetAutoCenter(const FJoystickInstanceId& InstanceId, const int Center)
{
	SDL_Haptic* HapticDevice = GetHapticDevice(InstanceId);
	if (HapticDevice == nullptr)
	{
		return false;
	}

	const int Result = SDL_HapticSetAutocenter(HapticDevice, FMath::Clamp(Center, 0, 100));
	if (Result == -1)
	{
		FJoystickLogManager::Get()->LogSDLError("SDL_HapticSetAutocenter Error");
		return false;
	}

	return true;
}

bool UJoystickHapticDeviceManager::SetGain(const FJoystickInstanceId& InstanceId, const int Gain)
{
	SDL_Haptic* HapticDevice = GetHapticDevice(InstanceId);
	if (HapticDevice == nullptr)
	{
		return false;
	}

	const int Result = SDL_HapticSetGain(HapticDevice, FMath::Clamp(Gain, 0, 100));
	if (Result == -1)
	{
		FJoystickLogManager::Get()->LogSDLError("SDL_HapticSetGain Error");
		return false;
	}

	return true;
}

int UJoystickHapticDeviceManager::GetEffectStatus(const FJoystickInstanceId& InstanceId, const int EffectId)
{
	SDL_Haptic* HapticDevice = GetHapticDevice(InstanceId);
	if (HapticDevice == nullptr)
	{
		return -1;
	}

	const int Result = SDL_HapticGetEffectStatus(HapticDevice, EffectId);
	if (Result == -1)
	{
		FJoystickLogManager::Get()->LogSDLError("SDL_HapticGetEffectStatus Error");
		return -1;
	}

	return Result;
}

bool UJoystickHapticDeviceManager::PlayRumble(const FJoystickInstanceId& InstanceId, const float LowFrequencyRumble, const float HighFrequencyRumble, const float Duration)
{
	if (LowFrequencyRumble == 0 && HighFrequencyRumble == 0)
	{
		return false;
	}

#if ENGINE_MAJOR_VERSION == 5
	const FDeviceInfoSDL* DeviceInfo = GetDeviceInfo(InstanceId);
	if (DeviceInfo == nullptr || DeviceInfo->SDLJoystick == nullptr)
	{
		return false;
	}

	const Uint16 LowFrequency = FMath::Clamp<Uint16>(LowFrequencyRumble * UINT16_MAX, 0, UINT16_MAX);
	const Uint16 HighFrequency = FMath::Clamp<Uint16>(HighFrequencyRumble * UINT16_MAX, 0, UINT16_MAX);
	const Uint32 ClampedDuration = Duration == -1 ? SDL_HAPTIC_INFINITY : FMath::Clamp<Uint32>(Duration * 1000.0f, 0, UINT32_MAX);
	const int Result = SDL_JoystickRumble(DeviceInfo->SDLJoystick, LowFrequency, HighFrequency, ClampedDuration);
	if (Result == -1)
	{
		FJoystickLogManager::Get()->LogSDLError("SDL_JoystickRumble Error");
		return false;
	}
	return true;
#else
	FJoystickLogManager::Get()->LogError(TEXT("PlayRumble is not supported on this engine version."));
	return false;
#endif
}

bool UJoystickHapticDeviceManager::StopRumble(const FJoystickInstanceId& InstanceId)
{
	SDL_Haptic* HapticDevice = GetHapticDevice(InstanceId);
	if (HapticDevice == nullptr)
	{
		return false;
	}

	const int Result = SDL_HapticRumbleStop(HapticDevice);
	if (Result == -1)
	{
		FJoystickLogManager::Get()->LogSDLError("SDL_HapticRumbleStop Error");
		return false;
	}

	return true;
}

int UJoystickHapticDeviceManager::CreateEffect(const FJoystickInstanceId& InstanceId, SDL_HapticEffect& Effect) const
{
	SDL_Haptic* HapticDevice = GetHapticDevice(InstanceId);
	if (HapticDevice == nullptr)
	{
		return -1;
	}

	const int EffectId = SDL_HapticNewEffect(HapticDevice, &Effect);
	if (EffectId == -1)
	{
		FJoystickLogManager::Get()->LogSDLError("SDL_HapticNewEffect Error");
	}

	return EffectId;
}

bool UJoystickHapticDeviceManager::UpdateEffect(const FJoystickInstanceId& InstanceId, const int EffectId, SDL_HapticEffect& Effect) const
{
	SDL_Haptic* HapticDevice = GetHapticDevice(InstanceId);
	if (HapticDevice == nullptr)
	{
		return false;
	}

	const int Result = SDL_HapticUpdateEffect(HapticDevice, EffectId, &Effect);
	if (Result != 0)
	{
		FJoystickLogManager::Get()->LogSDLError("SDL_HapticUpdateEffect Error");
		return false;
	}

	return true;
}

bool UJoystickHapticDeviceManager::RunEffect(const FJoystickInstanceId& InstanceId, const int EffectId, const int Iterations) const
{
	SDL_Haptic* HapticDevice = GetHapticDevice(InstanceId);
	if (HapticDevice == nullptr)
	{
		return false;
	}

	const int Result = SDL_HapticRunEffect(HapticDevice, EffectId, Iterations);
	if (Result != 0)
	{
		FJoystickLogManager::Get()->LogSDLError("SDL_HapticRunEffect Error");
		return false;
	}

	return true;
}

bool UJoystickHapticDeviceManager::StopEffect(const FJoystickInstanceId& InstanceId, const int EffectId) const
{
	SDL_Haptic* HapticDevice = GetHapticDevice(InstanceId);
	if (HapticDevice == nullptr)
	{
		return false;
	}

	const int Result = SDL_HapticStopEffect(HapticDevice, EffectId);
	if (Result != 0)
	{
		FJoystickLogManager::Get()->LogSDLError("SDL_HapticStopEffect Error");
		return false;
	}

	return true;
}

void UJoystickHapticDeviceManager::DestroyEffect(const FJoystickInstanceId& InstanceId, const int EffectId) const
{
	SDL_Haptic* HapticDevice = GetHapticDevice(InstanceId);
	if (HapticDevice == nullptr)
	{
		return;
	}

	SDL_HapticDestroyEffect(HapticDevice, EffectId);
}

bool UJoystickHapticDeviceManager::PauseDevice(const FJoystickInstanceId& InstanceId) const
{
	SDL_Haptic* HapticDevice = GetHapticDevice(InstanceId);
	if (HapticDevice == nullptr)
	{
		return false;
	}

	const int Result = SDL_HapticPause(HapticDevice);
	if (Result != 0)
	{
		FJoystickLogManager::Get()->LogSDLError("SDL_HapticPause Error");
		return false;
	}

	return true;
}

bool UJoystickHapticDeviceManager::UnpauseDevice(const FJoystickInstanceId& InstanceId) const
{
	SDL_Haptic* HapticDevice = GetHapticDevice(InstanceId);
	if (HapticDevice == nullptr)
	{
		return false;
	}

	const int Result = SDL_HapticUnpause(HapticDevice);
	if (Result != 0)
	{
		FJoystickLogManager::Get()->LogSDLError("SDL_HapticUnpause Error");
		return false;
	}

	return true;
}

bool UJoystickHapticDeviceManager::StopAllEffects(const FJoystickInstanceId& InstanceId) const
{
	SDL_Haptic* HapticDevice = GetHapticDevice(InstanceId);
	if (HapticDevice == nullptr)
	{
		return false;
	}

	const int Result = SDL_HapticStopAll(HapticDevice);
	if (Result != 0)
	{
		FJoystickLogManager::Get()->LogSDLError("SDL_HapticStopAll Error");
		return false;
	}

	return true;
}

int UJoystickHapticDeviceManager::GetNumEffects(const FJoystickInstanceId& InstanceId) const
{
	SDL_Haptic* HapticDevice = GetHapticDevice(InstanceId);
	if (HapticDevice == nullptr)
	{
		return -1;
	}

	const int Result = SDL_HapticNumEffects(HapticDevice);
	if (Result < 0)
	{
		FJoystickLogManager::Get()->LogSDLError("SDL_HapticNumEffects Error");
	}

	return Result;
}

int UJoystickHapticDeviceManager::GetNumEffectsPlaying(const FJoystickInstanceId& InstanceId) const
{
	SDL_Haptic* HapticDevice = GetHapticDevice(InstanceId);
	if (HapticDevice == nullptr)
	{
		return -1;
	}

	const int Result = SDL_HapticNumEffectsPlaying(HapticDevice);
	if (Result < 0)
	{
		FJoystickLogManager::Get()->LogSDLError("SDL_HapticNumEffectsPlaying Error");
	}

	return Result;
}
