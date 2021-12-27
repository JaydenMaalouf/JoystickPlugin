#include "JoystickInputSettings.h"

#include "Engine/Engine.h"
#include "JoystickSubsystem.h"
#include "JoystickDeviceManager.h"

FName UJoystickInputSettings::GetCategoryName() const
{
	return TEXT("Plugins");
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

	UJoystickDeviceManager* JoystickDeviceManager = UJoystickDeviceManager::GetJoystickDeviceManager();
	if (JoystickDeviceManager == nullptr)
	{
		return;
	}

	for(const int32& DeviceId : JoystickDeviceManager->GetDeviceIds())
	{
		JoystickSubsystem->ReinitialiseJoystickData(DeviceId);
	}
}

FText UJoystickInputSettings::GetSectionText() const
{
	return NSLOCTEXT("JoystickInputPlugin", "JoystickInputSettingsSection", "Joystick Input");
}
#endif