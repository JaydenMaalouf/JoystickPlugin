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
#include "JoystickDeviceManager.h"
#include "JoystickPlugin.h"

UJoystickDeviceManager* UJoystickFunctionLibrary::GetJoystickManager()
{
	const FJoystickPlugin& JoystickPlugin = static_cast<FJoystickPlugin&>(IJoystickPlugin::Get());
	if (!JoystickPlugin.IsAvailable())
	{
		return nullptr;
	}

	return JoystickPlugin.GetDeviceManager();
}

UJoystickHapticDeviceManager* UJoystickFunctionLibrary::GetJoystickHapticManager()
{
	const FJoystickPlugin& JoystickPlugin = static_cast<FJoystickPlugin&>(IJoystickPlugin::Get());
	if (!JoystickPlugin.IsAvailable())
	{
		return nullptr;
	}

	return JoystickPlugin.GetHapticDeviceManager();
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
