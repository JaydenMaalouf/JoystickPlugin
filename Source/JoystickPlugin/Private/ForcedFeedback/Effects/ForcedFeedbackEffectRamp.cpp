#include "ForcedFeedback/Effects/ForcedFeedbackEffectRamp.h"

SDL_HapticEffect UForcedFeedbackEffectRamp::ToSDLEffect()
{
	SDL_HapticEffect effect = Super::ToSDLEffect();

	effect.type = SDL_HAPTIC_RAMP;

	switch (EffectData.Direction.DirectionType)
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
	
	effect.ramp.direction.dir[0] = Sint32(EffectData.Direction.Direction.X);
	effect.ramp.direction.dir[1] = Sint32(EffectData.Direction.Direction.Y);
	effect.ramp.direction.dir[2] = Sint32(EffectData.Direction.Direction.Z);
	effect.ramp.start = FMath::Clamp(Sint16(EffectData.RampStart), Sint16(INT16_MIN), Sint16(INT16_MAX));
	effect.ramp.end = FMath::Clamp(Sint16(EffectData.RampEnd), Sint16(INT16_MIN), Sint16(INT16_MAX));
	effect.ramp.delay = FMath::Clamp(Uint16(EffectData.Delay), Uint16(0), Uint16(UINT16_MAX));
	effect.ramp.attack_length = FMath::Clamp(Uint16(EffectData.AttackLength), Uint16(0), Uint16(UINT16_MAX));
	effect.ramp.attack_level = FMath::Clamp(Uint16(EffectData.AttackLevel), Uint16(0), Uint16(UINT16_MAX));
	effect.ramp.fade_length = FMath::Clamp(Uint16(EffectData.FadeLength), Uint16(0), Uint16(UINT16_MAX));
	effect.ramp.fade_level = FMath::Clamp(Uint16(EffectData.FadeLevel), Uint16(0), Uint16(UINT16_MAX));

	if (EffectData.InfiniteLength)
	{
		effect.ramp.length = SDL_HAPTIC_INFINITY;
	}
	else
	{
		effect.ramp.length = FMath::Clamp(Uint32(EffectData.Length), Uint32(0), Uint32(UINT32_MAX));
	}

	return effect;
}