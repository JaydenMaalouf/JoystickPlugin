#pragma once

#include "ForceFeedback/Effects/ForceFeedbackEffectBase.h"
#include "ForceFeedback/Data/ForceFeedbackEffectPeriodicData.h"
#include "ForceFeedbackEffectPeriodic.generated.h"

UCLASS(Blueprintable)
class JOYSTICKPLUGIN_API UForceFeedbackEffectPeriodic : public UForceFeedbackEffectBase
{
    GENERATED_BODY()
public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Force Feedback|Effect Data")
		FForceFeedbackEffectPeriodicData EffectData;

protected:

	virtual SDL_HapticEffect ToSDLEffect() override;
};