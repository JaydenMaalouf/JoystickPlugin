// JoystickPlugin is licensed under the MIT License.
// Copyright Jayden Maalouf 2026. All Rights Reserved.

#include "ForceFeedback/Effects/ForceFeedbackEffectCondition.h"
#include "ForceFeedback/Types/ForceFeedbackConditionEffectType.h"

void UForceFeedbackEffectCondition::UpdateEffectData()
{
	Super::UpdateEffectData();

	switch (EffectData.EffectType)
	{
		default:
		case EForceFeedbackConditionEffectType::Spring:
			{
				Effect.type = SDL_HAPTIC_SPRING;
				break;
			}
		case EForceFeedbackConditionEffectType::Damper:
			{
				Effect.type = SDL_HAPTIC_DAMPER;
				break;
			}
		case EForceFeedbackConditionEffectType::Inertia:
			{
				Effect.type = SDL_HAPTIC_INERTIA;
				break;
			}
		case EForceFeedbackConditionEffectType::Friction:
			{
				Effect.type = SDL_HAPTIC_FRICTION;
				break;
			}
	}

	Effect.condition.direction = EffectData.DirectionData.ToSDLDirection();
	Effect.condition.length = GetEffectDuration();
	Effect.condition.delay = FForceFeedbackConversion::SecondsToMilliseconds(EffectData.ReplayData.Delay);
	Effect.condition.interval = FForceFeedbackConversion::SecondsToMilliseconds(EffectData.ReplayData.RetriggerDelay);

	Effect.condition.right_sat[0] = FForceFeedbackConversion::ToFullRangeLevel(EffectData.RightSat.X);
	Effect.condition.right_sat[1] = FForceFeedbackConversion::ToFullRangeLevel(EffectData.RightSat.Y);
	Effect.condition.right_sat[2] = FForceFeedbackConversion::ToFullRangeLevel(EffectData.RightSat.Z);
	Effect.condition.left_sat[0] = FForceFeedbackConversion::ToFullRangeLevel(EffectData.LeftSat.X);
	Effect.condition.left_sat[1] = FForceFeedbackConversion::ToFullRangeLevel(EffectData.LeftSat.Y);
	Effect.condition.left_sat[2] = FForceFeedbackConversion::ToFullRangeLevel(EffectData.LeftSat.Z);
	Effect.condition.right_coeff[0] = FForceFeedbackConversion::ToSignedLevel(EffectData.RightCoefficient.X);
	Effect.condition.right_coeff[1] = FForceFeedbackConversion::ToSignedLevel(EffectData.RightCoefficient.Y);
	Effect.condition.right_coeff[2] = FForceFeedbackConversion::ToSignedLevel(EffectData.RightCoefficient.Z);
	Effect.condition.left_coeff[0] = FForceFeedbackConversion::ToSignedLevel(EffectData.LeftCoefficient.X);
	Effect.condition.left_coeff[1] = FForceFeedbackConversion::ToSignedLevel(EffectData.LeftCoefficient.Y);
	Effect.condition.left_coeff[2] = FForceFeedbackConversion::ToSignedLevel(EffectData.LeftCoefficient.Z);
	Effect.condition.deadband[0] = FForceFeedbackConversion::ToFullRangeLevel(EffectData.DeadZone.X);
	Effect.condition.deadband[1] = FForceFeedbackConversion::ToFullRangeLevel(EffectData.DeadZone.Y);
	Effect.condition.deadband[2] = FForceFeedbackConversion::ToFullRangeLevel(EffectData.DeadZone.Z);
	Effect.condition.center[0] = FForceFeedbackConversion::ToSignedLevel(EffectData.Center.X);
	Effect.condition.center[1] = FForceFeedbackConversion::ToSignedLevel(EffectData.Center.Y);
	Effect.condition.center[2] = FForceFeedbackConversion::ToSignedLevel(EffectData.Center.Z);
}

uint32 UForceFeedbackEffectCondition::GetEffectDuration()
{
	return EffectData.DurationData.GetEffectDuration();
}
