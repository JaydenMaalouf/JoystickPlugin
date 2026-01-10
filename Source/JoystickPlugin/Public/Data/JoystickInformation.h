// JoystickPlugin is licensed under the MIT License.
// Copyright Jayden Maalouf 2026. All Rights Reserved.

#pragma once

#include "JoystickConnectionType.h"
#include "JoystickLedInformation.h"
#include "JoystickPowerInformation.h"
#include "JoystickRumbleInformation.h"
#include "Data/JoystickInstanceId.h"
#include "Data/JoystickType.h"
#include "Data/JoystickHapticInformation.h"
#include "Data/JoystickHapticRumbleInformation.h"
#include "Data/JoystickSensorInformation.h"

#include "JoystickInformation.generated.h"

USTRUCT(BlueprintType)
struct FJoystickInformation
{
	GENERATED_BODY()

	FJoystickInformation()
		: InstanceId(-1)
		  , Type(EJoystickType::Unknown)
		  , ProductId(0)
		  , FirmwareVersion(0)
		  , ProductVersion(0)
		  , VendorId(0)
		  , IsGamepad(false)
		  , Connection(EJoystickConnectionType::Unknown)
	{
	}

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Device Config")
	FJoystickInstanceId InstanceId;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Device Config")
	FString DeviceName;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Device Config")
	EJoystickType Type;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Device Config")
	int32 ProductId;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Device Config")
	FString DeviceHash;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Device Config")
	FGuid ProductGuid;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Device Config")
	FString SerialNumber;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Device Config")
	FString Path;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Device Config")
	int32 FirmwareVersion;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Device Config")
	int32 ProductVersion;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Device Config")
	int32 VendorId;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Device Config")
	bool IsGamepad;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Device Config")
	EJoystickConnectionType Connection;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Device Config|Power")
	FJoystickPowerInformation Power;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Device Config|Led")
	FJoystickLedInformation Led;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Device Config|Rumble")
	FJoystickRumbleInformation Rumble;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Device Config|Haptic")
	FJoystickHapticInformation Haptic;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Device Config|Haptic Rumble")
	FJoystickHapticRumbleInformation HapticRumble;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Device Config|Sensors")
	FJoystickSensorInformation Gyro;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Device Config|Sensors")
	FJoystickSensorInformation Accelerometer;
};
