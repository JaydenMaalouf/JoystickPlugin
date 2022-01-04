#pragma once

#include "ForceFeedback/Effects/ForceFeedbackEffectBase.h"

#include "JoystickHapticDeviceManager.generated.h"

UCLASS(BlueprintType)
class UJoystickHapticDeviceManager : public UObject
{
	GENERATED_BODY()
	
public:

	UFUNCTION(BlueprintPure, Category = "Joystick|Force Feedback|Functions")
		static UJoystickHapticDeviceManager* GetJoystickHapticDeviceManager() { return StaticClass()->GetDefaultObject<UJoystickHapticDeviceManager>(); }
	
	UFUNCTION(BlueprintCallable, Category = "Joystick|Force Feedback|Functions")
		bool SetAutoCenter(int32 DeviceId, int32 Center);
	
	UFUNCTION(BlueprintCallable, Category = "Joystick|Force Feedback|Functions")
		bool SetGain(int32 DeviceId, int32 Gain);
	
	UFUNCTION(BlueprintCallable, Category = "Joystick|Force Feedback|Functions")
		void PauseDevice(int32 DeviceId) const;
	
	UFUNCTION(BlueprintCallable, Category = "Joystick|Force Feedback|Functions")
		void UnpauseDevice(int32 DeviceId) const;
	
	UFUNCTION(BlueprintCallable, Category = "Joystick|Force Feedback|Functions")
		void StopAllEffects(int32 DeviceId) const;
	
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Joystick|Force Feedback|Functions")
		int32 GetNumEffects(int32 DeviceId) const;
	
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Joystick|Force Feedback|Functions")
		int32 GetNumEffectsPlaying(int32 DeviceId) const;
	
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Joystick|Force Feedback|Functions")
		int32 GetEffectStatus(int32 DeviceId, int32 EffectId);

	SDL_Haptic* GetHapticDevice(int32 DeviceId) const;
};
