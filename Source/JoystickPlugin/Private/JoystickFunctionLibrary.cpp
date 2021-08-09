/*
*
* Copyright (C) <2014> samiljan <Sam Persson>, tsky <thomas.kollakowksy@w-hs.de>
* All rights reserved.
*
* This software may be modified and distributed under the terms
* of the BSD license.  See the LICENSE file for details.
*/

#include "JoystickFunctionLibrary.h"
#include "Interfaces/JoystickPluginInterface.h"
#include "Interfaces/JoystickInterface.h"
#include "JoystickDevice.h"
#include "JoystickPlugin.h"

int32 UJoystickFunctionLibrary::GetDefaultDeviceId()
{
	TSharedPtr<FJoystickDevice> Device = static_cast<FJoystickPlugin&>(IJoystickPlugin::Get()).JoystickDevice;
	if (Device->InputDevices.Num() == 0) 
	{
		return -1;
	}
	
	TArray<FJoystickInfo> deviceArray;
	Device->InputDevices.GenerateValueArray(deviceArray);

	if (deviceArray.Num() == 0) 
	{
		return -1;
	}

	return deviceArray[0].DeviceId;
}

FVector2D UJoystickFunctionLibrary::POVAxis(EJoystickPOVDirection Direction)
{
	switch (Direction)
	{
	case EJoystickPOVDirection::DIRECTION_NONE:
		return FVector2D(0, 0);
	case EJoystickPOVDirection::DIRECTION_UP:
		return FVector2D(0, 1);
	case EJoystickPOVDirection::DIRECTION_UP_RIGHT:
		return FVector2D(1, 1);
	case EJoystickPOVDirection::DIRECTION_RIGHT:
		return FVector2D(1, 0);
	case EJoystickPOVDirection::DIRECTION_DOWN_RIGHT:
		return FVector2D(1, -1);
	case EJoystickPOVDirection::DIRECTION_DOWN:
		return FVector2D(0, -1);
	case EJoystickPOVDirection::DIRECTION_DOWN_LEFT:
		return FVector2D(-1, -1);
	case EJoystickPOVDirection::DIRECTION_LEFT:
		return FVector2D(-1, 0);
	case EJoystickPOVDirection::DIRECTION_UP_LEFT:
		return FVector2D(-1, 1);
	default:
		return FVector2D(0, 0);
	}
}

FJoystickInfo UJoystickFunctionLibrary::GetJoystick(int32 DeviceId)
{
	if (!IJoystickPlugin::IsAvailable())
		return FJoystickInfo();

	TSharedPtr<FJoystickDevice> Device = static_cast<FJoystickPlugin&>(IJoystickPlugin::Get()).JoystickDevice;
	if (!Device->InputDevices.Contains(FDeviceId(DeviceId)))
		return FJoystickInfo();

	return Device->InputDevices[FDeviceId(DeviceId)];
}

FJoystickState UJoystickFunctionLibrary::GetJoystickState(int32 DeviceId)
{
	if (!IJoystickPlugin::IsAvailable())
		return FJoystickState();

	TSharedPtr<FJoystickDevice> Device = static_cast<FJoystickPlugin&>(IJoystickPlugin::Get()).JoystickDevice;
	if (!Device->InputDevices.Contains(FDeviceId(DeviceId)))
		return FJoystickState();

	return Device->CurrentState[FDeviceId(DeviceId)];
}

FJoystickState UJoystickFunctionLibrary::GetPreviousJoystickState(int32 DeviceId)
{
	if (!IJoystickPlugin::IsAvailable())
		return FJoystickState();

	TSharedPtr<FJoystickDevice> Device = static_cast<FJoystickPlugin&>(IJoystickPlugin::Get()).JoystickDevice;
	if (!Device->InputDevices.Contains(FDeviceId(DeviceId)))
		return FJoystickState();

	return Device->PreviousState[FDeviceId(DeviceId)];
}

int32 UJoystickFunctionLibrary::JoystickCount()
{
	if (!IJoystickPlugin::IsAvailable())
		return 0;

	TSharedPtr<FJoystickDevice> Device = static_cast<FJoystickPlugin&>(IJoystickPlugin::Get()).JoystickDevice;
	return Device->InputDevices.Num();
}

void UJoystickFunctionLibrary::RegisterForJoystickEvents(UObject* Listener)
{
	if (!IJoystickPlugin::IsAvailable())
		return;

	TSharedPtr<FJoystickDevice> Device = static_cast<FJoystickPlugin&>(IJoystickPlugin::Get()).JoystickDevice;
	Device->AddEventListener(Listener);
}

void UJoystickFunctionLibrary::MapJoystickDeviceToPlayer(int32 DeviceId, int32 Player)
{
	if (!IJoystickPlugin::IsAvailable())
		return;

	TSharedPtr<FJoystickDevice> Device = static_cast<FJoystickPlugin&>(IJoystickPlugin::Get()).JoystickDevice;
	if (!Device->InputDevices.Contains(FDeviceId(DeviceId)))
		return;

	Device->InputDevices[FDeviceId(DeviceId)].Player = Player;
}

void UJoystickFunctionLibrary::IgnoreGameControllers(bool bIgnore)
{
	if (!IJoystickPlugin::IsAvailable()) return;

	TSharedPtr<FJoystickDevice> Device = static_cast<FJoystickPlugin&>(IJoystickPlugin::Get()).JoystickDevice;
	Device->IgnoreGameControllers(bIgnore);
}
