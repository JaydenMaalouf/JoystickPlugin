#pragma once

#include "ForceFeedback/Effects/ForceFeedbackEffectBase.h"
#include "ForceFeedback/Data/ForceFeedbackEffectRampData.h"
#include "ForceFeedbackEffectRamp.generated.h"

UCLASS(Blueprintable)
class UForceFeedbackEffectRamp : public UForceFeedbackEffectBase
{
    GENERATED_BODY()
public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FForceFeedbackEffectRampData EffectData;

protected:

	SDL_HapticEffect ToSDLEffect() override;
};