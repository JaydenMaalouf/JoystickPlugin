// JoystickPlugin is licensed under the MIT License.
// Copyright Jayden Maalouf 2026. All Rights Reserved.

#include "Managers/JoystickHapticDeviceManager.h"

#include "Data/DeviceInfoSDL.h"
#include "Engine/Engine.h"
#include "ForceFeedback/ForceFeedbackConversion.h"
#include "Managers/JoystickLogManager.h"
#include "Managers/JoystickProfileManager.h"
#include "JoystickSubsystem.h"
#include "Runtime/Launch/Resources/Version.h"

THIRD_PARTY_INCLUDES_START

#include "JoystickSDL.h"

THIRD_PARTY_INCLUDES_END

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

	const bool Result = SDL_SetHapticAutocenter(HapticDevice, FForceFeedbackConversion::ToPercentage(Center));
	if (Result == false)
	{
		FJoystickLogManager::Get()->LogSDLError(TEXT("SDL_SetHapticAutocenter failed"));
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

	const bool Result = SDL_SetHapticGain(HapticDevice, FForceFeedbackConversion::ToPercentage(Gain));
	if (Result == false)
	{
		FJoystickLogManager::Get()->LogSDLError(TEXT("SDL_SetHapticGain failed"));
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
		FJoystickLogManager::Get()->LogSDLError(TEXT("SDL_PauseHaptic failed"));
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
		FJoystickLogManager::Get()->LogSDLError(TEXT("SDL_ResumeHaptic failed"));
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
		FJoystickLogManager::Get()->LogSDLError(TEXT("SDL_StopHapticEffects failed"));
		return false;
	}

	return true;
}

int UJoystickHapticDeviceManager::GetMaxNumEffects(const FJoystickInstanceId& InstanceId) const
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
		FJoystickLogManager::Get()->LogSDLError(TEXT("SDL_GetMaxHapticEffects failed"));
	}

	return Result;
}

int UJoystickHapticDeviceManager::GetMaxNumEffectsPlaying(const FJoystickInstanceId& InstanceId) const
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
		FJoystickLogManager::Get()->LogSDLError(TEXT("SDL_GetMaxHapticEffectsPlaying failed"));
	}

	return Result;
}

