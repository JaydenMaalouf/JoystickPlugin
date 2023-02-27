// JoystickPlugin is licensed under the MIT License.
// Copyright Jayden Maalouf. All Rights Reserved.

#include "JoystickFunctionLibrary.h"
#include "JoystickInputSettings.h"

THIRD_PARTY_INCLUDES_START

#include "SDL.h"
#include "SDL_joystick.h"

THIRD_PARTY_INCLUDES_END

FVector2D UJoystickFunctionLibrary::POVAxis(const EJoystickPOVDirection Direction)
{
	switch (Direction)
	{
		case EJoystickPOVDirection::Direction_None:
			return FVector2D(0, 0);
		case EJoystickPOVDirection::Direction_Up:
			return FVector2D(0, 1);
		case EJoystickPOVDirection::Direction_Up_Right:
			return FVector2D(1, 1);
		case EJoystickPOVDirection::Direction_Right:
			return FVector2D(1, 0);
		case EJoystickPOVDirection::Direction_Down_Right:
			return FVector2D(1, -1);
		case EJoystickPOVDirection::Direction_Down:
			return FVector2D(0, -1);
		case EJoystickPOVDirection::Direction_Down_Left:
			return FVector2D(-1, -1);
		case EJoystickPOVDirection::Direction_Left:
			return FVector2D(-1, 0);
		case EJoystickPOVDirection::Direction_Up_Left:
			return FVector2D(-1, 1);
		default:
			return FVector2D(0, 0);
	}
}

EJoystickPOVDirection UJoystickFunctionLibrary::HatValueToDirection(const int8 Value)
{
	switch (Value)
	{
		case SDL_HAT_CENTERED: return EJoystickPOVDirection::Direction_None;
		case SDL_HAT_UP: return EJoystickPOVDirection::Direction_Up;
		case SDL_HAT_RIGHTUP: return EJoystickPOVDirection::Direction_Up_Right;
		case SDL_HAT_RIGHT: return EJoystickPOVDirection::Direction_Right;
		case SDL_HAT_RIGHTDOWN: return EJoystickPOVDirection::Direction_Down_Right;
		case SDL_HAT_DOWN: return EJoystickPOVDirection::Direction_Down;
		case SDL_HAT_LEFTDOWN: return EJoystickPOVDirection::Direction_Down_Left;
		case SDL_HAT_LEFT: return EJoystickPOVDirection::Direction_Left;
		case SDL_HAT_LEFTUP: return EJoystickPOVDirection::Direction_Up_Left;
		default:
			return EJoystickPOVDirection::Direction_None;
	}
}