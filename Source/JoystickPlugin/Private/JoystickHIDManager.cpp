// JoystickPlugin is licensed under the MIT License.
// Copyright Jayden Maalouf. All Rights Reserved.

#include "JoystickHIDManager.h"

#include "JoystickLogManager.h"
#include "Data/HID/DeviceInfoHID.h"
#include "Data/HID/DeviceHID.h"

THIRD_PARTY_INCLUDES_START

#include "SDL_hidapi.h"

THIRD_PARTY_INCLUDES_END

TArray<FDeviceInfoHID> UJoystickHIDManager::EnumerateHIDs(const int VendorId, const int ProductId)
{
	SDL_hid_device_info* DeviceList = SDL_hid_enumerate(VendorId, ProductId);
	const SDL_hid_device_info* CurrentDevice = DeviceList;

	TArray<FDeviceInfoHID> Devices;
	FDeviceInfoHID DeviceInfo;
	while (CurrentDevice)
	{
		if (CurrentDevice->vendor_id == VendorId && CurrentDevice->product_id == ProductId)
		{
			DeviceInfo.DevicePath = CurrentDevice->path;
			DeviceInfo.VendorId = CurrentDevice->vendor_id;
			DeviceInfo.ProductId = CurrentDevice->product_id;
			DeviceInfo.SerialNumber = CurrentDevice->serial_number;
			DeviceInfo.ReleaseNumber = CurrentDevice->release_number;
			DeviceInfo.ManufacturerString = CurrentDevice->manufacturer_string;
			DeviceInfo.ProductString = CurrentDevice->product_string;
			DeviceInfo.UsagePage = CurrentDevice->usage_page;
			DeviceInfo.Usage = CurrentDevice->usage;
			DeviceInfo.InterfaceNumber = CurrentDevice->interface_number;
			DeviceInfo.InterfaceClass = CurrentDevice->interface_class;
			DeviceInfo.InterfaceSubclass = CurrentDevice->interface_subclass;
			DeviceInfo.InterfaceProtocol = CurrentDevice->interface_protocol;
			Devices.Add(DeviceInfo);
		}
		CurrentDevice = CurrentDevice->next;
	}

	SDL_hid_free_enumeration(DeviceList);
	return Devices;
}

bool UJoystickHIDManager::OpenHID(const int32 VendorId, const int32 ProductId, FDeviceHID& Device)
{
	return Internal_OpenHID(VendorId, ProductId, Device, nullptr);
}

bool UJoystickHIDManager::OpenHIDWithSerialNumber(const int32 VendorId, const int32 ProductId, FDeviceHID& Device, const FString& SerialNumber)
{
	return Internal_OpenHID(VendorId, ProductId, Device, &SerialNumber);
}

bool UJoystickHIDManager::Internal_OpenHID(const int32 VendorId, const int32 ProductId, FDeviceHID& Device, const FString* SerialNumber) const
{
	const wchar_t* SerialW = nullptr;
#if PLATFORM_WINDOWS
	SerialW = SerialNumber ? **SerialNumber : nullptr; // TCHAR == wchar_t on Windows
#else
	// Convert to wchar_t explicitly
	FString Tmp = SerialNumber ? *SerialNumber : FString();
	SerialW = SerialNumber ? (const wchar_t*)*Tmp : nullptr; // not actually safe if wchar_t != TCHAR
#endif

	SDL_hid_device* HIDDevice = SDL_hid_open(VendorId, ProductId, SerialW);
	if (!HIDDevice)
	{
		FJoystickLogManager::Get()->LogSDLError(TEXT("OpenHIDByPath Error"));
		return false;
	}

	Device.Device = HIDDevice;
	return true;
}

bool UJoystickHIDManager::OpenHIDByPath(const FString& DevicePath, const bool Exclusive, FDeviceHID& Device)
{
	SDL_hid_device* HIDDevice = SDL_hid_open_path(TCHAR_TO_UTF8(*DevicePath), Exclusive);
	if (!HIDDevice)
	{
		FJoystickLogManager::Get()->LogSDLError(TEXT("OpenHIDByPath Error"));
		return false;
	}

	Device.Device = HIDDevice;
	return true;
}

bool UJoystickHIDManager::CloseHID(const FDeviceHID& Device)
{
	if (!Device.Device)
	{
		FJoystickLogManager::Get()->LogError(TEXT("CloseHID - Device is NULL"));
		return false;
	}

	SDL_hid_close(Device.Device);
	return true;
}

bool UJoystickHIDManager::WriteReport(const FDeviceHID& Device, const TArray<uint8> Data)
{
	if (!Device.Device)
	{
		FJoystickLogManager::Get()->LogError(TEXT("CloseHID - Device is NULL"));
		return false;
	}

	const int Result = SDL_hid_write(Device.Device, Data.GetData(), Data.Num());
	if (Result == -1)
	{
		FJoystickLogManager::Get()->LogSDLError(TEXT("Write"));
		return false;
	}

	return true;
}
