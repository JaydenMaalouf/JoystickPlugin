#include "ForcedFeedback/Effects/ForcedFeedbackEffectLeftRight.h"

SDL_HapticEffect UForcedFeedbackEffectLeftRight::ToSDLEffect()
{
	SDL_HapticEffect effect = Super::ToSDLEffect();

	effect.type = SDL_HAPTIC_LEFTRIGHT;

	return effect;
}