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

float FForceFeedbackEffectEvaluation::EvaluateConditionResponse(const FForceFeedbackEffectConditionData& EffectData, const float Input)
{
	// Only the first axis, as that is the one a wheel steers with. The dead zone is a half-width either side of the centre, as SDL describes it (full range at 1).
	const float Center = FMath::Clamp(static_cast<float>(EffectData.Center.X), -1.0f, 1.0f);
	const float DeadZone = FMath::Clamp(static_cast<float>(EffectData.DeadZone.X), 0.0f, 1.0f);

	// SDL's right_* fields are the positive side and left_* the negative side, which UpdateEffectData fills from Right* and Left* respectively.
	// The force builds from the edge of the dead zone, so it is continuous across it.
	float Coefficient = 0.0f;
	float Saturation = 0.0f;
	float Displacement = 0.0f;
	if (Input > Center + DeadZone)
	{
		Coefficient = EffectData.RightCoefficient.X;
		Saturation = EffectData.RightSat.X;
		Displacement = Input - (Center + DeadZone);
	}
	else if (Input < Center - DeadZone)
	{
		Coefficient = EffectData.LeftCoefficient.X;
		Saturation = EffectData.LeftSat.X;
		Displacement = Input - (Center - DeadZone);
	}
	else
	{
		return 0.0f;
	}

	// Saturation caps the magnitude of the push in either direction.
	Saturation = FMath::Clamp(Saturation, 0.0f, 1.0f);

	// Friction is a constant drag against the direction of motion (Coulomb friction), so its strength doesn't grow with speed the way a damper's does.
	if (EffectData.EffectType == EForceFeedbackConditionEffectType::Friction)
	{
		return -FMath::Sign(Displacement) * FMath::Clamp(Coefficient, -Saturation, Saturation);
	}

	return -FMath::Clamp(Coefficient * Displacement, -Saturation, Saturation);
}

float FForceFeedbackEffectEvaluation::EvaluateConditionMotion(const FForceFeedbackEffectConditionData& EffectData, const float Position, const float Velocity, const float Acceleration)
{
	switch (EffectData.EffectType)
	{
		case EForceFeedbackConditionEffectType::Spring:
			return EvaluateConditionResponse(EffectData, Position);
		case EForceFeedbackConditionEffectType::Damper:
		case EForceFeedbackConditionEffectType::Friction:
			return EvaluateConditionResponse(EffectData, Velocity);
		case EForceFeedbackConditionEffectType::Inertia:
			return EvaluateConditionResponse(EffectData, Acceleration);
	}

	return 0.0f;
}

float FForceFeedbackEffectEvaluation::EvaluateLeftRight(const FForceFeedbackEffectLeftRightData& EffectData, const float TimeSeconds)
{
	if (TimeSeconds < 0.0f || (!EffectData.DurationData.InfiniteDuration && TimeSeconds > EffectData.DurationData.Duration))
	{
		return 0.0f;
	}

	// Left/right drives two independent motors with no shared waveform, so average the pair for the graph.
	return FMath::Clamp((EffectData.LargeMagnitude + EffectData.SmallMagnitude) * 0.5f, 0.0f, 1.0f);
}
