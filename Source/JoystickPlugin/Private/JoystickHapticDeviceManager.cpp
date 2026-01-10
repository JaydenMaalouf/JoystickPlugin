// JoystickPlugin is licensed under the MIT License.
// Copyright Jayden Maalouf 2026. All Rights Reserved.

#include "JoystickHapticDeviceManager.h"

#include "Data/DeviceInfoSDL.h"
#include "Engine/Engine.h"
#include "JoystickLogManager.h"
#include "JoystickSubsystem.h"
#include "Runtime/Launch/Resources/Version.h"

THIRD_PARTY_INCLUDES_START

#include "SDL_haptic.h"

THIRD_PARTY_INCLUDES_END

const UJoystickHapticDeviceManager* UJoystickHapticDeviceManager::GetJoystickHapticDeviceManager()
{
	return GetDefault<UJoystickHapticDeviceManager>();
}

bool UJoystickHapticDeviceManager::SetAutoCenter(const FJoystickInstanceId& InstanceId, const int Center) const
{
	auto [HapticDevice, HapticDeviceResult] = GetHapticDevice(InstanceId);
	if (HapticDevice == nullptr || HapticDeviceResult.bSuccess == false)
	{
		FJoystickLogManager::Get()->LogError(HapticDeviceResult);
		return false;
	}

	const int Result = SDL_HapticSetAutocenter(HapticDevice, FMath::Clamp(Center, 0, 100));
	if (Result == -1)
	{
		FJoystickLogManager::Get()->LogSDLError(TEXT("SDL_HapticSetAutocenter failed"));
		return false;
	}

	return true;
}

bool UJoystickHapticDeviceManager::SetGain(const FJoystickInstanceId& InstanceId, const int Gain) const
{
	auto [HapticDevice, HapticDeviceResult] = GetHapticDevice(InstanceId);
	if (HapticDevice == nullptr || HapticDeviceResult.bSuccess == false)
	{
		FJoystickLogManager::Get()->LogError(HapticDeviceResult);
		return false;
	}

	const int Result = SDL_HapticSetGain(HapticDevice, FMath::Clamp(Gain, 0, 100));
	if (Result == -1)
	{
		FJoystickLogManager::Get()->LogSDLError(TEXT("SDL_HapticSetGain failed"));
		return false;
	}

	return true;
}

bool UJoystickHapticDeviceManager::PauseDevice(const FJoystickInstanceId& InstanceId) const
{
	auto [HapticDevice, HapticDeviceResult] = GetHapticDevice(InstanceId);
	if (HapticDevice == nullptr || HapticDeviceResult.bSuccess == false)
	{
		FJoystickLogManager::Get()->LogError(HapticDeviceResult);
		return false;
	}

	const int Result = SDL_HapticPause(HapticDevice);
	if (Result != 0)
	{
		FJoystickLogManager::Get()->LogSDLError(TEXT("SDL_HapticPause failed"));
		return false;
	}

	return true;
}

bool UJoystickHapticDeviceManager::UnpauseDevice(const FJoystickInstanceId& InstanceId) const
{
	auto [HapticDevice, HapticDeviceResult] = GetHapticDevice(InstanceId);
	if (HapticDevice == nullptr || HapticDeviceResult.bSuccess == false)
	{
		FJoystickLogManager::Get()->LogError(HapticDeviceResult);
		return false;
	}

	const int Result = SDL_HapticUnpause(HapticDevice);
	if (Result != 0)
	{
		FJoystickLogManager::Get()->LogSDLError(TEXT("SDL_HapticUnpause failed"));
		return false;
	}

	return true;
}

