// JoystickPlugin is licensed under the MIT License.
// Copyright Jayden Maalouf. All Rights Reserved.

#pragma once

struct FDeviceInfoHID;
struct FDeviceHID;

#include "JoystickHIDManager.generated.h"

UCLASS(BlueprintType, DisplayName="Joystick HID Manager")
class JOYSTICKPLUGIN_API UJoystickHIDManager final : public UObject
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintPure, Category="Joystick|HID", DisplayName="Get Joystick HID Manager")
	static UJoystickHIDManager* GetJoystickHIDManager() { return StaticClass()->GetDefaultObject<UJoystickHIDManager>(); }

	UFUNCTION(BlueprintCallable, Category="Joystick|HID")
	TArray<FDeviceInfoHID> EnumerateHIDs(const int32 VendorId, const int32 ProductId);

	UFUNCTION(BlueprintCallable, Category="Joystick|HID")
	bool OpenHID(const int32 VendorId, const int32 ProductId, FDeviceHID& Device);

	UFUNCTION(BlueprintCallable, Category="Joystick|HID", DisplayName="Open HID with Serial Number")
	bool OpenHIDSerialNumber(const int32 VendorId, const int32 ProductId, FDeviceHID& Device, const FString& SerialNumber);

	UFUNCTION(BlueprintCallable, Category="Joystick|HID", DisplayName="Open HID by Path")
	bool OpenHIDPath(const FString& DevicePath, const bool Exclusive, FDeviceHID& Device);

	UFUNCTION(BlueprintCallable, Category="Joystick|HID")
	bool CloseHID(const FDeviceHID& Device);

	UFUNCTION(BlueprintCallable, Category="Joystick|HID|Read")
	bool ReadInputReport(const FDeviceHID& Device, TArray<uint8>& Data);

	UFUNCTION(BlueprintCallable, Category="Joystick|HID|Read", DisplayName="Read Input Report with Timeout")
	bool ReadInputReportTimeout(const FDeviceHID& Device, TArray<uint8>& Data, const int Timeout);

	UFUNCTION(BlueprintCallable, Category="Joystick|HID|Read")
	bool ReadFeatureReport(const FDeviceHID& Device, TArray<uint8>& Data);

	UFUNCTION(BlueprintCallable, Category="Joystick|HID|Write")
	bool WriteOutputReport(const FDeviceHID& Device, const TArray<uint8>& Data);

	UFUNCTION(BlueprintCallable, Category="Joystick|HID|Write")
	bool SendFeatureReport(const FDeviceHID& Device, const TArray<uint8>& Data);

private:
	bool Internal_OpenHID(const int32 VendorId, const int32 ProductId, FDeviceHID& Device, const FString* SerialNumber) const;
};
