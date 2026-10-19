// JoystickPlugin is licensed under the MIT License.
// Copyright Jayden Maalouf 2026. All Rights Reserved.

#include "Managers/JoystickProfileManager.h"

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

const UJoystickProfileManager* UJoystickProfileManager::GetJoystickProfileManager()
{
	return GetDefault<UJoystickProfileManager>();
}

bool UJoystickProfileManager::UpdateAxisConfiguration(const FKey& AxisKey, FJoystickInputDeviceAxisProperties Properties)
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

	UJoystickSubsystem* JoystickSubsystem = GetJoystickSubsystem();
	if (!IsValid(JoystickSubsystem))
	{
		return false;
	}

	FJoystickInputDevice* InputDevice = JoystickSubsystem->GetInputDevice();
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

	UJoystickInputSettings* JoystickInputSettings = GetMutableDefault<UJoystickInputSettings>();
	if (!IsValid(JoystickInputSettings))
	{
		return false;
	}

	FJoystickInputDeviceConfiguration* InputDeviceConfiguration = JoystickInputSettings->GetDeviceConfiguration(JoystickInformation);
	if (!InputDeviceConfiguration)
	{
		FJoystickInputDeviceConfiguration DeviceConfiguration;
		DeviceConfiguration.DeviceHash = JoystickInformation.DeviceHash;
		DeviceConfiguration.AxisProperties.Add(Properties);
		JoystickInputSettings->AddDeviceConfiguration(DeviceConfiguration);
	}
	else
	{
		if (FJoystickInputDeviceAxisProperties* AxisProperties = InputDeviceConfiguration->GetAxisProperties(AxisIndex))
		{
			AxisProperties->UpdateProperties(Properties);
		}
		else
		{
			InputDeviceConfiguration->AxisProperties.Add(Properties);
		}
	}

	JoystickInputSettings->TryUpdateDefaultConfigFile();
	RebuildDeviceConfigurations();
	return true;
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

	UJoystickSubsystem* JoystickSubsystem = GetJoystickSubsystem();
	if (!IsValid(JoystickSubsystem))
	{
		return false;
	}

	FJoystickInputDevice* InputDevice = JoystickSubsystem->GetInputDevice();
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

	UJoystickInputSettings* JoystickInputSettings = GetMutableDefault<UJoystickInputSettings>();
	if (!IsValid(JoystickInputSettings))
	{
		return false;
	}

	FJoystickInputDeviceConfiguration* InputDeviceConfiguration = JoystickInputSettings->GetDeviceConfiguration(JoystickInformation);
	if (!InputDeviceConfiguration)
	{
		FJoystickInputDeviceConfiguration DeviceConfiguration;
		DeviceConfiguration.DeviceHash = JoystickInformation.DeviceHash;
		DeviceConfiguration.ButtonProperties.Add(Properties);
		JoystickInputSettings->AddDeviceConfiguration(DeviceConfiguration);
	}
	else
	{
		if (FJoystickInputDeviceButtonProperties* ButtonProperties = InputDeviceConfiguration->GetButtonProperties(ButtonIndex))
		{
			ButtonProperties->UpdateProperties(Properties);
		}
		else
		{
			InputDeviceConfiguration->ButtonProperties.Add(Properties);
		}
	}

	JoystickInputSettings->TryUpdateDefaultConfigFile();
	RebuildDeviceConfigurations();
	return true;
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

