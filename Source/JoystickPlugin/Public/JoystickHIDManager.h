// JoystickPlugin is licensed under the MIT License.
// Copyright Jayden Maalouf. All Rights Reserved.

#pragma once

struct FDeviceInfoHID;
struct FDeviceHID;

#include "JoystickHIDManager.generated.h"

UCLASS(BlueprintType)
class JOYSTICKPLUGIN_API UJoystickHIDManager final : public UObject
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintPure, Category = "Joystick|HID")
	static UJoystickHIDManager* GetJoystickHIDManager() { return StaticClass()->GetDefaultObject<UJoystickHIDManager>(); }

	UFUNCTION(BlueprintCallable, Category = "Joystick|HID")
	TArray<FDeviceInfoHID> EnumerateHIDs(const int32 VendorId, const int32 ProductId);

	UFUNCTION(BlueprintCallable, Category = "Joystick|HID")
	bool OpenHID(const int32 VendorId, const int32 ProductId, FDeviceHID& Device);

	UFUNCTION(BlueprintCallable, Category = "Joystick|HID", DisplayName="Open HID with Serial Number")
	bool OpenHIDWithSerialNumber(const int32 VendorId, const int32 ProductId, FDeviceHID& Device, const FString& SerialNumber);

	UFUNCTION(BlueprintCallable, Category = "Joystick|HID", DisplayName="Open HID by Path")
	bool OpenHIDByPath(const FString& DevicePath, const bool Exclusive, FDeviceHID& Device);

	UFUNCTION(BlueprintCallable, Category = "Joystick|HID")
	bool CloseHID(const FDeviceHID& Device);

	UFUNCTION(BlueprintCallable, Category = "Joystick|HID")
	bool WriteReport(const FDeviceHID& Device, const TArray<uint8> Data);

private:
	bool Internal_OpenHID(const int VendorId, const int ProductId, FDeviceHID& Device, const FString* SerialNumber) const;
};
