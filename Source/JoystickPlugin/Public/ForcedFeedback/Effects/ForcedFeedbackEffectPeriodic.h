#pragma once

#include "ForcedFeedback/Effects/ForcedFeedbackEffectBase.h"
#include "ForcedFeedback/Data/ForcedFeedbackEffectPeriodicData.h"
#include "ForcedFeedbackEffectPeriodic.generated.h"

UCLASS(Blueprintable)
class UForcedFeedbackEffectPeriodic : public UForcedFeedbackEffectBase
{
    GENERATED_BODY()
public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FForcedFeedbackEffectPeriodicData EffectData;

	SDL_HapticEffect ToSDLEffect() override;
};