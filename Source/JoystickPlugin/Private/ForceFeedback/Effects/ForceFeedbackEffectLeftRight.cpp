// JoystickPlugin is licensed under the MIT License.
// Copyright Jayden Maalouf 2026. All Rights Reserved.

#include "ForceFeedback/Effects/ForceFeedbackEffectLeftRight.h"

#include "ForceFeedback/ForceFeedbackEffectEvaluation.h"

void UForceFeedbackEffectLeftRight::DescribeEffect(FForceFeedbackEffectDebugInfo& OutInfo) const
{
	Super::DescribeEffect(OutInfo);

	OutInfo.Kind = TEXT("LeftRight");
	OutInfo.Level = FMath::Max(EffectData.LargeMagnitude, EffectData.SmallMagnitude);
	OutInfo.EvaluatedLevel = FForceFeedbackEffectEvaluation::EvaluateLeftRight(EffectData, GetEvaluationTime());
}

void UForceFeedbackEffectLeftRight::UpdateEffectData()
{
	Super::UpdateEffectData();

	Effect.type = SDL_HAPTIC_LEFTRIGHT;

	Effect.leftright.length = GetEffectDuration();

	Effect.leftright.large_magnitude = FForceFeedbackConversion::ToUnsignedLevel(EffectData.LargeMagnitude);
	Effect.leftright.small_magnitude = FForceFeedbackConversion::ToUnsignedLevel(EffectData.SmallMagnitude);
}

uint32 UForceFeedbackEffectLeftRight::GetEffectDuration() const
{
	return EffectData.DurationData.GetEffectDuration();
}
