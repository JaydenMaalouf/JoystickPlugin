#include "ForceFeedback/Effects/ForceFeedbackEffectConstant.h"

void UForceFeedbackEffectConstant::UpdateEffectData()
{
	Super::UpdateEffectData();

	Effect.type = SDL_HAPTIC_CONSTANT;
	//effect.constant.type = SDL_HAPTIC_CONSTANT;

	Effect.constant.direction = EffectData.DirectionData.ToSDLDirection();
	Effect.constant.length = EffectData.DurationData.InfiniteDuration ? SDL_HAPTIC_INFINITY : FMath::Clamp(EffectData.DurationData.Duration * 1000.0f, 0, UINT32_MAX);
	Effect.constant.delay = FMath::Clamp(EffectData.ReplayData.Delay * 1000.0f, 0, UINT16_MAX);
	Effect.constant.interval = FMath::Clamp(EffectData.ReplayData.RetriggerDelay * 1000.0f, 0, UINT16_MAX);

	Effect.constant.level = FMath::Clamp(EffectData.EffectLevel * INT16_MAX, INT16_MIN, INT16_MAX);
	Effect.constant.attack_length = FMath::Clamp(EffectData.EnvelopeData.AttackDuration * 1000.0f, 0, UINT16_MAX);
	Effect.constant.attack_level = FMath::Clamp(EffectData.EnvelopeData.AttackLevel * UINT16_MAX, 0, UINT16_MAX);
	Effect.constant.fade_length = FMath::Clamp(EffectData.EnvelopeData.FadeDuration * 1000.0f, 0, UINT16_MAX);
	Effect.constant.fade_level = FMath::Clamp(EffectData.EnvelopeData.FadeLevel * UINT16_MAX, 0, UINT16_MAX);
}
