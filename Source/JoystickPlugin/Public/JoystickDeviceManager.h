#pragma once
#include <SDL_events.h>

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

	UFUNCTION(BlueprintPure)
		static UJoystickDeviceManager* GetJoystickDeviceManager() { return StaticClass()->GetDefaultObject<UJoystickDeviceManager>(); }

	UFUNCTION(BlueprintPure)
		int32 JoystickCount() const;
	
	UFUNCTION(BlueprintCallable)
		FJoystickDeviceData GetJoystickData(const int32 DeviceId) const;

	UFUNCTION(BlueprintCallable)
		FJoystickInfo GetJoystickInfo(const int32 DeviceId) const;

	UFUNCTION(BlueprintCallable)
		void MapJoystickDeviceToPlayer(int32 DeviceId, int32 Player);

	UFUNCTION(BlueprintCallable)
		void SetIgnoreGameControllers(bool IgnoreControllers);

	UFUNCTION(BlueprintCallable)
		TArray<int32> GetDeviceIds() const;

	UPROPERTY(BlueprintAssignable)
		FOnJoystickPluggedIn JoystickPluggedIn;
	UPROPERTY(BlueprintAssignable)
		FOnJoystickUnplugged JoystickUnplugged;
};
