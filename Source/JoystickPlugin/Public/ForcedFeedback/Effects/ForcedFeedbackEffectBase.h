#pragma once

THIRD_PARTY_INCLUDES_START

#include "SDL.h"

THIRD_PARTY_INCLUDES_END

#include "ForcedFeedbackEffectBase.generated.h"

UCLASS(BlueprintType)
class UForcedFeedbackEffectBase : public UObject
{
    GENERATED_BODY()
public:

    void PostInitProperties() override;

	void BeginDestroy() override;

    UFUNCTION(BlueprintCallable)
        void Init();

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
		int32 EffectId;

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

	virtual SDL_HapticEffect ToSDLEffect();

private:

    UWorld* GetWorld() const;
};