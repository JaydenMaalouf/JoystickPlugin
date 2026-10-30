// JoystickPlugin is licensed under the MIT License.
// Copyright Jayden Maalouf 2026. All Rights Reserved.

#include "Managers/JoystickProfileManager.h"

#include "Algo/AllOf.h"
#include "Engine/Engine.h"
#include "HAL/FileManager.h"
#include "Misc/ConfigCacheIni.h"
#include "Misc/Paths.h"
#include "JoystickInputDevice.h"
#include "JoystickInputSettings.h"
#include "Managers/JoystickLogManager.h"
#include "JoystickPluginModule.h"
#include "JoystickSubsystem.h"
#include "Data/Settings/JoystickInputDeviceConfiguration.h"

#define LOCTEXT_NAMESPACE "JoystickPlugin"

const UJoystickProfileManager* UJoystickProfileManager::GetJoystickProfileManager()
{
	return GetDefault<UJoystickProfileManager>();
}

bool UJoystickProfileManager::UpdateAxisConfiguration(const FKey& AxisKey, FJoystickInputDeviceAxisProperties Properties)
{
	return UpdateAxisConfigurationByKey(AxisKey, Properties, false);
}

bool UJoystickProfileManager::GetAxisConfiguration(const FKey& AxisKey, FJoystickInputDeviceAxisProperties& Properties)
{
	if (const FJoystickInputDeviceAxisProperties* AxisProperties = GetAxisPropertiesByKey(AxisKey))
	{
		Properties = *AxisProperties;
		return true;
	}

	return false;
}

bool UJoystickProfileManager::UpdateButtonConfiguration(const FKey& ButtonKey, FJoystickInputDeviceButtonProperties Properties)
{
	return UpdateButtonConfigurationByKey(ButtonKey, Properties, false);
}

bool UJoystickProfileManager::GetButtonConfiguration(const FKey& AxisKey, FJoystickInputDeviceButtonProperties& Properties)
{
	if (const FJoystickInputDeviceButtonProperties* ButtonProperties = GetButtonPropertiesByKey(AxisKey))
	{
		Properties = *ButtonProperties;
		return true;
	}

	return false;
}

bool UJoystickProfileManager::ApplyAxisProperties(const FJoystickInformation& Device, const FJoystickInputDeviceAxisProperties& Properties, const bool AllowProfileEdits)
{
	const int AxisIndex = Properties.AxisIndex;
	return ApplyConfigurationUpdate(Device, AllowProfileEdits, [AxisIndex](FJoystickInputDeviceConfiguration& Configuration)
	                                {
		                                return Configuration.GetAxisProperties(AxisIndex) != nullptr;
	                                }, [&Properties](FJoystickInputDeviceConfiguration& Configuration)
	                                {
		                                Configuration.AddOrUpdateAxisProperties(Properties);
	                                });
}

bool UJoystickProfileManager::ApplyButtonProperties(const FJoystickInformation& Device, const FJoystickInputDeviceButtonProperties& Properties, const bool AllowProfileEdits)
{
	const int ButtonIndex = Properties.ButtonIndex;
	return ApplyConfigurationUpdate(Device, AllowProfileEdits, [ButtonIndex](FJoystickInputDeviceConfiguration& Configuration)
	                                {
		                                return Configuration.GetButtonProperties(ButtonIndex) != nullptr;
	                                }, [&Properties](FJoystickInputDeviceConfiguration& Configuration)
	                                {
		                                Configuration.AddOrUpdateButtonProperties(Properties);
	                                });
}

bool UJoystickProfileManager::UpdateAxisConfigurationByKey(const FKey& AxisKey, FJoystickInputDeviceAxisProperties Properties, const bool AllowProfileEdits)
{
	if (!AxisKey.IsValid() || !(AxisKey.IsAxis1D() || AxisKey.IsAxis2D() || AxisKey.IsAxis3D()))
	{
		FJoystickLogManager::Get()->LogDebug(TEXT("Attempted to update an invalid or non-Axis key axis configuration"));
		return false;
	}

	FJoystickInformation JoystickInformation;
	if (!GetDeviceInfoByKey(AxisKey, JoystickInformation))
	{
		FJoystickLogManager::Get()->LogDebug(TEXT("Failed to retrieve joystick information"));
		return false;
	}

	const UJoystickSubsystem* JoystickSubsystem = GetJoystickSubsystem();
	if (!IsValid(JoystickSubsystem))
	{
		return false;
	}

	const FJoystickInputDevice* InputDevice = JoystickSubsystem->GetInputDevice();
	if (InputDevice == nullptr)
	{
		return false;
	}

	const int AxisIndex = InputDevice->GetAxisIndexFromKey(AxisKey);
	if (AxisIndex == INDEX_NONE)
	{
		return false;
	}

	Properties.AxisIndex = AxisIndex;
	return ApplyAxisProperties(JoystickInformation, Properties, AllowProfileEdits);
}

