
/*
*
* Copyright (C) <2014> samiljan <Sam Persson>, tsky <thomas.kollakowksy@w-hs.de>
* All rights reserved.
*
* This software may be modified and distributed under the terms
* of the BSD license.  See the LICENSE file for details.
*/

#include "JoystickDeviceManager.h"

#include "Engine/Engine.h"
#include "JoystickSubsystem.h"

int32 UJoystickDeviceManager::GetJoystickCount() const
{
	UJoystickSubsystem* JoystickSubsystem = GEngine->GetEngineSubsystem<UJoystickSubsystem>();
	if (JoystickSubsystem == nullptr)
	{
		return -1;
	}

	return JoystickSubsystem->GetJoystickCount();
}

int32 UJoystickDeviceManager::GetValidDeviceCount() const
{
	UJoystickSubsystem* JoystickSubsystem = GEngine->GetEngineSubsystem<UJoystickSubsystem>();
	if (JoystickSubsystem == nullptr)
	{
		return -1;
	}

	FJoystickInputDevice* InputDevice = JoystickSubsystem->GetInputDevice();
	if (InputDevice == nullptr)
	{
		return -1;
	}

	return InputDevice->GetDeviceCount();
}

FJoystickDeviceData UJoystickDeviceManager::GetJoystickData(const int32 DeviceId) const
{
	UJoystickSubsystem* JoystickSubsystem = GEngine->GetEngineSubsystem<UJoystickSubsystem>();
	if (JoystickSubsystem == nullptr)
	{
		return FJoystickDeviceData();
	}

	FJoystickInputDevice* InputDevice = JoystickSubsystem->GetInputDevice();
	if (InputDevice == nullptr)
	{
		return FJoystickDeviceData();
	}

	return InputDevice->GetDeviceData(DeviceId);
}

FJoystickInfo UJoystickDeviceManager::GetJoystickInfo(const int32 DeviceId) const
{	
	UJoystickSubsystem* JoystickSubsystem = GEngine->GetEngineSubsystem<UJoystickSubsystem>();
	if (JoystickSubsystem == nullptr)
	{
		return FJoystickInfo();
	}

	FJoystickInputDevice* InputDevice = JoystickSubsystem->GetInputDevice();
	if (InputDevice == nullptr)
	{
		return FJoystickInfo();
	}

	return InputDevice->GetDeviceInfo(DeviceId);
}

void UJoystickDeviceManager::MapJoystickDeviceToPlayer(const int32 DeviceId, const int32 PlayerId)
{
	UJoystickSubsystem* JoystickSubsystem = GEngine->GetEngineSubsystem<UJoystickSubsystem>();
	if (JoystickSubsystem == nullptr)
	{
		return;
	}

	FJoystickInputDevice* InputDevice = JoystickSubsystem->GetInputDevice();
	if (InputDevice == nullptr)
	{
		return;
	}
	
	InputDevice->SetPlayerOwnership(DeviceId, PlayerId);
}

void UJoystickDeviceManager::SetIgnoreGameControllers(const bool IgnoreControllers)
{
	UJoystickSubsystem* JoystickSubsystem = GEngine->GetEngineSubsystem<UJoystickSubsystem>();
	if (JoystickSubsystem == nullptr)
	{
		return;
	}

	JoystickSubsystem->SetIgnoreGameControllers(IgnoreControllers);
}

TArray<int32> UJoystickDeviceManager::GetDeviceIds() const
{
	TArray<int32> DeviceIds;
	UJoystickSubsystem* JoystickSubsystem = GEngine->GetEngineSubsystem<UJoystickSubsystem>();
	if (JoystickSubsystem == nullptr)
	{
		return DeviceIds;
	}

	FJoystickInputDevice* InputDevice = JoystickSubsystem->GetInputDevice();
	if (InputDevice == nullptr)
	{
		return DeviceIds;
	}

	InputDevice->GetDeviceIds(DeviceIds);
	return DeviceIds;
}
