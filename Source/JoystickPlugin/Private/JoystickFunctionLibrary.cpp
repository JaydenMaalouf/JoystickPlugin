// JoystickPlugin is licensed under the MIT License.
// Copyright Jayden Maalouf. All Rights Reserved.

#include "JoystickFunctionLibrary.h"

#include "Engine/Engine.h"
#include "JoystickInputDevice.h"
#include "JoystickSubsystem.h"

THIRD_PARTY_INCLUDES_START

#include "SDL_joystick.h"

THIRD_PARTY_INCLUDES_END

FVector2D UJoystickFunctionLibrary::HatDirectionToFVector2D(const EHatDirection Direction)
{
	switch (Direction)
	{
	case EHatDirection::None:
		return FVector2D(0, 0);
	case EHatDirection::Up:
		return FVector2D(0, 1);
	case EHatDirection::Right_Up:
		return FVector2D(1, 1);
	case EHatDirection::Right:
		return FVector2D(1, 0);
	case EHatDirection::Right_Down:
		return FVector2D(1, -1);
	case EHatDirection::Down:
		return FVector2D(0, -1);
	case EHatDirection::Left_Down:
		return FVector2D(-1, -1);
	case EHatDirection::Left:
		return FVector2D(-1, 0);
	case EHatDirection::Left_Up:
		return FVector2D(-1, 1);
	default:
		return FVector2D(0, 0);
	}
}

float UJoystickFunctionLibrary::NormalizeAxisRaw(const int16 Value)
{
	return Value < 0
		       ? static_cast<float>(Value) / 32768.0f
		       : static_cast<float>(Value) / 32767.0f;
}

EHatDirection UJoystickFunctionLibrary::HatValueToDirection(const int8 Value)
{
	switch (Value)
	{
	case SDL_HAT_CENTERED: return EHatDirection::None;
	case SDL_HAT_UP: return EHatDirection::Up;
	case SDL_HAT_RIGHTUP: return EHatDirection::Right_Up;
	case SDL_HAT_RIGHT: return EHatDirection::Right;
	case SDL_HAT_RIGHTDOWN: return EHatDirection::Right_Down;
	case SDL_HAT_DOWN: return EHatDirection::Down;
	case SDL_HAT_LEFTDOWN: return EHatDirection::Left_Down;
	case SDL_HAT_LEFT: return EHatDirection::Left;
	case SDL_HAT_LEFTUP: return EHatDirection::Left_Up;
	default:
		return EHatDirection::None;
	}
}

FString UJoystickFunctionLibrary::HatDirectionAsString(EHatDirection Value)
{
	static const UEnum* EnumPtr = StaticEnum<EHatDirection>();
	if (!EnumPtr)
	{
		return TEXT("Unknown");
	}

	return EnumPtr->GetNameStringByValue(static_cast<int64>(Value));
}

bool UJoystickFunctionLibrary::IsJoystickKey(IN const FKey& Key)
{
	if (!GEngine)
	{
		return false;
	}

	const UJoystickSubsystem* JoystickSubsystem = GEngine->GetEngineSubsystem<UJoystickSubsystem>();
	if (!JoystickSubsystem)
	{
		return false;
	}

	const FJoystickInputDevice* InputDevice = JoystickSubsystem->GetInputDevice();
	if (!InputDevice)
	{
		return false;
	}

	return InputDevice->GetInstanceIdByKey(Key) != -1;
}
