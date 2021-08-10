#pragma once

#include "ForcedFeedback/ForcedFeedbackEffect.h"
#include "ForcedFeedback/ForcedFeedbackEffectType.h"
#include "ForcedFeedbackEffectPeriodic.generated.h"

UCLASS(Blueprintable)
class UForcedFeedbackEffectPeriodic : public UForcedFeedbackEffect
{
    GENERATED_BODY()
public:

	SDL_HapticEffect ToSDLEffect() override;
};