// JoystickPlugin is licensed under the MIT License.
// Copyright Jayden Maalouf 2026. All Rights Reserved.

#include "Managers/JoystickHapticDeviceManager.h"

#include "Data/DeviceInfoSDL.h"
#include "Engine/Engine.h"
#include "Managers/JoystickLogManager.h"
#include "JoystickSubsystem.h"
#include "Runtime/Launch/Resources/Version.h"

#include "SDLDynamicLoader.h"

const UJoystickHapticDeviceManager* UJoystickHapticDeviceManager::GetJoystickHapticDeviceManager()
{
	return GetDefault<UJoystickHapticDeviceManager>();
}

bool UJoystickHapticDeviceManager::SetAutoCenter(const FJoystickInstanceId& InstanceId, const int Center) const
{
	auto [HapticDevice, HapticDeviceResult] = GetHapticDevice(InstanceId);
	if (HapticDeviceResult.bSuccess == false || HapticDevice == nullptr)
	{
		FJoystickLogManager::Get()->LogError(HapticDeviceResult);
		return false;
	}

	const bool Result = SDL_SetHapticAutocenter(HapticDevice, FMath::Clamp(Center, 0, 100));
	if (Result == false)
	{
		FJoystickLogManager::Get()->LogSDLError(TEXT("SDL_HapticSetAutocenter failed"));
		return false;
	}

	return true;
}

bool UJoystickHapticDeviceManager::SetGain(const FJoystickInstanceId& InstanceId, const int Gain) const
{
	auto [HapticDevice, HapticDeviceResult] = GetHapticDevice(InstanceId);
	if (HapticDeviceResult.bSuccess == false || HapticDevice == nullptr)
	{
		FJoystickLogManager::Get()->LogError(HapticDeviceResult);
		return false;
	}

	const bool Result = SDL_SetHapticGain(HapticDevice, FMath::Clamp(Gain, 0, 100));
	if (Result == false)
	{
		FJoystickLogManager::Get()->LogSDLError(TEXT("SDL_HapticSetGain failed"));
		return false;
	}

	return true;
}

bool UJoystickHapticDeviceManager::PauseDevice(const FJoystickInstanceId& InstanceId) const
{
	auto [HapticDevice, HapticDeviceResult] = GetHapticDevice(InstanceId);
	if (HapticDeviceResult.bSuccess == false || HapticDevice == nullptr)
	{
		FJoystickLogManager::Get()->LogError(HapticDeviceResult);
		return false;
	}

	const bool Result = SDL_PauseHaptic(HapticDevice);
	if (Result == false)
	{
		FJoystickLogManager::Get()->LogSDLError(TEXT("SDL_HapticPause failed"));
		return false;
	}

	return true;
}

bool UJoystickHapticDeviceManager::UnpauseDevice(const FJoystickInstanceId& InstanceId) const
{
	auto [HapticDevice, HapticDeviceResult] = GetHapticDevice(InstanceId);
	if (HapticDeviceResult.bSuccess == false || HapticDevice == nullptr)
	{
		FJoystickLogManager::Get()->LogError(HapticDeviceResult);
		return false;
	}

	const bool Result = SDL_ResumeHaptic(HapticDevice);
	if (Result == false)
	{
		FJoystickLogManager::Get()->LogSDLError(TEXT("SDL_HapticUnpause failed"));
		return false;
	}

	return true;
}

bool UJoystickHapticDeviceManager::StopAllEffects(const FJoystickInstanceId& InstanceId) const
{
	auto [HapticDevice, HapticDeviceResult] = GetHapticDevice(InstanceId);
	if (HapticDeviceResult.bSuccess == false || HapticDevice == nullptr)
	{
		FJoystickLogManager::Get()->LogError(HapticDeviceResult);
		return false;
	}

	const bool Result = SDL_StopHapticEffects(HapticDevice);
	if (Result == false)
	{
		FJoystickLogManager::Get()->LogSDLError(TEXT("SDL_HapticStopAll failed"));
		return false;
	}

	return true;
}

