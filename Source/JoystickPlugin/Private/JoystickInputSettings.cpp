// JoystickPlugin is licensed under the MIT License.
// Copyright Jayden Maalouf. All Rights Reserved.

#include "JoystickInputSettings.h"

#include "Data/JoystickInformation.h"
#include "Data/JoystickInstanceId.h"
#include "Data/Settings/JoystickInputDeviceAxisProperties.h"
#include "Data/Settings/JoystickInputDeviceConfiguration.h"
#include "Engine/Engine.h"
#include "JoystickInputDevice.h"
#include "JoystickSubsystem.h"

UJoystickInputSettings::UJoystickInputSettings() :
	UseDeviceName(false),
	IncludeDeviceIndex(true),
	IgnoreGameControllers(false),
	EnableLogs(true)
{
	MapHatAxisToKeys = static_cast<int32>(EHatDirection::Up | EHatDirection::Down | EHatDirection::Left | EHatDirection::Right);
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

const FJoystickInputDeviceConfiguration* UJoystickInputSettings::GetInputDeviceConfiguration(const FGuid& ProductGuid) const
{
	if (const FJoystickInputDeviceConfiguration* DeviceConfiguration = FindConfiguration(DeviceConfigurations, ProductGuid, true))
	{
		return DeviceConfiguration;
	}

	return FindConfiguration(ProfileConfigurations, ProductGuid, true);
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

const FJoystickInputDeviceConfiguration* UJoystickInputSettings::GetInputDeviceConfigurationByKey(const FKey& Key) const
{
	if (!IsValid(GEngine))
	{
		return nullptr;
	}

	UJoystickSubsystem* JoystickSubsystem = GEngine->GetEngineSubsystem<UJoystickSubsystem>();
	if (!IsValid(JoystickSubsystem))
	{
		return nullptr;
	}

	const FJoystickInputDevice* InputDevice = JoystickSubsystem->GetInputDevice();
	if (InputDevice == nullptr)
	{
		return nullptr;
	}

	FJoystickInformation DeviceInfo;
	const FJoystickInstanceId& InstanceId = InputDevice->GetInstanceIdByKey(Key);
	const bool Result = JoystickSubsystem->GetJoystickInfo(InstanceId, DeviceInfo);
	if (Result == false)
	{
		return nullptr;
	}

	return GetInputDeviceConfiguration(DeviceInfo.ProductGuid);
}

const FJoystickInputDeviceAxisProperties* UJoystickInputSettings::GetAxisPropertiesByKey(const FKey& AxisKey) const
{
	if (!IsValid(GEngine))
	{
		return nullptr;
	}

	const UJoystickSubsystem* JoystickSubsystem = GEngine->GetEngineSubsystem<UJoystickSubsystem>();
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
	if (AxisIndex == -1)
	{
		return nullptr;
	}

	const FJoystickInputDeviceConfiguration* DeviceConfiguration = GetInputDeviceConfigurationByKey(AxisKey);
	if (DeviceConfiguration == nullptr)
	{
		return nullptr;
	}

	return DeviceConfiguration->AxisProperties.FindByPredicate([AxisIndex](const FJoystickInputDeviceAxisProperties& AxisProperty)
	{
		return AxisProperty.AxisIndex != -1 && AxisProperty.AxisIndex == AxisIndex;
	});
}

#if WITH_EDITOR
void UJoystickInputSettings::PostEditChangeChainProperty(FPropertyChangedChainEvent& PropertyChangedEvent)
{
	Super::PostEditChangeChainProperty(PropertyChangedEvent);

	if (!IsValid(GEngine))
	{
		return;
	}

	const UJoystickSubsystem* JoystickSubsystem = GEngine->GetEngineSubsystem<UJoystickSubsystem>();
	if (!IsValid(JoystickSubsystem))
	{
		return;
	}

	FJoystickInputDevice* InputDevice = JoystickSubsystem->GetInputDevice();
	if (InputDevice == nullptr)
	{
		return;
	}

	InputDevice->UpdateAxisProperties();
}
#endif

void UJoystickInputSettings::AddDeviceConfiguration(const FJoystickInputDeviceConfiguration& InDeviceConfiguration)
{
	if (FindConfiguration(DeviceConfigurations, InDeviceConfiguration.ProductGuid))
	{
		return;
	}

	DeviceConfigurations.Add(InDeviceConfiguration);
}

void UJoystickInputSettings::AddProfileConfiguration(const FJoystickInputDeviceConfiguration& InDeviceConfiguration)
{
	if (FindConfiguration(ProfileConfigurations, InDeviceConfiguration.ProductGuid))
	{
		return;
	}

	ProfileConfigurations.Add(InDeviceConfiguration);
}

const FJoystickInputDeviceConfiguration* UJoystickInputSettings::FindConfiguration(const TArray<FJoystickInputDeviceConfiguration>& ConfigurationArray, const FGuid& ProductGuid, const bool IncludeEmptyGuids) const
{
	return ConfigurationArray.FindByPredicate([ProductGuid, IncludeEmptyGuids](const FJoystickInputDeviceConfiguration& PredicateDeviceConfig)
	{
		return ((IncludeEmptyGuids && !PredicateDeviceConfig.ProductGuid.IsValid()) || ProductGuid == PredicateDeviceConfig.ProductGuid);
	});
}
