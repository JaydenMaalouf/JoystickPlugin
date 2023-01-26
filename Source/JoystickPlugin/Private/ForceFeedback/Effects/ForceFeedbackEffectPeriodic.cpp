#include "ForceFeedback/Effects/ForceFeedbackEffectPeriodic.h"
#include "ForceFeedback/Types/ForceFeedbackPeriodicEffectType.h"

void UForceFeedbackEffectPeriodic::UpdateEffectData()
{
	Super::UpdateEffectData();

	switch (EffectData.EffectType)
	{
		case(EForceFeedbackPeriodicEffectType::SINE):
			{
				Effect.type = SDL_HAPTIC_SINE;
				//effect.periodic.type = SDL_HAPTIC_SINE;
				break;
			}
		case(EForceFeedbackPeriodicEffectType::TRIANGLE):
			{
				Effect.type = SDL_HAPTIC_TRIANGLE;
				//effect.periodic.type = SDL_HAPTIC_TRIANGLE;
				break;
			}
		case(EForceFeedbackPeriodicEffectType::SAWTOOTHUP):
			{
				Effect.type = SDL_HAPTIC_SAWTOOTHUP;
				//effect.periodic.type = SDL_HAPTIC_SAWTOOTHUP;
				break;
			}
		case(EForceFeedbackPeriodicEffectType::SAWTOOTHDOWN):
			{
				Effect.type = SDL_HAPTIC_SAWTOOTHDOWN;
				//effect.periodic.type = SDL_HAPTIC_SAWTOOTHDOWN;
				break;
			}
		default:
		case(EForceFeedbackPeriodicEffectType::LEFTRIGHT):
			{
				Effect.type = SDL_HAPTIC_LEFTRIGHT;
				//effect.periodic.type = SDL_HAPTIC_LEFTRIGHT;
				break;
			}
	}

	Effect.periodic.direction = EffectData.DirectionData.ToSDLDirection();
	Effect.periodic.length = EffectData.DurationData.InfiniteDuration ? SDL_HAPTIC_INFINITY : FMath::Clamp(EffectData.DurationData.Duration * 1000.0f, 0, UINT32_MAX);
	Effect.periodic.delay = FMath::Clamp(EffectData.ReplayData.Delay * 1000.0f, 0, UINT16_MAX);
	Effect.periodic.interval = FMath::Clamp(EffectData.ReplayData.RetriggerDelay * 1000.0f, 0, UINT16_MAX);

	Effect.periodic.period = FMath::Clamp(EffectData.Period * 1000.0f, 0, UINT16_MAX);
	Effect.periodic.magnitude = FMath::Clamp(EffectData.Magnitude * INT16_MAX, INT16_MIN, INT16_MAX);
	Effect.periodic.offset = FMath::Clamp(EffectData.Offset * INT16_MAX, INT16_MIN, INT16_MAX);
	Effect.periodic.phase = FMath::Clamp(EffectData.Phase * 100.0f, 0, UINT16_MAX);

	Effect.periodic.attack_length = FMath::Clamp(EffectData.EnvelopeData.AttackDuration * 1000.0f, 0, UINT16_MAX);
	Effect.periodic.attack_level = FMath::Clamp(EffectData.EnvelopeData.AttackLevel * UINT16_MAX, 0, UINT16_MAX);
	Effect.periodic.fade_length = FMath::Clamp(EffectData.EnvelopeData.FadeDuration * 1000.0f, 0, UINT16_MAX);
	Effect.periodic.fade_level = FMath::Clamp(EffectData.EnvelopeData.FadeLevel * UINT16_MAX, 0, UINT16_MAX);
}
