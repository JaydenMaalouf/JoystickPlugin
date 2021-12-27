#pragma once

#include "JoystickInputDevice.h"
#include "Subsystems/EngineSubsystem.h"

THIRD_PARTY_INCLUDES_START

#include "SDL.h"

THIRD_PARTY_INCLUDES_END

union SDL_Event;

#include "JoystickSubsystem.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogJoystickPlugin, Log, All);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnJoystickSubsystemReady);

UCLASS()
class UJoystickSubsystem : public UEngineSubsystem
{
	GENERATED_BODY()
public:
	UJoystickSubsystem();
	
	// Begin USubsystem
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;
	// End USubsystem

	void InitialiseInputDevice(const TSharedPtr<FJoystickInputDevice> NewInputDevice);
	void Update() const;
	
	void SetIgnoreGameControllers(bool IgnoreControllers);
	void ReinitialiseJoystickData(const int32 DeviceId);

	FString DeviceGUIDtoString(int32 DeviceIndex) const;
	FGuid DeviceIndexToGUID(int32 DeviceIndex) const;
	
	FDeviceInfoSDL* GetDeviceInfo(const int32 DeviceId);
	FJoystickDeviceData GetInitialDeviceState(int32 DeviceId);
	
	FJoystickInputDevice* GetInputDevice() const { return InputDevice.Get(); }
	
	int32 GetJoystickCount() const;

	UPROPERTY(BlueprintAssignable)
		FOnJoystickSubsystemReady JoystickSubsystemReady;
	
private:

	static int32 HandleSDLEvent(void* UserData, SDL_Event* Event);

	FDeviceInfoSDL AddDevice(int32 DeviceIndex);
	void RemoveDevice(int32 DeviceId);
	
	void JoystickPluggedIn(const FDeviceInfoSDL &Device) const;
	void JoystickUnplugged(int32 DeviceId) const;

	TMap<int32, FDeviceInfoSDL> Devices;
	TMap<int32, int32> DeviceMapping;
	
	TSharedPtr<FJoystickInputDevice> InputDevice;
	
	bool IgnoreGameControllers;
	bool OwnsSDL;
};