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

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Force Feedback|Direction|Data")
	EForceFeedbackDirectionType DirectionType;

	UPROPERTY(BlueprintReadWrite, EditAnywhere,
		meta = (ToolTip =
			"A vector describing the direction and magnitude of the effect on each axis. Each individual axis has a range of -1.0 to 1.0 and is independent of the other axes. Specifying a negative value for an axis reverses the input values from the axis."
			, UIMin="-1", UIMax="1", ClampMin="-1", ClampMax="1"), Category = "Force Feedback|Direction|Data")
	FVector Direction;

	SDL_HapticDirection ToSDLDirection() const
	{
		SDL_HapticDirection HapticDirection;

		HapticDirection.dir[0] = FMath::Clamp<Sint32>(Direction.X * INT32_MAX, INT32_MIN, INT32_MAX);
		HapticDirection.dir[1] = FMath::Clamp<Sint32>(Direction.Y * INT32_MAX, INT32_MIN, INT32_MAX);
		HapticDirection.dir[2] = FMath::Clamp<Sint32>(Direction.Z * INT32_MAX, INT32_MIN, INT32_MAX);

		switch (DirectionType)
		{
			case EForceFeedbackDirectionType::Polar:
				HapticDirection.type = SDL_HAPTIC_POLAR;
				break;
			case EForceFeedbackDirectionType::Spherical:
				HapticDirection.type = SDL_HAPTIC_SPHERICAL;
				break;
			case EForceFeedbackDirectionType::Cartesian:
				HapticDirection.type = SDL_HAPTIC_CARTESIAN;
				break;
			default:
			case EForceFeedbackDirectionType::SteeringAxis:
#if ENGINE_MAJOR_VERSION == 5
				HapticDirection.type = SDL_HAPTIC_STEERING_AXIS;
#else
				HapticDirection.type = SDL_HAPTIC_FIRST_AXIS;
#endif
				break;
		}

		return HapticDirection;
	};
};
