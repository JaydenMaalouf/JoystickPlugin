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
	case EForceFeedbackConditionEffectType::Friction:
		{
			Effect.type = SDL_HAPTIC_FRICTION;
			break;
		}
	case EForceFeedbackConditionEffectType::Inertia:
		{
			Effect.type = SDL_HAPTIC_INERTIA;
			break;
		}
	}

	Effect.condition.direction = EffectData.DirectionData.ToSDLDirection();
	Effect.condition.length = GetEffectDuration();
	Effect.condition.delay = FMath::Clamp<uint16>(EffectData.ReplayData.Delay * 1000.0f, 0, UINT16_MAX);
	Effect.condition.interval = FMath::Clamp<uint16>(EffectData.ReplayData.RetriggerDelay * 1000.0f, 0, UINT16_MAX);

	Effect.condition.right_sat[0] = FMath::Clamp<uint16>(EffectData.RightSat.X * UINT16_MAX, 0, UINT16_MAX);
	Effect.condition.right_sat[1] = FMath::Clamp<uint16>(EffectData.RightSat.Y * UINT16_MAX, 0, UINT16_MAX);
	Effect.condition.right_sat[2] = FMath::Clamp<uint16>(EffectData.RightSat.Z * UINT16_MAX, 0, UINT16_MAX);
	Effect.condition.left_sat[0] = FMath::Clamp<uint16>(EffectData.LeftSat.X * UINT16_MAX, 0, UINT16_MAX);
	Effect.condition.left_sat[1] = FMath::Clamp<uint16>(EffectData.LeftSat.Y * UINT16_MAX, 0, UINT16_MAX);
	Effect.condition.left_sat[2] = FMath::Clamp<uint16>(EffectData.LeftSat.Z * UINT16_MAX, 0, UINT16_MAX);
	Effect.condition.right_coeff[0] = FMath::Clamp<int16>(EffectData.RightCoefficient.X * INT16_MAX, INT16_MIN, INT16_MAX);
	Effect.condition.right_coeff[1] = FMath::Clamp<int16>(EffectData.RightCoefficient.Y * INT16_MAX, INT16_MIN, INT16_MAX);
	Effect.condition.right_coeff[2] = FMath::Clamp<int16>(EffectData.RightCoefficient.Z * INT16_MAX, INT16_MIN, INT16_MAX);
	Effect.condition.left_coeff[0] = FMath::Clamp<int16>(EffectData.LeftCoefficient.X * INT16_MAX, INT16_MIN, INT16_MAX);
	Effect.condition.left_coeff[1] = FMath::Clamp<int16>(EffectData.LeftCoefficient.Y * INT16_MAX, INT16_MIN, INT16_MAX);
	Effect.condition.left_coeff[2] = FMath::Clamp<int16>(EffectData.LeftCoefficient.Z * INT16_MAX, INT16_MIN, INT16_MAX);
	Effect.condition.deadband[0] = FMath::Clamp<uint16>(EffectData.DeadZone.X * UINT16_MAX, 0, UINT16_MAX);
	Effect.condition.deadband[1] = FMath::Clamp<uint16>(EffectData.DeadZone.Y * UINT16_MAX, 0, UINT16_MAX);
	Effect.condition.deadband[2] = FMath::Clamp<uint16>(EffectData.DeadZone.Z * UINT16_MAX, 0, UINT16_MAX);
	Effect.condition.center[0] = FMath::Clamp<int16>(EffectData.Center.X * INT16_MAX, INT16_MIN, INT16_MAX);
	Effect.condition.center[1] = FMath::Clamp<int16>(EffectData.Center.Y * INT16_MAX, INT16_MIN, INT16_MAX);
	Effect.condition.center[2] = FMath::Clamp<int16>(EffectData.Center.Z * INT16_MAX, INT16_MIN, INT16_MAX);
}

uint32 UForceFeedbackEffectCondition::GetEffectDuration()
{
	return EffectData.DurationData.GetEffectDuration();
}
