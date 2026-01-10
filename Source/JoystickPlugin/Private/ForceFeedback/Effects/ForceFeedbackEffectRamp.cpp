// JoystickPlugin is licensed under the MIT License.
// Copyright Jayden Maalouf 2026. All Rights Reserved.

#include "ForceFeedback/Effects/ForceFeedbackEffectRamp.h"

void UForceFeedbackEffectRamp::UpdateEffectData()
{
	Super::UpdateEffectData();

	Effect.type = SDL_HAPTIC_RAMP;

	Effect.ramp.direction = EffectData.DirectionData.ToSDLDirection();
	Effect.ramp.length = GetEffectDuration();
	Effect.ramp.delay = FMath::Clamp<uint16>(EffectData.ReplayData.Delay * 1000.0f, 0, UINT16_MAX);
	Effect.ramp.interval = FMath::Clamp<uint16>(EffectData.ReplayData.RetriggerDelay * 1000.0f, 0, UINT16_MAX);

	Effect.ramp.start = FMath::Clamp<int16>(EffectData.RampStart * INT16_MAX, INT16_MIN, INT16_MAX);
	Effect.ramp.end = FMath::Clamp<int16>(EffectData.RampEnd * INT16_MAX, INT16_MIN, INT16_MAX);
	Effect.ramp.attack_length = FMath::Clamp<uint16>(EffectData.EnvelopeData.AttackDuration * 1000.0f, 0, UINT16_MAX);
	Effect.ramp.attack_level = FMath::Clamp<uint16>(EffectData.EnvelopeData.AttackLevel * UINT16_MAX, 0, UINT16_MAX);
	Effect.ramp.fade_length = FMath::Clamp<uint16>(EffectData.EnvelopeData.FadeDuration * 1000.0f, 0, UINT16_MAX);
	Effect.ramp.fade_level = FMath::Clamp<uint16>(EffectData.EnvelopeData.FadeLevel * UINT16_MAX, 0, UINT16_MAX);
}

uint32 UForceFeedbackEffectRamp::GetEffectDuration()
{
	return EffectData.DurationData.GetEffectDuration();
}
