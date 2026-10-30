// JoystickPlugin is licensed under the MIT License.
// Copyright Jayden Maalouf 2026. All Rights Reserved.

#include "ForceFeedback/Effects/ForceFeedbackEffectPeriodic.h"

#include "ForceFeedback/ForceFeedbackEffectEvaluation.h"
#include "ForceFeedback/Types/ForceFeedbackPeriodicEffectType.h"

void UForceFeedbackEffectPeriodic::DescribeEffect(FForceFeedbackEffectDebugInfo& OutInfo) const
{
	Super::DescribeEffect(OutInfo);

	OutInfo.Kind = FName(*StaticEnum<EForceFeedbackPeriodicEffectType>()->GetNameStringByValue(static_cast<int64>(EffectData.EffectType)));
	OutInfo.Level = EffectData.Magnitude;
	OutInfo.FrequencyHz = EffectData.Period > 0.0f ? 1.0f / EffectData.Period : 0.0f;
	OutInfo.EvaluatedLevel = FForceFeedbackEffectEvaluation::EvaluatePeriodic(EffectData, GetEvaluationTime());
}

void UForceFeedbackEffectPeriodic::UpdateEffectData()
{
	Super::UpdateEffectData();

	switch (EffectData.EffectType)
	{
		default:
		case EForceFeedbackPeriodicEffectType::Sine:
			{
				Effect.type = SDL_HAPTIC_SINE;
				break;
			}
		case EForceFeedbackPeriodicEffectType::Square:
			{
				Effect.type = SDL_HAPTIC_SQUARE;
				break;
			}
		case EForceFeedbackPeriodicEffectType::Triangle:
			{
				Effect.type = SDL_HAPTIC_TRIANGLE;
				break;
			}
		case EForceFeedbackPeriodicEffectType::SawtoothUp:
			{
				Effect.type = SDL_HAPTIC_SAWTOOTHUP;
				break;
			}
		case EForceFeedbackPeriodicEffectType::SawtoothDown:
			{
				Effect.type = SDL_HAPTIC_SAWTOOTHDOWN;
				break;
			}
	}

	Effect.periodic.direction = EffectData.DirectionData.ToSDLDirection();
	Effect.periodic.length = GetEffectDuration();
	Effect.periodic.delay = FForceFeedbackConversion::SecondsToMilliseconds(EffectData.ReplayData.Delay);
	Effect.periodic.interval = FForceFeedbackConversion::SecondsToMilliseconds(EffectData.ReplayData.RetriggerDelay);

	Effect.periodic.period = FForceFeedbackConversion::SecondsToMilliseconds(EffectData.Period);
	Effect.periodic.magnitude = FForceFeedbackConversion::ToSignedLevel(EffectData.Magnitude);
	Effect.periodic.offset = FForceFeedbackConversion::ToSignedLevel(EffectData.Offset);
	Effect.periodic.phase = FForceFeedbackConversion::DegreesToPhase(EffectData.Phase);

	Effect.periodic.attack_length = FForceFeedbackConversion::SecondsToMilliseconds(EffectData.EnvelopeData.AttackDuration);
	Effect.periodic.attack_level = FForceFeedbackConversion::ToUnsignedLevel(EffectData.EnvelopeData.AttackLevel);
	Effect.periodic.fade_length = FForceFeedbackConversion::SecondsToMilliseconds(EffectData.EnvelopeData.FadeDuration);
	Effect.periodic.fade_level = FForceFeedbackConversion::ToUnsignedLevel(EffectData.EnvelopeData.FadeLevel);
}

uint32 UForceFeedbackEffectPeriodic::GetEffectDuration() const
{
	return EffectData.DurationData.GetEffectDuration();
}

uint32 UForceFeedbackEffectPeriodic::GetEffectDelay() const
{
	return FForceFeedbackConversion::SecondsToMilliseconds(EffectData.ReplayData.Delay);
}
