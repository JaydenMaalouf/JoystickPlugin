#include "ForcedFeedback/Types/ForcedFeedbackEffectConstant.h"

SDL_HapticEffect UForcedFeedbackEffectConstant::ToSDLEffect()
{
	SDL_HapticEffect effect = Super::ToSDLEffect();
	FForcedFeedbackData effectData = GetEffect();

	effect.type = SDL_HAPTIC_CONSTANT;

	//TODO ADD A CHECK ON EFFECT TYPE

	switch (effectData.DirectionType)
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

	effect.constant.direction.dir[0] = Sint32(effectData.Direction.X * INT_MAX);
	effect.constant.direction.dir[1] = Sint32(effectData.Direction.Y * INT_MAX);
	effect.constant.direction.dir[2] = Sint32(effectData.Direction.Z * INT_MAX);
	effect.constant.level = FMath::Clamp(Sint16(effectData.Level * float(INT16_MAX)), Sint16(INT16_MIN), Sint16(INT16_MAX));
	effect.constant.delay = Uint16(effectData.Delay * 1000.0f);
	effect.constant.attack_length = Uint16(effectData.AttackLength * 1000.0f);
	effect.constant.attack_level = FMath::Clamp(Uint16(effectData.AttackLevel * float(INT16_MAX)), Uint16(0), Uint16(INT16_MAX));
	effect.constant.fade_length = Uint16(effectData.FadeLength * 1000.0f);
	effect.constant.fade_level = FMath::Clamp(Uint16(effectData.FadeLevel * float(INT16_MAX)), Uint16(0), Uint16(INT16_MAX));

	if (effectData.InfiniteLength) {
		effect.constant.length = SDL_HAPTIC_INFINITY;
	}
	else {
		effect.constant.length = Uint32(effectData.Length * 1000.0f);
	}

	return effect;
}