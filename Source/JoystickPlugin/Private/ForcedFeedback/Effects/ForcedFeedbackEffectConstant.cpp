#include "ForcedFeedback/Effects/ForcedFeedbackEffectConstant.h"

SDL_HapticEffect UForcedFeedbackEffectConstant::ToSDLEffect()
{
	SDL_HapticEffect effect = Super::ToSDLEffect();

	effect.type = SDL_HAPTIC_CONSTANT;
	//effect.constant.type = SDL_HAPTIC_CONSTANT;

	effect.constant.direction = EffectData.Direction.ToSDLDirection();
	effect.constant.length = EffectData.InfiniteDuration ? SDL_HAPTIC_INFINITY : FMath::Clamp(Uint32(EffectData.Duration * 1000.0f), Uint32(0), Uint32(UINT32_MAX));
	effect.constant.delay = FMath::Clamp(Uint16(EffectData.Delay * 1000.0f), Uint16(0), Uint16(UINT16_MAX));
	effect.constant.interval = FMath::Clamp(Uint16(EffectData.RetriggerDelay * 1000.0f), Uint16(0), Uint16(UINT16_MAX));

	effect.constant.level = FMath::Clamp(Sint16(EffectData.EffectLevel * float(INT16_MAX)), Sint16(INT16_MIN), Sint16(INT16_MAX));
	effect.constant.attack_length = FMath::Clamp(Uint16(EffectData.AttackDuration * 1000.0f), Uint16(0), Uint16(UINT16_MAX));
	effect.constant.attack_level = FMath::Clamp(Uint16(EffectData.AttackLevel * float(UINT16_MAX)), Uint16(0), Uint16(UINT16_MAX));
	effect.constant.fade_length = FMath::Clamp(Uint16(EffectData.FadeDuration * 1000.0f), Uint16(0), Uint16(UINT16_MAX));
	effect.constant.fade_level = FMath::Clamp(Uint16(EffectData.FadeLevel * float(UINT16_MAX)), Uint16(0), Uint16(UINT16_MAX));

	return effect;
}