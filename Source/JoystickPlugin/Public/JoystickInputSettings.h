#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"

#include "Data/Settings/JoystickInputDeviceConfiguration.h"
#include "Data/Settings/JoystickInputDeviceInformation.h"

#include "JoystickInputSettings.generated.h"

UCLASS(config=Input, DefaultConfig)
class JOYSTICKPLUGIN_API UJoystickInputSettings : public UDeveloperSettings
{
	GENERATED_BODY()

public:
#if WITH_EDITOR
	virtual void PostEditChangeChainProperty(FPropertyChangedChainEvent& PropertyChangedEvent) override;
	virtual FText GetSectionText() const override;
#endif

	virtual FName GetCategoryName() const override;

	void DeviceAdded(FJoystickInputDeviceInformation JoystickInfo);
	void DeviceRemoved(FGuid JoystickGuid);
	void ResetDevices();

	UPROPERTY(VisibleAnywhere, Category="Device Configurations")
	TArray<FJoystickInputDeviceInformation> ConnectedDevices;

	UPROPERTY(config, EditAnywhere, Category="Device Configurations")
	TArray<FJoystickInputDeviceConfiguration> DeviceConfigurations;
};
