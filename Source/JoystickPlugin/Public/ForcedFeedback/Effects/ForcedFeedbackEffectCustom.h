#pragma once

#include "ForcedFeedback/Effects/ForcedFeedbackEffectBase.h"
#include "ForcedFeedback/Data/ForcedFeedbackEffectCustomData.h"
#include "ForcedFeedbackEffectCustom.generated.h"

UCLASS(Blueprintable)
class UForcedFeedbackEffectCustom : public UForcedFeedbackEffectBase
{
    GENERATED_BODY()
public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FForcedFeedbackEffectCustomData EffectData;

	SDL_HapticEffect ToSDLEffect() override;
};