int UJoystickHapticDeviceManager::GetNumEffects(const FJoystickInstanceId& InstanceId) const
{
	auto [HapticDevice, HapticDeviceResult] = GetHapticDevice(InstanceId);
	if (HapticDeviceResult.bSuccess == false || HapticDevice == nullptr)
	{
		FJoystickLogManager::Get()->LogError(HapticDeviceResult);
		return -1;
	}

	const int Result = SDL_GetMaxHapticEffects(HapticDevice);
	if (Result < 0)
	{
		FJoystickLogManager::Get()->LogSDLError(TEXT("SDL_HapticNumEffects failed"));
	}

	return Result;
}

int UJoystickHapticDeviceManager::GetNumEffectsPlaying(const FJoystickInstanceId& InstanceId) const
{
	auto [HapticDevice, HapticDeviceResult] = GetHapticDevice(InstanceId);
	if (HapticDeviceResult.bSuccess == false || HapticDevice == nullptr)
	{
		FJoystickLogManager::Get()->LogError(HapticDeviceResult);
		return -1;
	}

	const int Result = SDL_GetMaxHapticEffectsPlaying(HapticDevice);
	if (Result < 0)
	{
		FJoystickLogManager::Get()->LogSDLError(TEXT("SDL_HapticNumEffectsPlaying failed"));
	}

	return Result;
}

bool UJoystickHapticDeviceManager::GetEffectStatus(const FJoystickInstanceId& InstanceId, const int EffectId) const
{
	auto [DeviceInfo, DeviceInfoResult] = GetDeviceInfo(InstanceId);
	if (DeviceInfo == nullptr || DeviceInfoResult.bSuccess == false)
	{
		FJoystickLogManager::Get()->LogError(DeviceInfoResult);
		return false;
	}

	if (DeviceInfo->Haptic.Status == false)
	{
		FJoystickLogManager::Get()->LogError(TEXT("Cannot check effect status. Device does not support effect status queries"));
		return false;
	}

	SDL_Haptic* HapticDevice = DeviceInfo->SDLHaptic;
	if (HapticDevice == nullptr)
	{
		return false;
	}

	return SDL_GetHapticEffectStatus(HapticDevice, EffectId);
}

bool UJoystickHapticDeviceManager::PlayRumble(const FJoystickInstanceId& InstanceId, const float LowFrequencyRumble, const float HighFrequencyRumble, const float Duration) const
{
	if (LowFrequencyRumble == 0 && HighFrequencyRumble == 0)
	{
		return false;
	}

	auto [DeviceInfo, DeviceInfoResult] = GetDeviceInfo(InstanceId);
	if (DeviceInfo == nullptr || DeviceInfoResult.bSuccess == false)
	{
		FJoystickLogManager::Get()->LogError(DeviceInfoResult);
		return false;
	}

	if (!DeviceInfo->Rumble.StandardRumble)
	{
		return false;
	}

	const uint16 LowFrequency = FMath::Clamp<uint16>(LowFrequencyRumble * UINT16_MAX, 0, UINT16_MAX);
	const uint16 HighFrequency = FMath::Clamp<uint16>(HighFrequencyRumble * UINT16_MAX, 0, UINT16_MAX);
	const uint32 ClampedDuration = Duration == -1 ? SDL_HAPTIC_INFINITY : FMath::Clamp<uint32>(Duration * 1000.0f, 0, UINT32_MAX);
	const bool Result = SDL_RumbleJoystick(DeviceInfo->SDLJoystick, LowFrequency, HighFrequency, ClampedDuration);
	if (Result == false)
	{
		FJoystickLogManager::Get()->LogSDLError(TEXT("SDL_JoystickRumble failed"));
		return false;
	}

	return true;
}

