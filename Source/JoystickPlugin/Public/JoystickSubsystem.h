// JoystickPlugin is licensed under the MIT License.
// Copyright Jayden Maalouf 2026. All Rights Reserved.

#pragma once

#include "Data/DeviceInfoSDL.h"
#include "Data/JoystickDeviceState.h"
#include "Data/JoystickSensorType.h"
#include "Data/ResultMessage.h"
#include "Subsystems/EngineSubsystem.h"

THIRD_PARTY_INCLUDES_START

#include "SDL_events.h"

THIRD_PARTY_INCLUDES_END

#include "JoystickSubsystem.generated.h"

class FJoystickInputDevice;
struct FJoystickInfo;
struct FJoystickInformation;
struct FJoystickInstanceId;
struct FInputDeviceId;
struct FPlatformUserId;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnJoystickSubsystemReady);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FJoystickEvent, const FJoystickInstanceId&, InstanceId);

DECLARE_MULTICAST_DELEGATE_OneParam(FInternalJoystickEvent, const FJoystickInstanceId&);

UCLASS(BlueprintType)
class JOYSTICKPLUGIN_API UJoystickSubsystem : public UEngineSubsystem
{
	GENERATED_BODY()

public:
	UJoystickSubsystem();

	// UEngineSubsystem
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;

	UFUNCTION(BlueprintCallable, BlueprintPure, Category="Joystick Subsystem",
		meta=(ToolTip="Checks whether the Subsystem and Input Device are ready"))
	bool IsReady() const;

	UFUNCTION(BlueprintCallable, BlueprintPure, Category="Joystick Subsystem",
		meta=(ToolTip="Checks whether the Subsystem is ready - this does NOT indicate that the Input Device has been created"))
	bool IsInitialised() const;

	UFUNCTION(BlueprintPure, Category="Joystick",
		meta=(ToolTip="The number of joysticks captured by SDL2. This does not reflect the joystick count monitored by the plugin - please use GetJoystickCount() or GetConnectedJoystickCount() instead."))
	int GetRawJoystickCount() const;

	UFUNCTION(BlueprintPure, Category="Joystick",
		meta=(ToolTip="The number of joysticks captured by this plugin. This does not reflect the current joystick count, as disconnected joysticks will persist in this count - please use GetConnectedJoystickCount() instead."))
	int GetJoystickCount() const;

	UFUNCTION(BlueprintPure, Category="Joystick", meta=(ToolTip="The number of connected joysticks monitored by this plugin."))
	int GetConnectedJoystickCount() const;

	UFUNCTION(BlueprintCallable, BlueprintPure=false, Category="Joystick", meta=(ToolTip="The raw input state of the specified joystick."))
	bool GetJoystickState(const FJoystickInstanceId& InstanceId, FJoystickDeviceState& JoystickDeviceState) const;

	UFUNCTION(BlueprintCallable, Category="Joystick")
	bool GetJoystickInfo(const FJoystickInstanceId& InstanceId, FJoystickInformation& JoystickInfo);

	UFUNCTION(BlueprintCallable, Category="Joystick")
	EJoystickType GetJoystickType(const FJoystickInstanceId& InstanceId);

	UFUNCTION(BlueprintCallable, Category="Joystick")
	EJoystickPowerLevel GetJoystickPowerLevel(const FJoystickInstanceId& InstanceId);

	UFUNCTION(BlueprintCallable, Category="Joystick")
	void MapJoystickDeviceToPlayer(const FJoystickInstanceId& InstanceId, const int PlayerId);

	UFUNCTION(BlueprintCallable, Category="Joystick")
	void SetIgnoreGameControllers(const bool IgnoreControllers);

	UFUNCTION(BlueprintCallable, Category="Joystick")
	bool SetJoystickSensorEnabled(const FJoystickInstanceId& InstanceId, const EJoystickSensorType SensorType, const bool Enabled);

	UFUNCTION(BlueprintCallable, Category="Joystick")
	bool SetJoystickLedColor(const FJoystickInstanceId& InstanceId, const FColor Color);

	UFUNCTION(BlueprintCallable, Category="Joystick")
	void GetInstanceIds(TArray<FJoystickInstanceId>& InstanceIds, const bool IncludeDisconnected = false) const;

	UFUNCTION(BlueprintCallable, Category="Joystick")
	bool HasRumbleDevice() const;

	UFUNCTION(BlueprintCallable, Category="Joystick")
	bool IsConnected(const FJoystickInstanceId& InstanceId) const;

	UPROPERTY(BlueprintAssignable, Category="Joystick|Delegates")
	FJoystickEvent JoystickPluggedInDelegate;
	FInternalJoystickEvent Internal_JoystickPluggedInDelegate;

	UPROPERTY(BlueprintAssignable, Category="Joystick|Delegates")
	FJoystickEvent JoystickUnpluggedDelegate;
	FInternalJoystickEvent Internal_JoystickUnpluggedDelegate;

	void InitialiseInputDevice(const TSharedPtr<FJoystickInputDevice>& NewInputDevice);
	void Update() const;

	TTuple<FDeviceInfoSDL*, FInternalResultMessage> GetDeviceInfo(const FJoystickInstanceId& InstanceId);
	FJoystickDeviceState CreateInitialDeviceState(const FJoystickInstanceId& InstanceId);

	FJoystickInputDevice* GetInputDevice() const;

	const TMap<FJoystickInstanceId, FDeviceInfoSDL>& GetDevices();

	UPROPERTY(BlueprintAssignable, Category="Joystick|Subsystem|Delegates")
	FOnJoystickSubsystemReady JoystickSubsystemReady;

	UPROPERTY(BlueprintAssignable, Category="Joystick|Subsystem|Delegates")
	FOnJoystickSubsystemReady JoystickSubsystemInitialised;

private:
	static int HandleSDLEvent(void* UserData, SDL_Event* Event);

	bool AddDevice(const int DeviceIndex);
	void AddHapticDevice(FDeviceInfoSDL& Device) const;
	void AddSensorDevice(FDeviceInfoSDL& Device) const;
	bool RemoveDevice(const FJoystickInstanceId& InstanceId);
	bool RemoveDeviceByIndex(const int DeviceIndex);

	bool FindExistingDevice(const FDeviceInfoSDL& Device, FJoystickInstanceId& PreviousJoystickInstanceId, FInputDeviceId& ExistingInputDeviceId, FPlatformUserId& ExistingPlatformUserId);

	void InitialiseExistingJoysticks();

	void JoystickPluggedIn(const FDeviceInfoSDL& Device) const;
	void JoystickUnplugged(const FJoystickInstanceId& InstanceId, const FInputDeviceId& InputDeviceId) const;

	void LoadGameControllerMappings() const;

	void ConvertSDLGuid(const SDL_JoystickGUID& SdlGuid, FGuid& OutGuid) const;
	FString GenerateDeviceHash(const FDeviceInfoSDL& Device) const;
	FString SafelyStringify(const char* Input) const;

	TMap<FJoystickInstanceId, FDeviceInfoSDL> Devices;

	TSharedPtr<FJoystickInputDevice> InputDevicePtr;

	bool OwnsSDL;
	bool bIsInitialised;

	static FString GameControllerMappingFile;
	static FString AxisPropertiesSection;
	static FString ButtonPropertiesSection;
	static FString JoystickConfigurationSection;
};
