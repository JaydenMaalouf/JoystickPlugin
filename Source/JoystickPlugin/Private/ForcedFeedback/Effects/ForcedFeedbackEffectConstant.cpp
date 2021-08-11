#include "ForcedFeedback/Effects/ForcedFeedbackEffectConstant.h"

SDL_HapticEffect UForcedFeedbackEffectConstant::ToSDLEffect()
{
	SDL_HapticEffect effect = Super::ToSDLEffect();

	effect.type = SDL_HAPTIC_CONSTANT;

	switch (EffectData.Direction.DirectionType)
	{
	case(EForcedFeedbackDirectionType::CARTESIAN): {
		effect.constant.direction.type = SDL_HAPTIC_CARTESIAN;
		break;
	}
	case(EForcedFeedbackDirectionType::POLAR): {
		effect.constant.direction.type = SDL_HAPTIC_POLAR;
		break;
	}
	case(EForcedFeedbackDirectionType::SPHERICAL): {
		effect.constant.direction.type = SDL_HAPTIC_SPHERICAL;
		break;
	}
	default: {}
	}

	effect.constant.direction.dir[0] = Sint32(EffectData.Direction.Direction.X);
	effect.constant.direction.dir[1] = Sint32(EffectData.Direction.Direction.Y);
	effect.constant.direction.dir[2] = Sint32(EffectData.Direction.Direction.Z);
	effect.constant.level = FMath::Clamp(Sint16(EffectData.Level), Sint16(INT16_MIN), Sint16(INT16_MAX));
	effect.constant.delay = FMath::Clamp(Uint16(EffectData.Delay), Uint16(0), Uint16(UINT16_MAX));
	effect.constant.attack_length = FMath::Clamp(Uint16(EffectData.AttackLength), Uint16(0), Uint16(UINT16_MAX));
	effect.constant.attack_level = FMath::Clamp(Uint16(EffectData.AttackLevel), Uint16(0), Uint16(UINT16_MAX));
	effect.constant.fade_length = FMath::Clamp(Uint16(EffectData.FadeLength), Uint16(0), Uint16(UINT16_MAX));
	effect.constant.fade_level = FMath::Clamp(Uint16(EffectData.FadeLevel), Uint16(0), Uint16(UINT16_MAX));

	if (EffectData.InfiniteLength) {
		effect.constant.length = SDL_HAPTIC_INFINITY;
	}
	else {
		effect.constant.length = FMath::Clamp(Uint32(EffectData.Length), Uint32(0), Uint32(UINT32_MAX));
	}

	return effect;
}