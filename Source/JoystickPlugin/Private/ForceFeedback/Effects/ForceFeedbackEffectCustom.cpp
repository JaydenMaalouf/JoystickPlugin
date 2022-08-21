#include "ForceFeedback/Effects/ForceFeedbackEffectCustom.h"

void UForceFeedbackEffectCustom::UpdateEffectData()
{
	Super::UpdateEffectData();

	Effect.type = SDL_HAPTIC_CUSTOM;
	//effect.custom.type = SDL_HAPTIC_CUSTOM;

	Effect.custom.direction = EffectData.DirectionData.ToSDLDirection();
	Effect.custom.length = EffectData.DurationData.InfiniteDuration ? SDL_HAPTIC_INFINITY : FMath::Clamp(static_cast<Uint32>(EffectData.DurationData.Duration * 1000.0f), static_cast<Uint32>(0), static_cast<Uint32>(UINT32_MAX));
	Effect.custom.delay = FMath::Clamp(static_cast<Uint16>(EffectData.ReplayData.Delay * 1000.0f), static_cast<Uint16>(0), static_cast<Uint16>(UINT16_MAX));
	Effect.custom.interval = FMath::Clamp(static_cast<Uint16>(EffectData.ReplayData.RetriggerDelay * 1000.0f), static_cast<Uint16>(0), static_cast<Uint16>(UINT16_MAX));

	Effect.custom.channels = FMath::Clamp(static_cast<Uint8>(EffectData.Channels), static_cast<Uint8>(0), static_cast<Uint8>(UINT8_MAX));
	Effect.custom.period = FMath::Clamp(static_cast<Uint16>(EffectData.Period), static_cast<Uint16>(0), static_cast<Uint16>(UINT16_MAX));
	Effect.custom.samples = FMath::Clamp(static_cast<Uint16>(EffectData.Samples), static_cast<Uint16>(0), static_cast<Uint16>(UINT16_MAX));

	Uint16* dataArray = new Uint16[EffectData.Data.Num()]();
	for (int32 i = 0; i < EffectData.Data.Num(); i++)
	{
		dataArray[i] = FMath::Clamp(static_cast<Uint16>(EffectData.Data[i]), static_cast<Uint16>(0), static_cast<Uint16>(UINT16_MAX));
	}
	Effect.custom.data = dataArray;

	Effect.custom.attack_length = FMath::Clamp(static_cast<Uint16>(EffectData.EnvelopeData.AttackDuration * 1000.0f), static_cast<Uint16>(0), static_cast<Uint16>(UINT16_MAX));
	Effect.custom.attack_level = FMath::Clamp(static_cast<Uint16>(EffectData.EnvelopeData.AttackLevel * float(UINT16_MAX)), static_cast<Uint16>(0), static_cast<Uint16>(UINT16_MAX));
	Effect.custom.fade_length = FMath::Clamp(static_cast<Uint16>(EffectData.EnvelopeData.FadeDuration * 1000.0f), static_cast<Uint16>(0), static_cast<Uint16>(UINT16_MAX));
	Effect.custom.fade_level = FMath::Clamp(static_cast<Uint16>(EffectData.EnvelopeData.FadeLevel * float(UINT16_MAX)), static_cast<Uint16>(0), static_cast<Uint16>(UINT16_MAX));
}
