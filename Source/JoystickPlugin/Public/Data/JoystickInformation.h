﻿// JoystickPlugin is licensed under the MIT License.
// Copyright Jayden Maalouf. All Rights Reserved.

#pragma once

#include "Data/JoystickInstanceId.h"
#include "Data/JoystickType.h"
#include "Data/JoystickPowerLevel.h"

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
		  , PowerLevel(EJoystickPowerLevel::Empty)
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
	int ProductVersion;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Device Config")
	int VendorId;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Device Config")
	bool IsGamepad;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Device Config")
	bool RumbleSupport;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Device Config")
	bool HapticSupport;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Device Config")
	EJoystickPowerLevel PowerLevel;
};