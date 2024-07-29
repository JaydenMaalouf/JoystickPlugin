// JoystickPlugin is licensed under the MIT License.
// Copyright Jayden Maalouf. All Rights Reserved.

#include "JoystickInputSettings.h"
#include "JoystickInputDevice.h"
#include "JoystickSubsystem.h"
#include "Engine/Engine.h"

UJoystickInputSettings::UJoystickInputSettings()
{
	UseDeviceName = false;
	IgnoreGameControllers = false;
#if WITH_EDITOR
	EnableLogs = true;
#else
	EnableLogs = false;
#endif
}

void UJoystickInputSettings::DeviceAdded(const FJoystickInformation& JoystickInfo)
{
	if (ConnectedDevices.ContainsByPredicate([&](const FJoystickInformation& Device)
	{
		return Device.InstanceId == JoystickInfo.InstanceId;
	}))
	{
		return;
	}

	ConnectedDevices.Add(JoystickInfo);
}

void UJoystickInputSettings::DeviceRemoved(const FJoystickInstanceId& InstanceId)
{
	ConnectedDevices.RemoveAll([&](const FJoystickInformation& Device)
	{
		return Device.InstanceId == InstanceId;
	});
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