bool UJoystickHapticDeviceManager::PlayRumbleTriggers(const FJoystickInstanceId& InstanceId, const float LowFrequencyRumble, const float HighFrequencyRumble, const float Duration) const
{
	if (LowFrequencyRumble == 0 && HighFrequencyRumble == 0)
	{
		return false;
	}

	auto [DeviceInfo, DeviceInfoResult] = GetDeviceInfo(InstanceId);
	if (DeviceInfo == nullptr || DeviceInfoResult.bSuccess == false)
	{
		FJoystickLogManager::Get()->LogError(DeviceInfoResult);
		return false;
	}

	if (!DeviceInfo->Rumble.TriggerRumble)
	{
		return false;
	}

	const uint16 LowFrequency = FMath::Clamp<uint16>(LowFrequencyRumble * UINT16_MAX, 0, UINT16_MAX);
	const uint16 HighFrequency = FMath::Clamp<uint16>(HighFrequencyRumble * UINT16_MAX, 0, UINT16_MAX);
	const uint32 ClampedDuration = Duration == -1 ? SDL_HAPTIC_INFINITY : FMath::Clamp<uint32>(Duration * 1000.0f, 0, UINT32_MAX);
	const bool Result = SDL_RumbleJoystickTriggers(DeviceInfo->SDLJoystick, LowFrequency, HighFrequency, ClampedDuration);
	if (Result == false)
	{
		FJoystickLogManager::Get()->LogSDLError(TEXT("SDL_JoystickRumble failed"));
		return false;
	}

	return true;
}

bool UJoystickHapticDeviceManager::PlayHapticRumble(const FJoystickInstanceId& InstanceId, const float Strength, const float Duration) const
{
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
	const uint32 ClampedDuration = Duration == -1 ? SDL_HAPTIC_INFINITY : FMath::Clamp<uint32>(Duration * 1000.0f, 0, UINT32_MAX);
	const bool Result = SDL_PlayHapticRumble(DeviceInfo->SDLHaptic, ClampedStrength, ClampedDuration);
	if (Result == false)
	{
		FJoystickLogManager::Get()->LogSDLError(TEXT("SDL_HapticRumblePlay failed"));
		return false;
	}
	return true;
}

bool UJoystickHapticDeviceManager::StopRumble(const FJoystickInstanceId& InstanceId) const
{
	auto [DeviceInfo, DeviceInfoResult] = GetDeviceInfo(InstanceId);
	if (DeviceInfo == nullptr || DeviceInfoResult.bSuccess == false)
	{
		FJoystickLogManager::Get()->LogError(DeviceInfoResult);
		return false;
	}

	const bool Result = SDL_RumbleJoystick(DeviceInfo->SDLJoystick, 0.0f, 0.0f, 0.0f);
	if (Result == false)
	{
		FJoystickLogManager::Get()->LogSDLError(TEXT("SDL_JoystickRumble(Stop) failed"));
		return false;
	}

	return true;
}

bool UJoystickHapticDeviceManager::StopRumbleTriggers(const FJoystickInstanceId& InstanceId) const
{
	auto [DeviceInfo, DeviceInfoResult] = GetDeviceInfo(InstanceId);
	if (DeviceInfo == nullptr || DeviceInfoResult.bSuccess == false)
	{
		FJoystickLogManager::Get()->LogError(DeviceInfoResult);
		return false;
	}

	const bool Result = SDL_RumbleJoystickTriggers(DeviceInfo->SDLJoystick, 0.0f, 0.0f, 0.0f);
	if (Result == false)
	{
		FJoystickLogManager::Get()->LogSDLError(TEXT("SDL_JoystickRumble(Stop) failed"));
		return false;
	}

	return true;
}

