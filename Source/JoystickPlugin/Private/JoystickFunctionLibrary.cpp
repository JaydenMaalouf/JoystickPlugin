/*
*
* Copyright (C) <2014> samiljan <Sam Persson>, tsky <thomas.kollakowksy@w-hs.de>
* All rights reserved.
*
* This software may be modified and distributed under the terms
* of the BSD license.  See the LICENSE file for details.
*/

#include "JoystickFunctionLibrary.h"

THIRD_PARTY_INCLUDES_START

#include "SDL.h"
#include "SDL_joystick.h"

THIRD_PARTY_INCLUDES_END

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

EJoystickPOVDirection UJoystickFunctionLibrary::HatValueToDirection(const int8 Value)
{
	switch (Value)
	{
	case SDL_HAT_CENTERED:  return EJoystickPOVDirection::DIRECTION_NONE;
	case SDL_HAT_UP:        return EJoystickPOVDirection::DIRECTION_UP;
	case SDL_HAT_RIGHTUP:   return EJoystickPOVDirection::DIRECTION_UP_RIGHT;
	case SDL_HAT_RIGHT:	    return EJoystickPOVDirection::DIRECTION_RIGHT;
	case SDL_HAT_RIGHTDOWN: return EJoystickPOVDirection::DIRECTION_DOWN_RIGHT;
	case SDL_HAT_DOWN:	    return EJoystickPOVDirection::DIRECTION_DOWN;
	case SDL_HAT_LEFTDOWN:  return EJoystickPOVDirection::DIRECTION_DOWN_LEFT;
	case SDL_HAT_LEFT:	    return EJoystickPOVDirection::DIRECTION_LEFT;
	case SDL_HAT_LEFTUP:    return EJoystickPOVDirection::DIRECTION_UP_LEFT;
	default:
		//UE_LOG(LogTemp, Warning, TEXT("Warning, POV unhandled case. %d"), (int32)value);
		return EJoystickPOVDirection::DIRECTION_NONE;
	}
}
