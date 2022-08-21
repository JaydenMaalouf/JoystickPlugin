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
		case(EForceFeedbackPeriodicEffectType::LEFTRIGHT):
			{
				Effect.type = SDL_HAPTIC_LEFTRIGHT;
				//effect.periodic.type = SDL_HAPTIC_LEFTRIGHT;
				break;
			}
		default:
			{
			}
	}

	Effect.periodic.direction = EffectData.DirectionData.ToSDLDirection();
	Effect.periodic.length = EffectData.DurationData.InfiniteDuration ? SDL_HAPTIC_INFINITY : FMath::Clamp(static_cast<Uint32>(EffectData.DurationData.Duration * 1000.0f), static_cast<Uint32>(0), static_cast<Uint32>(UINT32_MAX));
	Effect.periodic.delay = FMath::Clamp(static_cast<Uint16>(EffectData.ReplayData.Delay * 1000.0f), static_cast<Uint16>(0), static_cast<Uint16>(UINT16_MAX));
	Effect.periodic.interval = FMath::Clamp(static_cast<Uint16>(EffectData.ReplayData.RetriggerDelay * 1000.0f), static_cast<Uint16>(0), static_cast<Uint16>(UINT16_MAX));

	Effect.periodic.period = FMath::Clamp(static_cast<Uint16>(EffectData.Period * 1000.0f), static_cast<Uint16>(0), static_cast<Uint16>(UINT16_MAX));
	Effect.periodic.magnitude = FMath::Clamp(static_cast<Sint16>(EffectData.Magnitude * float(INT16_MAX)), static_cast<Sint16>(INT16_MIN), static_cast<Sint16>(INT16_MAX));
	Effect.periodic.offset = FMath::Clamp(static_cast<Sint16>(EffectData.Offset * 32767.0f), static_cast<Sint16>(INT16_MIN), static_cast<Sint16>(INT16_MAX));
	Effect.periodic.phase = FMath::Clamp(static_cast<Uint16>(EffectData.Phase * 100.0f), static_cast<Uint16>(0), static_cast<Uint16>(UINT16_MAX));

	Effect.periodic.attack_length = FMath::Clamp(static_cast<Uint16>(EffectData.EnvelopeData.AttackDuration * 1000.0f), static_cast<Uint16>(0), static_cast<Uint16>(UINT16_MAX));
	Effect.periodic.attack_level = FMath::Clamp(static_cast<Uint16>(EffectData.EnvelopeData.AttackLevel * float(UINT16_MAX)), static_cast<Uint16>(0), static_cast<Uint16>(UINT16_MAX));
	Effect.periodic.fade_length = FMath::Clamp(static_cast<Uint16>(EffectData.EnvelopeData.FadeDuration * 1000.0f), static_cast<Uint16>(0), static_cast<Uint16>(UINT16_MAX));
	Effect.periodic.fade_level = FMath::Clamp(static_cast<Uint16>(EffectData.EnvelopeData.FadeLevel * float(UINT16_MAX)), static_cast<Uint16>(0), static_cast<Uint16>(UINT16_MAX));
}
