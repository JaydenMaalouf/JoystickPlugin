// JoystickPlugin is licensed under the MIT License.
// Copyright Jayden Maalouf. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"

#include "Data/Settings/JoystickInputDeviceConfiguration.h"
#include "Data/Settings/JoystickInputDeviceInformation.h"

#include "JoystickInputSettings.generated.h"

UCLASS(config=Input, DefaultConfig)
class JOYSTICKPLUGIN_API UJoystickInputSettings final : public UObject
{
	GENERATED_BODY()

public:
	UJoystickInputSettings();
#if WITH_EDITOR
	virtual void PostEditChangeChainProperty(FPropertyChangedChainEvent& PropertyChangedEvent) override;
#endif

	void DeviceAdded(FJoystickInputDeviceInformation JoystickInfo);
	void DeviceRemoved(FGuid JoystickGuid);
	void ResetDevices();
	
	const FJoystickInputDeviceConfiguration* GetInputDeviceConfiguration(const FJoystickInfo& Device) const;
	const FJoystickInputDeviceConfiguration* GetInputDeviceConfigurationByKey(const FKey& Key) const;
	const FJoystickInputDeviceAxisProperties* GetAxisPropertiesByKey(const FKey& AxisKey) const;

	UPROPERTY(VisibleAnywhere, Category="Information")
	TArray<FJoystickInputDeviceInformation> ConnectedDevices;
	
	UPROPERTY(config, EditAnywhere, Category="Joystick Input Settings", meta=(ToolTip="When creating the input keys for devices, use the device name in the key. Default will prefix the key with \"Joystick\" instead.", ConfigRestartRequired=true))
	bool UseDeviceName;
	
	UPROPERTY(config, EditAnywhere, Category="Joystick Input Settings", meta=(ToolTip="Useful if you want input for controllers (ie. XInput) to be handled by UE directly, instead of via this plugin.", ConfigRestartRequired=true))
	bool IgnoreGameControllers;
	
	UPROPERTY(config, EditAnywhere, Category="Joystick Input Settings", meta=(ToolTip="Enable/disable debug logging from the plugin."))
	bool DebugLogs;

	UPROPERTY(config, EditAnywhere, Category="Joystick Input Settings")
	TArray<FJoystickInputDeviceConfiguration> DeviceConfigurations;

	bool GetIgnoreGameControllers() const;
	bool SetIgnoreGameControllers(const bool NewIgnoreGameControllers);
	
private:
	int GetDeviceIndexByKey(const FKey& Key) const;
};
