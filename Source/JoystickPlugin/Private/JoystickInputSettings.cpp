// JoystickPlugin is licensed under the MIT License.
// Copyright Jayden Maalouf. All Rights Reserved.

#include "JoystickInputSettings.h"

#include "JoystickInputDevice.h"
#include "JoystickSubsystem.h"

void UJoystickInputSettings::DeviceAdded(const FJoystickInputDeviceInformation JoystickInfo)
{
	if (ConnectedDevices.ContainsByPredicate([&](const FJoystickInputDeviceInformation& Device)
	{
		return Device.ProductId == JoystickInfo.ProductId;
	}))
	{
		return;
	}

	ConnectedDevices.Add(JoystickInfo);
}

void UJoystickInputSettings::DeviceRemoved(const FGuid JoystickGuid)
{
	ConnectedDevices.RemoveAll([&](const FJoystickInputDeviceInformation& Device)
	{
		return Device.ProductId == JoystickGuid;
	});
}

void UJoystickInputSettings::ResetDevices()
{
	ConnectedDevices.Empty();
}

const FJoystickInputDeviceConfiguration* UJoystickInputSettings::GetInputDeviceConfiguration(const FJoystickInfo& Device) const
{
	return DeviceConfigurations.FindByPredicate([&](const FJoystickInputDeviceConfiguration& PredicateDeviceConfig)
	{
		return (!PredicateDeviceConfig.ProductId.IsValid() || Device.ProductId == PredicateDeviceConfig.ProductId);
	});
}

int UJoystickInputSettings::GetDeviceIndexByKey(const FKey& Key) const
{
	const UJoystickSubsystem* JoystickSubsystem = GEngine->GetEngineSubsystem<UJoystickSubsystem>();
	if (!IsValid(JoystickSubsystem))
	{
		return -1;
	}

	FJoystickInputDevice* InputDevice = JoystickSubsystem->GetInputDevice();
	if (InputDevice == nullptr)
	{
		return -1;
	}
	
	return InputDevice->GetDeviceIndexByKey(Key);
}

const FJoystickInputDeviceConfiguration* UJoystickInputSettings::GetInputDeviceConfigurationByKey(const FKey& Key) const
{
	const UJoystickSubsystem* JoystickSubsystem = GEngine->GetEngineSubsystem<UJoystickSubsystem>();
	if (!IsValid(JoystickSubsystem))
	{
		return nullptr;
	}
	
	FJoystickInfo DeviceInfo;
	const int DeviceIndex = GetDeviceIndexByKey(Key);
	const bool Result = JoystickSubsystem->GetJoystickInfo(DeviceIndex, DeviceInfo);
	if (Result == false)
	{
		return nullptr;
	}
	return GetInputDeviceConfiguration(DeviceInfo);
}

const FJoystickInputDeviceAxisProperties* UJoystickInputSettings::GetAxisPropertiesByKey(const FKey& AxisKey) const
{		
	const FJoystickInputDeviceConfiguration* DeviceConfiguration = GetInputDeviceConfigurationByKey(AxisKey);
	if (DeviceConfiguration == nullptr)
	{
		return nullptr;
	}
	
	const int KeyIndex = GetDeviceIndexByKey(AxisKey);
	if (KeyIndex == -1)
	{
		return nullptr;
	}
	
	return DeviceConfiguration->AxisProperties.FindByPredicate([&](const FJoystickInputDeviceAxisProperties& AxisProperty)
	{
		return AxisProperty.AxisIndex != -1 && AxisProperty.AxisIndex == KeyIndex;
	});
}

#if WITH_EDITOR
void UJoystickInputSettings::PostEditChangeChainProperty(FPropertyChangedChainEvent& PropertyChangedEvent)
{
	Super::PostEditChangeChainProperty(PropertyChangedEvent);

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
