#include "ForceFeedback/Effects/ForceFeedbackEffectRamp.h"

void UForceFeedbackEffectRamp::UpdateEffectData()
{
	Super::UpdateEffectData();

	Effect.type = SDL_HAPTIC_RAMP;
	//effect.ramp.type = SDL_HAPTIC_RAMP;

	Effect.ramp.direction = EffectData.DirectionData.ToSDLDirection();
	Effect.ramp.length = EffectData.DurationData.InfiniteDuration ? SDL_HAPTIC_INFINITY : FMath::Clamp(EffectData.DurationData.Duration * 1000.0f, 0, UINT32_MAX);
	Effect.ramp.delay = FMath::Clamp(EffectData.ReplayData.Delay * 1000.0f, 0, UINT16_MAX);
	Effect.ramp.interval = FMath::Clamp(EffectData.ReplayData.RetriggerDelay * 1000.0f, 0, UINT16_MAX);

	Effect.ramp.start = FMath::Clamp(EffectData.RampStart * INT16_MAX, INT16_MIN, INT16_MAX);
	Effect.ramp.end = FMath::Clamp(EffectData.RampEnd * INT16_MAX, INT16_MIN, INT16_MAX);
	Effect.ramp.attack_length = FMath::Clamp(EffectData.EnvelopeData.AttackDuration * 1000.0f, 0, UINT16_MAX);
	Effect.ramp.attack_level = FMath::Clamp(EffectData.EnvelopeData.AttackLevel * UINT16_MAX, 0, UINT16_MAX);
	Effect.ramp.fade_length = FMath::Clamp(EffectData.EnvelopeData.FadeDuration * 1000.0f, 0, UINT16_MAX);
	Effect.ramp.fade_level = FMath::Clamp(EffectData.EnvelopeData.FadeLevel * UINT16_MAX, 0, UINT16_MAX);
}
