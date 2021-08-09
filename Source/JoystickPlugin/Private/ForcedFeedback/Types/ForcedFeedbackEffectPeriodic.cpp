#include "ForcedFeedback/Types/ForcedFeedbackEffectPeriodic.h"

SDL_HapticEffect UForcedFeedbackEffectPeriodic::ToSDLEffect()
{
	SDL_HapticEffect effect = Super::ToSDLEffect();
	FForcedFeedbackData effectData = GetEffect();

	//TODO ADD A CHECK ON EFFECT TYPE

	switch (EffectType) {
	case(EForcedFeedbackEffectType::SINE): {
		effect.type = SDL_HAPTIC_SINE;
		break;
	}
	case(EForcedFeedbackEffectType::TRIANGLE): {
		effect.type = SDL_HAPTIC_TRIANGLE;
		break;
	}
	case(EForcedFeedbackEffectType::SAWTOOTHUP): {
		effect.type = SDL_HAPTIC_SAWTOOTHUP;
		break;
	}
	case(EForcedFeedbackEffectType::SAWTOOTHDOWN): {
		effect.type = SDL_HAPTIC_SAWTOOTHDOWN;
		break;
	}
	default: {}
	}

	switch (effectData.DirectionType)
	{
	case(EForcedFeedbackDirectionType::CARTESIAN): {
		effect.periodic.direction.type = SDL_HAPTIC_CARTESIAN;
		break;
	}
	case(EForcedFeedbackDirectionType::POLAR): {
		effect.periodic.direction.type = SDL_HAPTIC_POLAR;
		break;
	}
	case(EForcedFeedbackDirectionType::SPHERICAL): {
		effect.periodic.direction.type = SDL_HAPTIC_SPHERICAL;
		break;
	}
	default: {}
	}

	effect.periodic.direction.dir[0] = Sint32(effectData.Direction.X * INT_MAX);
	effect.periodic.direction.dir[1] = Sint32(effectData.Direction.Y * INT_MAX);
	effect.periodic.direction.dir[2] = Sint32(effectData.Direction.Z * INT_MAX);
	effect.periodic.period = Uint16(effectData.Period * 1000.0f);
	effect.periodic.magnitude = FMath::Clamp(Uint16(effectData.Magnitude * float(INT16_MAX)), Uint16(0), Uint16(INT16_MAX));
	effect.periodic.offset = Sint16(effectData.Offset * 32767.0f);
	effect.periodic.phase = Uint16(effectData.Phase * 100.0f);
	effect.periodic.delay = Uint16(effectData.Delay * 1000.0f);
	effect.periodic.attack_length = Uint16(effectData.AttackLength * 1000.0f);
	effect.periodic.attack_level = FMath::Clamp(Uint16(effectData.AttackLevel * float(INT16_MAX)), Uint16(0), Uint16(INT16_MAX));
	effect.periodic.fade_length = Uint16(effectData.FadeLength * 1000.0f);
	effect.periodic.fade_level = FMath::Clamp(Uint16(effectData.FadeLevel * float(INT16_MAX)), Uint16(0), Uint16(INT16_MAX));

	if (effectData.InfiniteLength) 
	{
		effect.periodic.length = SDL_HAPTIC_INFINITY;
	}
	else 
	{
		effect.periodic.length = Uint32(effectData.Length * 1000.0f);
	}

	return effect;
}