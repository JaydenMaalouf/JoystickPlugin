#include "JoystickInputSettings.h"

#include "JoystickInputDevice.h"
#include "Engine/Engine.h"
#include "JoystickSubsystem.h"

FName UJoystickInputSettings::GetCategoryName() const
{
	return TEXT("Engine");
}

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

FText UJoystickInputSettings::GetSectionText() const
{
	return NSLOCTEXT("JoystickInputPlugin", "JoystickInputSettingsSection", "Joystick Input");
}
#endif
