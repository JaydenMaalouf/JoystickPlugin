#include "ForcedFeedback/Types/ForcedFeedbackEffectCondition.h"

SDL_HapticEffect UForcedFeedbackEffectCondition::ToSDLEffect()
{
	SDL_HapticEffect effect = Super::ToSDLEffect();
	FForcedFeedbackData effectData = GetEffect();

	//TODO ADD A CHECK ON EFFECT TYPE

	switch (EffectType) {
	case(EForcedFeedbackEffectType::SPRING): {
		effect.type = SDL_HAPTIC_SPRING;
		break;
	}
	case(EForcedFeedbackEffectType::DAMPER): {
		effect.type = SDL_HAPTIC_DAMPER;
		break;
	}
	case(EForcedFeedbackEffectType::FRICTION): {
		effect.type = SDL_HAPTIC_FRICTION;
		break;
	}
	case(EForcedFeedbackEffectType::INERTIA): {
		effect.type = SDL_HAPTIC_INERTIA;
		break;
	}
	default: {}
	}

	switch (effectData.DirectionType)
	{
	case(EForcedFeedbackDirectionType::CARTESIAN): {
		effect.condition.direction.type = SDL_HAPTIC_CARTESIAN;
		break;
	}
	case(EForcedFeedbackDirectionType::POLAR): {
		effect.condition.direction.type = SDL_HAPTIC_POLAR;
		break;
	}
	case(EForcedFeedbackDirectionType::SPHERICAL): {
		effect.condition.direction.type = SDL_HAPTIC_SPHERICAL;
		break;
	}
	default: {}
	}

	effect.condition.direction.dir[0] = Sint32(effectData.Direction.X * INT_MAX);
	effect.condition.direction.dir[1] = Sint32(effectData.Direction.Y * INT_MAX);
	effect.condition.direction.dir[2] = Sint32(effectData.Direction.Z * INT_MAX);
	effect.condition.delay = Uint16(effectData.Delay * 1000.0f);
	effect.condition.center[0] = Sint16(effectData.Center.X * INT16_MAX);
	effect.condition.center[1] = Sint16(effectData.Center.Y * INT16_MAX);
	effect.condition.center[2] = Sint16(effectData.Center.Z * INT16_MAX);
	effect.condition.deadband[0] = FMath::Clamp(Uint16(effectData.Deadband.X * float(UINT16_MAX)), Uint16(0), Uint16(UINT16_MAX));
	effect.condition.deadband[1] = FMath::Clamp(Uint16(effectData.Deadband.Y * float(UINT16_MAX)), Uint16(0), Uint16(UINT16_MAX));
	effect.condition.deadband[2] = FMath::Clamp(Uint16(effectData.Deadband.Z * float(UINT16_MAX)), Uint16(0), Uint16(UINT16_MAX));
	effect.condition.left_coeff[0] = FMath::Clamp(Sint16(effectData.LeftCoeff.X * float(INT16_MAX)), Sint16(INT16_MIN), Sint16(INT16_MAX));
	effect.condition.left_coeff[1] = FMath::Clamp(Sint16(effectData.LeftCoeff.Y * float(INT16_MAX)), Sint16(INT16_MIN), Sint16(INT16_MAX));
	effect.condition.left_coeff[2] = FMath::Clamp(Sint16(effectData.LeftCoeff.Z * float(INT16_MAX)), Sint16(INT16_MIN), Sint16(INT16_MAX));
	effect.condition.left_sat[0] = FMath::Clamp(Uint16(effectData.LeftSat.X * float(UINT16_MAX)), Uint16(0), Uint16(UINT16_MAX));
	effect.condition.left_sat[1] = FMath::Clamp(Uint16(effectData.LeftSat.Y * float(UINT16_MAX)), Uint16(0), Uint16(UINT16_MAX));
	effect.condition.left_sat[2] = FMath::Clamp(Uint16(effectData.LeftSat.Z * float(UINT16_MAX)), Uint16(0), Uint16(UINT16_MAX));
	effect.condition.right_coeff[0] = FMath::Clamp(Sint16(effectData.RightCoeff.X * float(INT16_MAX)), Sint16(INT16_MIN), Sint16(INT16_MAX));
	effect.condition.right_coeff[1] = FMath::Clamp(Sint16(effectData.RightCoeff.Y * float(INT16_MAX)), Sint16(INT16_MIN), Sint16(INT16_MAX));
	effect.condition.right_coeff[2] = FMath::Clamp(Sint16(effectData.RightCoeff.Z * float(INT16_MAX)), Sint16(INT16_MIN), Sint16(INT16_MAX));
	effect.condition.right_sat[0] = FMath::Clamp(Uint16(effectData.RightSat.X * float(UINT16_MAX)), Uint16(0), Uint16(UINT16_MAX));
	effect.condition.right_sat[1] = FMath::Clamp(Uint16(effectData.RightSat.Y * float(UINT16_MAX)), Uint16(0), Uint16(UINT16_MAX));
	effect.condition.right_sat[2] = FMath::Clamp(Uint16(effectData.RightSat.Z * float(UINT16_MAX)), Uint16(0), Uint16(UINT16_MAX));

	if (effectData.InfiniteLength)
	{
		effect.condition.length = SDL_HAPTIC_INFINITY;
	}
	else {
		effect.condition.length = Uint32(effectData.Length * 1000.0f);
	}

	return effect;
}