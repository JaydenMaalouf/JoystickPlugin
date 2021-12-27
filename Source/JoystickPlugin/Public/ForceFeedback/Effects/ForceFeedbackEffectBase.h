#pragma once

#include "UObject/Object.h"

THIRD_PARTY_INCLUDES_START

#include "SDL_haptic.h"

THIRD_PARTY_INCLUDES_END

#include "ForceFeedbackEffectBase.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnInitialisedEffect, UForceFeedbackEffectBase*, effect);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnStartedEffect, UForceFeedbackEffectBase*, effect);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnStoppedEffect, UForceFeedbackEffectBase*, effect);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnUpdatedEffect, UForceFeedbackEffectBase*, effect);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDestroyedEffect, UForceFeedbackEffectBase*, effect);

UCLASS(BlueprintType)
class JOYSTICKPLUGIN_API UForceFeedbackEffectBase : public UObject
{
    GENERATED_BODY()
public:

	void BeginDestroy() override;

    UFUNCTION(BlueprintCallable, Category = "Force Feedback|Functions")
        void InitialiseEffect();

	UFUNCTION(BlueprintCallable, Category = "Force Feedback|Functions")
		void StartEffect();

    UFUNCTION(BlueprintCallable, Category = "Force Feedback|Functions")
		void StopEffect();

	UFUNCTION(BlueprintCallable, Category = "Force Feedback|Functions")
		void DestroyEffect();

	UFUNCTION(BlueprintCallable, Category = "Force Feedback|Functions")
		void UpdateEffect();

	UFUNCTION(BlueprintImplementableEvent, Category = "Force Feedback|Events")
		void OnInitialisedEffect();

	UFUNCTION(BlueprintImplementableEvent, Category = "Force Feedback|Events")
		void OnStartedEffect();

	UFUNCTION(BlueprintImplementableEvent, Category = "Force Feedback|Events")
		void OnStoppedEffect();

	UFUNCTION(BlueprintImplementableEvent, Category = "Force Feedback|Events")
		void OnUpdatedEffect();

	UFUNCTION(BlueprintImplementableEvent, Category = "Force Feedback|Events")
		void OnDestroyedEffect();

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Force Feedback|Functions")
		int32 EffectStatus();

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Force Feedback|Effect Data", meta = (ExposeOnSpawn = true))
		int32 DeviceId;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadonly, Category = "Force Feedback|Effect Data")
		int32 EffectId;

	UPROPERTY(VisibleAnywhere, BlueprintReadonly, Category = "Force Feedback|Effect Data")
		bool IsInitialised = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Force Feedback|Effect Data", meta = (ExposeOnSpawn = true))
		bool AutoStartOnInit = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Force Feedback|Effect Data", meta = (ExposeOnSpawn = true))
		int32 Iterations = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Force Feedback|Effect Data", meta = (ExposeOnSpawn = true))
		bool Infinite = false;

	UPROPERTY(BlueprintAssignable, meta = (DisplayName = "OnInitialisedEffect"), Category = "Force Feedback|Delegates")
		FOnInitialisedEffect OnInitialisedEffectDelegate;

	UPROPERTY(BlueprintAssignable, meta = (DisplayName = "OnStartedEffect"), Category = "Force Feedback|Delegates")
		FOnStartedEffect OnStartedEffectDelegate;

	UPROPERTY(BlueprintAssignable, meta = (DisplayName = "OnStoppedEffect"), Category = "Force Feedback|Delegates")
		FOnStoppedEffect OnStoppedEffectDelegate;

	UPROPERTY(BlueprintAssignable, meta = (DisplayName = "OnUpdatedEffect"), Category = "Force Feedback|Delegates")
		FOnUpdatedEffect OnUpdatedEffectDelegate;

	UPROPERTY(BlueprintAssignable, meta = (DisplayName = "OnDestroyedEffect"), Category = "Force Feedback|Delegates")
		FOnDestroyedEffect OnDestroyedEffectDelegate;

protected:

	virtual SDL_HapticEffect ToSDLEffect();
};