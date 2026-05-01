// JoystickPlugin is licensed under the MIT License.
// Copyright Jayden Maalouf 2026. All Rights Reserved.

#pragma once

#include "Data/JoystickInformation.h"
#include "Data/Settings/JoystickInputKeyConfiguration.h"
#include "InputCoreTypes.h"
#include "Data/Settings/JoystickPrioritisationType.h"

#include "JoystickInputSettings.generated.h"

struct FJoystickInputDeviceConfiguration;

UCLASS(config = Input, DefaultConfig)
class JOYSTICKPLUGIN_API UJoystickInputSettings final : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category="Information")
	TArray<FJoystickInformation> ConnectedDevices;

	UPROPERTY(config, EditAnywhere, Category="Joystick Settings",
		meta=(ToolTip="When creating the input keys for devices, use the device name in the key. Default will prefix the key with \"Joystick\" instead.", ConfigRestartRequired=true))
	bool UseDeviceName;

	UPROPERTY(config, EditAnywhere, Category="Joystick Settings", meta=(ToolTip="Include Device Index in Key names. Disable this if SDL2 is not guaranteeing the same index for your device.", ConfigRestartRequired=true))
	bool IncludeDeviceIndex;

	UPROPERTY(config, EditAnywhere, Category="Joystick Settings", meta=(ToolTip="Useful if you want input for controllers (ie. XInput) to be handled by UE directly, instead of via this plugin.", ConfigRestartRequired=true))
	bool IgnoreGameControllers;

	UPROPERTY(config, EditAnywhere, Category="Joystick Settings|Advanced", meta=(ToolTip="If false, will not add a Joystick if a mapped device with the same hash exists.", ConfigRestartRequired=true))
	bool AllowDuplicateHashedDevices;

	UPROPERTY(config, EditAnywhere, Category="Joystick Settings|Advanced", DisplayName="Prioritise Devices by Collection Index",
		meta=(ToolTip="If true, will skip devices with the same GUID based on the driver collection index prioritisation method. Useful for drivers that map multiple devices for the same GUID.",
			EditConditionHides, ConfigRestartRequired=true))
	bool PrioritiseCollectionIndex;

	UPROPERTY(config, EditAnywhere, Category="Joystick Settings|Advanced",
		meta=(ToolTip="The prioritisation method to use.", EditCondition="PrioritiseCollectionIndex", ConfigRestartRequired=true))
	EJoystickPrioritisationType PrioritisationMethod;

	UPROPERTY(config, EditAnywhere, Category="Joystick Settings|Advanced",
		meta=(ToolTip="Use the device name as the hardware device identifier. Useful if you need specific hardware identification (ie. for icons).", ConfigRestartRequired=true))
	bool UseDeviceNameAsHardwareDeviceIdentifier;

	UPROPERTY(config, EditAnywhere, Category="Joystick Settings|Advanced", meta=(ToolTip="Key name indexes should start from 0 (if false, starts from 1)", ConfigRestartRequired=true))
	bool ZeroBasedIndexing;

	UPROPERTY(config, EditAnywhere, Category="Joystick Settings|Debugging", meta=(ToolTip="Enable debug logging from the plugin."))
	bool EnableLogs;

#if WITH_EDITORONLY_DATA
	UPROPERTY(config, EditAnywhere, Category="Joystick Settings|Debugging", meta=(ToolTip="Debug Axis values by printing them to screen"))
	bool DebugAxis;
#endif

	UPROPERTY(config, EditAnywhere, Category="Joystick Settings", meta=(ToolTip="Instead of a separate key for X and Y axis keys, a FVector2D key will be created alongside the existing keys."))
	bool EnablePairedKeys;

	UPROPERTY(config, EditAnywhere, Category="Joystick Settings",
		meta=(Bitmask, BitmaskEnum="/Script/JoystickPlugin.EHatDirection", ToolTip="Map Hat Axis to Keys. Select which direction keys you would like mapped", ConfigRestartRequired=true))
	int32 MapHatAxisToKeys;

	UPROPERTY(VisibleAnywhere, Category="Joystick Settings", meta=(ToolTip="A container to hold the configurations loaded from the profiles directory"))
	TArray<FJoystickInputDeviceConfiguration> ProfileConfigurations;

	UPROPERTY(config, EditAnywhere, Category="Joystick Settings", meta=(ToolTip="Contains Device specific configurations"))
	TArray<FJoystickInputDeviceConfiguration> DeviceConfigurations;

	UPROPERTY(config, EditAnywhere, Category="Joystick Settings")
	TMap<FKey, FJoystickInputKeyConfiguration> KeyConfigurations;

	UJoystickInputSettings();

	void AddDeviceConfiguration(const FJoystickInputDeviceConfiguration& InDeviceConfiguration);
	void AddProfileConfiguration(const FJoystickInputDeviceConfiguration& InDeviceConfiguration);
	void DeviceAdded(const FJoystickInformation& JoystickInfo);
	void DeviceRemoved(const FJoystickInstanceId& InstanceId);
	void ResetDevices();

	bool GetIgnoreGameControllers() const;
	bool SetIgnoreGameControllers(const bool NewIgnoreGameControllers);

	FJoystickInputDeviceConfiguration* GetDeviceConfiguration(const FJoystickInformation& Device);
	FJoystickInputDeviceConfiguration* GetProfileConfiguration(const FJoystickInformation& Device);

#if WITH_EDITOR
	virtual void PostEditChangeChainProperty(FPropertyChangedChainEvent& PropertyChangedEvent) override;
#endif

private:
	FJoystickInputDeviceConfiguration* FindConfiguration(TArray<FJoystickInputDeviceConfiguration>& ConfigurationArray, const FJoystickInformation& Device, bool IncludeEmptyGuids = false) const;
	FJoystickInputDeviceConfiguration* FindConfiguration(TArray<FJoystickInputDeviceConfiguration>& ConfigurationArray, const FJoystickInputDeviceConfiguration& Device, bool IncludeEmptyGuids = false) const;
};
