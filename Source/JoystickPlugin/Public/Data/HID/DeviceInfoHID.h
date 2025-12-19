// JoystickPlugin is licensed under the MIT License.
// Copyright Jayden Maalouf. All Rights Reserved.

#pragma once

#include "DeviceInfoHID.generated.h"

USTRUCT(BlueprintType)
struct FDeviceInfoHID
{
	GENERATED_BODY()

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