bool UJoystickHapticDeviceManager::StopAllEffects(const FJoystickInstanceId& InstanceId) const
{
	auto [HapticDevice, HapticDeviceResult] = GetHapticDevice(InstanceId);
	if (HapticDevice == nullptr || HapticDeviceResult.bSuccess == false)
	{
		FJoystickLogManager::Get()->LogError(HapticDeviceResult);
		return false;
	}

	const int Result = SDL_HapticStopAll(HapticDevice);
	if (Result != 0)
	{
		FJoystickLogManager::Get()->LogSDLError(TEXT("SDL_HapticStopAll failed"));
		return false;
	}

	return true;
}

int UJoystickHapticDeviceManager::GetNumEffects(const FJoystickInstanceId& InstanceId) const
{
	auto [HapticDevice, HapticDeviceResult] = GetHapticDevice(InstanceId);
	if (HapticDevice == nullptr || HapticDeviceResult.bSuccess == false)
	{
		FJoystickLogManager::Get()->LogError(HapticDeviceResult);
		return -1;
	}

	const int Result = SDL_HapticNumEffects(HapticDevice);
	if (Result < 0)
	{
		FJoystickLogManager::Get()->LogSDLError(TEXT("SDL_HapticNumEffects failed"));
	}

	return Result;
}

int UJoystickHapticDeviceManager::GetNumEffectsPlaying(const FJoystickInstanceId& InstanceId) const
{
	auto [HapticDevice, HapticDeviceResult] = GetHapticDevice(InstanceId);
	if (HapticDevice == nullptr || HapticDeviceResult.bSuccess == false)
	{
		FJoystickLogManager::Get()->LogError(HapticDeviceResult);
		return -1;
	}

	const int Result = SDL_HapticNumEffectsPlaying(HapticDevice);
	if (Result < 0)
	{
		FJoystickLogManager::Get()->LogSDLError(TEXT("SDL_HapticNumEffectsPlaying failed"));
	}

	return Result;
}

