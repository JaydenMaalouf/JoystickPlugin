#pragma once


#include "ForceFeedback/Types/ForceFeedbackDirectionType.h"

THIRD_PARTY_INCLUDES_START

#include "SDL_haptic.h"

THIRD_PARTY_INCLUDES_END

#include "ForceFeedbackEffectDirectionData.Generated.h"

USTRUCT(BlueprintType)
struct JOYSTICKPLUGIN_API FForceFeedbackEffectDirectionData
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		EForceFeedbackDirectionType DirectionType = EForceFeedbackDirectionType::CARTESIAN;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (ToolTip = "A vector describing the direction and magnitude of the effect on each axis. Each individual axis has a range of -1.0 to 1.0 and is independent of the other axes. Specifying a negative value for an axis reverses the input values from the axis."))
		FVector Direction = FVector::ZeroVector;

	SDL_HapticDirection ToSDLDirection()
	{
		SDL_HapticDirection hapticDirection;

		hapticDirection.dir[0] = Sint32(Direction.X * INT32_MAX);
		hapticDirection.dir[1] = Sint32(Direction.Y * INT32_MAX);
		hapticDirection.dir[2] = Sint32(Direction.Z * INT32_MAX);

		switch (DirectionType) {
		case EForceFeedbackDirectionType::POLAR:
			hapticDirection.type = SDL_HAPTIC_POLAR;
			break;
		case EForceFeedbackDirectionType::CARTESIAN:
			hapticDirection.type = SDL_HAPTIC_CARTESIAN;
			break;
		case EForceFeedbackDirectionType::SPHERICAL:
			hapticDirection.type = SDL_HAPTIC_SPHERICAL;
			break;
		case EForceFeedbackDirectionType::STEERING_AXIS:
			hapticDirection.type = SDL_HAPTIC_FIRST_AXIS;
			break;
		}

		return hapticDirection;
	};

};

