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
	ConnectedDevices.Add(JoystickInfo.InstanceId, JoystickInfo);
}

void UJoystickInputSettings::DeviceRemoved(const FJoystickInstanceId& InstanceId)
{
	ConnectedDevices.Remove(InstanceId);
}

void UJoystickInputSettings::ResetDevices()
{
	ConnectedDevices.Empty();
}

const FJoystickInputDeviceConfiguration* UJoystickInputSettings::GetInputDeviceConfiguration(const FGuid& ProductId) const
{
	return DeviceConfigurations.FindByPredicate([&](const FJoystickInputDeviceConfiguration& PredicateDeviceConfig)
	{
		return (!PredicateDeviceConfig.ProductGuid.IsValid() || ProductId == PredicateDeviceConfig.ProductGuid);
	});
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

	return DeviceConfiguration->AxisProperties.FindByPredicate([&](const FJoystickInputDeviceAxisProperties& AxisProperty)
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
	const bool ContainsDeviceConfiguration = DeviceConfigurations.ContainsByPredicate([&](const FJoystickInputDeviceConfiguration& DeviceConfiguration)
	{
		return DeviceConfiguration.ProductGuid == InDeviceConfiguration.ProductGuid;
	});

	if (ContainsDeviceConfiguration)
	{
		return;
	}

	DeviceConfigurations.Add(InDeviceConfiguration);
}
