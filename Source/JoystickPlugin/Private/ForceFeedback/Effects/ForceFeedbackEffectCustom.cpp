// JoystickPlugin is licensed under the MIT License.
// Copyright Jayden Maalouf 2026. All Rights Reserved.

#include "ForceFeedback/Effects/ForceFeedbackEffectCustom.h"

void UForceFeedbackEffectCustom::UpdateEffectData()
{
	Super::UpdateEffectData();

	Effect.type = SDL_HAPTIC_CUSTOM;

	Effect.custom.direction = EffectData.DirectionData.ToSDLDirection();
	Effect.custom.length = GetEffectDuration();
	Effect.custom.delay = FMath::Clamp<uint16>(EffectData.ReplayData.Delay * 1000.0f, 0, UINT16_MAX);
	Effect.custom.interval = FMath::Clamp<uint16>(EffectData.ReplayData.RetriggerDelay * 1000.0f, 0, UINT16_MAX);

	Effect.custom.channels = FMath::Clamp<uint8>(EffectData.Channels, 0, UINT8_MAX);
	Effect.custom.period = FMath::Clamp<uint16>(EffectData.Period * UINT16_MAX, 0, UINT16_MAX);
	Effect.custom.samples = FMath::Clamp<uint16>(EffectData.Samples, 0, UINT16_MAX);

	uint16* DataArray = new uint16[EffectData.Data.Num()]();
	for (int i = 0; i < EffectData.Data.Num(); i++)
	{
		DataArray[i] = FMath::Clamp<uint16>(EffectData.Data[i], 0, UINT16_MAX);
	}
	Effect.custom.data = DataArray;

	Effect.custom.attack_length = FMath::Clamp<uint16>(EffectData.EnvelopeData.AttackDuration * 1000.0f, 0, UINT16_MAX);
	Effect.custom.attack_level = FMath::Clamp<uint16>(EffectData.EnvelopeData.AttackLevel * UINT16_MAX, 0, UINT16_MAX);
	Effect.custom.fade_length = FMath::Clamp<uint16>(EffectData.EnvelopeData.FadeDuration * 1000.0f, 0, UINT16_MAX);
	Effect.custom.fade_level = FMath::Clamp<uint16>(EffectData.EnvelopeData.FadeLevel * UINT16_MAX, 0, UINT16_MAX);
}

uint32 UForceFeedbackEffectCustom::GetEffectDuration()
{
	return EffectData.DurationData.GetEffectDuration();
}
