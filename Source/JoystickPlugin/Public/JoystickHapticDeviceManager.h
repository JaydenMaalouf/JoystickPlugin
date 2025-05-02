// JoystickPlugin is licensed under the MIT License.
// Copyright Jayden Maalouf. All Rights Reserved.

#pragma once

#include "Data/JoystickInstanceId.h"

THIRD_PARTY_INCLUDES_START

#include "SDL_haptic.h"

THIRD_PARTY_INCLUDES_END

#include "JoystickHapticDeviceManager.generated.h"

struct FDeviceInfoSDL;
using SDL_Haptic = _SDL_Haptic;
union SDL_HapticEffect;

UCLASS(BlueprintType)
class JOYSTICKPLUGIN_API UJoystickHapticDeviceManager final : public UObject
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintPure, Category = "Joystick|Force Feedback")
	static UJoystickHapticDeviceManager* GetJoystickHapticDeviceManager() { return StaticClass()->GetDefaultObject<UJoystickHapticDeviceManager>(); }

	UFUNCTION(BlueprintCallable, Category = "Joystick|Force Feedback")
	bool SetAutoCenter(const FJoystickInstanceId& InstanceId, const int Center);

	UFUNCTION(BlueprintCallable, Category = "Joystick|Force Feedback")
	bool SetGain(const FJoystickInstanceId& InstanceId, const int Gain);

	UFUNCTION(BlueprintCallable, Category = "Joystick|Force Feedback")
	bool PauseDevice(const FJoystickInstanceId& InstanceId) const;

	UFUNCTION(BlueprintCallable, Category = "Joystick|Force Feedback")
	bool UnpauseDevice(const FJoystickInstanceId& InstanceId) const;

	UFUNCTION(BlueprintCallable, Category = "Joystick|Force Feedback")
	bool StopAllEffects(const FJoystickInstanceId& InstanceId) const;

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Joystick|Force Feedback")
	int GetNumEffects(const FJoystickInstanceId& InstanceId) const;

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Joystick|Force Feedback")
	int GetNumEffectsPlaying(const FJoystickInstanceId& InstanceId) const;

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Joystick|Force Feedback")
	int GetEffectStatus(const FJoystickInstanceId& InstanceId, const int EffectId);

	UFUNCTION(BlueprintCallable, Category = "Joystick|Force Feedback")
	bool PlayRumble(const FJoystickInstanceId& InstanceId, const float LowFrequencyRumble, const float HighFrequencyRumble, UPARAM(DisplayName = "Duration (in seconds)") const float Duration);
	UFUNCTION(BlueprintCallable, Category = "Joystick|Force Feedback")
	bool PlayHapticRumble(const FJoystickInstanceId& InstanceId, const float Strength, const float Duration);

	UFUNCTION(BlueprintCallable, Category = "Joystick|Force Feedback")
	bool StopRumble(const FJoystickInstanceId& InstanceId);
	UFUNCTION(BlueprintCallable, Category = "Joystick|Force Feedback")
	bool StopHapticRumble(const FJoystickInstanceId& InstanceId);

	int CreateEffect(const FJoystickInstanceId& InstanceId, SDL_HapticEffect& Effect) const;
	bool UpdateEffect(const FJoystickInstanceId& InstanceId, const int EffectId, SDL_HapticEffect& Effect) const;
	bool RunEffect(const FJoystickInstanceId& InstanceId, const int EffectId, const int Iterations) const;
	bool StopEffect(const FJoystickInstanceId& InstanceId, const int EffectId) const;
	void DestroyEffect(const FJoystickInstanceId& InstanceId, const int EffectId) const;

private:
	TTuple<SDL_Haptic*, FResultMessage> GetHapticDevice(const FJoystickInstanceId& InstanceId) const;
	TTuple<FDeviceInfoSDL*, FResultMessage> GetDeviceInfo(const FJoystickInstanceId& InstanceId) const;
};
