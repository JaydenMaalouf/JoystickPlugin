#pragma once
#include "JoystickInputDevice.h"
#include "Data/JoystickDeviceData.h"
#include "Data/DeviceInfoSDL.h"

#include "JoystickDeviceManager.generated.h"

UCLASS(BlueprintType)
class UJoystickDeviceManager : public UObject
{
	GENERATED_BODY()
	
public:
	void SetInputDevice(const TSharedPtr<FJoystickInputDevice> NewInputDevice);

	UFUNCTION(BlueprintPure)
		int32 JoystickCount() const;
	
	UFUNCTION(BlueprintCallable)
		FJoystickDeviceData GetJoystickData(const int32 DeviceId) const;

	UFUNCTION(BlueprintCallable)
		FJoystickInfo GetJoystickInfo(const int32 DeviceId) const;

	UFUNCTION(BlueprintCallable)
		void MapJoystickDeviceToPlayer(int32 DeviceId, int32 Player);

	UFUNCTION(BlueprintCallable)
		void IgnoreGameControllers(bool IgnoreControllers) const;

	UFUNCTION(BlueprintCallable)
		void ReinitialiseJoystickData(const int32 DeviceId) const;

	UFUNCTION(BlueprintCallable)
		TArray<int32> GetDeviceIds() const;

	FDeviceSDL* GetDeviceSDL() const;
	FDeviceInfoSDL* GetDeviceInfo(const int32 DeviceId) const;

private:
	TSharedPtr<FJoystickInputDevice> InputDevice;
};
