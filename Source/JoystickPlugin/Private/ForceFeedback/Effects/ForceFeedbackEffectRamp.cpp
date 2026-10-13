// JoystickPlugin is licensed under the MIT License.
// Copyright Jayden Maalouf 2026. All Rights Reserved.

#include "ForceFeedback/Effects/ForceFeedbackEffectRamp.h"

void UForceFeedbackEffectRamp::UpdateEffectData()
{
	Super::UpdateEffectData();

	Effect.type = SDL_HAPTIC_RAMP;

	Effect.ramp.direction = EffectData.DirectionData.ToSDLDirection();
	Effect.ramp.length = GetEffectDuration();
	Effect.ramp.delay = FForceFeedbackConversion::SecondsToMilliseconds(EffectData.ReplayData.Delay);
	Effect.ramp.interval = FForceFeedbackConversion::SecondsToMilliseconds(EffectData.ReplayData.RetriggerDelay);

	Effect.ramp.start = FForceFeedbackConversion::ToSignedLevel(EffectData.RampStart);
	Effect.ramp.end = FForceFeedbackConversion::ToSignedLevel(EffectData.RampEnd);
	Effect.ramp.attack_length = FForceFeedbackConversion::SecondsToMilliseconds(EffectData.EnvelopeData.AttackDuration);
	Effect.ramp.attack_level = FForceFeedbackConversion::ToUnsignedLevel(EffectData.EnvelopeData.AttackLevel);
	Effect.ramp.fade_length = FForceFeedbackConversion::SecondsToMilliseconds(EffectData.EnvelopeData.FadeDuration);
	Effect.ramp.fade_level = FForceFeedbackConversion::ToUnsignedLevel(EffectData.EnvelopeData.FadeLevel);
}

uint32 UForceFeedbackEffectRamp::GetEffectDuration()
{
	return EffectData.DurationData.GetEffectDuration();
}
