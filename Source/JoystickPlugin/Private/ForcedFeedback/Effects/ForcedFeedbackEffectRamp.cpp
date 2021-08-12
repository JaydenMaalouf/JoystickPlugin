#include "ForcedFeedback/Effects/ForcedFeedbackEffectRamp.h"

SDL_HapticEffect UForcedFeedbackEffectRamp::ToSDLEffect()
{
	SDL_HapticEffect effect = Super::ToSDLEffect();

	effect.type = SDL_HAPTIC_RAMP;
	//effect.ramp.type = SDL_HAPTIC_RAMP;

	effect.ramp.direction = EffectData.Direction.ToSDLDirection();
	effect.ramp.length = EffectData.InfiniteDuration ? SDL_HAPTIC_INFINITY : FMath::Clamp(Uint32(EffectData.Duration * 1000.0f), Uint32(0), Uint32(UINT32_MAX));
	effect.ramp.delay = FMath::Clamp(Uint16(EffectData.Delay * 1000.0f), Uint16(0), Uint16(UINT16_MAX));
	effect.ramp.interval = FMath::Clamp(Uint16(EffectData.RetriggerDelay * 1000.0f), Uint16(0), Uint16(UINT16_MAX));

	effect.ramp.start = FMath::Clamp(Sint16(EffectData.RampStart * float(INT16_MAX)), Sint16(INT16_MIN), Sint16(INT16_MAX));
	effect.ramp.end = FMath::Clamp(Sint16(EffectData.RampEnd * float(INT16_MAX)), Sint16(INT16_MIN), Sint16(INT16_MAX));
	effect.ramp.attack_length = FMath::Clamp(Uint16(EffectData.AttackDuration * 1000.0f), Uint16(0), Uint16(UINT16_MAX));
	effect.ramp.attack_level = FMath::Clamp(Uint16(EffectData.AttackLevel * float(UINT16_MAX)), Uint16(0), Uint16(UINT16_MAX));
	effect.ramp.fade_length = FMath::Clamp(Uint16(EffectData.FadeDuration * 1000.0f), Uint16(0), Uint16(UINT16_MAX));
	effect.ramp.fade_level = FMath::Clamp(Uint16(EffectData.FadeLevel * float(UINT16_MAX)), Uint16(0), Uint16(UINT16_MAX));

	return effect;
}