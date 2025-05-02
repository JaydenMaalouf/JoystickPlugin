// JoystickPlugin is licensed under the MIT License.
// Copyright Jayden Maalouf. All Rights Reserved.

#include "ForceFeedback/Effects/ForceFeedbackEffectLeftRight.h"

void UForceFeedbackEffectLeftRight::UpdateEffectData()
{
	Super::UpdateEffectData();

	Effect.type = SDL_HAPTIC_LEFTRIGHT;
	//effect.leftright.type = SDL_HAPTIC_LEFTRIGHT;

	Effect.leftright.length = GetEffectDuration();

	Effect.leftright.large_magnitude = FMath::Clamp<Uint16>(EffectData.LargeMagnitude * UINT16_MAX, 0, UINT16_MAX);
	Effect.leftright.small_magnitude = FMath::Clamp<Uint16>(EffectData.SmallMagnitude * UINT16_MAX, 0, UINT16_MAX);
}

uint32 UForceFeedbackEffectLeftRight::GetEffectDuration()
{
	return EffectData.DurationData.GetEffectDuration();
}
