#include "ForceFeedback/Effects/ForceFeedbackEffectCustom.h"

void UForceFeedbackEffectCustom::UpdateEffectData()
{
	Super::UpdateEffectData();

	Effect.type = SDL_HAPTIC_CUSTOM;
	//effect.custom.type = SDL_HAPTIC_CUSTOM;

	Effect.custom.direction = EffectData.DirectionData.ToSDLDirection();
	Effect.custom.length = EffectData.DurationData.InfiniteDuration ? SDL_HAPTIC_INFINITY : FMath::Clamp(EffectData.DurationData.Duration * 1000.0f, 0, UINT32_MAX);
	Effect.custom.delay = FMath::Clamp(EffectData.ReplayData.Delay * 1000.0f, 0, UINT16_MAX);
	Effect.custom.interval = FMath::Clamp(EffectData.ReplayData.RetriggerDelay * 1000.0f, 0, UINT16_MAX);

	Effect.custom.channels = FMath::Clamp(EffectData.Channels, static_cast<Uint8>(0), UINT8_MAX);
	Effect.custom.period = FMath::Clamp(EffectData.Period * UINT16_MAX, 0, UINT16_MAX);
	Effect.custom.samples = FMath::Clamp(static_cast<Uint16>(EffectData.Samples), static_cast<Uint16>(0), UINT16_MAX);

	Uint16* DataArray = new Uint16[EffectData.Data.Num()]();
	for (int i = 0; i < EffectData.Data.Num(); i++)
	{
		DataArray[i] = FMath::Clamp(static_cast<Uint16>(EffectData.Data[i]), static_cast<Uint16>(0), UINT16_MAX);
	}
	Effect.custom.data = DataArray;

	Effect.custom.attack_length = FMath::Clamp(EffectData.EnvelopeData.AttackDuration * 1000.0f, 0, UINT16_MAX);
	Effect.custom.attack_level = FMath::Clamp(EffectData.EnvelopeData.AttackLevel * UINT16_MAX, 0, UINT16_MAX);
	Effect.custom.fade_length = FMath::Clamp(EffectData.EnvelopeData.FadeDuration * 1000.0f, 0, UINT16_MAX);
	Effect.custom.fade_level = FMath::Clamp(EffectData.EnvelopeData.FadeLevel * UINT16_MAX, 0, UINT16_MAX);
}
