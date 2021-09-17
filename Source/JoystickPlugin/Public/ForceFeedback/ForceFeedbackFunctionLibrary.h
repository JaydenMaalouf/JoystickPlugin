#pragma once

#include "ForceFeedback/Effects/ForceFeedbackEffectBase.h"

#include "Templates/SubclassOf.h"
#include "Kismet/BlueprintFunctionLibrary.h"

#include "ForceFeedbackFunctionLibrary.generated.h"

UCLASS()
class JOYSTICKPLUGIN_API UForceFeedbackFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:

	UFUNCTION(BlueprintCallable, Category = "SDL2 Force Feedback", meta = (DeterminesOutputType = "classType"))
		static UForceFeedbackEffectBase* CreateEffect(TSubclassOf<class UForceFeedbackEffectBase> classType, bool AutoStart = false);

	UFUNCTION(BlueprintPure, Category = "SDL2 Force Feedback")
		static int32 GetNumEffects(int32 DeviceId);

	UFUNCTION(BlueprintPure, Category = "SDL2 Force Feedback")
		static int32 GetNumEffectsPlaying(int32 DeviceId);

	UFUNCTION(BlueprintPure, Category = "SDL2 Force Feedback")
		static int32 GetEffectStatus(int32 DeviceId, int32 EffectId);

	UFUNCTION(BlueprintCallable, Category = "SDL2 Force Feedback")
		static bool SetAutocenter(int32 DeviceId, int32 Center);

	UFUNCTION(BlueprintCallable, Category = "SDL2 Force Feedback")
		static bool SetGain(int32 DeviceId, int32 Gain);

	UFUNCTION(BlueprintCallable, Category = "SDL2 Force Feedback")
		static void PauseDevice(int32 DeviceId);

	UFUNCTION(BlueprintCallable, Category = "SDL2 Force Feedback")
		static void UnpauseDevice(int32 DeviceId);

	UFUNCTION(BlueprintCallable, Category = "SDL2 Force Feedback")
		static void StopAllEffects(int32 DeviceId);
};
