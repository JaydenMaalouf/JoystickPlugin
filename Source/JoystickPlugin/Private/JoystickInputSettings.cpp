// JoystickPlugin is licensed under the MIT License.
// Copyright Jayden Maalouf 2026. All Rights Reserved.

#include "JoystickInputSettings.h"

#include "Data/JoystickInformation.h"
#include "Data/JoystickInstanceId.h"
#include "Data/Settings/JoystickInputDeviceConfiguration.h"
#include "Engine/Engine.h"
#include "Managers/JoystickProfileManager.h"
#include "JoystickSubsystem.h"

UJoystickInputSettings::UJoystickInputSettings() : UseDeviceName(true),
                                                   IncludeDeviceIndex(false),
                                                   IgnoreGamepads(false),
                                                   AllowDuplicateHashedDevices(true),
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
		if (ConfigurationMatchesDevice(DeviceConfiguration, ConnectedDevice))
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

FJoystickInputDeviceConfiguration* UJoystickInputSettings::GetDeviceConfiguration(const FJoystickInformation& Device)
{
	return FindConfiguration(DeviceConfigurations, Device, true);
}

FJoystickInputDeviceConfiguration* UJoystickInputSettings::GetProfileConfiguration(const FJoystickInformation& Device)
{
	return FindConfiguration(ProfileConfigurations, Device, true);
}

#if WITH_EDITOR
void UJoystickInputSettings::PostEditChangeChainProperty(FPropertyChangedChainEvent& PropertyChangedEvent)
{
	Super::PostEditChangeChainProperty(PropertyChangedEvent);

	UJoystickProfileManager* JoystickProfileManager = GetMutableDefault<UJoystickProfileManager>();
	if (!IsValid(JoystickProfileManager))
	{
		return;
	}

	JoystickProfileManager->RebuildDeviceConfigurations();
}
#endif

void UJoystickInputSettings::AddDeviceConfiguration(const FJoystickInputDeviceConfiguration& InDeviceConfiguration)
{
	if (ConfigurationExists(DeviceConfigurations, InDeviceConfiguration))
	{
		return;
	}

	DeviceConfigurations.Add(InDeviceConfiguration);
}

void UJoystickInputSettings::RemoveDeviceConfiguration(const int DeviceConfigurationIndex)
{
	DeviceConfigurations.RemoveAt(DeviceConfigurationIndex);
	TryUpdateDefaultConfigFile();
}

void UJoystickInputSettings::AddProfileConfiguration(const FJoystickInputDeviceConfiguration& InDeviceConfiguration)
{
	if (ConfigurationExists(ProfileConfigurations, InDeviceConfiguration))
	{
		return;
	}

	ProfileConfigurations.Add(InDeviceConfiguration);
}

void UJoystickInputSettings::ClearProfiles()
{
	ProfileConfigurations.Empty();
}

FJoystickInputDeviceConfiguration* UJoystickInputSettings::FindConfiguration(TArray<FJoystickInputDeviceConfiguration>& ConfigurationArray, const FJoystickInformation& Device, const bool IncludeEmptyGuids) const
{
	return ConfigurationArray.FindByPredicate([Device, IncludeEmptyGuids](const FJoystickInputDeviceConfiguration& PredicateDeviceConfig)
	{
		return ConfigurationMatchesDevice(PredicateDeviceConfig, Device, IncludeEmptyGuids);
	});
}

bool UJoystickInputSettings::ConfigurationMatchesDevice(const FJoystickInputDeviceConfiguration& DeviceConfiguration, const FJoystickInformation& ConnectedDevice, const bool IncludeEmptyGuids)
{
	switch (DeviceConfiguration.DeviceIdentifyMethod)
	{
	case EJoystickIdentifierType::Legacy:
		return (IncludeEmptyGuids && !DeviceConfiguration.ProductGuid.IsValid()) || ConnectedDevice.ProductGuid == DeviceConfiguration.ProductGuid;
	case EJoystickIdentifierType::Hashed:
	default:
		return (IncludeEmptyGuids && DeviceConfiguration.DeviceHash.IsEmpty()) || ConnectedDevice.DeviceHash == DeviceConfiguration.DeviceHash;
	}
}

bool UJoystickInputSettings::ConfigurationExists(TArray<FJoystickInputDeviceConfiguration>& ConfigurationArray, const FJoystickInputDeviceConfiguration& Device, const bool IncludeEmptyGuids) const
{
	FJoystickInformation DummyJoystickInformation;
	DummyJoystickInformation.ProductGuid = Device.ProductGuid;
	DummyJoystickInformation.DeviceHash = Device.DeviceHash;
	return FindConfiguration(ConfigurationArray, DummyJoystickInformation, IncludeEmptyGuids) != nullptr;
}
