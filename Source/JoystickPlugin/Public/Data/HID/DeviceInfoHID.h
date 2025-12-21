// JoystickPlugin is licensed under the MIT License.
// Copyright Jayden Maalouf. All Rights Reserved.

#pragma once

#include "DeviceInfoHID.generated.h"

USTRUCT(BlueprintType)
struct FDeviceInfoHID
{
	GENERATED_BODY()

	FDeviceInfoHID() : VendorId(-1),
	                   ProductId(-1),
	                   ReleaseNumber(-1),
	                   UsagePage(-1), Usage(-1),
	                   InterfaceNumber(-1),
	                   InterfaceClass(-1),
	                   InterfaceSubclass(-1),
	                   InterfaceProtocol(-1)
	{
	}

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="HID Config")
	FString DevicePath;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="HID Config")
	int32 VendorId;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="HID Config")
	int32 ProductId;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="HID Config")
	FString SerialNumber;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="HID Config")
	int32 ReleaseNumber;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="HID Config")
	FString ManufacturerString;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="HID Config")
	FString ProductString;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="HID Config")
	int32 UsagePage;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="HID Config")
	int32 Usage;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="HID Config")
	int InterfaceNumber;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="HID Config")
	int InterfaceClass;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="HID Config")
	int InterfaceSubclass;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="HID Config")
	int InterfaceProtocol;
};
