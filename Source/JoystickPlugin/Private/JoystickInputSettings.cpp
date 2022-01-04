#include "JoystickInputSettings.h"

#include "Engine/Engine.h"
#include "JoystickSubsystem.h"

FName UJoystickInputSettings::GetCategoryName() const
{
	return TEXT("Plugins");
}

void UJoystickInputSettings::DeviceAdded(const FJoystickInputDeviceInformation JoystickInfo)
{
	const bool Exists = Devices.ContainsByPredicate([=](const FJoystickInputDeviceInformation& Device) { return Device.ProductId == JoystickInfo.ProductId; });
	if (Exists)
	{
		return;
	}
	
	Devices.Add(JoystickInfo);
}

void UJoystickInputSettings::DeviceRemoved(const FGuid JoystickGuid)
{
	Devices.RemoveAll([=](const FJoystickInputDeviceInformation& Device) { return Device.ProductId == JoystickGuid; });
}

#if WITH_EDITOR
void UJoystickInputSettings::PostEditChangeChainProperty(FPropertyChangedChainEvent& PropertyChangedEvent)
{
	Super::PostEditChangeChainProperty(PropertyChangedEvent);
	
	UJoystickSubsystem* JoystickSubsystem = GEngine->GetEngineSubsystem<UJoystickSubsystem>();
	if (JoystickSubsystem == nullptr)
	{
		return;
	}
	
	FJoystickInputDevice* InputDevice = JoystickSubsystem->GetInputDevice();
	if (InputDevice == nullptr)
	{
		return;
	}

	TArray<int32> DeviceIds;
	InputDevice->GetDeviceIds(DeviceIds);

	for(const int32& DeviceId : DeviceIds)
	{
		JoystickSubsystem->ReinitialiseJoystickData(DeviceId);
	}
}

FText UJoystickInputSettings::GetSectionText() const
{
	return NSLOCTEXT("JoystickInputPlugin", "JoystickInputSettingsSection", "Joystick Input");
}
#endif