bool UJoystickHapticDeviceManager::GetEffectStatus(const FJoystickInstanceId& InstanceId, const int EffectId) const
{
	auto [DeviceInfo, DeviceInfoResult] = GetDeviceInfo(InstanceId);
	if (DeviceInfoResult.bSuccess == false || DeviceInfo == nullptr)
	{
		FJoystickLogManager::Get()->LogError(DeviceInfoResult);
		return false;
	}

	if (DeviceInfo->Haptic.Status == false)
	{
		FJoystickLogManager::Get()->LogDebug(TEXT("Cannot check effect status. Device does not support effect status queries"));
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
	if (DeviceInfoResult.bSuccess == false || DeviceInfo == nullptr)
	{
		FJoystickLogManager::Get()->LogError(DeviceInfoResult);
		return false;
	}

	if (!DeviceInfo->Rumble.StandardRumble)
	{
		return false;
	}

	const uint16 LowFrequency = FForceFeedbackConversion::ToFullRangeLevel(LowFrequencyRumble);
	const uint16 HighFrequency = FForceFeedbackConversion::ToFullRangeLevel(HighFrequencyRumble);
	const uint32 ClampedDuration = Duration == -1 ? SDL_HAPTIC_INFINITY : FForceFeedbackConversion::SecondsToDuration(Duration);

	const bool Result = SDL_RumbleJoystick(DeviceInfo->SDLJoystick, LowFrequency, HighFrequency, ClampedDuration);
	if (Result == false)
	{
		FJoystickLogManager::Get()->LogSDLError(TEXT("SDL_RumbleJoystick failed"));
		return false;
	}

	return true;
}

bool UJoystickHapticDeviceManager::PlayRumbleTriggers(const FJoystickInstanceId& InstanceId, const float LeftRumble, const float RightRumble, const float Duration) const
{
	if (LeftRumble == 0 && RightRumble == 0)
	{
		return false;
	}

	auto [DeviceInfo, DeviceInfoResult] = GetDeviceInfo(InstanceId);
	if (DeviceInfoResult.bSuccess == false || DeviceInfo == nullptr)
	{
		FJoystickLogManager::Get()->LogError(DeviceInfoResult);
		return false;
	}

	if (!DeviceInfo->Rumble.TriggerRumble)
	{
		return false;
	}

	const uint16 ClampedLeftRumble = FForceFeedbackConversion::ToFullRangeLevel(LeftRumble);
	const uint16 ClampedRightRumble = FForceFeedbackConversion::ToFullRangeLevel(RightRumble);
	const uint32 ClampedDuration = Duration == -1 ? SDL_HAPTIC_INFINITY : FForceFeedbackConversion::SecondsToDuration(Duration);

	const bool Result = SDL_RumbleJoystickTriggers(DeviceInfo->SDLJoystick, ClampedLeftRumble, ClampedRightRumble, ClampedDuration);
	if (Result == false)
	{
		FJoystickLogManager::Get()->LogSDLError(TEXT("SDL_RumbleJoystickTriggers failed"));
		return false;
	}

	return true;
}

bool UJoystickHapticDeviceManager::PlayHapticRumble(const FJoystickInstanceId& InstanceId, const float Strength, const float Duration) const
{
	auto [DeviceInfo, DeviceInfoResult] = GetDeviceInfo(InstanceId);
	if (DeviceInfoResult.bSuccess == false || DeviceInfo == nullptr)
	{
		FJoystickLogManager::Get()->LogError(DeviceInfoResult);
		return false;
	}

	if (!DeviceInfo->HapticRumble.Enabled)
	{
		return false;
	}

	const float ClampedStrength = FForceFeedbackConversion::ToStrength(Strength);
	const uint32 ClampedDuration = Duration == -1 ? SDL_HAPTIC_INFINITY : FForceFeedbackConversion::SecondsToDuration(Duration);
	const bool Result = SDL_PlayHapticRumble(DeviceInfo->SDLHaptic, ClampedStrength, ClampedDuration);
	if (Result == false)
	{
		FJoystickLogManager::Get()->LogSDLError(TEXT("SDL_PlayHapticRumble failed"));
		return false;
	}
	return true;
}

bool UJoystickHapticDeviceManager::StopRumble(const FJoystickInstanceId& InstanceId) const
{
	auto [DeviceInfo, DeviceInfoResult] = GetDeviceInfo(InstanceId);
	if (DeviceInfoResult.bSuccess == false || DeviceInfo == nullptr)
	{
		FJoystickLogManager::Get()->LogError(DeviceInfoResult);
		return false;
	}

	const bool Result = SDL_RumbleJoystick(DeviceInfo->SDLJoystick, 0.0f, 0.0f, 0.0f);
	if (Result == false)
	{
		FJoystickLogManager::Get()->LogSDLError(TEXT("SDL_RumbleJoystick(Stop) failed"));
		return false;
	}

	return true;
}

bool UJoystickHapticDeviceManager::StopRumbleTriggers(const FJoystickInstanceId& InstanceId) const
{
	auto [DeviceInfo, DeviceInfoResult] = GetDeviceInfo(InstanceId);
	if (DeviceInfoResult.bSuccess == false || DeviceInfo == nullptr)
	{
		FJoystickLogManager::Get()->LogError(DeviceInfoResult);
		return false;
	}

	const bool Result = SDL_RumbleJoystickTriggers(DeviceInfo->SDLJoystick, 0.0f, 0.0f, 0.0f);
	if (Result == false)
	{
		FJoystickLogManager::Get()->LogSDLError(TEXT("SDL_RumbleJoystickTriggers(Stop) failed"));
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
		FJoystickLogManager::Get()->LogSDLError(TEXT("SDL_StopHapticRumble failed"));
		return false;
	}

	return true;
}

int UJoystickHapticDeviceManager::CreateEffect(const FJoystickInstanceId& InstanceId, SDL_HapticEffect& Effect) const
{
	auto [DeviceInfo, Result] = GetDeviceInfo(InstanceId);
	if (DeviceInfo == nullptr || DeviceInfo->SDLHaptic == nullptr || Result.bSuccess == false)
	{
		FJoystickLogManager::Get()->LogError(Result);
		return -1;
	}

	SDL_HapticEffect DeviceEffect = Effect;
	ApplyForceFeedbackProperties(*DeviceInfo, DeviceEffect);
	const int EffectId = SDL_CreateHapticEffect(DeviceInfo->SDLHaptic, &DeviceEffect);
	if (EffectId == -1)
	{
		FJoystickLogManager::Get()->LogSDLError(TEXT("SDL_CreateHapticEffect failed"));
	}

	return EffectId;
}

bool UJoystickHapticDeviceManager::UpdateEffect(const FJoystickInstanceId& InstanceId, const int EffectId, SDL_HapticEffect& Effect) const
{
	auto [DeviceInfo, DeviceInfoResult] = GetDeviceInfo(InstanceId);
	if (DeviceInfoResult.bSuccess == false || DeviceInfo == nullptr || DeviceInfo->SDLHaptic == nullptr)
	{
		FJoystickLogManager::Get()->LogError(DeviceInfoResult);
		return false;
	}

	SDL_HapticEffect DeviceEffect = Effect;
	ApplyForceFeedbackProperties(*DeviceInfo, DeviceEffect);
	const bool Result = SDL_UpdateHapticEffect(DeviceInfo->SDLHaptic, EffectId, &DeviceEffect);
	if (Result == false)
	{
		FJoystickLogManager::Get()->LogSDLError(TEXT("SDL_UpdateHapticEffect failed"));
		return false;
	}

	return true;
}

bool UJoystickHapticDeviceManager::RunEffect(const FJoystickInstanceId& InstanceId, const int EffectId, const uint32 Iterations) const
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
		FJoystickLogManager::Get()->LogSDLError(TEXT("SDL_RunHapticEffect failed"));
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
		FJoystickLogManager::Get()->LogSDLError(TEXT("SDL_StopHapticEffect failed"));
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

	SDL_DestroyHapticEffect(HapticDevice, EffectId);
}

