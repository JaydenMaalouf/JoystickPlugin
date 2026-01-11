// JoystickPlugin is licensed under the MIT License.
// Copyright Jayden Maalouf 2026. All Rights Reserved.

#pragma once

#include "Data/JoystickInstanceId.h"
#include "Data/ResultMessage.h"

THIRD_PARTY_INCLUDES_START

#include "SDL_haptic.h"

THIRD_PARTY_INCLUDES_END

#include "JoystickHapticDeviceManager.generated.h"

struct FDeviceInfoSDL;

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
	int GetNumEffects(const FJoystickInstanceId& InstanceId) const;

	UFUNCTION(BlueprintCallable, BlueprintPure, Category="Joystick|Force Feedback")
	int GetNumEffectsPlaying(const FJoystickInstanceId& InstanceId) const;

	UFUNCTION(BlueprintCallable, BlueprintPure, Category="Joystick|Force Feedback")
	bool GetEffectStatus(const FJoystickInstanceId& InstanceId, const int EffectId) const;

	UFUNCTION(BlueprintCallable, BlueprintPure=false, Category="Joystick|Force Feedback")
	bool PlayRumble(const FJoystickInstanceId& InstanceId, const float LowFrequencyRumble, const float HighFrequencyRumble, UPARAM(DisplayName="Duration (in seconds)")
	                const float Duration) const;

	UFUNCTION(BlueprintCallable, BlueprintPure=false, Category="Joystick|Force Feedback")
	bool PlayRumbleTriggers(const FJoystickInstanceId& InstanceId, float LowFrequencyRumble, float HighFrequencyRumble, UPARAM(DisplayName="Duration (in seconds)")
	                        float Duration) const;

	UFUNCTION(BlueprintCallable, BlueprintPure=false, Category="Joystick|Force Feedback")
	bool PlayHapticRumble(const FJoystickInstanceId& InstanceId, const float Strength, const float Duration) const;

	UFUNCTION(BlueprintCallable, BlueprintPure=false, Category="Joystick|Force Feedback")
	bool StopRumble(const FJoystickInstanceId& InstanceId) const;

	UFUNCTION(BlueprintCallable, BlueprintPure=false, Category="Joystick|Force Feedback")
	bool StopRumbleTriggers(const FJoystickInstanceId& InstanceId) const;

	UFUNCTION(BlueprintCallable, BlueprintPure=false, Category="Joystick|Force Feedback")
	bool StopHapticRumble(const FJoystickInstanceId& InstanceId) const;

	int CreateEffect(const FJoystickInstanceId& InstanceId, const SDL_HapticEffect& Effect) const;
	bool UpdateEffect(const FJoystickInstanceId& InstanceId, const int EffectId, const SDL_HapticEffect& Effect) const;
	bool RunEffect(const FJoystickInstanceId& InstanceId, const int EffectId, const int Iterations) const;
	bool StopEffect(const FJoystickInstanceId& InstanceId, const int EffectId) const;
	void DestroyEffect(const FJoystickInstanceId& InstanceId, const int EffectId) const;

private:
	TTuple<SDL_Haptic*, FInternalResultMessage> GetHapticDevice(const FJoystickInstanceId& InstanceId) const;
	TTuple<FDeviceInfoSDL*, FInternalResultMessage> GetDeviceInfo(const FJoystickInstanceId& InstanceId) const;
};
