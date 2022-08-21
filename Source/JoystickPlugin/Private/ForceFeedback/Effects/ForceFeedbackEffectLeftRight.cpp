#include "ForceFeedback/Effects/ForceFeedbackEffectLeftRight.h"

void UForceFeedbackEffectLeftRight::UpdateEffectData()
{
	Super::UpdateEffectData();

	Effect.type = SDL_HAPTIC_LEFTRIGHT;
	//effect.leftright.type = SDL_HAPTIC_LEFTRIGHT;

	Effect.leftright.length = EffectData.DurationData.InfiniteDuration ? SDL_HAPTIC_INFINITY : FMath::Clamp(static_cast<Uint32>(EffectData.DurationData.Duration * 1000.0f), static_cast<Uint32>(0), static_cast<Uint32>(UINT32_MAX));

	Effect.leftright.large_magnitude = FMath::Clamp(static_cast<Uint16>(EffectData.LargeMagnitude * float(UINT16_MAX)), static_cast<Uint16>(0), static_cast<Uint16>(UINT16_MAX));
	Effect.leftright.small_magnitude = FMath::Clamp(static_cast<Uint16>(EffectData.SmallMagnitude * float(UINT16_MAX)), static_cast<Uint16>(0), static_cast<Uint16>(UINT16_MAX));
}
