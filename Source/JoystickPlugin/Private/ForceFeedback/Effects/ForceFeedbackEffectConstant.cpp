// JoystickPlugin is licensed under the MIT License.
// Copyright Jayden Maalouf 2026. All Rights Reserved.

#include "ForceFeedback/Effects/ForceFeedbackEffectConstant.h"

void UForceFeedbackEffectConstant::UpdateEffectData()
{
	Super::UpdateEffectData();

	Effect.type = SDL_HAPTIC_CONSTANT;

	Effect.constant.direction = EffectData.DirectionData.ToSDLDirection();
	Effect.constant.length = GetEffectDuration();
	Effect.constant.delay = FMath::Clamp<Uint16>(EffectData.ReplayData.Delay * 1000.0f, 0, UINT16_MAX);
	Effect.constant.interval = FMath::Clamp<Uint16>(EffectData.ReplayData.RetriggerDelay * 1000.0f, 0, UINT16_MAX);

	Effect.constant.level = FMath::Clamp<Sint16>(EffectData.EffectLevel * INT16_MAX, INT16_MIN, INT16_MAX);
	Effect.constant.attack_length = FMath::Clamp<Uint16>(EffectData.EnvelopeData.AttackDuration * 1000.0f, 0, UINT16_MAX);
	Effect.constant.attack_level = FMath::Clamp<Uint16>(EffectData.EnvelopeData.AttackLevel * UINT16_MAX, 0, UINT16_MAX);
	Effect.constant.fade_length = FMath::Clamp<Uint16>(EffectData.EnvelopeData.FadeDuration * 1000.0f, 0, UINT16_MAX);
	Effect.constant.fade_level = FMath::Clamp<Uint16>(EffectData.EnvelopeData.FadeLevel * UINT16_MAX, 0, UINT16_MAX);
}

uint32 UForceFeedbackEffectConstant::GetEffectDuration()
{
	return EffectData.DurationData.GetEffectDuration();
}
