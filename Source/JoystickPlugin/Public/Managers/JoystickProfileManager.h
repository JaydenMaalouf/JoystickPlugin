// JoystickPlugin is licensed under the MIT License.
// Copyright Jayden Maalouf 2026. All Rights Reserved.

#pragma once
#include "Data/JoystickInformation.h"
#include "Data/Settings/JoystickInputDeviceAxisProperties.h"
#include "Data/Settings/JoystickInputDeviceConfiguration.h"

#include "JoystickProfileManager.generated.h"

class FConfigSection;
struct FConfigValue;
class UJoystickSubsystem;
class FJoystickInputDevice;

UCLASS(BlueprintType, DisplayName="Joystick Profile Manager")
class JOYSTICKPLUGIN_API UJoystickProfileManager : public UObject
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

#if WITH_EDITOR
	bool UpdateAxisProfileConfiguration(const FKey& AxisKey, const FJoystickInputDeviceAxisProperties& Properties);
	bool UpdateButtonProfileConfiguration(const FKey& ButtonKey, const FJoystickInputDeviceButtonProperties& Properties);
#endif

	bool ApplyAxisProperties(const FJoystickInformation& Device, const FJoystickInputDeviceAxisProperties& Properties, bool AllowProfileEdits);
	bool ApplyButtonProperties(const FJoystickInformation& Device, const FJoystickInputDeviceButtonProperties& Properties, bool AllowProfileEdits);
	FJoystickInputDeviceConfiguration* FindOwningConfiguration(const FJoystickInformation& Device, bool AllowProfileEdits, const TFunctionRef<bool(FJoystickInputDeviceConfiguration&)>& HasProperty) const;

	void LoadJoystickProfiles();
	void ReloadJoystickProfiles();
	void RebuildInputDeviceConfigurations();

	FString GetProfileFilePath(const FString& ProfileFileName) const;

	FString GetProfileFileName(const FString& ProfileName) const;
	bool CreateJoystickProfile(const FString& ProfileFileName, const FJoystickInputDeviceConfiguration& DeviceConfiguration);

	FJoystickInputDeviceConfiguration* GetInputDeviceConfiguration(const FJoystickInformation& Device);
	FJoystickInputDeviceConfiguration* GetInputDeviceConfigurationByKey(const FKey& Key);
	const FJoystickInputDeviceAxisProperties* GetAxisPropertiesByKey(const FKey& AxisKey);
	const FJoystickInputDeviceButtonProperties* GetButtonPropertiesByKey(const FKey& AxisKey);

private:
	UJoystickSubsystem* GetJoystickSubsystem() const;
	bool GetDeviceInfoByKey(const FKey& Key, FJoystickInformation& OutJoystickInformation) const;

	bool UpdateAxisConfigurationByKey(const FKey& AxisKey, FJoystickInputDeviceAxisProperties Properties, bool AllowProfileEdits);
	bool UpdateButtonConfigurationByKey(const FKey& ButtonKey, FJoystickInputDeviceButtonProperties Properties, bool AllowProfileEdits);
	bool ApplyConfigurationUpdate(const FJoystickInformation& Device, bool AllowProfileEdits, const TFunctionRef<bool(FJoystickInputDeviceConfiguration&)>& HasProperty,
	                              const TFunctionRef<void(FJoystickInputDeviceConfiguration&)>& ApplyProperties);

	void ComputeInputDeviceConfigurations();

	FString GetProfilesDirectory() const;
	bool ReadProfile(const FString& ProfileFilePath, FConfigFile& OutProfileConfigFile) const;
	bool ReadProfileConfiguration(const FString& ProfileFileName, FJoystickInputDeviceConfiguration& OutDeviceConfiguration) const;
	void ReadJoystickSection(const FConfigSection& Section, FJoystickInputDeviceConfiguration& DeviceConfiguration) const;
	FJoystickInputDeviceAxisProperties ReadAxisSection(const FString& SectionName, const FConfigSection& Section) const;
	FJoystickInputDeviceButtonProperties ReadButtonSection(const FString& SectionName, const FConfigSection& Section) const;
	void ReadForceFeedbackSection(const FConfigSection& Section, FJoystickInputDeviceForceFeedbackProperties& ForceFeedbackProperties) const;
	int32 GetSectionIndex(const FString& SectionName, const FString& SectionPrefix) const;

	bool SaveProfile(const FJoystickInputDeviceConfiguration& DeviceConfiguration, const FString& ProfileFileName);
	void WriteConfigurationToProfile(FConfigFile& ProfileConfigFile, const FJoystickInputDeviceConfiguration& DeviceConfiguration) const;
	void WriteAxisPropertiesToProfile(FConfigFile& ProfileConfigFile, const FJoystickInputDeviceAxisProperties& AxisProperties) const;
	void WriteRerangeToProfile(FConfigFile& ProfileConfigFile, const FString& Section, const FJoystickInputDeviceAxisProperties& AxisProperties, const FName RerangeName, const FName RangeMinName, const FName RangeMaxName) const;
	bool WriteChangedAxisFloatToProfile(FConfigFile& ProfileConfigFile, const FString& Section, const FJoystickInputDeviceAxisProperties& AxisProperties, const FName PropertyName) const;
	void WriteButtonPropertiesToProfile(FConfigFile& ProfileConfigFile, const FJoystickInputDeviceButtonProperties& ButtonProperties) const;
	bool IsBlueprintReadWrite(const FProperty* Property) const;
	void WriteChangedStructPropsToProfile(FConfigFile& ProfileConfigFile, const UScriptStruct* StructType, const void* CurrentValue, const FString& Section) const;

	FString AsString(const FConfigValue& Value) const;
	bool AsBoolean(const FString& Input) const;
	float AsFloat(const FString& Input) const;
	int32 AsInteger(const FString& Input) const;

	TMap<FString, FJoystickInputDeviceConfiguration> InputDeviceConfigurations;
	static FString ProfilesRootDirectory;
	static FString JoystickConfigurationSection;
	static FString AxisPropertiesSection;
	static FString ButtonPropertiesSection;
	static FString ForceFeedbackPropertiesSection;
};
