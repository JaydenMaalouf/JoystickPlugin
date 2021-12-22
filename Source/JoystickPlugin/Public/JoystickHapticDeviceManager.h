#pragma once
#include "JoystickInputDevice.h"
#include "ForceFeedback/Effects/ForceFeedbackEffectBase.h"

#include "JoystickHapticDeviceManager.generated.h"

UCLASS(BlueprintType)
class UJoystickHapticDeviceManager : public UObject
{
	GENERATED_BODY()
	
public:
	void SetInputDevice(const TSharedPtr<FJoystickInputDevice> NewInputDevice);
	
	UFUNCTION(BlueprintCallable)
		bool SetAutoCenter(int32 DeviceId, int32 Center);
	
	UFUNCTION(BlueprintCallable)
		bool SetGain(int32 DeviceId, int32 Gain);
	
	UFUNCTION(BlueprintCallable)
		void PauseDevice(int32 DeviceId) const;
	
	UFUNCTION(BlueprintCallable)
		void UnpauseDevice(int32 DeviceId) const;
	
	UFUNCTION(BlueprintCallable)
		void StopAllEffects(int32 DeviceId) const;
	
	UFUNCTION(BlueprintCallable)
		UForceFeedbackEffectBase* CreateEffect(TSubclassOf<UForceFeedbackEffectBase> ClassType, bool AutoStart) const;
	
	UFUNCTION(BlueprintCallable, BlueprintPure)
		int32 GetNumEffects(int32 DeviceId) const;
	
	UFUNCTION(BlueprintCallable, BlueprintPure)
		int32 GetNumEffectsPlaying(int32 DeviceId) const;
	
	UFUNCTION(BlueprintCallable, BlueprintPure)
		int32 GetEffectStatus(int32 DeviceId, int32 EffectId);

	SDL_Haptic* GetHapticDevice(int32 DeviceId) const;
	
private:
	TSharedPtr<FJoystickInputDevice> InputDevice;
};
