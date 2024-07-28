// JoystickPlugin is licensed under the MIT License.
// Copyright Jayden Maalouf. All Rights Reserved.

#pragma once

#include "Data/JoystickInstanceId.h"
#include "ForceFeedback/Effects/ForceFeedbackEffectBase.h"

#include "JoystickHapticDeviceManager.generated.h"

struct FDeviceInfoSDL;
using SDL_Haptic = _SDL_Haptic;
union SDL_HapticEffect;

UCLASS(BlueprintType)
class JOYSTICKPLUGIN_API UJoystickHapticDeviceManager final : public UObject
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintPure, Category = "Joystick|Force Feedback|Functions")
	static UJoystickHapticDeviceManager* GetJoystickHapticDeviceManager() { return StaticClass()->GetDefaultObject<UJoystickHapticDeviceManager>(); }

	UFUNCTION(BlueprintCallable, Category = "Joystick|Force Feedback|Functions")
	bool SetAutoCenter(const FJoystickInstanceId& InstanceId, const int Center);

	UFUNCTION(BlueprintCallable, Category = "Joystick|Force Feedback|Functions")
	bool SetGain(const FJoystickInstanceId& InstanceId, const int Gain);

	UFUNCTION(BlueprintCallable, Category = "Joystick|Force Feedback|Functions")
	bool PauseDevice(const FJoystickInstanceId& InstanceId) const;

	UFUNCTION(BlueprintCallable, Category = "Joystick|Force Feedback|Functions")
	bool UnpauseDevice(const FJoystickInstanceId& InstanceId) const;

	UFUNCTION(BlueprintCallable, Category = "Joystick|Force Feedback|Functions")
	bool StopAllEffects(const FJoystickInstanceId& InstanceId) const;

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Joystick|Force Feedback|Functions")
	int GetNumEffects(const FJoystickInstanceId& InstanceId) const;

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Joystick|Force Feedback|Functions")
	int GetNumEffectsPlaying(const FJoystickInstanceId& InstanceId) const;

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Joystick|Force Feedback|Functions")
	int GetEffectStatus(const FJoystickInstanceId& InstanceId, const int EffectId);

	UFUNCTION(BlueprintCallable, Category = "Joystick|Force Feedback|Functions")
	bool PlayRumble(const FJoystickInstanceId& InstanceId, const float LowFrequencyRumble, const float HighFrequencyRumble, UPARAM(DisplayName = "Duration (in seconds)") const float Duration);

	UFUNCTION(BlueprintCallable, Category = "Joystick|Force Feedback|Functions")
	bool StopRumble(const FJoystickInstanceId& InstanceId);

	int CreateEffect(const FJoystickInstanceId& InstanceId, SDL_HapticEffect& Effect) const;
	bool UpdateEffect(const FJoystickInstanceId& InstanceId, const int EffectId, SDL_HapticEffect& Effect) const;
	bool RunEffect(const FJoystickInstanceId& InstanceId, const int EffectId, const int Iterations) const;
	bool StopEffect(const FJoystickInstanceId& InstanceId, const int EffectId) const;
	void DestroyEffect(const FJoystickInstanceId& InstanceId, const int EffectId) const;

private:
	SDL_Haptic* GetHapticDevice(const FJoystickInstanceId& InstanceId) const;
	FDeviceInfoSDL* GetDeviceInfo(const FJoystickInstanceId& InstanceId) const;
};
