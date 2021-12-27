#pragma once

#include "ForceFeedback/Effects/ForceFeedbackEffectBase.h"
#include "ForceFeedback/Data/ForceFeedbackEffectRampData.h"
#include "ForceFeedbackEffectRamp.generated.h"

UCLASS(Blueprintable)
class JOYSTICKPLUGIN_API UForceFeedbackEffectRamp : public UForceFeedbackEffectBase
{
    GENERATED_BODY()
public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Force Feedback|Effect Data")
		FForceFeedbackEffectRampData EffectData;

protected:

	virtual SDL_HapticEffect ToSDLEffect() override;
};