bool UJoystickProfileManager::UpdateButtonConfigurationByKey(const FKey& ButtonKey, FJoystickInputDeviceButtonProperties Properties, const bool AllowProfileEdits)
{
	if (!ButtonKey.IsValid() || ButtonKey.IsAxis1D() || ButtonKey.IsAxis2D() || ButtonKey.IsAxis3D())
	{
		FJoystickLogManager::Get()->LogDebug(TEXT("Attempted to update an invalid key button configuration"));
		return false;
	}

	FJoystickInformation JoystickInformation;
	if (!GetDeviceInfoByKey(ButtonKey, JoystickInformation))
	{
		return false;
	}

	const UJoystickSubsystem* JoystickSubsystem = GetJoystickSubsystem();
	if (!IsValid(JoystickSubsystem))
	{
		return false;
	}

	const FJoystickInputDevice* InputDevice = JoystickSubsystem->GetInputDevice();
	if (InputDevice == nullptr)
	{
		return false;
	}

	const int ButtonIndex = InputDevice->GetButtonIndexFromKey(ButtonKey);
	if (ButtonIndex == INDEX_NONE)
	{
		return false;
	}

	Properties.ButtonIndex = ButtonIndex;
	return ApplyButtonProperties(JoystickInformation, Properties, AllowProfileEdits);
}

#if WITH_EDITOR
bool UJoystickProfileManager::UpdateAxisProfileConfiguration(const FKey& AxisKey, const FJoystickInputDeviceAxisProperties& Properties)
{
	return UpdateAxisConfigurationByKey(AxisKey, Properties, true);
}

bool UJoystickProfileManager::UpdateButtonProfileConfiguration(const FKey& ButtonKey, const FJoystickInputDeviceButtonProperties& Properties)
{
	return UpdateButtonConfigurationByKey(ButtonKey, Properties, true);
}
#endif

FJoystickInputDeviceConfiguration* UJoystickProfileManager::FindOwningConfiguration(const FJoystickInformation& Device, const bool AllowProfileEdits, const TFunctionRef<bool(FJoystickInputDeviceConfiguration&)>& HasProperty) const
{
	UJoystickInputSettings* JoystickInputSettings = GetMutableDefault<UJoystickInputSettings>();
	if (!IsValid(JoystickInputSettings))
	{
		return nullptr;
	}

	FJoystickInputDeviceConfiguration* DeviceConfiguration = JoystickInputSettings->GetDeviceConfiguration(Device.DeviceHash);
	if (AllowProfileEdits == false)
	{
		return DeviceConfiguration;
	}

	if (DeviceConfiguration != nullptr && HasProperty(*DeviceConfiguration))
	{
		return DeviceConfiguration;
	}

	FJoystickInputDeviceConfiguration* ProfileConfiguration = JoystickInputSettings->GetProfileConfiguration(Device.DeviceHash);
	if (ProfileConfiguration == nullptr)
	{
		return DeviceConfiguration;
	}

	return ProfileConfiguration;
}

void UJoystickProfileManager::LoadJoystickProfiles()
{
	UJoystickInputSettings* JoystickInputSettings = GetMutableDefault<UJoystickInputSettings>();
	if (!IsValid(JoystickInputSettings))
	{
		return;
	}

	const FString ProfilesDirectory = GetProfilesDirectory();
	FJoystickLogManager::Get()->LogDebug(TEXT("Enumerating profiles in: %s"), *ProfilesDirectory);

	TArray<FString> Files;
	IFileManager::Get().FindFiles(Files, *ProfilesDirectory, TEXT("*.ini"));
	for (const FString& FileName : Files)
	{
		FJoystickInputDeviceConfiguration DeviceConfiguration;
		if (ReadProfileConfiguration(FileName, DeviceConfiguration) == false)
		{
			continue;
		}

		const FString ProfileName = FPaths::GetBaseFilename(FileName);
		if (JoystickInputSettings->AddProfileConfiguration(DeviceConfiguration) == false)
		{
			const FJoystickInputDeviceConfiguration* ExistingProfile = JoystickInputSettings->GetProfileConfiguration(DeviceConfiguration.DeviceHash);
			FJoystickLogManager::Get()->LogWarning(TEXT("Skipping joystick profile %s: device %s already has a profile in %s"), *ProfileName, *DeviceConfiguration.DeviceHash, *ExistingProfile->ProfileFileName);
			continue;
		}

		FJoystickLogManager::Get()->LogInformation(TEXT("Added joystick %s configuration from profile: %s"), *DeviceConfiguration.DeviceHash, *ProfileName);
	}

	RebuildInputDeviceConfigurations();
}

