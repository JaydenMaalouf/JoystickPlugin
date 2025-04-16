// JoystickPlugin is licensed under the MIT License.
// Copyright Jayden Maalouf. All Rights Reserved.

#pragma once

#include "Runtime/Launch/Resources/Version.h"
#include "ForceFeedback/Types/ForceFeedbackDirectionType.h"

#include "ForceFeedbackEffectDirectionData.generated.h"

USTRUCT(BlueprintType)
struct JOYSTICKPLUGIN_API FForceFeedbackEffectDirectionData
{
	GENERATED_BODY()

	FForceFeedbackEffectDirectionData()
		: DirectionType(EForceFeedbackDirectionType::Cartesian)
		  , Direction(FVector::ZeroVector)
	{
	}

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Joystick|Force Feedback|Direction")
	EForceFeedbackDirectionType DirectionType;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Joystick|Force Feedback|Direction",
		meta = (ToolTip =
			"A vector describing the direction and magnitude of the effect on each axis. Each individual axis has a range of -1.0 to 1.0 and is independent of the other axes. Specifying a negative value for an axis reverses the input values from the axis."
			, EditCondition = "DirectionType != EForceFeedbackDirectionType::SteeringAxis", EditConditionHides))
	FIntVector Direction;

	SDL_HapticDirection ToSDLDirection() const
	{
		SDL_HapticDirection HapticDirection;

		switch (DirectionType)
		{
		case EForceFeedbackDirectionType::Polar:
			HapticDirection.type = SDL_HAPTIC_POLAR;
			HapticDirection.dir[0] = FMath::Clamp<Sint32>(Direction.X, 0, 36000);
			HapticDirection.dir[1] = 0;
			HapticDirection.dir[2] = 0;
			break;
		case EForceFeedbackDirectionType::Spherical:
			HapticDirection.type = SDL_HAPTIC_SPHERICAL;
			HapticDirection.dir[0] = FMath::Clamp<Sint32>(Direction.X, 0, 36000);
			HapticDirection.dir[1] = FMath::Clamp<Sint32>(Direction.Y, -9000, 9000);
			HapticDirection.dir[2] = 0;
			break;
		case EForceFeedbackDirectionType::Cartesian:
			HapticDirection.type = SDL_HAPTIC_CARTESIAN;
			HapticDirection.dir[0] = FMath::Clamp<Sint32>(Direction.X, INT32_MIN, INT32_MAX);
			HapticDirection.dir[1] = FMath::Clamp<Sint32>(Direction.Y, INT32_MIN, INT32_MAX);
			HapticDirection.dir[2] = FMath::Clamp<Sint32>(Direction.Z, INT32_MIN, INT32_MAX);
			break;
		default:
		case EForceFeedbackDirectionType::SteeringAxis:
#if ENGINE_MAJOR_VERSION == 5
			HapticDirection.type = SDL_HAPTIC_STEERING_AXIS;
#else
				HapticDirection.type = SDL_HAPTIC_FIRST_AXIS;
#endif
			HapticDirection.dir[0] = 0;
			HapticDirection.dir[1] = 0;
			HapticDirection.dir[2] = 0;
			break;
		}

		return HapticDirection;
	};
};
