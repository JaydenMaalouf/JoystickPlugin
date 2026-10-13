// JoystickPlugin is licensed under the MIT License.
// Copyright Jayden Maalouf 2026. All Rights Reserved.

#include "ForceFeedback/ForceFeedbackEffectEvaluation.h"

#include "ForceFeedback/Types/ForceFeedbackPeriodicEffectType.h"

float FForceFeedbackEffectEvaluation::EvaluateEnvelope(const FForceFeedbackEffectEnvelopeData& EnvelopeData, const FForceFeedbackEffectDurationData& DurationData, const float TimeSeconds)
{
	if (TimeSeconds < 0.0f)
	{
		return 0.0f;
	}

	if (EnvelopeData.AttackDuration > 0.0f && TimeSeconds < EnvelopeData.AttackDuration)
	{
		return FMath::Lerp(EnvelopeData.AttackLevel, 1.0f, TimeSeconds / EnvelopeData.AttackDuration);
	}

	// SDL_HAPTIC_INFINITY effects never reach their fade window, they only ever attack then sustain.
	if (!DurationData.InfiniteDuration && EnvelopeData.FadeDuration > 0.0f)
	{
		const float FadeStart = DurationData.Duration - EnvelopeData.FadeDuration;
		if (TimeSeconds > FadeStart)
		{
			const float FadeAlpha = FMath::Clamp((TimeSeconds - FadeStart) / EnvelopeData.FadeDuration, 0.0f, 1.0f);
			return FMath::Lerp(1.0f, EnvelopeData.FadeLevel, FadeAlpha);
		}
	}

	return 1.0f;
}

float FForceFeedbackEffectEvaluation::EvaluatePeriodic(const FForceFeedbackEffectPeriodicData& EffectData, const float TimeSeconds)
{
	if (TimeSeconds < 0.0f || (!EffectData.DurationData.InfiniteDuration && TimeSeconds > EffectData.DurationData.Duration))
	{
		return 0.0f;
	}

	float Raw = 0.0f;
	if (EffectData.Period > 0.0f)
	{
		const float PhaseFraction = EffectData.Phase / 360.0f;
		const float CyclePosition = FMath::Frac(TimeSeconds / EffectData.Period + PhaseFraction);

		switch (EffectData.EffectType)
		{
			case EForceFeedbackPeriodicEffectType::Sine:
				Raw = FMath::Sin(2.0f * PI * CyclePosition);
				break;
			case EForceFeedbackPeriodicEffectType::Triangle:
				Raw = CyclePosition < 0.5f ? (-1.0f + 4.0f * CyclePosition) : (3.0f - 4.0f * CyclePosition);
				break;
			case EForceFeedbackPeriodicEffectType::SawtoothUp:
				Raw = 2.0f * CyclePosition - 1.0f;
				break;
			case EForceFeedbackPeriodicEffectType::SawtoothDown:
				Raw = 1.0f - 2.0f * CyclePosition;
				break;
			case EForceFeedbackPeriodicEffectType::Square:
				Raw = CyclePosition < 0.5f ? 1.0f : -1.0f;
				break;
			default:
				break;
		}
	}

	const float Value = Raw * EffectData.Magnitude + EffectData.Offset;
	const float Envelope = EvaluateEnvelope(EffectData.EnvelopeData, EffectData.DurationData, TimeSeconds);
	return FMath::Clamp(Value * Envelope, -1.0f, 1.0f);
}

float FForceFeedbackEffectEvaluation::EvaluateConstant(const FForceFeedbackEffectConstantData& EffectData, const float TimeSeconds)
{
	if (TimeSeconds < 0.0f || (!EffectData.DurationData.InfiniteDuration && TimeSeconds > EffectData.DurationData.Duration))
	{
		return 0.0f;
	}

	const float Envelope = EvaluateEnvelope(EffectData.EnvelopeData, EffectData.DurationData, TimeSeconds);
	return FMath::Clamp(EffectData.EffectLevel * Envelope, -1.0f, 1.0f);
}

float FForceFeedbackEffectEvaluation::EvaluateRamp(const FForceFeedbackEffectRampData& EffectData, const float TimeSeconds)
{
	if (TimeSeconds < 0.0f || (!EffectData.DurationData.InfiniteDuration && TimeSeconds > EffectData.DurationData.Duration))
	{
		return 0.0f;
	}

	const float RampDuration = EffectData.DurationData.InfiniteDuration ? 1.0f : FMath::Max(EffectData.DurationData.Duration, KINDA_SMALL_NUMBER);
	const float RampAlpha = FMath::Clamp(TimeSeconds / RampDuration, 0.0f, 1.0f);
	const float Value = FMath::Lerp(EffectData.RampStart, EffectData.RampEnd, RampAlpha);
	const float Envelope = EvaluateEnvelope(EffectData.EnvelopeData, EffectData.DurationData, TimeSeconds);
	return FMath::Clamp(Value * Envelope, -1.0f, 1.0f);
}

float FForceFeedbackEffectEvaluation::EvaluateLeftRight(const FForceFeedbackEffectLeftRightData& EffectData, const float TimeSeconds)
{
	if (TimeSeconds < 0.0f || (!EffectData.DurationData.InfiniteDuration && TimeSeconds > EffectData.DurationData.Duration))
	{
		return 0.0f;
	}

	// SDL_HAPTIC_LEFTRIGHT drives two independent motors with no shared waveform, envelope, or offset;
	// average the pair so the single-line graph still shows the effect running at some output level.
	return FMath::Clamp((EffectData.LargeMagnitude + EffectData.SmallMagnitude) * 0.5f, 0.0f, 1.0f);
}
