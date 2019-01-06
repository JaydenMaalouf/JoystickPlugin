#pragma once

#include "JoystickInterface.h"
#include "FeedbackEffect.h"

#include "FeedbackFunctions.generated.h"

UCLASS(BlueprintType)
class UFeedbackFunctions : public UObject
{
	GENERATED_UCLASS_BODY()
public:

	UFUNCTION(BlueprintPure, Category = "Force Feedback")
	static int GetNumEffects(int32 DeviceId);

	UFUNCTION(BlueprintPure, Category = "Force Feedback")
	static int GetNumEffectsPlaying(int32 DeviceId);

	UFUNCTION(BlueprintPure, Category = "Force Feedback")
	static int GetEffectStatus(int32 DeviceId, int32 EffectId);

	UFUNCTION(BlueprintCallable, Category = "Force Feedback")
	static bool SetAutocenter(int32 DeviceId, int Center);

	UFUNCTION(BlueprintCallable, Category = "Force Feedback")
	static bool SetGain(int32 DeviceId, int Gain);

	UFUNCTION(BlueprintCallable, Category = "Force Feedback")
	static int32 CreateEffect(int32 DeviceId, const FFeedbackData EffectData, int NumIterations = 1, bool Infinity = false);

	UFUNCTION(BlueprintCallable, Category = "Force Feedback")
	static void UpdateEffect(int32 DeviceId, int32 EffectId, FFeedbackData EffectData);

	UFUNCTION(BlueprintCallable, Category = "Force Feedback")
	static void PauseDevice(int32 DeviceId);

	UFUNCTION(BlueprintCallable, Category = "Force Feedback")
	static void UnpauseDevice(int32 DeviceId);

	UFUNCTION(BlueprintCallable, Category = "Force Feedback")
	static void StopEffect(int32 DeviceId, int32 EffectId);

	UFUNCTION(BlueprintCallable, Category = "Force Feedback")
	static void StopAllEffects(int32 DeviceId);

	UFUNCTION(BlueprintCallable, Category = "Force Feedback")
	static void DestroyEffect(int32 DeviceId, int32 EffectId);
};
