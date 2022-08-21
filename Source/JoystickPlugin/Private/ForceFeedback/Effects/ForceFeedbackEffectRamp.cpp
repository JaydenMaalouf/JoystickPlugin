#include "ForceFeedback/Effects/ForceFeedbackEffectRamp.h"

void UForceFeedbackEffectRamp::UpdateEffectData()
{
	Super::UpdateEffectData();

	Effect.type = SDL_HAPTIC_RAMP;
	//effect.ramp.type = SDL_HAPTIC_RAMP;

	Effect.ramp.direction = EffectData.DirectionData.ToSDLDirection();
	Effect.ramp.length = EffectData.DurationData.InfiniteDuration ? SDL_HAPTIC_INFINITY : FMath::Clamp(static_cast<Uint32>(EffectData.DurationData.Duration * 1000.0f), static_cast<Uint32>(0), static_cast<Uint32>(UINT32_MAX));
	Effect.ramp.delay = FMath::Clamp(static_cast<Uint16>(EffectData.ReplayData.Delay * 1000.0f), static_cast<Uint16>(0), static_cast<Uint16>(UINT16_MAX));
	Effect.ramp.interval = FMath::Clamp(static_cast<Uint16>(EffectData.ReplayData.RetriggerDelay * 1000.0f), static_cast<Uint16>(0), static_cast<Uint16>(UINT16_MAX));

	Effect.ramp.start = FMath::Clamp(static_cast<Sint16>(EffectData.RampStart * float(INT16_MAX)), static_cast<Sint16>(INT16_MIN), static_cast<Sint16>(INT16_MAX));
	Effect.ramp.end = FMath::Clamp(static_cast<Sint16>(EffectData.RampEnd * float(INT16_MAX)), static_cast<Sint16>(INT16_MIN), static_cast<Sint16>(INT16_MAX));
	Effect.ramp.attack_length = FMath::Clamp(static_cast<Uint16>(EffectData.EnvelopeData.AttackDuration * 1000.0f), static_cast<Uint16>(0), static_cast<Uint16>(UINT16_MAX));
	Effect.ramp.attack_level = FMath::Clamp(static_cast<Uint16>(EffectData.EnvelopeData.AttackLevel * float(UINT16_MAX)), static_cast<Uint16>(0), static_cast<Uint16>(UINT16_MAX));
	Effect.ramp.fade_length = FMath::Clamp(static_cast<Uint16>(EffectData.EnvelopeData.FadeDuration * 1000.0f), static_cast<Uint16>(0), static_cast<Uint16>(UINT16_MAX));
	Effect.ramp.fade_level = FMath::Clamp(static_cast<Uint16>(EffectData.EnvelopeData.FadeLevel * float(UINT16_MAX)), static_cast<Uint16>(0), static_cast<Uint16>(UINT16_MAX));
}
