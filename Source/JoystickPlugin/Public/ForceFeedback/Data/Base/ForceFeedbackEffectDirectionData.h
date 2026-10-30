// JoystickPlugin is licensed under the MIT License.
// Copyright Jayden Maalouf 2026. All Rights Reserved.

#pragma once

#include "Runtime/Launch/Resources/Version.h"
#include "ForceFeedback/ForceFeedbackConversion.h"
#include "ForceFeedback/Types/ForceFeedbackDirectionType.h"

THIRD_PARTY_INCLUDES_START

#include "JoystickSDL.h"

THIRD_PARTY_INCLUDES_END

#include "ForceFeedbackEffectDirectionData.generated.h"

USTRUCT(BlueprintType)
struct JOYSTICKPLUGIN_API FForceFeedbackEffectDirectionData
{
	GENERATED_BODY()

	FForceFeedbackEffectDirectionData()
		: DirectionType(EForceFeedbackDirectionType::Cartesian)
		  , Direction(1.0f, 0.0f, 0.0f)
	{
	}

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Joystick|Force Feedback|Direction")
	EForceFeedbackDirectionType DirectionType;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Joystick|Force Feedback|Direction",
		meta=(ToolTip=
			"The direction the force comes from, encoded according to the Direction Type. Cartesian: X, Y and Z are whole number axis positions where only the ratio matters, e.g. East is (1, 0, 0) and North is (0, -1, 0). Polar: X is the angle in hundredths of a degree clockwise from North (0 to 36000, East is 9000), Y and Z are unused. Spherical: X is the angle in hundredths of a degree from (1, 0) rotated towards (0, 1) (0 to 36000), Y is the elevation in hundredths of a degree (-9000 to 9000), Z is unused."
			, EditCondition="DirectionType != EForceFeedbackDirectionType::SteeringAxis", EditConditionHides))
	FIntVector Direction;

	SDL_HapticDirection ToSDLDirection() const
	{
		// Zeroed including its padding, so effects holding the same direction compare as equal
		SDL_HapticDirection HapticDirection;
		SDL_zero(HapticDirection);

		switch (DirectionType)
		{
			case EForceFeedbackDirectionType::Polar:
				HapticDirection.type = SDL_HAPTIC_POLAR;
				HapticDirection.dir[0] = FForceFeedbackConversion::ToDirectionAngle(Direction.X);
				HapticDirection.dir[1] = 0;
				HapticDirection.dir[2] = 0;
				break;
			case EForceFeedbackDirectionType::Spherical:
				HapticDirection.type = SDL_HAPTIC_SPHERICAL;
				HapticDirection.dir[0] = FForceFeedbackConversion::ToDirectionAngle(Direction.X);
				HapticDirection.dir[1] = FForceFeedbackConversion::ToDirectionElevation(Direction.Y);
				HapticDirection.dir[2] = 0;
				break;
			case EForceFeedbackDirectionType::Cartesian:
				HapticDirection.type = SDL_HAPTIC_CARTESIAN;
				HapticDirection.dir[0] = Direction.X;
				HapticDirection.dir[1] = Direction.Y;
				HapticDirection.dir[2] = Direction.Z;
				break;
			default:
			case EForceFeedbackDirectionType::SteeringAxis:
				HapticDirection.type = SDL_HAPTIC_STEERING_AXIS;
				HapticDirection.dir[0] = 0;
				HapticDirection.dir[1] = 0;
				HapticDirection.dir[2] = 0;
				break;
		}

		return HapticDirection;
	}
};