void UJoystickProfileManager::ReloadJoystickProfiles()
{
	UJoystickInputSettings* JoystickInputSettings = GetMutableDefault<UJoystickInputSettings>();
	if (!IsValid(JoystickInputSettings))
	{
		return;
	}

	JoystickInputSettings->ClearProfiles();
	LoadJoystickProfiles();
}

void UJoystickProfileManager::RebuildInputDeviceConfigurations()
{
	ComputeInputDeviceConfigurations();

	const UJoystickSubsystem* JoystickSubsystem = GetJoystickSubsystem();
	if (!IsValid(JoystickSubsystem))
	{
		return;
	}

	FJoystickInputDevice* InputDevice = JoystickSubsystem->GetInputDevice();
	if (InputDevice == nullptr)
	{
		return;
	}

	InputDevice->UpdateDeviceProperties();
}

FString UJoystickProfileManager::GetProfileFilePath(const FString& ProfileFileName) const
{
	return FPaths::Combine(GetProfilesDirectory(), ProfileFileName);
}

FString UJoystickProfileManager::GetProfileFileName(const FString& ProfileName) const
{
	return FString::Printf(TEXT("%s.ini"), *ProfileName.Replace(TEXT(" "), TEXT("")));
}

bool UJoystickProfileManager::CreateJoystickProfile(const FString& ProfileFileName, const FJoystickInputDeviceConfiguration& DeviceConfiguration)
{
	FJoystickInputDeviceConfiguration ProfileConfiguration = DeviceConfiguration;
	ProfileConfiguration.SortProperties();
	return SaveProfile(ProfileConfiguration, ProfileFileName);
}

FJoystickInputDeviceConfiguration* UJoystickProfileManager::GetInputDeviceConfiguration(const FJoystickInformation& Device)
{
	return InputDeviceConfigurations.Find(Device.DeviceHash);
}

FJoystickInputDeviceConfiguration* UJoystickProfileManager::GetInputDeviceConfigurationByKey(const FKey& Key)
{
	FJoystickInformation DeviceInfo;
	if (!GetDeviceInfoByKey(Key, DeviceInfo))
	{
		return nullptr;
	}

	return GetInputDeviceConfiguration(DeviceInfo);
}

const FJoystickInputDeviceAxisProperties* UJoystickProfileManager::GetAxisPropertiesByKey(const FKey& AxisKey)
{
	const UJoystickSubsystem* JoystickSubsystem = GetJoystickSubsystem();
	if (!IsValid(JoystickSubsystem))
	{
		return nullptr;
	}

	const FJoystickInputDevice* InputDevice = JoystickSubsystem->GetInputDevice();
	if (InputDevice == nullptr)
	{
		return nullptr;
	}

	const int AxisIndex = InputDevice->GetAxisIndexFromKey(AxisKey);
	if (AxisIndex == INDEX_NONE)
	{
		return nullptr;
	}

	FJoystickInputDeviceConfiguration* DeviceConfiguration = GetInputDeviceConfigurationByKey(AxisKey);
	if (DeviceConfiguration == nullptr)
	{
		return nullptr;
	}

	return DeviceConfiguration->GetAxisProperties(AxisIndex);
}

const FJoystickInputDeviceButtonProperties* UJoystickProfileManager::GetButtonPropertiesByKey(const FKey& AxisKey)
{
	const UJoystickSubsystem* JoystickSubsystem = GetJoystickSubsystem();
	if (!IsValid(JoystickSubsystem))
	{
		return nullptr;
	}

	const FJoystickInputDevice* InputDevice = JoystickSubsystem->GetInputDevice();
	if (InputDevice == nullptr)
	{
		return nullptr;
	}

	const int ButtonIndex = InputDevice->GetButtonIndexFromKey(AxisKey);
	if (ButtonIndex == INDEX_NONE)
	{
		return nullptr;
	}

	FJoystickInputDeviceConfiguration* DeviceConfiguration = GetInputDeviceConfigurationByKey(AxisKey);
	if (DeviceConfiguration == nullptr)
	{
		return nullptr;
	}

	return DeviceConfiguration->GetButtonProperties(ButtonIndex);
}

UJoystickSubsystem* UJoystickProfileManager::GetJoystickSubsystem() const
{
	if (!IsValid(GEngine))
	{
		return nullptr;
	}

	return GEngine->GetEngineSubsystem<UJoystickSubsystem>();
}

bool UJoystickProfileManager::GetDeviceInfoByKey(const FKey& Key, FJoystickInformation& OutJoystickInformation) const
{
	UJoystickSubsystem* JoystickSubsystem = GetJoystickSubsystem();
	if (!IsValid(JoystickSubsystem))
	{
		return false;
	}

	FJoystickInstanceId InstanceId;
	if (!JoystickSubsystem->GetInstanceIdFromKey(Key, InstanceId))
	{
		return false;
	}

	return JoystickSubsystem->GetJoystickInfo(InstanceId, OutJoystickInformation);
}

