// JoystickPlugin is licensed under the MIT License.
// Copyright Jayden Maalouf 2026. All Rights Reserved.

#include "ForceFeedback/Effects/ForceFeedbackEffectPeriodic.h"
#include "ForceFeedback/Types/ForceFeedbackPeriodicEffectType.h"

void UForceFeedbackEffectPeriodic::UpdateEffectData()
{
	Super::UpdateEffectData();

	switch (EffectData.EffectType)
	{
	case EForceFeedbackPeriodicEffectType::Sine:
		{
			Effect.type = SDL_HAPTIC_SINE;
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
	default:
	case EForceFeedbackPeriodicEffectType::LeftRight:
		{
			Effect.type = SDL_HAPTIC_LEFTRIGHT;
			break;
		}
	}

	Effect.periodic.direction = EffectData.DirectionData.ToSDLDirection();
	Effect.periodic.length = GetEffectDuration();
	Effect.periodic.delay = FMath::Clamp<uint16>(EffectData.ReplayData.Delay * 1000.0f, 0, UINT16_MAX);
	Effect.periodic.interval = FMath::Clamp<uint16>(EffectData.ReplayData.RetriggerDelay * 1000.0f, 0, UINT16_MAX);

	Effect.periodic.period = FMath::Clamp<uint16>(EffectData.Period * 1000.0f, 0, UINT16_MAX);
	Effect.periodic.magnitude = FMath::Clamp<int16>(EffectData.Magnitude * INT16_MAX, INT16_MIN, INT16_MAX);
	Effect.periodic.offset = FMath::Clamp<int16>(EffectData.Offset * INT16_MAX, INT16_MIN, INT16_MAX);
	Effect.periodic.phase = FMath::Clamp<uint16>(EffectData.Phase * 100.0f, 0, UINT16_MAX);

	Effect.periodic.attack_length = FMath::Clamp<uint16>(EffectData.EnvelopeData.AttackDuration * 1000.0f, 0, UINT16_MAX);
	Effect.periodic.attack_level = FMath::Clamp<uint16>(EffectData.EnvelopeData.AttackLevel * UINT16_MAX, 0, UINT16_MAX);
	Effect.periodic.fade_length = FMath::Clamp<uint16>(EffectData.EnvelopeData.FadeDuration * 1000.0f, 0, UINT16_MAX);
	Effect.periodic.fade_level = FMath::Clamp<uint16>(EffectData.EnvelopeData.FadeLevel * UINT16_MAX, 0, UINT16_MAX);
}

uint32 UForceFeedbackEffectPeriodic::GetEffectDuration()
{
	return EffectData.DurationData.GetEffectDuration();
}
