#include "ForcedFeedback/Effects/ForcedFeedbackEffectCustom.h"

SDL_HapticEffect UForcedFeedbackEffectCustom::ToSDLEffect()
{
	SDL_HapticEffect effect = Super::ToSDLEffect();

	effect.type = SDL_HAPTIC_CUSTOM;

	return effect;
}