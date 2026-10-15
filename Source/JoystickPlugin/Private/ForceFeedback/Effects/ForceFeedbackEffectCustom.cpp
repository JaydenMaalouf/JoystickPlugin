// JoystickPlugin is licensed under the MIT License.
// Copyright Jayden Maalouf 2026. All Rights Reserved.

#include "ForceFeedback/Effects/ForceFeedbackEffectCustom.h"

void UForceFeedbackEffectCustom::DescribeEffect(FForceFeedbackEffectDebugInfo& OutInfo) const
{
	Super::DescribeEffect(OutInfo);

	OutInfo.Kind = TEXT("Custom");
	OutInfo.FrequencyHz = EffectData.Period > 0.0f ? 1.0f / EffectData.Period : 0.0f;
}

void UForceFeedbackEffectCustom::UpdateEffectData()
{
	Super::UpdateEffectData();

	Effect.type = SDL_HAPTIC_CUSTOM;

	Effect.custom.direction = EffectData.DirectionData.ToSDLDirection();
	Effect.custom.length = GetEffectDuration();
	Effect.custom.delay = FForceFeedbackConversion::SecondsToMilliseconds(EffectData.ReplayData.Delay);
	Effect.custom.interval = FForceFeedbackConversion::SecondsToMilliseconds(EffectData.ReplayData.RetriggerDelay);

	Effect.custom.channels = EffectData.Channels;
	Effect.custom.period = FForceFeedbackConversion::SecondsToMilliseconds(EffectData.Period);
	Effect.custom.samples = FForceFeedbackConversion::ToSampleCount(EffectData.Samples);

	// SDL reads channels * samples entries, so size the buffer to that and leave anything Data doesn't cover at zero.
	const int32 RequiredSamples = static_cast<int32>(Effect.custom.channels) * static_cast<int32>(Effect.custom.samples);
	SampleData.Reset();
	SampleData.SetNumZeroed(RequiredSamples);

	const int32 SamplesToCopy = FMath::Min(EffectData.Data.Num(), RequiredSamples);
	for (int32 i = 0; i < SamplesToCopy; i++)
	{
		SampleData[i] = FForceFeedbackConversion::ToSample(EffectData.Data[i]);
	}
	Effect.custom.data = SampleData.GetData();

	Effect.custom.attack_length = FForceFeedbackConversion::SecondsToMilliseconds(EffectData.EnvelopeData.AttackDuration);
	Effect.custom.attack_level = FForceFeedbackConversion::ToUnsignedLevel(EffectData.EnvelopeData.AttackLevel);
	Effect.custom.fade_length = FForceFeedbackConversion::SecondsToMilliseconds(EffectData.EnvelopeData.FadeDuration);
	Effect.custom.fade_level = FForceFeedbackConversion::ToUnsignedLevel(EffectData.EnvelopeData.FadeLevel);
}

uint32 UForceFeedbackEffectCustom::GetEffectDuration() const
{
	return EffectData.DurationData.GetEffectDuration();
}

uint32 UForceFeedbackEffectCustom::GetEffectDelay() const
{
	return FForceFeedbackConversion::SecondsToMilliseconds(EffectData.ReplayData.Delay);
}
