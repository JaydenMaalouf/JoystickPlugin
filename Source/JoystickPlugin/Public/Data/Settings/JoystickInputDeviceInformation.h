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
		: IsGamepad(false)
		  , HasRumble(false)
	{
	}

	FJoystickInputDeviceInformation(const FJoystickInfo& Device)
		: DeviceName(Device.DeviceName)
		  , ProductName(Device.ProductName)
		  , ProductId(Device.ProductId)
		  , IsGamepad(Device.IsGamepad)
		  , HasRumble(Device.HasRumble)
	{
	}

	FJoystickInputDeviceInformation(const FString InputDeviceName, const FString InputProductName, const FGuid InputProductId, const bool InputIsGamepad, const bool InputHasRumble)
		: DeviceName(InputDeviceName)
		  , ProductName(InputProductName)
		  , ProductId(InputProductId)
		  , IsGamepad(InputIsGamepad)
		  , HasRumble(InputHasRumble)
	{
	}

	UPROPERTY(VisibleAnywhere, Category="Device Config")
	FString DeviceName;

	UPROPERTY(VisibleAnywhere, Category="Device Config")
	FString ProductName;

	UPROPERTY(VisibleAnywhere, Category="Device Config")
	FGuid ProductId;

	UPROPERTY(VisibleAnywhere, Category="Device Config")
	bool IsGamepad;

	UPROPERTY(VisibleAnywhere, Category="Device Config")
	bool HasRumble;
};
