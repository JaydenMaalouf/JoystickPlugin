#pragma once

#include "Data/DeviceInfoSDL.h"
#include "Subsystems/EngineSubsystem.h"

struct FJoystickInfo;
struct FJoystickDeviceData;
class FJoystickInputDevice;

THIRD_PARTY_INCLUDES_START

#include "SDL.h"

THIRD_PARTY_INCLUDES_END

union SDL_Event;

#include "JoystickSubsystem.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogJoystickPlugin, Log, All);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnJoystickSubsystemReady);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnJoystickEvent, int32, DeviceId);

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
	int32 GetJoystickCount() const;

	UFUNCTION(BlueprintPure, Category = "Joystick|Functions")
	int32 GetRegisteredDeviceCount() const;

	UFUNCTION(BlueprintCallable, Category = "Joystick|Functions")
	bool GetJoystickData(const int32 DeviceId, FJoystickDeviceData& DeviceData) const;

	UFUNCTION(BlueprintCallable, Category = "Joystick|Functions")
	bool GetJoystickInfo(const int32 DeviceId, FJoystickInfo& DeviceInfo) const;

	UFUNCTION(BlueprintCallable, Category = "Joystick|Functions")
	void MapJoystickDeviceToPlayer(const int32 DeviceId, const int32 PlayerId);

	UFUNCTION(BlueprintCallable, Category = "Joystick|Functions")
	void SetIgnoreGameControllers(const bool IgnoreControllers);

	UFUNCTION(BlueprintCallable, Category = "Joystick|Functions")
	void GetDeviceIds(TArray<int32>& DeviceIds) const;

	UPROPERTY(BlueprintAssignable, Category = "Joystick|Delegates")
	FOnJoystickEvent JoystickPluggedInDelegate;

	UPROPERTY(BlueprintAssignable, Category = "Joystick|Delegates")
	FOnJoystickEvent JoystickUnpluggedDelegate;

	void InitialiseInputDevice(const TSharedPtr<FJoystickInputDevice> NewInputDevice);
	void Update() const;

	FString GetDeviceIndexGuidString(int32 DeviceIndex) const;
	void GetDeviceIndexGuid(const int32 DeviceIndex, FGuid& Guid) const;

	bool GetDeviceInfo(const int32 DeviceId, FDeviceInfoSDL& DeviceInfoSDL);
	bool GetInitialDeviceState(const int32 DeviceId, FJoystickDeviceData& DeviceState);

	FJoystickInputDevice* GetInputDevice() const;

	UPROPERTY(BlueprintAssignable, Category = "Joystick Subsystem|Delegates")
	FOnJoystickSubsystemReady JoystickSubsystemReady;

private:
	static int32 HandleSDLEvent(void* UserData, SDL_Event* Event);

	bool AddDevice(const int32 DeviceIndex);
	void AddHapticDevice(FDeviceInfoSDL& Device);
	bool RemoveDevice(const int32 DeviceId);

	void JoystickPluggedIn(const FDeviceInfoSDL& Device) const;
	void JoystickUnplugged(const int32 DeviceId) const;

	TMap<int32, FDeviceInfoSDL> Devices;
	TMap<int32, int32> DeviceMapping;

	TSharedPtr<FJoystickInputDevice> InputDevicePtr;

	bool IgnoreGameControllers;
	bool OwnsSDL;
	bool IsInitialised;
};
