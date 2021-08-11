#include "ForcedFeedback/Effects/ForcedFeedbackEffectCondition.h"
#include "ForcedFeedback/Types/ForcedFeedbackConditionEffectType.h"

SDL_HapticEffect UForcedFeedbackEffectCondition::ToSDLEffect()
{
	SDL_HapticEffect effect = Super::ToSDLEffect();

	switch (EffectData.EffectType) {
	case(EForcedFeedbackConditionEffectType::SPRING): {
		effect.type = SDL_HAPTIC_SPRING;
		break;
	}
	case(EForcedFeedbackConditionEffectType::DAMPER): {
		effect.type = SDL_HAPTIC_DAMPER;
		break;
	}
	case(EForcedFeedbackConditionEffectType::FRICTION): {
		effect.type = SDL_HAPTIC_FRICTION;
		break;
	}
	case(EForcedFeedbackConditionEffectType::INERTIA): {
		effect.type = SDL_HAPTIC_INERTIA;
		break;
	}
	default: {}
	}

	switch (EffectData.Direction.DirectionType)
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

	effect.condition.direction.dir[0] = Sint32(EffectData.Direction.Direction.X);
	effect.condition.direction.dir[1] = Sint32(EffectData.Direction.Direction.Y);
	effect.condition.direction.dir[2] = Sint32(EffectData.Direction.Direction.Z);
	effect.condition.delay = FMath::Clamp(Uint16(EffectData.Delay), Uint16(0), Uint16(UINT16_MAX));
	effect.condition.center[0] = FMath::Clamp(Sint16(EffectData.Center.X), Sint16(INT16_MIN), Sint16(INT16_MAX));
	effect.condition.center[1] = FMath::Clamp(Sint16(EffectData.Center.Y), Sint16(INT16_MIN), Sint16(INT16_MAX));
	effect.condition.center[2] = FMath::Clamp(Sint16(EffectData.Center.Z), Sint16(INT16_MIN), Sint16(INT16_MAX));
	effect.condition.deadband[0] = FMath::Clamp(Uint16(EffectData.Deadband.X), Uint16(0), Uint16(UINT16_MAX));
	effect.condition.deadband[1] = FMath::Clamp(Uint16(EffectData.Deadband.Y), Uint16(0), Uint16(UINT16_MAX));
	effect.condition.deadband[2] = FMath::Clamp(Uint16(EffectData.Deadband.Z), Uint16(0), Uint16(UINT16_MAX));
	effect.condition.left_coeff[0] = FMath::Clamp(Sint16(EffectData.LeftCoeff.X), Sint16(INT16_MIN), Sint16(INT16_MAX));
	effect.condition.left_coeff[1] = FMath::Clamp(Sint16(EffectData.LeftCoeff.Y), Sint16(INT16_MIN), Sint16(INT16_MAX));
	effect.condition.left_coeff[2] = FMath::Clamp(Sint16(EffectData.LeftCoeff.Z), Sint16(INT16_MIN), Sint16(INT16_MAX));
	effect.condition.left_sat[0] = FMath::Clamp(Uint16(EffectData.LeftSat.X), Uint16(0), Uint16(UINT16_MAX));
	effect.condition.left_sat[1] = FMath::Clamp(Uint16(EffectData.LeftSat.Y), Uint16(0), Uint16(UINT16_MAX));
	effect.condition.left_sat[2] = FMath::Clamp(Uint16(EffectData.LeftSat.Z), Uint16(0), Uint16(UINT16_MAX));
	effect.condition.right_coeff[0] = FMath::Clamp(Sint16(EffectData.RightCoeff.X), Sint16(INT16_MIN), Sint16(INT16_MAX));
	effect.condition.right_coeff[1] = FMath::Clamp(Sint16(EffectData.RightCoeff.Y), Sint16(INT16_MIN), Sint16(INT16_MAX));
	effect.condition.right_coeff[2] = FMath::Clamp(Sint16(EffectData.RightCoeff.Z), Sint16(INT16_MIN), Sint16(INT16_MAX));
	effect.condition.right_sat[0] = FMath::Clamp(Uint16(EffectData.RightSat.X), Uint16(0), Uint16(UINT16_MAX));
	effect.condition.right_sat[1] = FMath::Clamp(Uint16(EffectData.RightSat.Y), Uint16(0), Uint16(UINT16_MAX));
	effect.condition.right_sat[2] = FMath::Clamp(Uint16(EffectData.RightSat.Z), Uint16(0), Uint16(UINT16_MAX));

	if (EffectData.InfiniteLength)
	{
		effect.condition.length = SDL_HAPTIC_INFINITY;
	}
	else {
		effect.condition.length = FMath::Clamp(Uint32(EffectData.Length), Uint32(0), Uint32(UINT32_MAX));
	}

	return effect;
}