// JoystickPlugin is licensed under the MIT License.
// Copyright Jayden Maalouf 2026. All Rights Reserved.

#include "ForceFeedback/Effects/ForceFeedbackEffectLeftRight.h"

void UForceFeedbackEffectLeftRight::UpdateEffectData()
{
	Super::UpdateEffectData();

	Effect.type = SDL_HAPTIC_LEFTRIGHT;

	Effect.leftright.length = GetEffectDuration();

	Effect.leftright.large_magnitude = FForceFeedbackConversion::ToUnsignedLevel(EffectData.LargeMagnitude);
	Effect.leftright.small_magnitude = FForceFeedbackConversion::ToUnsignedLevel(EffectData.SmallMagnitude);
}

uint32 UForceFeedbackEffectLeftRight::GetEffectDuration()
{
	return EffectData.DurationData.GetEffectDuration();
}
