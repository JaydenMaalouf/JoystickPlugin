// JoystickPlugin is licensed under the MIT License.
// Copyright Jayden Maalouf. All Rights Reserved.

#pragma once

#include "Data/DeviceInfoSDL.h"
#include "Subsystems/EngineSubsystem.h"

struct FJoystickInfo;
struct FJoystickDeviceData;
class FJoystickInputDevice;

THIRD_PARTY_INCLUDES_START

#include "SDL.h"

THIRD_PARTY_INCLUDES_END

#include "JoystickSubsystem.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnJoystickSubsystemReady);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnJoystickEvent, int, DeviceId);

UCLASS(BlueprintType)
class JOYSTICKPLUGIN_API UJoystickSubsystem : public UEngineSubsystem
{
	GENERATED_BODY()
public:
	UJoystickSubsystem();

	// Begin USubsystem
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;
	// End USubsystem

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Joystick Subsystem|Functions")
	bool IsReady() const;

	UFUNCTION(BlueprintPure, Category = "Joystick|Functions")
	int GetJoystickCount() const;

	UFUNCTION(BlueprintPure, Category = "Joystick|Functions")
	int GetRegisteredDeviceCount() const;

	UFUNCTION(BlueprintCallable, Category = "Joystick|Functions")
	bool GetJoystickData(const int DeviceId, FJoystickDeviceData& JoystickDeviceData) const;

	UFUNCTION(BlueprintCallable, Category = "Joystick|Functions")
	bool GetJoystickInfo(const int DeviceId, FJoystickInfo& JoystickInfo) const;

	UFUNCTION(BlueprintCallable, Category = "Joystick|Functions")
	void MapJoystickDeviceToPlayer(const int DeviceId, const int PlayerId);

	UFUNCTION(BlueprintCallable, Category = "Joystick|Functions")
	void SetIgnoreGameControllers(const bool IgnoreControllers);

	UFUNCTION(BlueprintCallable, Category = "Joystick|Functions")
	void GetDeviceIds(TArray<int>& DeviceIds) const;

	UPROPERTY(BlueprintAssignable, Category = "Joystick|Delegates")
	FOnJoystickEvent JoystickPluggedInDelegate;

	UPROPERTY(BlueprintAssignable, Category = "Joystick|Delegates")
	FOnJoystickEvent JoystickUnpluggedDelegate;

	void InitialiseInputDevice(const TSharedPtr<FJoystickInputDevice> NewInputDevice);
	void Update() const;

	FString GetDeviceIndexGuidString(int DeviceIndex) const;
	void GetDeviceIndexGuid(const int DeviceIndex, FGuid& Guid) const;

	FDeviceInfoSDL* GetDeviceInfo(const int DeviceId);
	FJoystickDeviceData CreateInitialDeviceState(const int DeviceId);

	FJoystickInputDevice* GetInputDevice() const;

	UPROPERTY(BlueprintAssignable, Category = "Joystick Subsystem|Delegates")
	FOnJoystickSubsystemReady JoystickSubsystemReady;

private:
	static int HandleSDLEvent(void* UserData, SDL_Event* Event);

	bool AddDevice(const int DeviceIndex);
	void AddHapticDevice(FDeviceInfoSDL& Device) const;
	bool RemoveDevice(const int DeviceId);

	void JoystickPluggedIn(const FDeviceInfoSDL& Device) const;
	void JoystickUnplugged(const int DeviceId) const;

	TMap<int, FDeviceInfoSDL> Devices;
	TMap<int, int> DeviceMapping;

	TSharedPtr<FJoystickInputDevice> InputDevicePtr;

	bool OwnsSDL;
	bool IsInitialised;
};
