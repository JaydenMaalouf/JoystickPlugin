#include "ForceFeedback/Effects/ForceFeedbackEffectPeriodic.h"
#include "ForceFeedback/Types/ForceFeedbackPeriodicEffectType.h"

SDL_HapticEffect UForceFeedbackEffectPeriodic::ToSDLEffect()
{
	SDL_HapticEffect effect = Super::ToSDLEffect();

	switch (EffectData.EffectType) {
	case(EForceFeedbackPeriodicEffectType::SINE): {
		effect.type = SDL_HAPTIC_SINE;
		//effect.periodic.type = SDL_HAPTIC_SINE;
		break;
	}
	case(EForceFeedbackPeriodicEffectType::TRIANGLE): {
		effect.type = SDL_HAPTIC_TRIANGLE;
		//effect.periodic.type = SDL_HAPTIC_TRIANGLE;
		break;
	}
	case(EForceFeedbackPeriodicEffectType::SAWTOOTHUP): {
		effect.type = SDL_HAPTIC_SAWTOOTHUP;
		//effect.periodic.type = SDL_HAPTIC_SAWTOOTHUP;
		break;
	}
	case(EForceFeedbackPeriodicEffectType::SAWTOOTHDOWN): {
		effect.type = SDL_HAPTIC_SAWTOOTHDOWN;
		//effect.periodic.type = SDL_HAPTIC_SAWTOOTHDOWN;
		break;
	}
	case(EForceFeedbackPeriodicEffectType::LEFTRIGHT): {
		effect.type = SDL_HAPTIC_LEFTRIGHT;
		//effect.periodic.type = SDL_HAPTIC_LEFTRIGHT;
		break;
	}
	default: {}
	}

	effect.periodic.direction = EffectData.Direction.ToSDLDirection();
	effect.periodic.length = EffectData.InfiniteDuration ? SDL_HAPTIC_INFINITY : FMath::Clamp(Uint32(EffectData.Duration * 1000.0f), Uint32(0), Uint32(UINT32_MAX));
	effect.periodic.delay = FMath::Clamp(Uint16(EffectData.Delay * 1000.0f), Uint16(0), Uint16(UINT16_MAX));
	effect.periodic.interval = FMath::Clamp(Uint16(EffectData.RetriggerDelay * 1000.0f), Uint16(0), Uint16(UINT16_MAX));

	effect.periodic.period = FMath::Clamp(Uint16(EffectData.Period * 1000.0f), Uint16(0), Uint16(UINT16_MAX));
	effect.periodic.magnitude = FMath::Clamp(Sint16(EffectData.Magnitude * float(INT16_MAX)), Sint16(INT16_MIN), Sint16(INT16_MAX));
	effect.periodic.offset = FMath::Clamp(Sint16(EffectData.Offset * 32767.0f), Sint16(INT16_MIN), Sint16(INT16_MAX));
	effect.periodic.phase = FMath::Clamp(Uint16(EffectData.Phase * 100.0f), Uint16(0), Uint16(UINT16_MAX));

	effect.periodic.attack_length = FMath::Clamp(Uint16(EffectData.AttackDuration * 1000.0f), Uint16(0), Uint16(UINT16_MAX));
	effect.periodic.attack_level = FMath::Clamp(Uint16(EffectData.AttackLevel * float(UINT16_MAX)), Uint16(0), Uint16(UINT16_MAX));
	effect.periodic.fade_length = FMath::Clamp(Uint16(EffectData.FadeDuration * 1000.0f), Uint16(0), Uint16(UINT16_MAX));
	effect.periodic.fade_level = FMath::Clamp(Uint16(EffectData.FadeLevel * float(UINT16_MAX)), Uint16(0), Uint16(UINT16_MAX));

	return effect;
}