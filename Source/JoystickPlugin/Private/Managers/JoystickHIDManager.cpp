// JoystickPlugin is licensed under the MIT License.
// Copyright Jayden Maalouf 2026. All Rights Reserved.

#include "Managers/JoystickHIDManager.h"
#include "Managers/JoystickLogManager.h"
#include "JoystickFunctionLibrary.h"
#include "Data/HID/DeviceInfoHID.h"
#include "Data/HID/DeviceHID.h"

#include "SDLDynamicLoader.h"

const UJoystickHIDManager* UJoystickHIDManager::GetJoystickHIDManager()
{
	return GetDefault<UJoystickHIDManager>();
}

TArray<FDeviceInfoHID> UJoystickHIDManager::EnumerateHIDs(const int32 VendorId, const int32 ProductId)
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
			DeviceInfo.SerialNumber = UJoystickFunctionLibrary::SafelyStringify(CurrentDevice->serial_number);
			DeviceInfo.ReleaseNumber = CurrentDevice->release_number;
			DeviceInfo.ManufacturerString = UJoystickFunctionLibrary::SafelyStringify(CurrentDevice->manufacturer_string);
			DeviceInfo.ProductString = UJoystickFunctionLibrary::SafelyStringify(CurrentDevice->product_string);
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

bool UJoystickHIDManager::OpenHID(const int32 VendorId, const int32 ProductId, FDeviceHID& Device) const
{
	return Internal_OpenHID(VendorId, ProductId, Device, nullptr);
}

bool UJoystickHIDManager::OpenHIDSerialNumber(const int32 VendorId, const int32 ProductId, FDeviceHID& Device, const FString& SerialNumber) const
{
	return Internal_OpenHID(VendorId, ProductId, Device, &SerialNumber);
}

bool UJoystickHIDManager::OpenHIDPath(const FString& DevicePath, FDeviceHID& Device)
{
	SDL_hid_device* HIDDevice = SDL_hid_open_path(TCHAR_TO_UTF8(*DevicePath));
	if (!HIDDevice)
	{
		FJoystickLogManager::Get()->LogSDLError(TEXT("SDL_hid_open_path failed"));
		return false;
	}

	Device.Device = HIDDevice;
	return true;
}

bool UJoystickHIDManager::CloseHID(const FDeviceHID& Device)
{
	if (!Device.Device)
	{
		FJoystickLogManager::Get()->LogError(TEXT("Failed to close HID. Device is null"));
		return false;
	}

	SDL_hid_close(Device.Device);
	return true;
}

bool UJoystickHIDManager::ReadInputReport(const FDeviceHID& Device, TArray<uint8>& Data)
{
	if (!Device.Device)
	{
		FJoystickLogManager::Get()->LogError(TEXT("Failed to read input report. Device is null"));
		return false;
	}

	const int Result = SDL_hid_read(Device.Device, Data.GetData(), Data.Num());
	if (Result == -1)
	{
		FJoystickLogManager::Get()->LogSDLError(TEXT("SDL_hid_read failed"));
		return false;
	}

	return true;
}

bool UJoystickHIDManager::ReadInputReportTimeout(const FDeviceHID& Device, TArray<uint8>& Data, const int Timeout)
{
	if (!Device.Device)
	{
		FJoystickLogManager::Get()->LogError(TEXT("Failed to read input report with timeout. Device is null"));
		return false;
	}

	const int Result = SDL_hid_read_timeout(Device.Device, Data.GetData(), Data.Num(), Timeout);
	if (Result == -1)
	{
		FJoystickLogManager::Get()->LogSDLError(TEXT("SDL_hid_read_timeout failed"));
		return false;
	}

	return true;
}

bool UJoystickHIDManager::ReadFeatureReport(const FDeviceHID& Device, TArray<uint8>& Data)
{
	if (!Device.Device)
	{
		FJoystickLogManager::Get()->LogError(TEXT("Failed to read feature report. Device is null"));
		return false;
	}

	const int Result = SDL_hid_get_feature_report(Device.Device, Data.GetData(), Data.Num());
	if (Result == -1)
	{
		FJoystickLogManager::Get()->LogSDLError(TEXT("SDL_hid_get_feature_report failed"));
		return false;
	}

	return true;
}

bool UJoystickHIDManager::WriteOutputReport(const FDeviceHID& Device, const TArray<uint8>& Data)
{
	if (!Device.Device)
	{
		FJoystickLogManager::Get()->LogError(TEXT("Failed to write output report. Device is null"));
		return false;
	}

	const int Result = SDL_hid_write(Device.Device, Data.GetData(), Data.Num());
	if (Result == -1)
	{
		FJoystickLogManager::Get()->LogSDLError(TEXT("SDL_hid_write failed"));
		return false;
	}

	return true;
}

bool UJoystickHIDManager::SendFeatureReport(const FDeviceHID& Device, const TArray<uint8>& Data)
{
	if (!Device.Device)
	{
		FJoystickLogManager::Get()->LogError(TEXT("Failed to send feature report. Device is null"));
		return false;
	}

	const int Result = SDL_hid_send_feature_report(Device.Device, Data.GetData(), Data.Num());
	if (Result == -1)
	{
		FJoystickLogManager::Get()->LogSDLError(TEXT("SDL_hid_send_feature_report failed"));
		return false;
	}

	return true;
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
		FJoystickLogManager::Get()->LogSDLError(TEXT("SDL_hid_open failed"));
		return false;
	}

	Device.Device = HIDDevice;
	return true;
}