bool UJoystickProfileManager::ApplyConfigurationUpdate(const FJoystickInformation& Device, const bool AllowProfileEdits, const TFunctionRef<bool(FJoystickInputDeviceConfiguration&)>& HasProperty,
                                                       const TFunctionRef<void(FJoystickInputDeviceConfiguration&)>& ApplyProperties)
{
	UJoystickInputSettings* JoystickInputSettings = GetMutableDefault<UJoystickInputSettings>();
	if (!IsValid(JoystickInputSettings))
	{
		return false;
	}

	FJoystickInputDeviceConfiguration* OwningConfiguration = FindOwningConfiguration(Device, AllowProfileEdits, HasProperty);
	if (OwningConfiguration == nullptr)
	{
		FJoystickInputDeviceConfiguration DeviceConfiguration(Device.DeviceHash);
		ApplyProperties(DeviceConfiguration);
		return JoystickInputSettings->AddDeviceConfiguration(DeviceConfiguration);
	}

	if (OwningConfiguration->IsProfile())
	{
		FJoystickInputDeviceConfiguration ProfileConfiguration = *OwningConfiguration;
		ApplyProperties(ProfileConfiguration);
		return SaveProfile(ProfileConfiguration, ProfileConfiguration.ProfileFileName);
	}

	ApplyProperties(*OwningConfiguration);
	return JoystickInputSettings->SaveDeviceConfigurations();
}

void UJoystickProfileManager::ComputeInputDeviceConfigurations()
{
	InputDeviceConfigurations.Empty();

	const UJoystickInputSettings* JoystickInputSettings = GetDefault<UJoystickInputSettings>();
	if (!IsValid(JoystickInputSettings))
	{
		return;
	}

	for (const FJoystickInputDeviceConfiguration& ProfileConfiguration : JoystickInputSettings->ProfileConfigurations)
	{
		if (ProfileConfiguration.DeviceHash.IsEmpty())
		{
			continue;
		}

		InputDeviceConfigurations.Emplace(ProfileConfiguration.DeviceHash, ProfileConfiguration);
	}

	for (const FJoystickInputDeviceConfiguration& DeviceConfiguration : JoystickInputSettings->DeviceConfigurations)
	{
		if (DeviceConfiguration.DeviceHash.IsEmpty())
		{
			continue;
		}

		if (FJoystickInputDeviceConfiguration* ExistingConfiguration = InputDeviceConfigurations.Find(DeviceConfiguration.DeviceHash))
		{
			ExistingConfiguration->Merge(DeviceConfiguration);
			continue;
		}

		InputDeviceConfigurations.Emplace(DeviceConfiguration.DeviceHash, DeviceConfiguration);
	}
}

FString UJoystickProfileManager::GetProfilesDirectory() const
{
	return FPaths::Combine(FJoystickPluginModule::PluginDirectory, ProfilesRootDirectory);
}

bool UJoystickProfileManager::ReadProfile(const FString& ProfileFilePath, FConfigFile& OutProfileConfigFile) const
{
	if (IFileManager::Get().FileExists(*ProfileFilePath) == false)
	{
		FJoystickLogManager::Get()->LogWarning(TEXT("Joystick profile no longer exists: %s"), *ProfileFilePath);
		return false;
	}

	OutProfileConfigFile.Read(ProfileFilePath);
	return true;
}

bool UJoystickProfileManager::ReadProfileConfiguration(const FString& ProfileFileName, FJoystickInputDeviceConfiguration& OutDeviceConfiguration) const
{
	FConfigFile ConfigFile;
	if (ReadProfile(GetProfileFilePath(ProfileFileName), ConfigFile) == false)
	{
		return false;
	}

	const FString ProfileName = FPaths::GetBaseFilename(ProfileFileName);
	FJoystickLogManager::Get()->LogDebug(TEXT("Loading joystick profile: %s"), *ProfileName);

	OutDeviceConfiguration = FJoystickInputDeviceConfiguration();
	OutDeviceConfiguration.ProfileFileName = ProfileFileName;
	for (const TTuple<FString, FConfigSection>& ConfigSection : AsConst(ConfigFile))
	{
		if (ConfigSection.Key.Equals(JoystickConfigurationSection, ESearchCase::IgnoreCase))
		{
			ReadJoystickSection(ConfigSection.Value, OutDeviceConfiguration);
		}
		else if (ConfigSection.Key.StartsWith(AxisPropertiesSection))
		{
			OutDeviceConfiguration.AddOrUpdateAxisProperties(ReadAxisSection(ConfigSection.Key, ConfigSection.Value));
		}
		else if (ConfigSection.Key.StartsWith(ButtonPropertiesSection))
		{
			OutDeviceConfiguration.AddOrUpdateButtonProperties(ReadButtonSection(ConfigSection.Key, ConfigSection.Value));
		}
		else if (ConfigSection.Key.Equals(ForceFeedbackPropertiesSection, ESearchCase::IgnoreCase))
		{
			OutDeviceConfiguration.OverrideForceFeedbackProperties = true;
			ReadForceFeedbackSection(ConfigSection.Value, OutDeviceConfiguration.ForceFeedbackProperties);
		}
	}

	if (OutDeviceConfiguration.HasDeviceIdentifier() == false)
	{
		FJoystickLogManager::Get()->LogWarning(TEXT("Skipping joystick profile without a device hash: %s"), *ProfileName);
		return false;
	}

	return true;
}

