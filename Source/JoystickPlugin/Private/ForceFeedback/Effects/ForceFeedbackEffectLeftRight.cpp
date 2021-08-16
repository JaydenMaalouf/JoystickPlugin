#include "ForceFeedback/Effects/ForceFeedbackEffectLeftRight.h"

SDL_HapticEffect UForceFeedbackEffectLeftRight::ToSDLEffect()
{
	SDL_HapticEffect effect = Super::ToSDLEffect();

	effect.type = SDL_HAPTIC_LEFTRIGHT;
	//effect.leftright.type = SDL_HAPTIC_LEFTRIGHT;

	effect.leftright.length = EffectData.DurationData.InfiniteDuration ? SDL_HAPTIC_INFINITY : FMath::Clamp(Uint32(EffectData.DurationData.Duration * 1000.0f), Uint32(0), Uint32(UINT32_MAX));

	effect.leftright.large_magnitude = FMath::Clamp(Uint16(EffectData.LargeMagnitude * float(UINT16_MAX)), Uint16(0), Uint16(UINT16_MAX));
	effect.leftright.small_magnitude = FMath::Clamp(Uint16(EffectData.SmallMagnitude * float(UINT16_MAX)), Uint16(0), Uint16(UINT16_MAX));

	return effect;
}