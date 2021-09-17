#pragma once

#include "ForceFeedback/Effects/ForceFeedbackEffectBase.h"
#include "ForceFeedback/Data/ForceFeedbackEffectCustomData.h"
#include "ForceFeedbackEffectCustom.generated.h"

UCLASS(Blueprintable)
class JOYSTICKPLUGIN_API UForceFeedbackEffectCustom : public UForceFeedbackEffectBase
{
    GENERATED_BODY()
public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FForceFeedbackEffectCustomData EffectData;

protected:

	SDL_HapticEffect ToSDLEffect() override;
};