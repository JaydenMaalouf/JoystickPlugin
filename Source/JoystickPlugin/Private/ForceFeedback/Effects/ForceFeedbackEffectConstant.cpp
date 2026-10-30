// JoystickPlugin is licensed under the MIT License.
// Copyright Jayden Maalouf 2026. All Rights Reserved.

#include "ForceFeedback/Effects/ForceFeedbackEffectConstant.h"

#include "ForceFeedback/ForceFeedbackEffectEvaluation.h"

void UForceFeedbackEffectConstant::DescribeEffect(FForceFeedbackEffectDebugInfo& OutInfo) const
{
	Super::DescribeEffect(OutInfo);

	OutInfo.Kind = TEXT("Constant");
	OutInfo.Level = EffectData.EffectLevel;
	OutInfo.EvaluatedLevel = FForceFeedbackEffectEvaluation::EvaluateConstant(EffectData, GetEvaluationTime());
}

void UForceFeedbackEffectConstant::UpdateEffectData()
{
	Super::UpdateEffectData();

	Effect.type = SDL_HAPTIC_CONSTANT;

	Effect.constant.direction = EffectData.DirectionData.ToSDLDirection();
	Effect.constant.length = GetEffectDuration();
	Effect.constant.delay = FForceFeedbackConversion::SecondsToMilliseconds(EffectData.ReplayData.Delay);
	Effect.constant.interval = FForceFeedbackConversion::SecondsToMilliseconds(EffectData.ReplayData.RetriggerDelay);

	Effect.constant.level = FForceFeedbackConversion::ToSignedLevel(EffectData.EffectLevel);
	Effect.constant.attack_length = FForceFeedbackConversion::SecondsToMilliseconds(EffectData.EnvelopeData.AttackDuration);
	Effect.constant.attack_level = FForceFeedbackConversion::ToUnsignedLevel(EffectData.EnvelopeData.AttackLevel);
	Effect.constant.fade_length = FForceFeedbackConversion::SecondsToMilliseconds(EffectData.EnvelopeData.FadeDuration);
	Effect.constant.fade_level = FForceFeedbackConversion::ToUnsignedLevel(EffectData.EnvelopeData.FadeLevel);
}

uint32 UForceFeedbackEffectConstant::GetEffectDuration() const
{
	return EffectData.DurationData.GetEffectDuration();
}

uint32 UForceFeedbackEffectConstant::GetEffectDelay() const
{
	return FForceFeedbackConversion::SecondsToMilliseconds(EffectData.ReplayData.Delay);
}
