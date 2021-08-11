#include "ForcedFeedback/Effects/ForcedFeedbackEffectPeriodic.h"
#include "ForcedFeedback/Types/ForcedFeedbackPeriodicEffectType.h"

SDL_HapticEffect UForcedFeedbackEffectPeriodic::ToSDLEffect()
{
	SDL_HapticEffect effect = Super::ToSDLEffect();

	switch (EffectData.EffectType) {
	case(EForcedFeedbackPeriodicEffectType::SINE): {
		effect.type = SDL_HAPTIC_SINE;
		break;
	}
	case(EForcedFeedbackPeriodicEffectType::TRIANGLE): {
		effect.type = SDL_HAPTIC_TRIANGLE;
		break;
	}
	case(EForcedFeedbackPeriodicEffectType::SAWTOOTHUP): {
		effect.type = SDL_HAPTIC_SAWTOOTHUP;
		break;
	}
	case(EForcedFeedbackPeriodicEffectType::SAWTOOTHDOWN): {
		effect.type = SDL_HAPTIC_SAWTOOTHDOWN;
		break;
	}
	default: {}
	}

	switch (EffectData.Direction.DirectionType)
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

	effect.periodic.direction.dir[0] = Sint32(EffectData.Direction.Direction.X);
	effect.periodic.direction.dir[1] = Sint32(EffectData.Direction.Direction.Y);
	effect.periodic.direction.dir[2] = Sint32(EffectData.Direction.Direction.Z);
	effect.periodic.period = FMath::Clamp(Uint16(EffectData.Period), Uint16(0), Uint16(UINT16_MAX));
	effect.periodic.magnitude = FMath::Clamp(Sint16(EffectData.Magnitude), Sint16(INT16_MIN), Sint16(INT16_MAX));
	effect.periodic.offset = FMath::Clamp(Sint16(EffectData.Offset), Sint16(INT16_MIN), Sint16(INT16_MAX));
	effect.periodic.phase = FMath::Clamp(Uint16(EffectData.Phase), Uint16(0), Uint16(UINT16_MAX));
	effect.periodic.delay = FMath::Clamp(Uint16(EffectData.Delay), Uint16(0), Uint16(UINT16_MAX));
	effect.periodic.attack_length = FMath::Clamp(Uint16(EffectData.AttackLength), Uint16(0), Uint16(UINT16_MAX));
	effect.periodic.attack_level = FMath::Clamp(Uint16(EffectData.AttackLevel), Uint16(0), Uint16(UINT16_MAX));
	effect.periodic.fade_length = FMath::Clamp(Uint16(EffectData.FadeLength), Uint16(0), Uint16(UINT16_MAX));
	effect.periodic.fade_level = FMath::Clamp(Uint16(EffectData.FadeLevel), Uint16(0), Uint16(UINT16_MAX));

	if (EffectData.InfiniteLength) 
	{
		effect.periodic.length = SDL_HAPTIC_INFINITY;
	}
	else 
	{
		effect.periodic.length = FMath::Clamp(Uint32(EffectData.Length), Uint32(0), Uint32(UINT32_MAX));
	}

	return effect;
}