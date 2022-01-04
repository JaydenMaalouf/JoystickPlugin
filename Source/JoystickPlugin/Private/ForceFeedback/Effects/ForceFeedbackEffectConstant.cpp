#include "ForceFeedback/Effects/ForceFeedbackEffectConstant.h"

void UForceFeedbackEffectConstant::UpdateEffectData()
{
	Super::UpdateEffectData();

	Effect.type = SDL_HAPTIC_CONSTANT;
	//effect.constant.type = SDL_HAPTIC_CONSTANT;

	Effect.constant.direction = EffectData.DirectionData.ToSDLDirection();
	Effect.constant.length = EffectData.DurationData.InfiniteDuration ? SDL_HAPTIC_INFINITY : FMath::Clamp(Uint32(EffectData.DurationData.Duration * 1000.0f), Uint32(0), Uint32(UINT32_MAX));
	Effect.constant.delay = FMath::Clamp(Uint16(EffectData.ReplayData.Delay * 1000.0f), Uint16(0), Uint16(UINT16_MAX));
	Effect.constant.interval = FMath::Clamp(Uint16(EffectData.ReplayData.RetriggerDelay * 1000.0f), Uint16(0), Uint16(UINT16_MAX));

	Effect.constant.level = FMath::Clamp(Sint16(EffectData.EffectLevel * float(INT16_MAX)), Sint16(INT16_MIN), Sint16(INT16_MAX));
	Effect.constant.attack_length = FMath::Clamp(Uint16(EffectData.EnvelopeData.AttackDuration * 1000.0f), Uint16(0), Uint16(UINT16_MAX));
	Effect.constant.attack_level = FMath::Clamp(Uint16(EffectData.EnvelopeData.AttackLevel * float(UINT16_MAX)), Uint16(0), Uint16(UINT16_MAX));
	Effect.constant.fade_length = FMath::Clamp(Uint16(EffectData.EnvelopeData.FadeDuration * 1000.0f), Uint16(0), Uint16(UINT16_MAX));
	Effect.constant.fade_level = FMath::Clamp(Uint16(EffectData.EnvelopeData.FadeLevel * float(UINT16_MAX)), Uint16(0), Uint16(UINT16_MAX));
}