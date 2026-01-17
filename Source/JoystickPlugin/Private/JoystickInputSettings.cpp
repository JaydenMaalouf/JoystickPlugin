// JoystickPlugin is licensed under the MIT License.
// Copyright Jayden Maalouf 2026. All Rights Reserved.

#include "JoystickInputSettings.h"

#include "Data/JoystickInformation.h"
#include "Data/JoystickInstanceId.h"
#include "Data/Settings/JoystickInputDeviceConfiguration.h"
#include "Engine/Engine.h"
#include "JoystickInputDevice.h"
#include "Managers/JoystickProfileManager.h"
#include "JoystickSubsystem.h"

UJoystickInputSettings::UJoystickInputSettings() : UseDeviceName(true),
                                                   IncludeDeviceIndex(false),
                                                   IgnoreGameControllers(false),
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

bool UJoystickInputSettings::GetIgnoreGameControllers() const
{
	return IgnoreGameControllers;
}

bool UJoystickInputSettings::SetIgnoreGameControllers(const bool NewIgnoreGameControllers)
{
	const bool OldIgnoreGameControllers = IgnoreGameControllers;
	IgnoreGameControllers = NewIgnoreGameControllers;
	return OldIgnoreGameControllers != NewIgnoreGameControllers;
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
	if (FindConfiguration(DeviceConfigurations, InDeviceConfiguration))
	{
		return;
	}

	DeviceConfigurations.Add(InDeviceConfiguration);
}

void UJoystickInputSettings::AddProfileConfiguration(const FJoystickInputDeviceConfiguration& InDeviceConfiguration)
{
	if (FindConfiguration(ProfileConfigurations, InDeviceConfiguration))
	{
		return;
	}

	ProfileConfigurations.Add(InDeviceConfiguration);
}

FJoystickInputDeviceConfiguration* UJoystickInputSettings::FindConfiguration(TArray<FJoystickInputDeviceConfiguration>& ConfigurationArray, const FJoystickInformation& Device, const bool IncludeEmptyGuids) const
{
	return ConfigurationArray.FindByPredicate([Device, IncludeEmptyGuids](const FJoystickInputDeviceConfiguration& PredicateDeviceConfig)
	{
		if (PredicateDeviceConfig.DeviceIdentifyMethod == EJoystickIdentifierType::Legacy)
		{
			return (IncludeEmptyGuids && !PredicateDeviceConfig.ProductGuid.IsValid()) || Device.ProductGuid == PredicateDeviceConfig.ProductGuid;
		}

		return (IncludeEmptyGuids && PredicateDeviceConfig.DeviceHash.IsEmpty()) || Device.DeviceHash == PredicateDeviceConfig.DeviceHash;
	});
}

FJoystickInputDeviceConfiguration* UJoystickInputSettings::FindConfiguration(TArray<FJoystickInputDeviceConfiguration>& ConfigurationArray, const FJoystickInputDeviceConfiguration& Device, const bool IncludeEmptyGuids) const
{
	FJoystickInformation DummyJoystickInformation;
	DummyJoystickInformation.ProductGuid = Device.ProductGuid;
	DummyJoystickInformation.DeviceHash = Device.DeviceHash;
	return FindConfiguration(ConfigurationArray, DummyJoystickInformation, IncludeEmptyGuids);
}
