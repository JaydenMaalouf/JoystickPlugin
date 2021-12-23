#pragma once

#include "JoystickInputDeviceAxisProperties.h"
#include "JoystickInputDeviceConfiguration.generated.h"

USTRUCT()
struct JOYSTICKPLUGIN_API FJoystickInputDeviceConfiguration
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category="Device Config")
	FString DeviceGUID;

	UPROPERTY(EditAnywhere, Category="Device Config")
	FString DeviceName;

	UPROPERTY(EditAnywhere, Category="Device Config")
	TArray<FJoystickInputDeviceAxisProperties> AxisProperties;
};