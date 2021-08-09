#include "ForcedFeedback/Types/ForcedFeedbackEffectRamp.h"

SDL_HapticEffect UForcedFeedbackEffectRamp::ToSDLEffect()
{
	SDL_HapticEffect effect = Super::ToSDLEffect();
	FForcedFeedbackData effectData = GetEffect();

	effect.type = SDL_HAPTIC_RAMP;

	//TODO ADD A CHECK ON EFFECT TYPE

	switch (effectData.DirectionType)
	{
	case(EForcedFeedbackDirectionType::CARTESIAN): {
		effect.ramp.direction.type = SDL_HAPTIC_CARTESIAN;
		break;
	}
	case(EForcedFeedbackDirectionType::POLAR): {
		effect.ramp.direction.type = SDL_HAPTIC_POLAR;
		break;
	}
	case(EForcedFeedbackDirectionType::SPHERICAL): {
		effect.ramp.direction.type = SDL_HAPTIC_SPHERICAL;
		break;
	}
	default: {}
	}
	
	effect.ramp.direction.dir[0] = Sint32(effectData.Direction.X * INT_MAX);
	effect.ramp.direction.dir[1] = Sint32(effectData.Direction.Y * INT_MAX);
	effect.ramp.direction.dir[2] = Sint32(effectData.Direction.Z * INT_MAX);
	effect.ramp.start = FMath::Clamp(Sint16(effectData.RampStart * float(INT16_MAX)), Sint16(INT16_MIN), Sint16(INT16_MAX));
	effect.ramp.end = FMath::Clamp(Sint16(effectData.RampEnd * float(INT16_MAX)), Sint16(INT16_MIN), Sint16(INT16_MAX));
	effect.ramp.delay = Uint16(effectData.Delay * 1000.0f);
	effect.ramp.length = Uint16(effectData.Length * 1000.0f);
	effect.ramp.attack_length = Uint16(effectData.AttackLength * 1000.0f);
	effect.ramp.attack_level = FMath::Clamp(Uint16(effectData.AttackLevel * float(INT16_MAX)), Uint16(0), Uint16(INT16_MAX));
	effect.ramp.fade_length = Uint16(effectData.FadeLength * 1000.0f);
	effect.ramp.fade_level = FMath::Clamp(Uint16(effectData.FadeLevel * float(INT16_MAX)), Uint16(0), Uint16(INT16_MAX));

	return effect;
}