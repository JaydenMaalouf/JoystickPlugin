#pragma once
#include "JoystickInputDevice.h"
#include "Data/JoystickDeviceData.h"

#include "JoystickDeviceManager.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnJoystickPluggedIn, int32, DeviceId);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnJoystickUnplugged, int32, DeviceId);

UCLASS(BlueprintType)
class UJoystickDeviceManager : public UObject
{
	GENERATED_BODY()
	
public:

	UFUNCTION(BlueprintPure, Category = "Joystick|Functions")
		static UJoystickDeviceManager* GetJoystickDeviceManager() { return StaticClass()->GetDefaultObject<UJoystickDeviceManager>(); }

	UFUNCTION(BlueprintPure, Category = "Joystick|Functions")
		int32 GetJoystickCount() const;
	
	UFUNCTION(BlueprintPure, Category = "Joystick|Functions")
		int32 GetValidDeviceCount() const;

	UFUNCTION(BlueprintCallable, Category = "Joystick|Functions")
		FJoystickDeviceData GetJoystickData(const int32 DeviceId) const;

	UFUNCTION(BlueprintCallable, Category = "Joystick|Functions")
		FJoystickInfo GetJoystickInfo(const int32 DeviceId) const;

	UFUNCTION(BlueprintCallable, Category = "Joystick|Functions")
		void MapJoystickDeviceToPlayer(int32 DeviceId, int32 PlayerId);

	UFUNCTION(BlueprintCallable, Category = "Joystick|Functions")
		void SetIgnoreGameControllers(bool IgnoreControllers);

	UFUNCTION(BlueprintCallable, Category = "Joystick|Functions")
		TArray<int32> GetDeviceIds() const;

	UPROPERTY(BlueprintAssignable, Category = "Joystick|Delegates")
		FOnJoystickPluggedIn JoystickPluggedIn;
	UPROPERTY(BlueprintAssignable, Category = "Joystick|Delegates")
		FOnJoystickUnplugged JoystickUnplugged;
};