void UJoystickProfileManager::ReadJoystickSection(const FConfigSection& Section, FJoystickInputDeviceConfiguration& DeviceConfiguration) const
{
	for (const TTuple<FName, FConfigValue>& Pair : Section)
	{
		const FName Key = Pair.Key;
		const FString ValueString = AsString(Pair.Value);

		if (Key == GET_MEMBER_NAME_CHECKED(FJoystickInputDeviceConfiguration, DeviceHash))
		{
			DeviceConfiguration.DeviceHash = ValueString;
		}
		else if (Key == GET_MEMBER_NAME_CHECKED(FJoystickInputDeviceConfiguration, DeviceName))
		{
			DeviceConfiguration.OverrideDeviceName = true;
			DeviceConfiguration.DeviceName = ValueString;
		}
		else if (Key == GET_MEMBER_NAME_CHECKED(FJoystickInputDeviceConfiguration, CollectionIndex))
		{
			DeviceConfiguration.LimitCollectionIndex = true;
			DeviceConfiguration.CollectionIndex = AsInteger(ValueString);
		}
	}
}

FJoystickInputDeviceAxisProperties UJoystickProfileManager::ReadAxisSection(const FString& SectionName, const FConfigSection& Section) const
{
	FJoystickInputDeviceAxisProperties AxisProperties;
	AxisProperties.AxisIndex = GetSectionIndex(SectionName, AxisPropertiesSection);

	for (const TTuple<FName, FConfigValue>& Pair : Section)
	{
		const FName Key = Pair.Key;
		const FString ValueString = AsString(Pair.Value);

		if (Key == GET_MEMBER_NAME_CHECKED(FJoystickInputDeviceAxisProperties, DisplayName))
		{
			AxisProperties.OverrideDisplayName = true;
			AxisProperties.DisplayName = ValueString;
		}
		else if (Key == GET_MEMBER_NAME_CHECKED(FJoystickInputDeviceAxisProperties, RemappingEnabled))
		{
			AxisProperties.RemappingEnabled = AsBoolean(ValueString);
		}
		else if (Key == GET_MEMBER_NAME_CHECKED(FJoystickInputDeviceAxisProperties, InputOffset))
		{
			AxisProperties.RemappingEnabled = true;
			AxisProperties.InputOffset = AsFloat(ValueString);
		}
		else if (Key == GET_MEMBER_NAME_CHECKED(FJoystickInputDeviceAxisProperties, InvertInput))
		{
			AxisProperties.RemappingEnabled = true;
			AxisProperties.InvertInput = AsBoolean(ValueString);
		}
		else if (Key == GET_MEMBER_NAME_CHECKED(FJoystickInputDeviceAxisProperties, RerangeInput))
		{
			AxisProperties.RemappingEnabled = true;
			AxisProperties.RerangeInput = AsBoolean(ValueString);
		}
		else if (Key == GET_MEMBER_NAME_CHECKED(FJoystickInputDeviceAxisProperties, RerangeOutput))
		{
			AxisProperties.RemappingEnabled = true;
			AxisProperties.RerangeOutput = AsBoolean(ValueString);
		}
		else if (Key == GET_MEMBER_NAME_CHECKED(FJoystickInputDeviceAxisProperties, InputRangeMin))
		{
			AxisProperties.RemappingEnabled = true;
			AxisProperties.RerangeInput = true;
			AxisProperties.InputRangeMin = AsFloat(ValueString);
		}
		else if (Key == GET_MEMBER_NAME_CHECKED(FJoystickInputDeviceAxisProperties, InputRangeMax))
		{
			AxisProperties.RemappingEnabled = true;
			AxisProperties.RerangeInput = true;
			AxisProperties.InputRangeMax = AsFloat(ValueString);
		}
		else if (Key == GET_MEMBER_NAME_CHECKED(FJoystickInputDeviceAxisProperties, OutputRangeMin))
		{
			AxisProperties.RemappingEnabled = true;
			AxisProperties.RerangeOutput = true;
			AxisProperties.OutputRangeMin = AsFloat(ValueString);
		}
		else if (Key == GET_MEMBER_NAME_CHECKED(FJoystickInputDeviceAxisProperties, OutputRangeMax))
		{
			AxisProperties.RemappingEnabled = true;
			AxisProperties.RerangeOutput = true;
			AxisProperties.OutputRangeMax = AsFloat(ValueString);
		}
		else if (Key == GET_MEMBER_NAME_CHECKED(FJoystickInputDeviceAxisProperties, InvertOutput))
		{
			AxisProperties.RemappingEnabled = true;
			AxisProperties.InvertOutput = AsBoolean(ValueString);
		}
	}

	return AxisProperties;
}

