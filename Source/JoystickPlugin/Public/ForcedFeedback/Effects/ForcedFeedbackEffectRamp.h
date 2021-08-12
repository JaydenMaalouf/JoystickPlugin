#pragma once

#include "ForcedFeedback/Effects/ForcedFeedbackEffectBase.h"
#include "ForcedFeedback/Data/ForcedFeedbackEffectRampData.h"
#include "ForcedFeedbackEffectRamp.generated.h"

UCLASS(Blueprintable)
class UForcedFeedbackEffectRamp : public UForcedFeedbackEffectBase
{
    GENERATED_BODY()
public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FForcedFeedbackEffectRampData EffectData;

protected:

	SDL_HapticEffect ToSDLEffect() override;
};