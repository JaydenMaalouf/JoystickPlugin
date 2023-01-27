// JoystickPlugin is licensed under the MIT License.
// Copyright Jayden Maalouf. All Rights Reserved.

#include "ForceFeedback/Effects/ForceFeedbackEffectCustom.h"

void UForceFeedbackEffectCustom::UpdateEffectData()
{
	Super::UpdateEffectData();

	Effect.type = SDL_HAPTIC_CUSTOM;
	//effect.custom.type = SDL_HAPTIC_CUSTOM;

	Effect.custom.direction = EffectData.DirectionData.ToSDLDirection();
	Effect.custom.length = EffectData.DurationData.InfiniteDuration ? SDL_HAPTIC_INFINITY : FMath::Clamp<Uint32>(EffectData.DurationData.Duration * 1000.0f, 0, UINT32_MAX);
	Effect.custom.delay = FMath::Clamp<Uint16>(EffectData.ReplayData.Delay * 1000.0f, 0, UINT16_MAX);
	Effect.custom.interval = FMath::Clamp<Uint16>(EffectData.ReplayData.RetriggerDelay * 1000.0f, 0, UINT16_MAX);

	Effect.custom.channels = FMath::Clamp<Uint8>(EffectData.Channels, 0, UINT8_MAX);
	Effect.custom.period = FMath::Clamp<Uint16>(EffectData.Period * UINT16_MAX, 0, UINT16_MAX);
	Effect.custom.samples = FMath::Clamp<Uint16>(EffectData.Samples, 0, UINT16_MAX);

	Uint16* DataArray = new Uint16[EffectData.Data.Num()]();
	for (int i = 0; i < EffectData.Data.Num(); i++)
	{
		DataArray[i] = FMath::Clamp<Uint16>(EffectData.Data[i], 0, UINT16_MAX);
	}
	Effect.custom.data = DataArray;

	Effect.custom.attack_length = FMath::Clamp<Uint16>(EffectData.EnvelopeData.AttackDuration * 1000.0f, 0, UINT16_MAX);
	Effect.custom.attack_level = FMath::Clamp<Uint16>(EffectData.EnvelopeData.AttackLevel * UINT16_MAX, 0, UINT16_MAX);
	Effect.custom.fade_length = FMath::Clamp<Uint16>(EffectData.EnvelopeData.FadeDuration * 1000.0f, 0, UINT16_MAX);
	Effect.custom.fade_level = FMath::Clamp<Uint16>(EffectData.EnvelopeData.FadeLevel * UINT16_MAX, 0, UINT16_MAX);
}
