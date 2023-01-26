#pragma once

#include "ForceFeedback/Types/ForceFeedbackDirectionType.h"

#include "ForceFeedbackEffectDirectionData.Generated.h"

USTRUCT(BlueprintType)
struct JOYSTICKPLUGIN_API FForceFeedbackEffectDirectionData
{
	GENERATED_BODY()

	FForceFeedbackEffectDirectionData()
		: DirectionType(EForceFeedbackDirectionType::CARTESIAN)
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

		HapticDirection.dir[0] = FMath::Clamp(Direction.X * INT32_MAX, INT32_MIN, INT32_MAX);
		HapticDirection.dir[1] = FMath::Clamp(Direction.Y * INT32_MAX, INT32_MIN, INT32_MAX);
		HapticDirection.dir[2] = FMath::Clamp(Direction.Z * INT32_MAX, INT32_MIN, INT32_MAX);

		switch (DirectionType)
		{
			case EForceFeedbackDirectionType::POLAR:
				HapticDirection.type = SDL_HAPTIC_POLAR;
				break;
			case EForceFeedbackDirectionType::SPHERICAL:
				HapticDirection.type = SDL_HAPTIC_SPHERICAL;
				break;
			default:
			case EForceFeedbackDirectionType::CARTESIAN:
				HapticDirection.type = SDL_HAPTIC_CARTESIAN;
				break;
		}

		return HapticDirection;
	};
};
