// JoystickPlugin is licensed under the MIT License.
// Copyright Jayden Maalouf. All Rights Reserved.

#pragma once

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
	bool SetAutoCenter(const int DeviceId, const int Center);

	UFUNCTION(BlueprintCallable, Category = "Joystick|Force Feedback|Functions")
	bool SetGain(const int DeviceId, const int Gain);

	UFUNCTION(BlueprintCallable, Category = "Joystick|Force Feedback|Functions")
	void PauseDevice(const int DeviceId) const;

	UFUNCTION(BlueprintCallable, Category = "Joystick|Force Feedback|Functions")
	void UnpauseDevice(const int DeviceId) const;

	UFUNCTION(BlueprintCallable, Category = "Joystick|Force Feedback|Functions")
	void StopAllEffects(const int DeviceId) const;

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Joystick|Force Feedback|Functions")
	int GetNumEffects(const int DeviceId) const;

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Joystick|Force Feedback|Functions")
	int GetNumEffectsPlaying(const int DeviceId) const;

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Joystick|Force Feedback|Functions")
	int GetEffectStatus(const int DeviceId, const int EffectId);

	UFUNCTION(BlueprintCallable, Category = "Joystick|Force Feedback|Functions")
	void PlayRumble(const int DeviceId, const float LowFrequencyRumble, const float HighFrequencyRumble, UPARAM(DisplayName = "Duration (in seconds)")const float Duration) const;

	UFUNCTION(BlueprintCallable, Category = "Joystick|Force Feedback|Functions")
	void StopRumble(const int DeviceId);

	int CreateEffect(const int DeviceId, SDL_HapticEffect& Effect) const;
	bool UpdateEffect(int DeviceId, const int EffectId, SDL_HapticEffect& Effect) const;
	bool RunEffect(const int DeviceId, const int EffectId, const int Iterations) const;
	bool StopEffect(const int DeviceId, const int EffectId) const;
	void DestroyEffect(const int DeviceId, const int EffectId) const;

private:
	SDL_Haptic* GetHapticDevice(const int DeviceId) const;
	FDeviceInfoSDL* GetDeviceInfo(const int DeviceId) const;
};