void UJoystickHapticDeviceManager::ApplyForceFeedbackProperties(const FDeviceInfoSDL& DeviceInfo, SDL_HapticEffect& Effect)
{
	UJoystickProfileManager* JoystickProfileManager = GetMutableDefault<UJoystickProfileManager>();
	if (!IsValid(JoystickProfileManager))
	{
		return;
	}

	const FJoystickInputDeviceConfiguration* DeviceConfiguration = JoystickProfileManager->GetInputDeviceConfiguration(DeviceInfo);
	if (DeviceConfiguration == nullptr || DeviceConfiguration->OverrideForceFeedbackProperties == false)
	{
		return;
	}

	const FJoystickInputDeviceForceFeedbackProperties& ForceFeedbackProperties = DeviceConfiguration->ForceFeedbackProperties;
	switch (Effect.type)
	{
		case SDL_HAPTIC_SPRING:
			ApplyConditionProperties(ForceFeedbackProperties, EForceFeedbackConditionEffectType::Spring, Effect.condition);
			break;
		case SDL_HAPTIC_DAMPER:
			ApplyConditionProperties(ForceFeedbackProperties, EForceFeedbackConditionEffectType::Damper, Effect.condition);
			break;
		case SDL_HAPTIC_INERTIA:
			ApplyConditionProperties(ForceFeedbackProperties, EForceFeedbackConditionEffectType::Inertia, Effect.condition);
			break;
		case SDL_HAPTIC_FRICTION:
			ApplyConditionProperties(ForceFeedbackProperties, EForceFeedbackConditionEffectType::Friction, Effect.condition);
			break;
		default:
			break;
	}
}

void UJoystickHapticDeviceManager::ApplyConditionProperties(const FJoystickInputDeviceForceFeedbackProperties& ForceFeedbackProperties, const EForceFeedbackConditionEffectType ConditionType, SDL_HapticCondition& Condition)
{
	if (!ForceFeedbackProperties.IsInverted(ConditionType))
	{
		return;
	}

	for (int AxisIndex = 0; AxisIndex < 3; AxisIndex++)
	{
		Swap(Condition.right_sat[AxisIndex], Condition.left_sat[AxisIndex]);
		Swap(Condition.right_coeff[AxisIndex], Condition.left_coeff[AxisIndex]);

		// Negated in int32, as -32768 has no positive int16.
		Condition.center[AxisIndex] = static_cast<Sint16>(FMath::Clamp(-static_cast<int32>(Condition.center[AxisIndex]), -32768, 32767));
	}
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
