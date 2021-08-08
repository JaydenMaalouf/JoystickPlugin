/*
*
* Copyright (C) <2014> samiljan <Sam Persson>, tsky <thomas.kollakowksy@w-hs.de>
* All rights reserved.
*
* This software may be modified and distributed under the terms
* of the BSD license.  See the LICENSE file for details.
*/

#include "JoystickFunctionLibrary.h"
#include "IJoystickPlugin.h"
#include "JoystickInterface.h"
#include "JoystickDevice.h"
#include "JoystickPlugin.h"
#include <Engine.h>

UJoystickFunctionLibrary::UJoystickFunctionLibrary(const class FObjectInitializer& PCIP)
	: Super(PCIP)
{

}

FVector2D UJoystickFunctionLibrary::POVAxis(EJoystickPOVDirection Direction)
{
	return ::POVAxis(Direction);
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
