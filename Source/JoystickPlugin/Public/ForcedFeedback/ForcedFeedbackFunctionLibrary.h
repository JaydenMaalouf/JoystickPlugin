#pragma once

#include "ForcedFeedbackData.h"

#include "ForcedFeedbackFunctionLibrary.generated.h"

UCLASS()
class UForcedFeedbackFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:

	UFUNCTION(BlueprintPure, Category = "SDL2 Force Feedback")
	static int GetNumEffects(int32 DeviceId);

	UFUNCTION(BlueprintPure, Category = "SDL2 Force Feedback")
	static int GetNumEffectsPlaying(int32 DeviceId);

	UFUNCTION(BlueprintPure, Category = "SDL2 Force Feedback")
	static int GetEffectStatus(int32 DeviceId, int32 EffectId);

	UFUNCTION(BlueprintCallable, Category = "SDL2 Force Feedback")
	static bool SetAutocenter(int32 DeviceId, int Center);

	UFUNCTION(BlueprintCallable, Category = "SDL2 Force Feedback")
	static bool SetGain(int32 DeviceId, int Gain);

	UFUNCTION(BlueprintCallable, Category = "SDL2 Force Feedback")
	static int32 CreateEffect(int32 DeviceId, const FForcedFeedbackData EffectData, int NumIterations = 1, bool Infinity = false);

	UFUNCTION(BlueprintCallable, Category = "SDL2 Force Feedback")
	static void UpdateEffect(int32 DeviceId, int32 EffectId, FForcedFeedbackData EffectData);

	UFUNCTION(BlueprintCallable, Category = "SDL2 Force Feedback")
	static void PauseDevice(int32 DeviceId);

	UFUNCTION(BlueprintCallable, Category = "SDL2 Force Feedback")
	static void UnpauseDevice(int32 DeviceId);

	UFUNCTION(BlueprintCallable, Category = "SDL2 Force Feedback")
	static void StopEffect(int32 DeviceId, int32 EffectId);

	UFUNCTION(BlueprintCallable, Category = "SDL2 Force Feedback")
	static void StopAllEffects(int32 DeviceId);

	UFUNCTION(BlueprintCallable, Category = "SDL2 Force Feedback")
		static void DestroyEffect(int32 DeviceId, int32 EffectId);
};