int UJoystickHapticDeviceManager::GetEffectStatus(const FJoystickInstanceId& InstanceId, const int EffectId) const
{
	auto [DeviceInfo, DeviceInfoResult] = GetDeviceInfo(InstanceId);
	if (DeviceInfo == nullptr || DeviceInfoResult.bSuccess == false)
	{
		FJoystickLogManager::Get()->LogError(DeviceInfoResult);
		return -1;
	}

	if (DeviceInfo->Haptic.Status == false)
	{
		FJoystickLogManager::Get()->LogError(TEXT("Cannot check effect status. Device does not support effect status queries"));
		return -1;
	}

	SDL_Haptic* HapticDevice = DeviceInfo->SDLHaptic;
	if (HapticDevice == nullptr)
	{
		return -1;
	}

	const int Result = SDL_HapticGetEffectStatus(HapticDevice, EffectId);
	if (Result == -1)
	{
		FJoystickLogManager::Get()->LogSDLError(TEXT("SDL_HapticGetEffectStatus failed"));
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
	auto [DeviceInfo, DeviceInfoResult] = GetDeviceInfo(InstanceId);
	if (DeviceInfo == nullptr || DeviceInfoResult.bSuccess == false)
	{
		FJoystickLogManager::Get()->LogError(DeviceInfoResult);
		return false;
	}

	if (!DeviceInfo->RumbleSupport)
	{
		return false;
	}

	const Uint16 LowFrequency = FMath::Clamp<Uint16>(LowFrequencyRumble * UINT16_MAX, 0, UINT16_MAX);
	const Uint16 HighFrequency = FMath::Clamp<Uint16>(HighFrequencyRumble * UINT16_MAX, 0, UINT16_MAX);
	const Uint32 ClampedDuration = Duration == -1 ? SDL_HAPTIC_INFINITY : FMath::Clamp<Uint32>(Duration * 1000.0f, 0, UINT32_MAX);
	const int Result = SDL_JoystickRumble(DeviceInfo->SDLJoystick, LowFrequency, HighFrequency, ClampedDuration);
	if (Result == -1)
	{
		FJoystickLogManager::Get()->LogSDLError(TEXT("SDL_JoystickRumble failed"));
		return false;
	}
	return true;
#else
	FJoystickLogManager::Get()->LogError(TEXT("PlayRumble is not supported on this engine version"));
	return false;
#endif
}

bool UJoystickHapticDeviceManager::PlayHapticRumble(const FJoystickInstanceId& InstanceId, const float Strength, const float Duration)
{
#if ENGINE_MAJOR_VERSION == 5
	auto [DeviceInfo, DeviceInfoResult] = GetDeviceInfo(InstanceId);
	if (DeviceInfo == nullptr || DeviceInfoResult.bSuccess == false)
	{
		FJoystickLogManager::Get()->LogError(DeviceInfoResult);
		return false;
	}

	if (!DeviceInfo->HapticRumble.Enabled)
	{
		return false;
	}

	const float ClampedStrength = FMath::Clamp<float>(Strength, 0, 1);
	const Uint32 ClampedDuration = Duration == -1 ? SDL_HAPTIC_INFINITY : FMath::Clamp<Uint32>(Duration * 1000.0f, 0, UINT32_MAX);
	const int Result = SDL_HapticRumblePlay(DeviceInfo->SDLHaptic, ClampedStrength, ClampedDuration);
	if (Result == -1)
	{
		FJoystickLogManager::Get()->LogSDLError(TEXT("SDL_HapticRumblePlay failed"));
		return false;
	}
	return true;
#else
	FJoystickLogManager::Get()->LogError(TEXT("PlayHapticRumble is not supported on this engine version"));
	return false;
#endif
}

bool UJoystickHapticDeviceManager::StopRumble(const FJoystickInstanceId& InstanceId)
{
#if ENGINE_MAJOR_VERSION == 5
	auto [DeviceInfo, DeviceInfoResult] = GetDeviceInfo(InstanceId);
	if (DeviceInfo == nullptr || DeviceInfoResult.bSuccess == false)
	{
		FJoystickLogManager::Get()->LogError(DeviceInfoResult);
		return false;
	}

	const int Result = SDL_JoystickRumble(DeviceInfo->SDLJoystick, 0.0f, 0.0f, 0.0f);
	if (Result == -1)
	{
		FJoystickLogManager::Get()->LogSDLError(TEXT("SDL_JoystickRumble(Stop) failed"));
		return false;
	}

	return true;
#else
	FJoystickLogManager::Get()->LogError(TEXT("StopRumble is not supported on this engine version"));
	return false;
#endif
}

bool UJoystickHapticDeviceManager::StopHapticRumble(const FJoystickInstanceId& InstanceId)
{
#if ENGINE_MAJOR_VERSION == 5
	auto [HapticDevice, HapticDeviceResult] = GetHapticDevice(InstanceId);
	if (HapticDevice == nullptr || HapticDeviceResult.bSuccess == false)
	{
		FJoystickLogManager::Get()->LogError(HapticDeviceResult);
		return false;
	}

	const int Result = SDL_HapticRumbleStop(HapticDevice);
	if (Result == -1)
	{
		FJoystickLogManager::Get()->LogSDLError(TEXT("SDL_HapticRumbleStop failed"));
		return false;
	}

	return true;
#else
	FJoystickLogManager::Get()->LogError(TEXT("StopHapticRumble is not supported on this engine version"));
	return false;
#endif
}

int UJoystickHapticDeviceManager::CreateEffect(const FJoystickInstanceId& InstanceId, SDL_HapticEffect& Effect) const
{
	auto [HapticDevice, Result] = GetHapticDevice(InstanceId);
	if (HapticDevice == nullptr || Result.bSuccess == false)
	{
		FJoystickLogManager::Get()->LogError(Result);
		return -1;
	}

	const int EffectId = SDL_HapticNewEffect(HapticDevice, &Effect);
	if (EffectId == -1)
	{
		FJoystickLogManager::Get()->LogSDLError(TEXT("SDL_HapticNewEffect failed"));
	}

	return EffectId;
}

bool UJoystickHapticDeviceManager::UpdateEffect(const FJoystickInstanceId& InstanceId, const int EffectId, SDL_HapticEffect& Effect) const
{
	auto [HapticDevice, HapticDeviceResult] = GetHapticDevice(InstanceId);
	if (HapticDevice == nullptr || HapticDeviceResult.bSuccess == false)
	{
		FJoystickLogManager::Get()->LogError(HapticDeviceResult);
		return false;
	}

	const int Result = SDL_HapticUpdateEffect(HapticDevice, EffectId, &Effect);
	if (Result != 0)
	{
		FJoystickLogManager::Get()->LogSDLError(TEXT("SDL_HapticUpdateEffect failed"));
		return false;
	}

	return true;
}

bool UJoystickHapticDeviceManager::RunEffect(const FJoystickInstanceId& InstanceId, const int EffectId, const int Iterations) const
{
	auto [HapticDevice, HapticDeviceResult] = GetHapticDevice(InstanceId);
	if (HapticDevice == nullptr || HapticDeviceResult.bSuccess == false)
	{
		FJoystickLogManager::Get()->LogError(HapticDeviceResult);
		return false;
	}

	const int Result = SDL_HapticRunEffect(HapticDevice, EffectId, Iterations);
	if (Result != 0)
	{
		FJoystickLogManager::Get()->LogSDLError(TEXT("SDL_HapticRunEffect failed"));
		return false;
	}

	return true;
}

bool UJoystickHapticDeviceManager::StopEffect(const FJoystickInstanceId& InstanceId, const int EffectId) const
{
	auto [HapticDevice, HapticDeviceResult] = GetHapticDevice(InstanceId);
	if (HapticDevice == nullptr || HapticDeviceResult.bSuccess == false)
	{
		FJoystickLogManager::Get()->LogError(HapticDeviceResult);
		return false;
	}

	const int Result = SDL_HapticStopEffect(HapticDevice, EffectId);
	if (Result != 0)
	{
		FJoystickLogManager::Get()->LogSDLError(TEXT("SDL_HapticStopEffect failed"));
		return false;
	}

	return true;
}

void UJoystickHapticDeviceManager::DestroyEffect(const FJoystickInstanceId& InstanceId, const int EffectId) const
{
	auto [HapticDevice, Result] = GetHapticDevice(InstanceId);
	if (HapticDevice == nullptr || Result.bSuccess == false)
	{
		FJoystickLogManager::Get()->LogError(Result);
		return;
	}

	SDL_HapticDestroyEffect(HapticDevice, EffectId);
}

TTuple<SDL_Haptic*, FInternalResultMessage> UJoystickHapticDeviceManager::GetHapticDevice(const FJoystickInstanceId& InstanceId) const
{
	auto [DeviceInfo, Result] = GetDeviceInfo(InstanceId);
	if (DeviceInfo == nullptr || Result.bSuccess == false)
	{
		return {nullptr, Result};
	}

	return {DeviceInfo->SDLHaptic, FInternalResultMessage(true)};
}

TTuple<FDeviceInfoSDL*, FInternalResultMessage> UJoystickHapticDeviceManager::GetDeviceInfo(const FJoystickInstanceId& InstanceId) const
{
	if (!IsValid(GEngine))
	{
		return {nullptr, FInternalResultMessage(false, TEXT("GEngine is null"))};
	}

	UJoystickSubsystem* JoystickSubsystem = GEngine->GetEngineSubsystem<UJoystickSubsystem>();
	if (!IsValid(JoystickSubsystem))
	{
		return {nullptr, FInternalResultMessage(false, TEXT("JoystickSubsystem is null"))};
	}

	auto [Device, Result] = JoystickSubsystem->GetDeviceInfo(InstanceId);
	if (Device == nullptr)
	{
		return {nullptr, Result};
	}

	return {Device, FInternalResultMessage(true)};
}
