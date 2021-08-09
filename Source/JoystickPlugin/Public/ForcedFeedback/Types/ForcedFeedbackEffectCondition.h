#pragma once

#include "ForcedFeedback/ForcedFeedbackEffect.h"
#include "ForcedFeedback/ForcedFeedbackEffectType.h"
#include "ForcedFeedback/ForcedFeedbackDirectionType.h"
#include "ForcedFeedbackEffectCondition.generated.h"

UCLASS(Blueprintable)
class UForcedFeedbackEffectCondition : public UForcedFeedbackEffect
{
    GENERATED_BODY()
public:

    SDL_HapticEffect ToSDLEffect() override;
};