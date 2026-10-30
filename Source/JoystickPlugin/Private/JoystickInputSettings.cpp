// JoystickPlugin is licensed under the MIT License.
// Copyright Jayden Maalouf 2026. All Rights Reserved.

#include "JoystickInputSettings.h"

#include "Data/JoystickInformation.h"
#include "Data/JoystickInstanceId.h"
#include "Data/Settings/JoystickInputDeviceConfiguration.h"
#include "Engine/Engine.h"
#include "Managers/JoystickLogManager.h"
#include "Managers/JoystickProfileManager.h"
#include "JoystickSubsystem.h"
#include "Misc/Paths.h"

#define LOCTEXT_NAMESPACE "JoystickPlugin"

UJoystickInputSettings::UJoystickInputSettings() : UseDeviceName(true),
                                                   IncludeDeviceIndex(false),
                                                   IgnoreGamepads(false),
                                                   AllowDuplicateHashedDevices(true),
                                                   ApplyEngineForceFeedbackToHapticDevices(false),
                                                   EngineRumbleFrequency(40),
                                                   PrioritiseCollectionIndex(false),
                                                   PrioritisationMethod(EJoystickPrioritisationType::Lowest),
                                                   UseDeviceNameAsHardwareDeviceIdentifier(false),
                                                   ZeroBasedIndexing(true),
                                                   EnableLogs(true),
                                                   EnablePairedKeys(false)
{
	MapHatAxisToKeys = static_cast<int32>(EHatDirection::Up | EHatDirection::Down | EHatDirection::Left | EHatDirection::Right);

#if WITH_EDITOR
	DebugAxis = false;
#endif
}

void UJoystickInputSettings::DeviceAdded(const FJoystickInformation& JoystickInfo)
{
	ConnectedDevices.Add(JoystickInfo);
}

void UJoystickInputSettings::DeviceRemoved(const FJoystickInstanceId& InstanceId)
{
	ConnectedDevices.RemoveAll([InstanceId](const FJoystickInformation& Item)
	{
		return Item.InstanceId == InstanceId;
	});
}

void UJoystickInputSettings::ResetDevices()
{
	ConnectedDevices.Empty();
}

FString UJoystickInputSettings::GetConnectedDeviceDisplayNameByConfiguration(const FJoystickInputDeviceConfiguration& DeviceConfiguration) const
{
	for (const FJoystickInformation& ConnectedDevice : ConnectedDevices)
	{
		if (ConnectedDevice.DeviceHash == DeviceConfiguration.DeviceHash)
		{
			return ConnectedDevice.GetDeviceDisplayName();
		}
	}

	return FString();
}

bool UJoystickInputSettings::GetIgnoreGamepads() const
{
	return IgnoreGamepads;
}

bool UJoystickInputSettings::SetIgnoreGamepads(const bool bIgnoreGamepads)
{
	const bool Changed = IgnoreGamepads != bIgnoreGamepads;
	IgnoreGamepads = bIgnoreGamepads;
	return Changed;
}

FJoystickInputDeviceConfiguration* UJoystickInputSettings::GetDeviceConfiguration(const FString& DeviceHash)
{
	return FindConfiguration(DeviceConfigurations, DeviceHash);
}

FJoystickInputDeviceConfiguration* UJoystickInputSettings::GetProfileConfiguration(const FString& DeviceHash)
{
	return FindConfiguration(ProfileConfigurations, DeviceHash);
}

FJoystickInputDeviceConfiguration* UJoystickInputSettings::GetProfileConfigurationByFileName(const FString& ProfileFileName)
{
	return ProfileConfigurations.FindByPredicate([&ProfileFileName](const FJoystickInputDeviceConfiguration& ProfileConfiguration)
	{
		return ProfileConfiguration.ProfileFileName.Equals(ProfileFileName, ESearchCase::IgnoreCase);
	});
}

#if WITH_EDITOR
void UJoystickInputSettings::PostEditChangeChainProperty(FPropertyChangedChainEvent& PropertyChangedEvent)
{
	Super::PostEditChangeChainProperty(PropertyChangedEvent);
	RebuildDeviceConfigurations();
}
#endif

