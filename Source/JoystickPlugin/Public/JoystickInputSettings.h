
#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"

#include "JoystickInputDeviceConfiguration.h"

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

	UPROPERTY(config, EditAnywhere, Category="Device Configurations")
	TArray<FJoystickInputDeviceConfiguration> DeviceConfigurations;
};