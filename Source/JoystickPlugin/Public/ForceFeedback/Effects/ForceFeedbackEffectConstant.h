#pragma once

#include "ForceFeedback/Effects/ForceFeedbackEffectBase.h"
#include "ForceFeedback/Data/ForceFeedbackEffectConstantData.h"
#include "ForceFeedbackEffectConstant.generated.h"

UCLASS(Blueprintable)
class JOYSTICKPLUGIN_API UForceFeedbackEffectConstant : public UForceFeedbackEffectBase
{
    GENERATED_BODY()
public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FForceFeedbackEffectConstantData EffectData;

protected:

	SDL_HapticEffect ToSDLEffect() override;
};