bool UJoystickInputSettings::AddDeviceConfiguration(const FJoystickInputDeviceConfiguration& InDeviceConfiguration)
{
	return AddDeviceConfigurations({InDeviceConfiguration});
}

bool UJoystickInputSettings::AddDeviceConfigurations(const TArray<FJoystickInputDeviceConfiguration>& InDeviceConfigurations)
{
	bool AddedConfiguration = false;
	for (const FJoystickInputDeviceConfiguration& InDeviceConfiguration : InDeviceConfigurations)
	{
		if (ConfigurationExists(DeviceConfigurations, InDeviceConfiguration.DeviceHash))
		{
			continue;
		}

		DeviceConfigurations.Add(InDeviceConfiguration);
		AddedConfiguration = true;
	}

	if (AddedConfiguration == false)
	{
		return false;
	}

	return SaveDeviceConfigurations();
}

bool UJoystickInputSettings::RemoveDeviceConfiguration(const int DeviceConfigurationIndex)
{
	if (DeviceConfigurations.IsValidIndex(DeviceConfigurationIndex) == false)
	{
		return false;
	}

	DeviceConfigurations.RemoveAt(DeviceConfigurationIndex);
	return SaveDeviceConfigurations();
}

bool UJoystickInputSettings::SaveDeviceConfigurations()
{
	const bool Saved = TryUpdateDefaultConfigFile();
	RebuildDeviceConfigurations();
	if (Saved)
	{
		return true;
	}

	const FText Message = FText::Format(
		LOCTEXT("DeviceConfigurationSaveFailed", "Failed to save joystick device configurations:\n{0}\n\nCheck the file isn't read-only or open in another program."), FText::FromString(GetDefaultConfigFilename()));
	FJoystickLogManager::Get()->LogErrorWithDialog(Message, LOCTEXT("DeviceConfigurationSaveFailedTitle", "Save Joystick Device Configurations Failed"));
	return false;
}

bool UJoystickInputSettings::AddProfileConfiguration(const FJoystickInputDeviceConfiguration& InDeviceConfiguration)
{
	if (ConfigurationExists(ProfileConfigurations, InDeviceConfiguration.DeviceHash))
	{
		return false;
	}

	ProfileConfigurations.Add(InDeviceConfiguration);
	return true;
}

void UJoystickInputSettings::ReplaceProfileConfiguration(const FJoystickInputDeviceConfiguration& InDeviceConfiguration)
{
	if (FJoystickInputDeviceConfiguration* ExistingProfile = GetProfileConfigurationByFileName(InDeviceConfiguration.ProfileFileName))
	{
		*ExistingProfile = InDeviceConfiguration;
		return;
	}

	ProfileConfigurations.Add(InDeviceConfiguration);
}

void UJoystickInputSettings::ClearProfiles()
{
	ProfileConfigurations.Empty();
}

void UJoystickInputSettings::RebuildDeviceConfigurations() const
{
	UJoystickProfileManager* JoystickProfileManager = GetMutableDefault<UJoystickProfileManager>();
	if (!IsValid(JoystickProfileManager))
	{
		return;
	}

	JoystickProfileManager->RebuildInputDeviceConfigurations();
}

FJoystickInputDeviceConfiguration* UJoystickInputSettings::FindConfiguration(TArray<FJoystickInputDeviceConfiguration>& ConfigurationArray, const FString& DeviceHash) const
{
	return ConfigurationArray.FindByPredicate([&DeviceHash](const FJoystickInputDeviceConfiguration& PredicateDeviceConfig)
	{
		return PredicateDeviceConfig.DeviceHash == DeviceHash;
	});
}

bool UJoystickInputSettings::ConfigurationExists(TArray<FJoystickInputDeviceConfiguration>& ConfigurationArray, const FString& DeviceHash) const
{
	return FindConfiguration(ConfigurationArray, DeviceHash) != nullptr;
}

#undef LOCTEXT_NAMESPACE
