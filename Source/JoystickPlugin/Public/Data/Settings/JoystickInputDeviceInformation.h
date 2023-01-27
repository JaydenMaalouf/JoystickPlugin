// JoystickPlugin is licensed under the MIT License.
// Copyright Jayden Maalouf. All Rights Reserved.

#pragma once
#include "Data/JoystickInfo.h"

#include "JoystickInputDeviceInformation.generated.h"

USTRUCT(BlueprintType)
struct JOYSTICKPLUGIN_API FJoystickInputDeviceInformation
{
	GENERATED_BODY()

	FJoystickInputDeviceInformation()
	{
	}

	FJoystickInputDeviceInformation(const FJoystickInfo& Device) :
		DeviceName(Device.DeviceName)
		, ProductName(Device.ProductName)
		, ProductId(Device.ProductId)
	{
	}

	FJoystickInputDeviceInformation(const FString InputDeviceName, const FString InputProductName, const FGuid InputProductId) :
		DeviceName(InputDeviceName)
		, ProductName(InputProductName)
		, ProductId(InputProductId)
	{
	}

	UPROPERTY(VisibleAnywhere, Category="Device Config")
	FString DeviceName;

	UPROPERTY(VisibleAnywhere, Category="Device Config")
	FString ProductName;

	UPROPERTY(VisibleAnywhere, Category="Device Config")
	FGuid ProductId;
};
