#pragma once

#include "ForcedFeedback/Effects/ForcedFeedbackEffectBase.h"
#include "ForcedFeedback/Data/ForcedFeedbackEffectLeftRightData.h"
#include "ForcedFeedbackEffectLeftRight.generated.h"

UCLASS(Blueprintable)
class UForcedFeedbackEffectLeftRight : public UForcedFeedbackEffectBase
{
    GENERATED_BODY()
public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FForcedFeedbackEffectLeftRightData EffectData;

	SDL_HapticEffect ToSDLEffect() override;
};