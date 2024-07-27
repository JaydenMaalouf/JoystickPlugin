// JoystickPlugin is licensed under the MIT License.
// Copyright Jayden Maalouf. All Rights Reserved.

#pragma once

#include "Data/JoystickInstanceId.h"
#include "Data/DeviceInfoSDL.h"
#include "Data/JoystickInformation.h"
#include "Data/JoystickPowerLevel.h"
#include "Data/JoystickType.h"
#include "Subsystems/EngineSubsystem.h"

#include "JoystickSubsystem.generated.h"

struct FJoystickInfo;
struct FJoystickDeviceState;
class FJoystickInputDevice;
union SDL_Event;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnJoystickSubsystemReady);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnJoystickEvent, const FJoystickInstanceId&, InstanceId);

UCLASS(BlueprintType)
class JOYSTICKPLUGIN_API UJoystickSubsystem : public UEngineSubsystem
{
	GENERATED_BODY()

public:
	UJoystickSubsystem();

	// Begin USubsystem
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;
	void InitialiseExistingJoysticks();
	// End USubsystem

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Joystick Subsystem|Functions",
		meta=(ToolTip="Checks whether the Subsystem and Input Device are ready"))
	bool IsReady() const;

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Joystick Subsystem|Functions",
		meta=(ToolTip="Checks whether the Subsystem is ready - this does NOT indicate that the Input Device has been created"))
	bool IsInitialised() const;

	UFUNCTION(BlueprintPure, Category = "Joystick|Functions",
		meta=(ToolTip="The number of joysticks captured by SDL2. This does not reflect the joystick count monitored by the plugin - please use GetJoystickCount() or GetConnectedJoystickCount() instead."))
	int GetRawJoystickCount() const;

	UFUNCTION(BlueprintPure, Category = "Joystick|Functions",
		meta=(ToolTip = "The number of joysticks captured by this plugin. This does not reflect the current joystick count, as disconnected joysticks will persist in this count - please use GetConnectedJoystickCount() instead."))
	int GetJoystickCount() const;

	UFUNCTION(BlueprintPure, Category = "Joystick|Functions", meta=(ToolTip = "The number of connected joysticks monitored by this plugin."))
	int GetConnectedJoystickCount() const;

	UFUNCTION(BlueprintCallable, Category = "Joystick|Functions", meta=(ToolTip = "The raw input state of the specified joystick."))
	bool GetJoystickState(const FJoystickInstanceId& InstanceId, FJoystickDeviceState& JoystickDeviceState);

	UFUNCTION(BlueprintCallable, Category = "Joystick|Functions")
	bool GetJoystickInfo(const FJoystickInstanceId& InstanceId, FJoystickInformation& JoystickInfo);

	UFUNCTION(BlueprintCallable, Category = "Joystick|Functions")
	EJoystickType GetJoystickType(const FJoystickInstanceId& InstanceId);

	UFUNCTION(BlueprintCallable, Category = "Joystick|Functions")
	EJoystickPowerLevel GetJoystickPowerLevel(const FJoystickInstanceId& InstanceId);

	UFUNCTION(BlueprintCallable, Category = "Joystick|Functions")
	void MapJoystickDeviceToPlayer(const FJoystickInstanceId& InstanceId, const int PlayerId);

	UFUNCTION(BlueprintCallable, Category = "Joystick|Functions")
	void SetIgnoreGameControllers(const bool IgnoreControllers);

	UFUNCTION(BlueprintCallable, Category = "Joystick|Functions")
	bool SetJoystickSensorEnabled(const FJoystickInstanceId& InstanceId, const EJoystickSensorType SensorType, const bool Enabled);

	UFUNCTION(BlueprintCallable, Category = "Joystick|Functions")
	bool SetJoystickLedColor(const FJoystickInstanceId& InstanceId, const FColor Color);

	UFUNCTION(BlueprintCallable, Category = "Joystick|Functions")
	void GetInstanceIds(TArray<FJoystickInstanceId>& InstanceIds) const;

	UFUNCTION(BlueprintCallable, Category = "Joystick|Functions")
	bool HasRumbleDevice() const;

	UFUNCTION(BlueprintCallable, Category = "Joystick|Functions")
	bool IsConnected(const FJoystickInstanceId& InstanceId) const;

	UPROPERTY(BlueprintAssignable, Category = "Joystick|Delegates")
	FOnJoystickEvent JoystickPluggedInDelegate;

	UPROPERTY(BlueprintAssignable, Category = "Joystick|Delegates")
	FOnJoystickEvent JoystickUnpluggedDelegate;

	void InitialiseInputDevice(const TSharedPtr<FJoystickInputDevice>& NewInputDevice);
	void Update() const;

	FDeviceInfoSDL* GetDeviceInfo(const FJoystickInstanceId& InstanceId);
	FJoystickDeviceState CreateInitialDeviceState(const FJoystickInstanceId& InstanceId);

	FJoystickInputDevice* GetInputDevice() const;

	const TMap<FJoystickInstanceId, FDeviceInfoSDL>& GetDevices();

	UPROPERTY(BlueprintAssignable, Category = "Joystick Subsystem|Delegates")
	FOnJoystickSubsystemReady JoystickSubsystemReady;

	UPROPERTY(BlueprintAssignable, Category = "Joystick Subsystem|Delegates")
	FOnJoystickSubsystemReady JoystickSubsystemInitialised;

private:
	static int HandleSDLEvent(void* UserData, SDL_Event* Event);

	bool AddDevice(const int DeviceIndex);
	void AddHapticDevice(FDeviceInfoSDL& Device) const;
	void AddSensorDevice(FDeviceInfoSDL& Device) const;
	bool RemoveDevice(const FJoystickInstanceId& InstanceId);
	bool RemoveDeviceByIndex(const int DeviceIndex);
	bool FindExistingDeviceIndex(const FDeviceInfoSDL& Device, int& ExistingDeviceIndex);

	void JoystickPluggedIn(const FDeviceInfoSDL& Device) const;
	void JoystickUnplugged(const FJoystickInstanceId& InstanceId) const;

	TMap<FJoystickInstanceId, FDeviceInfoSDL> Devices;

	TSharedPtr<FJoystickInputDevice> InputDevicePtr;

	bool OwnsSDL;
	bool bIsInitialised;
	int PersistentDeviceCount;
};