void UJoystickProfileManager::LoadJoystickProfiles()
{
	UJoystickInputSettings* JoystickInputSettings = GetMutableDefault<UJoystickInputSettings>();
	if (!IsValid(JoystickInputSettings))
	{
		return;
	}

	const FString ProfilesDirectory = FPaths::Combine(FJoystickPluginModule::PluginDirectory, ProfilesRootDirectory);
	FJoystickLogManager::Get()->LogDebug(TEXT("Enumerating profiles in: %s"), *ProfilesDirectory);

	IFileManager& FileManager = IFileManager::Get();
	if (FileManager.DirectoryExists(*ProfilesDirectory))
	{
		TArray<FString> Files;
		FileManager.FindFiles(Files, *ProfilesDirectory, TEXT("*.ini"));

		for (const FString& FileName : Files)
		{
			const FString FilePath = FPaths::Combine(ProfilesDirectory, FileName);
			if (!FileManager.FileExists(*FilePath))
			{
				continue;
			}

			const FString ProfileName = FileName.EndsWith(TEXT(".ini"), ESearchCase::IgnoreCase) ? FString(FileName.Left(FileName.Len() - 4)) : FileName;
			FJoystickLogManager::Get()->LogDebug(TEXT("Loading joystick profile: %s"), *ProfileName);

			FConfigFile ConfigFile;
			ConfigFile.Read(*FilePath);

			FJoystickInputDeviceConfiguration DeviceConfiguration;
			for (const TTuple<FString, FConfigSection>& ConfigSection : AsConst(ConfigFile))
			{
				if (ConfigSection.Key.Equals(JoystickConfigurationSection, ESearchCase::IgnoreCase))
				{
					for (const TTuple<FName, FConfigValue>& Pair : ConfigSection.Value)
					{
						const FName Key = Pair.Key;
						const FConfigValue& Value = Pair.Value;
						FString ValueString = Value.GetSavedValue().TrimStartAndEnd();

						if (Key == GET_MEMBER_NAME_CHECKED(FJoystickInputDeviceConfiguration, ProductGuid))
						{
							DeviceConfiguration.ProductGuid = FGuid(ValueString);
							DeviceConfiguration.DeviceIdentifyMethod = EJoystickIdentifierType::Legacy;
						}
						else if (Key == GET_MEMBER_NAME_CHECKED(FJoystickInputDeviceConfiguration, DeviceHash))
						{
							DeviceConfiguration.DeviceHash = ValueString;
							DeviceConfiguration.DeviceIdentifyMethod = EJoystickIdentifierType::Hashed;
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
				else if (ConfigSection.Key.StartsWith(AxisPropertiesSection))
				{
					FJoystickInputDeviceAxisProperties AxisProperties;
					FString AxisIndexString = ConfigSection.Key;
					AxisIndexString.RemoveFromStart(*AxisPropertiesSection, ESearchCase::IgnoreCase);
					AxisIndexString.TrimStartAndEndInline();
					AxisProperties.AxisIndex = AsInteger(AxisIndexString);

					for (const TTuple<FName, FConfigValue>& Pair : ConfigSection.Value)
					{
						const FName Key = Pair.Key;
						const FConfigValue& Value = Pair.Value;
						FString ValueString = Value.GetSavedValue().TrimStartAndEnd();

						if (Key == GET_MEMBER_NAME_CHECKED(FJoystickInputDeviceAxisProperties, DisplayName))
						{
							AxisProperties.OverrideDisplayName = true;
							AxisProperties.DisplayName = ValueString;
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

					DeviceConfiguration.AxisProperties.Add(AxisProperties);
				}
				else if (ConfigSection.Key.StartsWith(ButtonPropertiesSection))
				{
					FJoystickInputDeviceButtonProperties ButtonProperties;
					FString ButtonIndexString = ConfigSection.Key;
					ButtonIndexString.RemoveFromStart(*ButtonPropertiesSection, ESearchCase::IgnoreCase);
					ButtonIndexString.TrimStartAndEndInline();
					ButtonProperties.ButtonIndex = AsInteger(ButtonIndexString);

					for (const TTuple<FName, FConfigValue>& Pair : ConfigSection.Value)
					{
						const FName Key = Pair.Key;
						const FConfigValue& Value = Pair.Value;
						FString ValueString = Value.GetSavedValue().TrimStartAndEnd();

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

					DeviceConfiguration.ButtonProperties.Add(ButtonProperties);
				}
				else if (ConfigSection.Key.Equals(ForceFeedbackPropertiesSection, ESearchCase::IgnoreCase))
				{
					DeviceConfiguration.OverrideForceFeedbackProperties = true;
					FJoystickInputDeviceForceFeedbackProperties& ForceFeedbackProperties = DeviceConfiguration.ForceFeedbackProperties;

					for (const TTuple<FName, FConfigValue>& Pair : ConfigSection.Value)
					{
						const FName Key = Pair.Key;
						const FConfigValue& Value = Pair.Value;
						FString ValueString = Value.GetSavedValue().TrimStartAndEnd();

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
			}

			JoystickInputSettings->AddProfileConfiguration(DeviceConfiguration);
			if (DeviceConfiguration.DeviceIdentifyMethod == EJoystickIdentifierType::Legacy)
			{
				FJoystickLogManager::Get()->LogInformation(TEXT("Added joystick %s configuration from legacy profile: %s"), *DeviceConfiguration.ProductGuid.ToString(), *ProfileName);
			}
			else
			{
				FJoystickLogManager::Get()->LogInformation(TEXT("Added joystick %s configuration from profile: %s"), *DeviceConfiguration.DeviceHash, *ProfileName);
			}
		}
	}

	RebuildDeviceConfigurations();
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

void UJoystickProfileManager::RebuildDeviceConfigurations()
{
	const UJoystickSubsystem* JoystickSubsystem = GetJoystickSubsystem();
	if (!IsValid(JoystickSubsystem))
	{
		return;
	}

	const UJoystickInputSettings* JoystickInputSettings = GetDefault<UJoystickInputSettings>();
	if (!IsValid(JoystickInputSettings))
	{
		return;
	}

	InputDeviceConfigurations.Empty();

	TArray<FJoystickInputDeviceConfiguration> ProfileConfigurations = JoystickInputSettings->ProfileConfigurations;
	for (int i = 0; i < ProfileConfigurations.Num(); i++)
	{
		const FJoystickInputDeviceConfiguration& ProfileConfiguration = ProfileConfigurations[i];
		InputDeviceConfigurations.Emplace(ProfileConfiguration.DeviceHash, ProfileConfiguration);
	}

	TArray<FJoystickInputDeviceConfiguration> DeviceConfigurations = JoystickInputSettings->DeviceConfigurations;
	for (int i = 0; i < DeviceConfigurations.Num(); i++)
	{
		FJoystickInputDeviceConfiguration& DeviceConfiguration = DeviceConfigurations[i];
		if (FJoystickInputDeviceConfiguration* ExistingConfiguration = InputDeviceConfigurations.Find(DeviceConfiguration.DeviceHash))
		{
			ExistingConfiguration->Merge(DeviceConfiguration);
		}
		else
		{
			InputDeviceConfigurations.Emplace(DeviceConfiguration.DeviceHash, DeviceConfiguration);
		}
	}

	FJoystickInputDevice* InputDevice = JoystickSubsystem->GetInputDevice();
	if (InputDevice == nullptr)
	{
		return;
	}

	InputDevice->UpdateDeviceProperties();
}


bool UJoystickProfileManager::CreateJoystickProfile(const FString& DeviceProfileName, const FJoystickInputDeviceConfiguration& DeviceConfiguration)
{
	const FString ProfilesDirectory = FPaths::Combine(FJoystickPluginModule::PluginDirectory, ProfilesRootDirectory);
	const FString DeviceProfileFile = FString::Printf(TEXT("%s.ini"), *DeviceProfileName.Replace(TEXT(" "), TEXT("")));
	const FString DeviceProfile = FPaths::Combine(ProfilesDirectory, DeviceProfileFile);

	FConfigFile ProfileConfigFile;
	switch (DeviceConfiguration.DeviceIdentifyMethod)
	{
		case EJoystickIdentifierType::Legacy:
			ProfileConfigFile.SetString(*JoystickConfigurationSection, GET_MEMBER_NAME_STRING_CHECKED(FJoystickInputDeviceConfiguration, ProductGuid), *DeviceConfiguration.ProductGuid.ToString());
		case EJoystickIdentifierType::Hashed:
		default:
			ProfileConfigFile.SetString(*JoystickConfigurationSection, GET_MEMBER_NAME_STRING_CHECKED(FJoystickInputDeviceConfiguration, DeviceHash), *DeviceConfiguration.DeviceHash);
	}

	if (DeviceConfiguration.OverrideDeviceName)
	{
		ProfileConfigFile.SetString(*JoystickConfigurationSection, GET_MEMBER_NAME_STRING_CHECKED(FJoystickInputDeviceConfiguration, DeviceName), *DeviceConfiguration.DeviceName);
	}

	if (DeviceConfiguration.LimitCollectionIndex)
	{
		ProfileConfigFile.SetString(*JoystickConfigurationSection, GET_MEMBER_NAME_STRING_CHECKED(FJoystickInputDeviceConfiguration, CollectionIndex), *FString::FromInt(DeviceConfiguration.CollectionIndex));
	}

	for (int AxisPropertyIndex = 0; AxisPropertyIndex < DeviceConfiguration.AxisProperties.Num(); ++AxisPropertyIndex)
	{
		const FJoystickInputDeviceAxisProperties& AxisProperties = DeviceConfiguration.AxisProperties[AxisPropertyIndex];
		const FString AxisSection = FString::Printf(TEXT("%s%d"), *AxisPropertiesSection, AxisProperties.AxisIndex);
		WriteChangedStructPropsToProfile(ProfileConfigFile, FJoystickInputDeviceAxisProperties::StaticStruct(), &AxisProperties, AxisSection);
	}
	for (int ButtonPropertyIndex = 0; ButtonPropertyIndex < DeviceConfiguration.ButtonProperties.Num(); ++ButtonPropertyIndex)
	{
		const FJoystickInputDeviceButtonProperties& ButtonProperties = DeviceConfiguration.ButtonProperties[ButtonPropertyIndex];
		const FString ButtonSection = FString::Printf(TEXT("%s%d"), *ButtonPropertiesSection, ButtonProperties.ButtonIndex);
		WriteChangedStructPropsToProfile(ProfileConfigFile, FJoystickInputDeviceButtonProperties::StaticStruct(), &ButtonProperties, ButtonSection);
	}
	if (DeviceConfiguration.OverrideForceFeedbackProperties)
	{
		WriteChangedStructPropsToProfile(ProfileConfigFile, FJoystickInputDeviceForceFeedbackProperties::StaticStruct(), &DeviceConfiguration.ForceFeedbackProperties, ForceFeedbackPropertiesSection);
	}
	const bool Result = ProfileConfigFile.Write(DeviceProfile);
	if (Result)
	{
		ReloadJoystickProfiles();
	}

	return Result;
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

	const FJoystickInputDevice* InputDevice = JoystickSubsystem->GetInputDevice();
	if (InputDevice == nullptr)
	{
		return false;
	}

	const FJoystickInstanceId& InstanceId = InputDevice->GetInstanceIdByKey(Key);
	return JoystickSubsystem->GetJoystickInfo(InstanceId, OutJoystickInformation);
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
