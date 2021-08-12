#pragma once

#include "ForcedFeedback/Effects/ForcedFeedbackEffectBase.h"
#include "ForcedFeedback/Data/ForcedFeedbackEffectConditionData.h"
#include "ForcedFeedbackEffectCondition.generated.h"

UCLASS(Blueprintable)
class UForcedFeedbackEffectCondition : public UForcedFeedbackEffectBase
{
    GENERATED_BODY()
public:

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FForcedFeedbackEffectConditionData EffectData;

protected:

    SDL_HapticEffect ToSDLEffect() override;
};