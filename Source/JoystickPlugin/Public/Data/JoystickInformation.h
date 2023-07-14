// JoystickPlugin is licensed under the MIT License.
// Copyright Jayden Maalouf. All Rights Reserved.

#pragma once

#include "Data/JoystickInstanceId.h"
#include "Data/JoystickType.h"
#include "Data/JoystickPowerLevel.h"
#include "Data/JoystickSensorInformation.h"

#include "JoystickInformation.generated.h"

USTRUCT(BlueprintType)
struct JOYSTICKPLUGIN_API FJoystickInformation
{
	GENERATED_BODY()

	FJoystickInformation()
		: InstanceId(-1)
		  , Type(EJoystickType::Unknown)
		  , ProductId(0)
		  , ProductVersion(0)
		  , VendorId(0)
		  , IsGamepad(false)
		  , RumbleSupport(false)
		  , HapticSupport(false)
		  , LedSupport(false)
		  , PowerLevel(EJoystickPowerLevel::Unknown)
	{
	}

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Device Config")
	FJoystickInstanceId InstanceId;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Device Config")
	FString DeviceName;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Device Config")
	FString ProductName;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Device Config")
	EJoystickType Type;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Device Config")
	int ProductId;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Device Config")
	FGuid ProductGuid;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Device Config")
	FString SerialNumber;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Device Config")
	int ProductVersion;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Device Config")
	int VendorId;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Device Config")
	bool IsGamepad;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Device Config/Supported Features")
	bool RumbleSupport;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Device Config/Supported Features")
	bool HapticSupport;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Device Config/Supported Features")
	bool LedSupport;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Device Config")
	EJoystickPowerLevel PowerLevel;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Device Config/Sensors")
	FJoystickSensorInformation Gyro;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Device Config/Sensors")
	FJoystickSensorInformation Accelerometer;
};
