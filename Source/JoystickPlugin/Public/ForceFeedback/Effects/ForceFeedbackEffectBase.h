#pragma once

THIRD_PARTY_INCLUDES_START

#include "SDL_haptic.h"

THIRD_PARTY_INCLUDES_END

#include "UObject/Object.h"

#include "ForceFeedbackEffectBase.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnInitialisedEffect);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnStartedEffect);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnStoppedEffect);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnUpdatedEffect);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDestroyedEffect);

UCLASS(BlueprintType)
class UForceFeedbackEffectBase : public UObject
{
    GENERATED_BODY()
public:

	void BeginDestroy() override;

    UFUNCTION(BlueprintCallable)
        void InitialiseEffect();

	UFUNCTION(BlueprintCallable)
		void StartEffect();

    UFUNCTION(BlueprintCallable)
		void StopEffect();

	UFUNCTION(BlueprintCallable)
		void DestroyEffect();

	UFUNCTION(BlueprintCallable)
		void UpdateEffect();

	UFUNCTION(BlueprintImplementableEvent)
		void OnInitialisedEffect();

	UFUNCTION(BlueprintImplementableEvent)
		void OnStartedEffect();

	UFUNCTION(BlueprintImplementableEvent)
		void OnStoppedEffect();

	UFUNCTION(BlueprintImplementableEvent)
		void OnUpdatedEffect();

	UFUNCTION(BlueprintImplementableEvent)
		void OnDestroyedEffect();

	UFUNCTION(BlueprintCallable, BlueprintPure)
		int32 EffectStatus();

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effect Configuration", meta = (ExposeOnSpawn = true))
		int32 DeviceId;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadonly, Category = "Effect Configuration")
		int32 EffectId;

	UPROPERTY(VisibleAnywhere, BlueprintReadonly, Category = "Effect Configuration")
		bool IsInitialised = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effect Configuration", meta = (ExposeOnSpawn = true))
		bool AutoStartOnInit = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effect Configuration", meta = (ExposeOnSpawn = true))
		int32 Iterations = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effect Configuration", meta = (ExposeOnSpawn = true))
		bool Infinite = false;

	UPROPERTY(BlueprintAssignable, meta = (DisplayName = "OnInitialisedEffect"))
		FOnInitialisedEffect OnInitialisedEffectDelegate;

	UPROPERTY(BlueprintAssignable, meta = (DisplayName = "OnStartedEffect"))
		FOnStartedEffect OnStartedEffectDelegate;

	UPROPERTY(BlueprintAssignable, meta = (DisplayName = "OnStoppedEffect"))
		FOnStoppedEffect OnStoppedEffectDelegate;

	UPROPERTY(BlueprintAssignable, meta = (DisplayName = "OnUpdatedEffect"))
		FOnUpdatedEffect OnUpdatedEffectDelegate;

	UPROPERTY(BlueprintAssignable, meta = (DisplayName = "OnDestroyedEffect"))
		FOnDestroyedEffect OnDestroyedEffectDelegate;

protected:

	virtual SDL_HapticEffect ToSDLEffect();
};