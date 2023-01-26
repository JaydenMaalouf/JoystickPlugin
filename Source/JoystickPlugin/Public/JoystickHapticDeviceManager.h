#pragma once

#include "ForceFeedback/Effects/ForceFeedbackEffectBase.h"

#include "JoystickHapticDeviceManager.generated.h"

UCLASS(BlueprintType)
class JOYSTICKPLUGIN_API UJoystickHapticDeviceManager final : public UObject
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintPure, Category = "Joystick|Force Feedback|Functions")
	static UJoystickHapticDeviceManager* GetJoystickHapticDeviceManager() { return StaticClass()->GetDefaultObject<UJoystickHapticDeviceManager>(); }

	UFUNCTION(BlueprintCallable, Category = "Joystick|Force Feedback|Functions")
	bool SetAutoCenter(int DeviceId, int Center);

	UFUNCTION(BlueprintCallable, Category = "Joystick|Force Feedback|Functions")
	bool SetGain(int DeviceId, int Gain);

	UFUNCTION(BlueprintCallable, Category = "Joystick|Force Feedback|Functions")
	void PauseDevice(int DeviceId) const;

	UFUNCTION(BlueprintCallable, Category = "Joystick|Force Feedback|Functions")
	void UnpauseDevice(int DeviceId) const;

	UFUNCTION(BlueprintCallable, Category = "Joystick|Force Feedback|Functions")
	void StopAllEffects(int DeviceId) const;

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Joystick|Force Feedback|Functions")
	int GetNumEffects(int DeviceId) const;

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Joystick|Force Feedback|Functions")
	int GetNumEffectsPlaying(int DeviceId) const;

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Joystick|Force Feedback|Functions")
	int GetEffectStatus(int DeviceId, int EffectId);

	SDL_Haptic* GetHapticDevice(int DeviceId) const;
};
