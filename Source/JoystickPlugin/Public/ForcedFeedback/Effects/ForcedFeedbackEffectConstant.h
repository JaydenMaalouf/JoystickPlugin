#pragma once

#include "ForcedFeedback/Effects/ForcedFeedbackEffectBase.h"
#include "ForcedFeedback/Data/ForcedFeedbackEffectConstantData.h"
#include "ForcedFeedbackEffectConstant.generated.h"

UCLASS(Blueprintable)
class UForcedFeedbackEffectConstant : public UForcedFeedbackEffectBase
{
    GENERATED_BODY()
public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FForcedFeedbackEffectConstantData EffectData;

	SDL_HapticEffect ToSDLEffect() override;
};