FJoystickInputDeviceButtonProperties UJoystickProfileManager::ReadButtonSection(const FString& SectionName, const FConfigSection& Section) const
{
	FJoystickInputDeviceButtonProperties ButtonProperties;
	ButtonProperties.ButtonIndex = GetSectionIndex(SectionName, ButtonPropertiesSection);

	for (const TTuple<FName, FConfigValue>& Pair : Section)
	{
		const FName Key = Pair.Key;
		const FString ValueString = AsString(Pair.Value);

		if (Key == GET_MEMBER_NAME_CHECKED(FJoystickInputDeviceButtonProperties, DisplayName))
		{
			ButtonProperties.OverrideDisplayName = true;
			ButtonProperties.DisplayName = ValueString;
		}
		else if (Key == GET_MEMBER_NAME_CHECKED(FJoystickInputDeviceButtonProperties, InvertOutput))
		{
			ButtonProperties.InvertOutput = AsBoolean(ValueString);
		}
	}

	return ButtonProperties;
}

void UJoystickProfileManager::ReadForceFeedbackSection(const FConfigSection& Section, FJoystickInputDeviceForceFeedbackProperties& ForceFeedbackProperties) const
{
	for (const TTuple<FName, FConfigValue>& Pair : Section)
	{
		const FName Key = Pair.Key;
		const FString ValueString = AsString(Pair.Value);

		if (Key == GET_MEMBER_NAME_CHECKED(FJoystickInputDeviceForceFeedbackProperties, InvertConditions))
		{
			ForceFeedbackProperties.InvertConditions = AsBoolean(ValueString);
		}
		else if (Key == GET_MEMBER_NAME_CHECKED(FJoystickInputDeviceForceFeedbackProperties, InvertSpring))
		{
			ForceFeedbackProperties.InvertSpring = AsBoolean(ValueString);
		}
		else if (Key == GET_MEMBER_NAME_CHECKED(FJoystickInputDeviceForceFeedbackProperties, InvertDamper))
		{
			ForceFeedbackProperties.InvertDamper = AsBoolean(ValueString);
		}
		else if (Key == GET_MEMBER_NAME_CHECKED(FJoystickInputDeviceForceFeedbackProperties, InvertInertia))
		{
			ForceFeedbackProperties.InvertInertia = AsBoolean(ValueString);
		}
		else if (Key == GET_MEMBER_NAME_CHECKED(FJoystickInputDeviceForceFeedbackProperties, InvertFriction))
		{
			ForceFeedbackProperties.InvertFriction = AsBoolean(ValueString);
		}
	}
}

int32 UJoystickProfileManager::GetSectionIndex(const FString& SectionName, const FString& SectionPrefix) const
{
	FString IndexString = SectionName;
	IndexString.RemoveFromStart(SectionPrefix, ESearchCase::IgnoreCase);
	IndexString.TrimStartAndEndInline();

	const bool IsValidIndex = IndexString.IsEmpty() == false && Algo::AllOf(IndexString, [](const TCHAR Character)
	{
		return FChar::IsDigit(Character);
	});

	if (IsValidIndex == false)
	{
		FJoystickLogManager::Get()->LogWarning(TEXT("Skipping profile section with an invalid index: %s"), *SectionName);
		return INDEX_NONE;
	}

	return AsInteger(IndexString);
}

FString UJoystickProfileManager::AsString(const FConfigValue& Value) const
{
	return Value.GetSavedValue().TrimStartAndEnd();
}

bool UJoystickProfileManager::AsBoolean(const FString& Input) const
{
	return Input.Equals(TEXT("true"), ESearchCase::IgnoreCase) || Input.Equals(TEXT("1"), ESearchCase::IgnoreCase);
}

float UJoystickProfileManager::AsFloat(const FString& Input) const
{
	return FCString::Atof(*Input);
}

int32 UJoystickProfileManager::AsInteger(const FString& Input) const
{
	return FCString::Atoi(*Input);
}

