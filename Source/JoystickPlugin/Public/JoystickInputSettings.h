
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
	// Begin UObject Interface
	virtual void PostEditChangeChainProperty( struct FPropertyChangedChainEvent& PropertyChangedEvent ) override;
	// End UObject Interface
#endif

	// Begin UDeveloperSettings Interface
	virtual FName GetCategoryName() const override;
#if WITH_EDITOR
	virtual FText GetSectionText() const override;
#endif
	// End UDeveloperSettings Interface

	/** List of bindings to apply based on Vendor and Product Id, these are applied in order. To set a default, add an entry with empty product/vendor id at the end of the array */
	UPROPERTY(config, EditAnywhere, Category="Device Configurations")
	TArray<FJoystickInputDeviceConfiguration> DeviceConfigurations;
};