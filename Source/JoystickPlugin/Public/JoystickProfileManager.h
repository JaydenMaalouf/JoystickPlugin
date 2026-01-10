// JoystickPlugin is licensed under the MIT License.
// Copyright Jayden Maalouf 2026. All Rights Reserved.

#pragma once
#include "Data/JoystickInformation.h"
#include "Data/Settings/JoystickInputDeviceAxisProperties.h"
#include "Data/Settings/JoystickInputDeviceConfiguration.h"

#include "JoystickProfileManager.generated.h"

class UJoystickSubsystem;
class FJoystickInputDevice;

UCLASS(BlueprintType, DisplayName="Joystick HID Manager")
class UJoystickProfileManager : public UObject
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintPure, Category="Joystick|Profiles")
	static const UJoystickProfileManager* GetJoystickProfileManager();

	UFUNCTION(BlueprintCallable, Category="Joystick|Profiles")
	bool UpdateAxisConfiguration(const FKey& AxisKey, FJoystickInputDeviceAxisProperties Properties);

	UFUNCTION(BlueprintCallable, Category="Joystick|Profiles")
	bool GetAxisConfiguration(const FKey& AxisKey, FJoystickInputDeviceAxisProperties& Properties);

	UFUNCTION(BlueprintCallable, Category="Joystick|Profiles")
	bool UpdateButtonConfiguration(const FKey& ButtonKey, FJoystickInputDeviceButtonProperties Properties);

	UFUNCTION(BlueprintCallable, Category="Joystick|Profiles")
	bool GetButtonConfiguration(const FKey& AxisKey, FJoystickInputDeviceButtonProperties& Properties);

	void LoadJoystickProfiles();
	void RebuildDeviceConfigurations();

	FJoystickInputDeviceConfiguration* GetInputDeviceConfiguration(const FJoystickInformation& Device);
	FJoystickInputDeviceConfiguration* GetInputDeviceConfigurationByKey(const FKey& Key);
	const FJoystickInputDeviceAxisProperties* GetAxisPropertiesByKey(const FKey& AxisKey);
	const FJoystickInputDeviceButtonProperties* GetButtonPropertiesByKey(const FKey& AxisKey);

private:
	UJoystickSubsystem* GetJoystickSubsystem() const;
	bool GetDeviceInfoByKey(const FKey& Key, FJoystickInformation& OutJoystickInformation) const;

	bool AsBoolean(const FString& Input) const;
	float AsFloat(const FString& Input) const;
	int32 AsInteger(const FString& Input) const;
	bool IsBlueprintReadWrite(const FProperty* Property) const;
	void WriteChangedStructPropsToIni(const UScriptStruct* StructType, const void* CurrentValue, const FString& Section, const FString& IniFile) const;

	//TODO: Expose as an editor at some point
	void CreateJoystickProfile(const FKey& AxisKey, FJoystickInputDeviceAxisProperties Properties);

	TMap<FString, FJoystickInputDeviceConfiguration> InputDeviceConfigurations;

	static FString ProfilesRootDirectory;
	static FString JoystickConfigurationSection;
	static FString AxisPropertiesSection;
	static FString ButtonPropertiesSection;
};
