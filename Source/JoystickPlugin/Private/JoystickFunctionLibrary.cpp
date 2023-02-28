// JoystickPlugin is licensed under the MIT License.
// Copyright Jayden Maalouf. All Rights Reserved.

#include "JoystickFunctionLibrary.h"
#include "JoystickInputSettings.h"

THIRD_PARTY_INCLUDES_START

#include "SDL_joystick.h"

THIRD_PARTY_INCLUDES_END

FVector2D UJoystickFunctionLibrary::POVAxis(const EJoystickPointOfViewDirection Direction)
{
	switch (Direction)
	{
		case EJoystickPointOfViewDirection::None:
			return FVector2D(0, 0);
		case EJoystickPointOfViewDirection::Up:
			return FVector2D(0, 1);
		case EJoystickPointOfViewDirection::Up_Right:
			return FVector2D(1, 1);
		case EJoystickPointOfViewDirection::Right:
			return FVector2D(1, 0);
		case EJoystickPointOfViewDirection::Down_Right:
			return FVector2D(1, -1);
		case EJoystickPointOfViewDirection::Down:
			return FVector2D(0, -1);
		case EJoystickPointOfViewDirection::Down_Left:
			return FVector2D(-1, -1);
		case EJoystickPointOfViewDirection::Left:
			return FVector2D(-1, 0);
		case EJoystickPointOfViewDirection::Up_Left:
			return FVector2D(-1, 1);
		default:
			return FVector2D(0, 0);
	}
}

EJoystickPointOfViewDirection UJoystickFunctionLibrary::HatValueToDirection(const int8 Value)
{
	switch (Value)
	{
		case SDL_HAT_CENTERED: return EJoystickPointOfViewDirection::None;
		case SDL_HAT_UP: return EJoystickPointOfViewDirection::Up;
		case SDL_HAT_RIGHTUP: return EJoystickPointOfViewDirection::Up_Right;
		case SDL_HAT_RIGHT: return EJoystickPointOfViewDirection::Right;
		case SDL_HAT_RIGHTDOWN: return EJoystickPointOfViewDirection::Down_Right;
		case SDL_HAT_DOWN: return EJoystickPointOfViewDirection::Down;
		case SDL_HAT_LEFTDOWN: return EJoystickPointOfViewDirection::Down_Left;
		case SDL_HAT_LEFT: return EJoystickPointOfViewDirection::Left;
		case SDL_HAT_LEFTUP: return EJoystickPointOfViewDirection::Up_Left;
		default:
			return EJoystickPointOfViewDirection::None;
	}
}
