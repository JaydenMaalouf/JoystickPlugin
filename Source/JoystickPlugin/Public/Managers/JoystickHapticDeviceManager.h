// JoystickPlugin is licensed under the MIT License.
// Copyright Jayden Maalouf 2026. All Rights Reserved.

#pragma once

#include "Data/JoystickInstanceId.h"
#include "Data/ResultMessage.h"

THIRD_PARTY_INCLUDES_START

#include "JoystickSDL.h"

THIRD_PARTY_INCLUDES_END

#include "JoystickHapticDeviceManager.generated.h"

struct FDeviceInfoSDL;
struct FJoystickInputDeviceForceFeedbackProperties;
enum class EForceFeedbackConditionEffectType : uint8;

UCLASS(BlueprintType)
class JOYSTICKPLUGIN_API UJoystickHapticDeviceManager final : public UObject
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintPure, Category="Joystick|Force Feedback")
	static const UJoystickHapticDeviceManager* GetJoystickHapticDeviceManager();

	UFUNCTION(BlueprintCallable, BlueprintPure=false, Category="Joystick|Force Feedback")
	bool SetAutoCenter(const FJoystickInstanceId& InstanceId, const int Center) const;

	UFUNCTION(BlueprintCallable, BlueprintPure=false, Category="Joystick|Force Feedback")
	bool SetGain(const FJoystickInstanceId& InstanceId, const int Gain) const;

	UFUNCTION(BlueprintCallable, BlueprintPure=false, Category="Joystick|Force Feedback")
	bool PauseDevice(const FJoystickInstanceId& InstanceId) const;

	UFUNCTION(BlueprintCallable, BlueprintPure=false, Category="Joystick|Force Feedback")
	bool UnpauseDevice(const FJoystickInstanceId& InstanceId) const;

	UFUNCTION(BlueprintCallable, BlueprintPure=false, Category="Joystick|Force Feedback")
	bool StopAllEffects(const FJoystickInstanceId& InstanceId) const;

	UFUNCTION(BlueprintCallable, BlueprintPure, Category="Joystick|Force Feedback")
	int GetMaxNumEffects(const FJoystickInstanceId& InstanceId) const;

	UFUNCTION(BlueprintCallable, BlueprintPure, Category="Joystick|Force Feedback")
	int GetMaxNumEffectsPlaying(const FJoystickInstanceId& InstanceId) const;

	UFUNCTION(BlueprintCallable, BlueprintPure, Category="Joystick|Force Feedback")
	bool GetEffectStatus(const FJoystickInstanceId& InstanceId, const int EffectId) const;

	UFUNCTION(BlueprintCallable, BlueprintPure=false, Category="Joystick|Force Feedback")
	bool PlayRumble(const FJoystickInstanceId& InstanceId, const float LowFrequencyRumble, const float HighFrequencyRumble, UPARAM(DisplayName="Duration (in seconds)")
	                const float Duration) const;

	UFUNCTION(BlueprintCallable, BlueprintPure=false, Category="Joystick|Force Feedback")
	bool PlayRumbleTriggers(const FJoystickInstanceId& InstanceId, const float LeftRumble, const float RightRumble, UPARAM(DisplayName="Duration (in seconds)")
	                        const float Duration) const;

	UFUNCTION(BlueprintCallable, BlueprintPure=false, Category="Joystick|Force Feedback")
	bool PlayHapticRumble(const FJoystickInstanceId& InstanceId, const float Strength, const float Duration) const;

	UFUNCTION(BlueprintCallable, BlueprintPure=false, Category="Joystick|Force Feedback")
	bool StopRumble(const FJoystickInstanceId& InstanceId) const;

	UFUNCTION(BlueprintCallable, BlueprintPure=false, Category="Joystick|Force Feedback")
	bool StopRumbleTriggers(const FJoystickInstanceId& InstanceId) const;

	UFUNCTION(BlueprintCallable, BlueprintPure=false, Category="Joystick|Force Feedback")
	bool StopHapticRumble(const FJoystickInstanceId& InstanceId) const;

	int CreateEffect(const FJoystickInstanceId& InstanceId, SDL_HapticEffect& Effect) const;
	bool UpdateEffect(const FJoystickInstanceId& InstanceId, const int EffectId, SDL_HapticEffect& Effect) const;
	bool RunEffect(const FJoystickInstanceId& InstanceId, const int EffectId, const uint32 Iterations) const;
	bool StopEffect(const FJoystickInstanceId& InstanceId, const int EffectId) const;
	void DestroyEffect(const FJoystickInstanceId& InstanceId, const int EffectId) const;

private:
	friend class UJoystickSubsystem;
	friend class FJoystickInputDevice;

	static void InitialiseEngineRumble(FDeviceInfoSDL& DeviceInfo);
	bool PlayEngineForceFeedback(const FJoystickInstanceId& InstanceId, const float LargeValue, const float SmallValue) const;
	bool StopEngineForceFeedback(const FJoystickInstanceId& InstanceId) const;
	bool PlayEngineRumble(FDeviceInfoSDL& DeviceInfo, const float Strength) const;
	static bool BuildEngineRumbleEffect(const int16 Level, SDL_HapticEffect& Effect);

	static void ApplyForceFeedbackProperties(const FDeviceInfoSDL& DeviceInfo, SDL_HapticEffect& Effect);
	static void ApplyConditionProperties(const FJoystickInputDeviceForceFeedbackProperties& ForceFeedbackProperties, EForceFeedbackConditionEffectType ConditionType, SDL_HapticCondition& Condition);

	TTuple<SDL_Haptic*, FInternalResultMessage> GetHapticDevice(const FJoystickInstanceId& InstanceId) const;
	TTuple<FDeviceInfoSDL*, FInternalResultMessage> GetDeviceInfo(const FJoystickInstanceId& InstanceId) const;
};
