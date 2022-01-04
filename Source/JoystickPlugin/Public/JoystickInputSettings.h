
#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"

#include "JoystickInputDeviceConfiguration.h"
#include "JoystickInputDeviceInformation.h"

#include "JoystickInputSettings.generated.h"

UCLASS(config=Input, defaultconfig)
class JOYSTICKPLUGIN_API UJoystickInputSettings : public UDeveloperSettings
{
	GENERATED_BODY()

public:

#if WITH_EDITOR
	virtual void PostEditChangeChainProperty( struct FPropertyChangedChainEvent& PropertyChangedEvent ) override;
	virtual FText GetSectionText() const override;
#endif

	virtual FName GetCategoryName() const override;
	
    void DeviceAdded(FJoystickInputDeviceInformation JoystickInfo);
    void DeviceRemoved(FGuid JoystickGuid);

	UPROPERTY(config, VisibleAnywhere, Category="Device Configurations")
	TArray<FJoystickInputDeviceInformation> Devices;

	UPROPERTY(config, EditAnywhere, Category="Device Configurations")
	TArray<FJoystickInputDeviceConfiguration> DeviceConfigurations;
};