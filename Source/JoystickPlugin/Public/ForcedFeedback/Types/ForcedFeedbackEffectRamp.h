#pragma once

#include "ForcedFeedback/ForcedFeedbackEffect.h"
#include "ForcedFeedback/ForcedFeedbackEffectType.h"
#include "ForcedFeedbackEffectRamp.generated.h"

UCLASS(Blueprintable)
class UForcedFeedbackEffectRamp : public UForcedFeedbackEffect
{
    GENERATED_BODY()
public:

	SDL_HapticEffect ToSDLEffect() override;
};