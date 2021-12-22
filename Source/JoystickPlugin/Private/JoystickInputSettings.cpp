#include "JoystickInputSettings.h"

#include "JoystickPlugin.h"

FJoystickInputDeviceConfiguration::FJoystickInputDeviceConfiguration()
{
}

void UJoystickInputSettings::PostEditChangeChainProperty(FPropertyChangedChainEvent& PropertyChangedEvent)
{
	Super::PostEditChangeChainProperty(PropertyChangedEvent);

	FJoystickPlugin* JoystickPlugin = static_cast<FJoystickPlugin*>(&FJoystickPlugin::Get());
	if (JoystickPlugin == nullptr)
	{
		return;
	}
	
	UJoystickDeviceManager* JoystickDeviceManager = JoystickPlugin->GetDeviceManager();
	if (JoystickDeviceManager == nullptr)
	{
		return;
	}

	FDeviceSDL* DeviceSDL = JoystickDeviceManager->GetDeviceSDL();
	if (DeviceSDL == nullptr)
	{
		return;
	}

	for(const int32& DeviceId : JoystickDeviceManager->GetDeviceIds())
	{
		JoystickDeviceManager->ReinitialiseJoystickData(DeviceId);
	}
}

FName UJoystickInputSettings::GetCategoryName() const
{
	return TEXT("Plugins");
}

#if WITH_EDITOR
FText UJoystickInputSettings::GetSectionText() const
{
	return NSLOCTEXT("JoystickInputPlugin", "JoystickInputSettingsSection", "Joystick Input");
}
#endif