bool UJoystickHapticDeviceManager::StopHapticRumble(const FJoystickInstanceId& InstanceId) const
{
	auto [HapticDevice, HapticDeviceResult] = GetHapticDevice(InstanceId);
	if (HapticDeviceResult.bSuccess == false || HapticDevice == nullptr)
	{
		FJoystickLogManager::Get()->LogError(HapticDeviceResult);
		return false;
	}

	const bool Result = SDL_StopHapticRumble(HapticDevice);
	if (Result == false)
	{
		FJoystickLogManager::Get()->LogSDLError(TEXT("SDL_HapticRumbleStop failed"));
		return false;
	}

	return true;
}

int UJoystickHapticDeviceManager::CreateEffect(const FJoystickInstanceId& InstanceId, const SDL_HapticEffect& Effect) const
{
	auto [HapticDevice, Result] = GetHapticDevice(InstanceId);
	if (Result.bSuccess == false || HapticDevice == nullptr)
	{
		FJoystickLogManager::Get()->LogError(Result);
		return -1;
	}

	const int EffectId = SDL_CreateHapticEffect(HapticDevice, &Effect);
	if (EffectId == -1)
	{
		FJoystickLogManager::Get()->LogSDLError(TEXT("SDL_HapticNewEffect failed"));
	}

	return EffectId;
}

bool UJoystickHapticDeviceManager::UpdateEffect(const FJoystickInstanceId& InstanceId, const int EffectId, const SDL_HapticEffect& Effect) const
{
	auto [HapticDevice, HapticDeviceResult] = GetHapticDevice(InstanceId);
	if (HapticDeviceResult.bSuccess == false || HapticDevice == nullptr)
	{
		FJoystickLogManager::Get()->LogError(HapticDeviceResult);
		return false;
	}

	const bool Result = SDL_UpdateHapticEffect(HapticDevice, EffectId, &Effect);
	if (Result == false)
	{
		FJoystickLogManager::Get()->LogSDLError(TEXT("SDL_HapticUpdateEffect failed"));
		return false;
	}

	return true;
}

bool UJoystickHapticDeviceManager::RunEffect(const FJoystickInstanceId& InstanceId, const int EffectId, const int Iterations) const
{
	auto [HapticDevice, HapticDeviceResult] = GetHapticDevice(InstanceId);
	if (HapticDeviceResult.bSuccess == false || HapticDevice == nullptr)
	{
		FJoystickLogManager::Get()->LogError(HapticDeviceResult);
		return false;
	}

	const bool Result = SDL_RunHapticEffect(HapticDevice, EffectId, Iterations);
	if (Result == false)
	{
		FJoystickLogManager::Get()->LogSDLError(TEXT("SDL_HapticRunEffect failed"));
		return false;
	}

	return true;
}

bool UJoystickHapticDeviceManager::StopEffect(const FJoystickInstanceId& InstanceId, const int EffectId) const
{
	auto [HapticDevice, HapticDeviceResult] = GetHapticDevice(InstanceId);
	if (HapticDeviceResult.bSuccess == false || HapticDevice == nullptr)
	{
		FJoystickLogManager::Get()->LogError(HapticDeviceResult);
		return false;
	}

	const bool Result = SDL_StopHapticEffect(HapticDevice, EffectId);
	if (Result == false)
	{
		FJoystickLogManager::Get()->LogSDLError(TEXT("SDL_HapticStopEffect failed"));
		return false;
	}

	return true;
}

void UJoystickHapticDeviceManager::DestroyEffect(const FJoystickInstanceId& InstanceId, const int EffectId) const
{
	auto [HapticDevice, Result] = GetHapticDevice(InstanceId);
	if (Result.bSuccess == false || HapticDevice == nullptr)
	{
		FJoystickLogManager::Get()->LogError(Result);
		return;
	}

	SDL_DestroyHapticEffect(HapticDevice, EffectId);
}

TTuple<SDL_Haptic*, FInternalResultMessage> UJoystickHapticDeviceManager::GetHapticDevice(const FJoystickInstanceId& InstanceId) const
{
	auto [DeviceInfo, Result] = GetDeviceInfo(InstanceId);
	if (Result.bSuccess == false || DeviceInfo == nullptr)
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
