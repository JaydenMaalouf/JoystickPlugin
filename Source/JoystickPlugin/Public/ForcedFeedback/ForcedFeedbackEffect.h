#pragma once

#include "ForcedFeedbackData.h"
#include "ForcedFeedbackEffect.generated.h"

UCLASS(Blueprintable)
class UForcedFeedbackEffect : public UObject
{
    GENERATED_BODY()
public:

    void PostInitProperties() override;

	void BeginDestroy() override;


    UFUNCTION(BlueprintCallable)
        void Init();

    UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
        FForcedFeedbackData GetEffect();

    UFUNCTION(BlueprintCallable, BlueprintPure)
		int32 EffectStatus();

	UFUNCTION(BlueprintCallable)
		void StartEffect();

    UFUNCTION(BlueprintCallable)
		void StopEffect();

	UFUNCTION(BlueprintCallable)
		void DestroyEffect();

	UFUNCTION(BlueprintCallable)
		void UpdateEffect();

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effect Configuration")
		int32 DeviceId;

	UPROPERTY(VisibleAnywhere, BlueprintReadonly, Category = "Effect Configuration")
		bool IsReady = false;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effect Configuration")
		bool AutoInit = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effect Configuration")
		bool AutoStart = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effect Configuration")
		int32 Iterations = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effect Configuration")
		bool Infinite = false;

private:
    UPROPERTY(EditAnywhere)
        FForcedFeedbackData EffectData;

    int32 EffectId;

    UWorld* GetWorld() const;
};