#include "ForceFeedback/Effects/ForceFeedbackEffectRamp.h"

void UForceFeedbackEffectRamp::UpdateEffectData()
{
	Super::UpdateEffectData();

	Effect.type = SDL_HAPTIC_RAMP;
	//effect.ramp.type = SDL_HAPTIC_RAMP;

	Effect.ramp.direction = EffectData.DirectionData.ToSDLDirection();
	Effect.ramp.length = EffectData.DurationData.InfiniteDuration ? SDL_HAPTIC_INFINITY : FMath::Clamp(Uint32(EffectData.DurationData.Duration * 1000.0f), Uint32(0), Uint32(UINT32_MAX));
	Effect.ramp.delay = FMath::Clamp(Uint16(EffectData.ReplayData.Delay * 1000.0f), Uint16(0), Uint16(UINT16_MAX));
	Effect.ramp.interval = FMath::Clamp(Uint16(EffectData.ReplayData.RetriggerDelay * 1000.0f), Uint16(0), Uint16(UINT16_MAX));

	Effect.ramp.start = FMath::Clamp(Sint16(EffectData.RampStart * float(INT16_MAX)), Sint16(INT16_MIN), Sint16(INT16_MAX));
	Effect.ramp.end = FMath::Clamp(Sint16(EffectData.RampEnd * float(INT16_MAX)), Sint16(INT16_MIN), Sint16(INT16_MAX));
	Effect.ramp.attack_length = FMath::Clamp(Uint16(EffectData.EnvelopeData.AttackDuration * 1000.0f), Uint16(0), Uint16(UINT16_MAX));
	Effect.ramp.attack_level = FMath::Clamp(Uint16(EffectData.EnvelopeData.AttackLevel * float(UINT16_MAX)), Uint16(0), Uint16(UINT16_MAX));
	Effect.ramp.fade_length = FMath::Clamp(Uint16(EffectData.EnvelopeData.FadeDuration * 1000.0f), Uint16(0), Uint16(UINT16_MAX));
	Effect.ramp.fade_level = FMath::Clamp(Uint16(EffectData.EnvelopeData.FadeLevel * float(UINT16_MAX)), Uint16(0), Uint16(UINT16_MAX));
}