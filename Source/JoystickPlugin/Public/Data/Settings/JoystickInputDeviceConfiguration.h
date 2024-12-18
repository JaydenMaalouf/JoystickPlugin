// JoystickPlugin is licensed under the MIT License.
// Copyright Jayden Maalouf. All Rights Reserved.

#pragma once

#include "JoystickInputDeviceAxisProperties.h"

#include "JoystickInputDeviceConfiguration.generated.h"

USTRUCT()
struct JOYSTICKPLUGIN_API FJoystickInputDeviceConfiguration
{
	GENERATED_BODY()

	FJoystickInputDeviceConfiguration()
		: OverrideDeviceName(false)
	{
	}

	FJoystickInputDeviceConfiguration(const FGuid& JoystickProductId)
		: ProductGuid(JoystickProductId)
		  , OverrideDeviceName(false)
	{
	}

	UPROPERTY(EditAnywhere, Category="Device Config", meta=(ToolTip="Used to determine the device to apply the configuration to."))
	FGuid ProductGuid;

	UPROPERTY(EditAnywhere, Category="Device Config",
		meta=(ToolTip="If enabled and the global setting for using Device Names as input, it will use the name specified as part of the key name. Requires a restart to apply.", ConfigRestartRequired=true))
	bool OverrideDeviceName;

	UPROPERTY(EditAnywhere, Category="Device Config", meta=(EditCondition="OverrideDeviceName", EditConditionHides), NoClear)
	FString DeviceName;

	UPROPERTY(EditAnywhere, Category="Device Config", meta=(TitleProperty="AxisIndex"))
	TArray<FJoystickInputDeviceAxisProperties> AxisProperties;
};
