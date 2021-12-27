#pragma once

#include "ForceFeedback/Effects/ForceFeedbackEffectBase.h"
#include "ForceFeedback/Data/ForceFeedbackEffectConditionData.h"
#include "ForceFeedbackEffectCondition.generated.h"

UCLASS(Blueprintable)
class JOYSTICKPLUGIN_API UForceFeedbackEffectCondition : public UForceFeedbackEffectBase
{
    GENERATED_BODY()
public:

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Force Feedback|Effect Data")
		FForceFeedbackEffectConditionData EffectData;

protected:

	virtual SDL_HapticEffect ToSDLEffect() override;
};