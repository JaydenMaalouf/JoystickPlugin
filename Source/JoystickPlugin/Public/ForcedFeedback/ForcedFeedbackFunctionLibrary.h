#pragma once


#include "ForcedFeedbackFunctionLibrary.generated.h"

UCLASS()
class UForcedFeedbackFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:

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