bool UJoystickProfileManager::SaveProfile(const FJoystickInputDeviceConfiguration& DeviceConfiguration, const FString& ProfileFileName)
{
	FConfigFile ProfileConfigFile;
	WriteConfigurationToProfile(ProfileConfigFile, DeviceConfiguration);
	ProfileConfigFile.Dirty = true;
	const FString ProfileFilePath = GetProfileFilePath(ProfileFileName);
	if (ProfileConfigFile.Write(ProfileFilePath) == false)
	{
		const FText Message = FText::Format(LOCTEXT("ProfileSaveFailed", "Failed to save joystick profile:\n{0}\n\nCheck the file isn't read-only or open in another program."), FText::FromString(ProfileFilePath));
		FJoystickLogManager::Get()->LogErrorWithDialog(Message, LOCTEXT("ProfileSaveFailedTitle", "Save Joystick Profile Failed"));
		return false;
	}

	UJoystickInputSettings* JoystickInputSettings = GetMutableDefault<UJoystickInputSettings>();
	if (!IsValid(JoystickInputSettings))
	{
		return false;
	}

	FJoystickInputDeviceConfiguration SavedConfiguration;
	if (ReadProfileConfiguration(ProfileFileName, SavedConfiguration) == false)
	{
		return false;
	}

	JoystickInputSettings->ReplaceProfileConfiguration(SavedConfiguration);
	RebuildInputDeviceConfigurations();
	return true;
}

void UJoystickProfileManager::WriteConfigurationToProfile(FConfigFile& ProfileConfigFile, const FJoystickInputDeviceConfiguration& DeviceConfiguration) const
{
	ProfileConfigFile.SetString(*JoystickConfigurationSection, GET_MEMBER_NAME_STRING_CHECKED(FJoystickInputDeviceConfiguration, DeviceHash), *DeviceConfiguration.DeviceHash);

	if (DeviceConfiguration.OverrideDeviceName)
	{
		ProfileConfigFile.SetString(*JoystickConfigurationSection, GET_MEMBER_NAME_STRING_CHECKED(FJoystickInputDeviceConfiguration, DeviceName), *DeviceConfiguration.DeviceName);
	}

	if (DeviceConfiguration.LimitCollectionIndex)
	{
		ProfileConfigFile.SetString(*JoystickConfigurationSection, GET_MEMBER_NAME_STRING_CHECKED(FJoystickInputDeviceConfiguration, CollectionIndex), *FString::FromInt(DeviceConfiguration.CollectionIndex));
	}

	for (const FJoystickInputDeviceAxisProperties& AxisProperties : DeviceConfiguration.AxisProperties)
	{
		WriteAxisPropertiesToProfile(ProfileConfigFile, AxisProperties);
	}
	for (const FJoystickInputDeviceButtonProperties& ButtonProperties : DeviceConfiguration.ButtonProperties)
	{
		WriteButtonPropertiesToProfile(ProfileConfigFile, ButtonProperties);
	}
	if (DeviceConfiguration.OverrideForceFeedbackProperties)
	{
		WriteChangedStructPropsToProfile(ProfileConfigFile, FJoystickInputDeviceForceFeedbackProperties::StaticStruct(), &DeviceConfiguration.ForceFeedbackProperties, ForceFeedbackPropertiesSection);
	}
}

void UJoystickProfileManager::WriteAxisPropertiesToProfile(FConfigFile& ProfileConfigFile, const FJoystickInputDeviceAxisProperties& AxisProperties) const
{
	const FString Section = FString::Printf(TEXT("%s%d"), *AxisPropertiesSection, AxisProperties.AxisIndex);

	if (AxisProperties.OverrideDisplayName)
	{
		ProfileConfigFile.SetString(*Section, GET_MEMBER_NAME_STRING_CHECKED(FJoystickInputDeviceAxisProperties, DisplayName), *AxisProperties.DisplayName);
	}

	if (AxisProperties.RemappingEnabled == false)
	{
		return;
	}

	WriteChangedAxisFloatToProfile(ProfileConfigFile, Section, AxisProperties, GET_MEMBER_NAME_CHECKED(FJoystickInputDeviceAxisProperties, InputOffset));

	if (AxisProperties.InvertInput)
	{
		ProfileConfigFile.SetBool(*Section, GET_MEMBER_NAME_STRING_CHECKED(FJoystickInputDeviceAxisProperties, InvertInput), true);
	}

	if (AxisProperties.RerangeInput)
	{
		WriteRerangeToProfile(ProfileConfigFile, Section, AxisProperties, GET_MEMBER_NAME_CHECKED(FJoystickInputDeviceAxisProperties, RerangeInput),
		                      GET_MEMBER_NAME_CHECKED(FJoystickInputDeviceAxisProperties, InputRangeMin), GET_MEMBER_NAME_CHECKED(FJoystickInputDeviceAxisProperties, InputRangeMax));
	}

	if (AxisProperties.RerangeOutput)
	{
		WriteRerangeToProfile(ProfileConfigFile, Section, AxisProperties, GET_MEMBER_NAME_CHECKED(FJoystickInputDeviceAxisProperties, RerangeOutput),
		                      GET_MEMBER_NAME_CHECKED(FJoystickInputDeviceAxisProperties, OutputRangeMin), GET_MEMBER_NAME_CHECKED(FJoystickInputDeviceAxisProperties, OutputRangeMax));
	}

	if (AxisProperties.InvertOutput)
	{
		ProfileConfigFile.SetBool(*Section, GET_MEMBER_NAME_STRING_CHECKED(FJoystickInputDeviceAxisProperties, InvertOutput), true);
	}
}

