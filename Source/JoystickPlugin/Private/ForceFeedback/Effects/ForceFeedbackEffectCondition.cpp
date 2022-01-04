#include "ForceFeedback/Effects/ForceFeedbackEffectCondition.h"
#include "ForceFeedback/Types/ForceFeedbackConditionEffectType.h"

void UForceFeedbackEffectCondition::UpdateEffectData()
{
	Super::UpdateEffectData();
	
	switch (EffectData.EffectType)
	{
		case(EForceFeedbackConditionEffectType::SPRING): 
		{
			Effect.type = SDL_HAPTIC_SPRING;
			//effect.condition.type = SDL_HAPTIC_SPRING;
			break;
		}
		case(EForceFeedbackConditionEffectType::DAMPER): 
		{
			Effect.type = SDL_HAPTIC_DAMPER;
			//effect.condition.type = SDL_HAPTIC_DAMPER;
			break;
		}
		case(EForceFeedbackConditionEffectType::FRICTION): 
		{
			Effect.type = SDL_HAPTIC_FRICTION;
			//effect.condition.type = SDL_HAPTIC_FRICTION;
			break;
		}
		case(EForceFeedbackConditionEffectType::INERTIA):
		{
			Effect.type = SDL_HAPTIC_INERTIA;
			//effect.condition.type = SDL_HAPTIC_INERTIA;
			break;
		}
		default: {}
	}

	Effect.condition.direction = EffectData.DirectionData.ToSDLDirection();
	Effect.condition.length = EffectData.DurationData.InfiniteDuration ? SDL_HAPTIC_INFINITY : FMath::Clamp(Uint32(EffectData.DurationData.Duration * 1000.0f), Uint32(0), Uint32(UINT32_MAX));
	Effect.condition.delay = FMath::Clamp(Uint16(EffectData.ReplayData.Delay * 1000.0f), Uint16(0), Uint16(UINT16_MAX));
	Effect.condition.interval = FMath::Clamp(Uint16(EffectData.ReplayData.RetriggerDelay * 1000.0f), Uint16(0), Uint16(UINT16_MAX));

	Effect.condition.right_sat[0] = FMath::Clamp(Uint16(EffectData.RightSat.X * float(UINT16_MAX)), Uint16(0), Uint16(UINT16_MAX));
	Effect.condition.right_sat[1] = FMath::Clamp(Uint16(EffectData.RightSat.Y * float(UINT16_MAX)), Uint16(0), Uint16(UINT16_MAX));
	Effect.condition.right_sat[2] = FMath::Clamp(Uint16(EffectData.RightSat.Z * float(UINT16_MAX)), Uint16(0), Uint16(UINT16_MAX));
	Effect.condition.left_sat[0] = FMath::Clamp(Uint16(EffectData.LeftSat.X * float(UINT16_MAX)), Uint16(0), Uint16(UINT16_MAX));
	Effect.condition.left_sat[1] = FMath::Clamp(Uint16(EffectData.LeftSat.Y * float(UINT16_MAX)), Uint16(0), Uint16(UINT16_MAX));
	Effect.condition.left_sat[2] = FMath::Clamp(Uint16(EffectData.LeftSat.Z * float(UINT16_MAX)), Uint16(0), Uint16(UINT16_MAX));
	Effect.condition.right_coeff[0] = FMath::Clamp(Sint16(EffectData.RightCoeff.X * float(INT16_MAX)), Sint16(INT16_MIN), Sint16(INT16_MAX));
	Effect.condition.right_coeff[1] = FMath::Clamp(Sint16(EffectData.RightCoeff.Y * float(INT16_MAX)), Sint16(INT16_MIN), Sint16(INT16_MAX));
	Effect.condition.right_coeff[2] = FMath::Clamp(Sint16(EffectData.RightCoeff.Z * float(INT16_MAX)), Sint16(INT16_MIN), Sint16(INT16_MAX));
	Effect.condition.left_coeff[0] = FMath::Clamp(Sint16(EffectData.LeftCoeff.X * float(INT16_MAX)), Sint16(INT16_MIN), Sint16(INT16_MAX));
	Effect.condition.left_coeff[1] = FMath::Clamp(Sint16(EffectData.LeftCoeff.Y * float(INT16_MAX)), Sint16(INT16_MIN), Sint16(INT16_MAX));
	Effect.condition.left_coeff[2] = FMath::Clamp(Sint16(EffectData.LeftCoeff.Z * float(INT16_MAX)), Sint16(INT16_MIN), Sint16(INT16_MAX));
	Effect.condition.deadband[0] = FMath::Clamp(Uint16(EffectData.Deadband.X * float(UINT16_MAX)), Uint16(0), Uint16(UINT16_MAX));
	Effect.condition.deadband[1] = FMath::Clamp(Uint16(EffectData.Deadband.Y * float(UINT16_MAX)), Uint16(0), Uint16(UINT16_MAX));
	Effect.condition.deadband[2] = FMath::Clamp(Uint16(EffectData.Deadband.Z * float(UINT16_MAX)), Uint16(0), Uint16(UINT16_MAX));
	Effect.condition.center[0] = FMath::Clamp(Sint16(EffectData.Center.X * float(INT16_MAX)), Sint16(INT16_MIN), Sint16(INT16_MAX));
	Effect.condition.center[1] = FMath::Clamp(Sint16(EffectData.Center.Y * float(INT16_MAX)), Sint16(INT16_MIN), Sint16(INT16_MAX));
	Effect.condition.center[2] = FMath::Clamp(Sint16(EffectData.Center.Z * float(INT16_MAX)), Sint16(INT16_MIN), Sint16(INT16_MAX));
}
