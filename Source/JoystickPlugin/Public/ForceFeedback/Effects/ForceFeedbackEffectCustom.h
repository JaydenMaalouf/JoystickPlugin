#pragma once

#include "ForceFeedback/Effects/ForceFeedbackEffectBase.h"
#include "ForceFeedback/Data/ForceFeedbackEffectCustomData.h"
#include "ForceFeedbackEffectCustom.generated.h"

UCLASS(Blueprintable)
class UForceFeedbackEffectCustom : public UForceFeedbackEffectBase
{
    GENERATED_BODY()
public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FForceFeedbackEffectCustomData EffectData;

protected:

	SDL_HapticEffect ToSDLEffect() override;
};