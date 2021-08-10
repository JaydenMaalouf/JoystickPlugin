#pragma once

#include "ForcedFeedback/ForcedFeedbackEffect.h"
#include "ForcedFeedback/ForcedFeedbackEffectType.h"
#include "ForcedFeedbackEffectConstant.generated.h"

UCLASS(Blueprintable)
class UForcedFeedbackEffectConstant : public UForcedFeedbackEffect
{
    GENERATED_BODY()
public:

	SDL_HapticEffect ToSDLEffect() override;
};