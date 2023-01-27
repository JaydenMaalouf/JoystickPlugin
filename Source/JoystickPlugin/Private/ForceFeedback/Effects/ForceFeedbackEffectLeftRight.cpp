// JoystickPlugin is licensed under the MIT License.
// Copyright Jayden Maalouf. All Rights Reserved.

#include "ForceFeedback/Effects/ForceFeedbackEffectLeftRight.h"

void UForceFeedbackEffectLeftRight::UpdateEffectData()
{
	Super::UpdateEffectData();

	Effect.type = SDL_HAPTIC_LEFTRIGHT;
	//effect.leftright.type = SDL_HAPTIC_LEFTRIGHT;

	Effect.leftright.length = EffectData.DurationData.InfiniteDuration ? SDL_HAPTIC_INFINITY : FMath::Clamp<Uint32>(EffectData.DurationData.Duration * 1000.0f, 0, UINT32_MAX);

	Effect.leftright.large_magnitude = FMath::Clamp<Uint16>(EffectData.LargeMagnitude * UINT16_MAX, 0, UINT16_MAX);
	Effect.leftright.small_magnitude = FMath::Clamp<Uint16>(EffectData.SmallMagnitude * UINT16_MAX, 0, UINT16_MAX);
}
