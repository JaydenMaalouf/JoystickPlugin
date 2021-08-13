#pragma once

#include "ForceFeedback/Effects/ForceFeedbackEffectBase.h"
#include "ForceFeedback/Data/ForceFeedbackEffectPeriodicData.h"
#include "ForceFeedbackEffectPeriodic.generated.h"

UCLASS(Blueprintable)
class UForceFeedbackEffectPeriodic : public UForceFeedbackEffectBase
{
    GENERATED_BODY()
public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FForceFeedbackEffectPeriodicData EffectData;

protected:

	SDL_HapticEffect ToSDLEffect() override;
};