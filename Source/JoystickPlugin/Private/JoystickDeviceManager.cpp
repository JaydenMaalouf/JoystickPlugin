
/*
*
* Copyright (C) <2014> samiljan <Sam Persson>, tsky <thomas.kollakowksy@w-hs.de>
* All rights reserved.
*
* This software may be modified and distributed under the terms
* of the BSD license.  See the LICENSE file for details.
*/

#include "JoystickDeviceManager.h"

#include "DeviceSDL.h"
#include "JoystickPlugin.h"

void UJoystickDeviceManager::SetInputDevice(const TSharedPtr<FJoystickInputDevice> NewInputDevice)
{
	InputDevice = NewInputDevice;
}

int32 UJoystickDeviceManager::JoystickCount() const
{
	if (!InputDevice.IsValid())
	{
		return -1;
	}

	return InputDevice->GetDeviceCount();
}

FJoystickDeviceData UJoystickDeviceManager::GetJoystickData(const int32 DeviceId) const
{
	if (!InputDevice.IsValid())
	{
		return FJoystickDeviceData();
	}

	return InputDevice->GetDeviceData(DeviceId);
}

FJoystickInfo UJoystickDeviceManager::GetJoystickInfo(const int32 DeviceId) const
{	
	if (!InputDevice.IsValid())
	{
		return FJoystickInfo();
	}

	return InputDevice->GetDeviceInfo(DeviceId);
}

FDeviceSDL* UJoystickDeviceManager::GetDeviceSDL() const
{
	if (!InputDevice.IsValid())
	{
		return nullptr;
	}
	
	return InputDevice->GetDeviceSDL();
}

FDeviceInfoSDL* UJoystickDeviceManager::GetDeviceInfo(const int32 DeviceId) const
{
	if (!InputDevice.IsValid())
	{
		return nullptr;
	}

	FDeviceSDL* DeviceSDL = InputDevice->GetDeviceSDL();
	if (DeviceSDL == nullptr)
	{
		return nullptr;
	}

	FDeviceInfoSDL* DeviceInfoSDL = DeviceSDL->GetDevice(DeviceId);
	if (DeviceInfoSDL == nullptr)
	{
		return nullptr;
	}
	
	return DeviceInfoSDL;
}

void UJoystickDeviceManager::MapJoystickDeviceToPlayer(int32 DeviceId, int32 Player)
{
	//TODO: Implement
}

void UJoystickDeviceManager::IgnoreGameControllers(const bool IgnoreControllers) const
{
	if (!InputDevice.IsValid())
	{
		return;
	}
	
	InputDevice->IgnoreGameControllers(IgnoreControllers);
}

void UJoystickDeviceManager::ReinitialiseJoystickData(const int32 DeviceId) const
{	
	if (!InputDevice.IsValid())
	{
		return;
	}

	InputDevice->ReinitialiseJoystickData(DeviceId);
}

TArray<int32> UJoystickDeviceManager::GetDeviceIds() const
{
	TArray<int32> DeviceIds;
	if (!InputDevice.IsValid())
	{
		return DeviceIds;
	}

	InputDevice->GetDeviceIds(DeviceIds);
	return DeviceIds;
}
