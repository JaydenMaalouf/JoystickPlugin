// JoystickPlugin is licensed under the MIT License.
// Copyright Jayden Maalouf. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"
#include "InputCoreTypes.h"
#include "Data/JoystickInstanceId.h"
#include "Data/JoystickInformation.h"
#include "Data/Settings/JoystickInputDeviceConfiguration.h"
#include "Data/Settings/JoystickInputKeyConfiguration.h"

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

	void DeviceAdded(const FJoystickInformation& JoystickInfo);
	void DeviceRemoved(const FJoystickInstanceId& InstanceId);
	void ResetDevices();

	const FJoystickInputDeviceConfiguration* GetInputDeviceConfiguration(const FGuid& ProductId) const;
	const FJoystickInputDeviceConfiguration* GetInputDeviceConfigurationByKey(const FKey& Key) const;
	const FJoystickInputDeviceAxisProperties* GetAxisPropertiesByKey(const FKey& AxisKey) const;

	UPROPERTY(VisibleAnywhere, Category="Information")
	TArray<FJoystickInformation> ConnectedDevices;

	UPROPERTY(config, EditAnywhere, meta=(ToolTip="When creating the input keys for devices, use the device name in the key. Default will prefix the key with \"Joystick\" instead.", ConfigRestartRequired=true))
	bool UseDeviceName;

	UPROPERTY(config, EditAnywhere, meta=(ToolTip="Useful if you want input for controllers (ie. XInput) to be handled by UE directly, instead of via this plugin.", ConfigRestartRequired=true))
	bool IgnoreGameControllers;

	UPROPERTY(config, EditAnywhere, meta=(ToolTip="Enable debug logging from the plugin."))
	bool EnableLogs;

	UPROPERTY(config, EditAnywhere, meta = (Bitmask, BitmaskEnum = "/Script/JoystickPlugin.EHatDirection", ToolTip="Map Hat Axis to Keys. Select which direction keys you would like mapped", ConfigRestartRequired=true))
	int32 MapHatAxisToKeys;

	UPROPERTY(config, EditAnywhere)
	TArray<FJoystickInputDeviceConfiguration> DeviceConfigurations;

	UPROPERTY(config, EditAnywhere)
	TMap<FKey, FJoystickInputKeyConfiguration> KeyConfigurations;

	bool GetIgnoreGameControllers() const;
	bool SetIgnoreGameControllers(const bool NewIgnoreGameControllers);
};