void UJoystickProfileManager::WriteRerangeToProfile(FConfigFile& ProfileConfigFile, const FString& Section, const FJoystickInputDeviceAxisProperties& AxisProperties, const FName RerangeName, const FName RangeMinName,
                                                    const FName RangeMaxName) const
{
	const bool WroteRangeMin = WriteChangedAxisFloatToProfile(ProfileConfigFile, Section, AxisProperties, RangeMinName);
	const bool WroteRangeMax = WriteChangedAxisFloatToProfile(ProfileConfigFile, Section, AxisProperties, RangeMaxName);
	if (WroteRangeMin || WroteRangeMax)
	{
		return;
	}

	ProfileConfigFile.SetBool(*Section, *RerangeName.ToString(), true);
}

bool UJoystickProfileManager::WriteChangedAxisFloatToProfile(FConfigFile& ProfileConfigFile, const FString& Section, const FJoystickInputDeviceAxisProperties& AxisProperties, const FName PropertyName) const
{
	const FFloatProperty* Property = CastField<FFloatProperty>(FJoystickInputDeviceAxisProperties::StaticStruct()->FindPropertyByName(PropertyName));
	if (Property == nullptr)
	{
		return false;
	}

	const FJoystickInputDeviceAxisProperties DefaultProperties;
	const float Value = Property->GetPropertyValue_InContainer(&AxisProperties);
	if (Value == Property->GetPropertyValue_InContainer(&DefaultProperties))
	{
		return false;
	}

	ProfileConfigFile.SetFloat(*Section, *Property->GetName(), Value);
	return true;
}

void UJoystickProfileManager::WriteButtonPropertiesToProfile(FConfigFile& ProfileConfigFile, const FJoystickInputDeviceButtonProperties& ButtonProperties) const
{
	const FString Section = FString::Printf(TEXT("%s%d"), *ButtonPropertiesSection, ButtonProperties.ButtonIndex);

	if (ButtonProperties.OverrideDisplayName)
	{
		ProfileConfigFile.SetString(*Section, GET_MEMBER_NAME_STRING_CHECKED(FJoystickInputDeviceButtonProperties, DisplayName), *ButtonProperties.DisplayName);
	}

	if (ButtonProperties.InvertOutput)
	{
		ProfileConfigFile.SetBool(*Section, GET_MEMBER_NAME_STRING_CHECKED(FJoystickInputDeviceButtonProperties, InvertOutput), true);
	}
}

bool UJoystickProfileManager::IsBlueprintReadWrite(const FProperty* Property) const
{
	return Property
		&& Property->HasAnyPropertyFlags(CPF_BlueprintVisible)
		&& !Property->HasAnyPropertyFlags(CPF_BlueprintReadOnly);
}

void UJoystickProfileManager::WriteChangedStructPropsToProfile(FConfigFile& ProfileConfigFile, const UScriptStruct* StructType, const void* CurrentValue, const FString& Section) const
{
	const int32 StructSize = StructType->GetStructureSize();
	uint8* DefaultValue = static_cast<uint8*>(FMemory_Alloca(StructSize));
	StructType->InitializeStruct(DefaultValue);

	check(StructType && CurrentValue && DefaultValue);

	for (TFieldIterator<FProperty> It(StructType); It; ++It)
	{
		const FProperty* Property = *It;
		if (!IsBlueprintReadWrite(Property))
		{
			continue;
		}

		const void* CurrentProperty = Property->ContainerPtrToValuePtr<void>(CurrentValue);
		const void* DefaultProperty = Property->ContainerPtrToValuePtr<void>(DefaultValue);

		if (Property->Identical(CurrentProperty, DefaultProperty))
		{
			continue;
		}

		FString ExportedValue;
		Property->ExportTextItem_Direct(ExportedValue, CurrentProperty, DefaultProperty, nullptr, PPF_None);
		ProfileConfigFile.SetString(*Section, *Property->GetName(), *ExportedValue);
	}
}

FString UJoystickProfileManager::ProfilesRootDirectory("Profiles");

FString UJoystickProfileManager::JoystickConfigurationSection("Joystick");

FString UJoystickProfileManager::AxisPropertiesSection("AxisProperties_");

FString UJoystickProfileManager::ButtonPropertiesSection("ButtonProperties_");

FString UJoystickProfileManager::ForceFeedbackPropertiesSection("ForceFeedbackProperties");

#undef LOCTEXT_NAMESPACE
