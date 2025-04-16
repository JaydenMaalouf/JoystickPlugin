// JoystickPlugin is licensed under the MIT License.
// Copyright Jayden Maalouf. All Rights Reserved.

#pragma once

#include "Tickable.h"
#include "Data/JoystickInstanceId.h"
#include "ForceFeedback/JoystickForceFeedbackComponent.h"
#include "ForceFeedback/Data/Configuration/ForceFeedbackEffectConfiguration.h"

THIRD_PARTY_INCLUDES_START
#include "SDL_haptic.h"
THIRD_PARTY_INCLUDES_END

#include "ForceFeedbackEffectBase.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnInitialisedEffect, const UForceFeedbackEffectBase*, Effect);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnStartedEffect, const UForceFeedbackEffectBase*, Effect);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnStoppedEffect, const UForceFeedbackEffectBase*, Effect);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnUpdatedEffect, const UForceFeedbackEffectBase*, Effect);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDestroyedEffect, const UForceFeedbackEffectBase*, Effect);

UCLASS(BlueprintType)
class JOYSTICKPLUGIN_API UForceFeedbackEffectBase : public UObject, public FTickableGameObject
{
	GENERATED_BODY()

public:
	UForceFeedbackEffectBase(const FObjectInitializer& ObjectInitializer);

	virtual void BeginDestroy() override;

	// Begin FTickableGameObject Interface.
	virtual void Tick(float DeltaTime) final override;
	virtual bool IsTickable() const override { return Tickable && IsInitialised; }
	virtual bool IsTickableInEditor() const override { return TickableInEditor; }
	virtual bool IsTickableWhenPaused() const override { return TickableWhenPaused; }
	virtual TStatId GetStatId() const override { RETURN_QUICK_DECLARE_CYCLE_STAT(UForceFeedbackEffectBase, STATGROUP_Tickables); }
	// End FTickableGameObject Interface.

	UFUNCTION(BlueprintCallable, Category = "Joystick|Force Feedback")
	void InitialiseEffect();

	UFUNCTION(BlueprintCallable, Category = "Joystick|Force Feedback")
	void StartEffect();

	UFUNCTION(BlueprintCallable, Category = "Joystick|Force Feedback")
	void StopEffect();

	UFUNCTION(BlueprintCallable, Category = "Joystick|Force Feedback")
	void DestroyEffect();

	UFUNCTION(BlueprintCallable, Category = "Joystick|Force Feedback")
	void UpdateEffect();

	UFUNCTION(BlueprintImplementableEvent, Category = "Joystick|Force Feedback|Events")
	void OnInitialisedEffect();

	UFUNCTION(BlueprintImplementableEvent, Category = "Joystick|Force Feedback|Events")
	void OnStartedEffect();

	UFUNCTION(BlueprintImplementableEvent, Category = "Joystick|Force Feedback|Events")
	void OnStoppedEffect();

	UFUNCTION(BlueprintImplementableEvent, Category = "Joystick|Force Feedback|Events")
	void OnUpdatedEffect();

	UFUNCTION(BlueprintImplementableEvent, Category = "Joystick|Force Feedback|Events")
	void OnDestroyedEffect();

	UFUNCTION(BlueprintNativeEvent, meta=(DisplayName = "Tick"))
	void ReceiveTick(const float DeltaTime);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Joystick|Force Feedback")
	int EffectStatus() const;

	UFUNCTION(BlueprintCallable, Category = "Joystick|Force Feedback")
	void SetInstanceId(const FJoystickInstanceId& NewInstanceId);

	UFUNCTION(BlueprintCallable, Category = "Joystick|Force Feedback")
	const FJoystickInstanceId& GetInstanceId() const;

	UFUNCTION(BlueprintCallable, Category = "Joystick|Force Feedback")
	void SetTickable(const bool NewTickable);

	UFUNCTION(BlueprintCallable, Category = "Joystick|Force Feedback")
	void SetTickableInEditor(const bool NewTickableInEditor);

	UFUNCTION(BlueprintCallable, Category = "Joystick|Force Feedback")
	void SetTickableWhenPaused(const bool NewTickableWhenPaused);

	UFUNCTION(BlueprintCallable, Category = "Joystick|Force Feedback")
	AActor* GetOwningActor() const;

	template <class T>
	T* GetOwningActor() const
	{
		UObject* Outer = GetOuter();
		if (!IsValid(Outer))
		{
			return nullptr;
		}

		if (const UJoystickForceFeedbackComponent* OuterJoystick = Cast<UJoystickForceFeedbackComponent>(Outer))
		{
			return OuterJoystick->GetOwner<T>();
		}

		return Cast<T>(Outer);
	}

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Joystick|Force Feedback", meta = (ExposeOnSpawn = true))
	FJoystickInstanceId InstanceId;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadonly, Category = "Joystick|Force Feedback")
	int EffectId;

	UPROPERTY(VisibleAnywhere, BlueprintReadonly, Category = "Joystick|Force Feedback")
	bool IsInitialised;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Joystick|Force Feedback")
	FForceFeedbackEffectConfiguration Configuration;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Joystick|Force Feedback", meta = (ExposeOnSpawn = true, EditCondition="!InfiniteIterations", UIMin="0", ClampMin="0"))
	int Iterations;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Joystick|Force Feedback", meta = (ExposeOnSpawn = true))
	bool InfiniteIterations;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Joystick|Force Feedback|Tick")
	bool Tickable;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Joystick|Force Feedback|Tick")
	bool TickableInEditor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Joystick|Force Feedback|Tick")
	bool TickableWhenPaused;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Joystick|Force Feedback|Advanced", AdvancedDisplay, meta = (EditCondition = "Tickable"))
	bool ForceStopAfterDurationLapsed;

	UPROPERTY(BlueprintAssignable, Category = "Joystick|Force Feedback|Delegates", meta = (DisplayName = "OnInitialisedEffect"))
	FOnInitialisedEffect OnInitialisedEffectDelegate;

	UPROPERTY(BlueprintAssignable, Category = "Joystick|Force Feedback|Delegates", meta = (DisplayName = "OnStartedEffect"))
	FOnStartedEffect OnStartedEffectDelegate;

	UPROPERTY(BlueprintAssignable, Category = "Joystick|Force Feedback|Delegates", meta = (DisplayName = "OnStoppedEffect"))
	FOnStoppedEffect OnStoppedEffectDelegate;

	UPROPERTY(BlueprintAssignable, Category = "Joystick|Force Feedback|Delegates", meta = (DisplayName = "OnUpdatedEffect"))
	FOnUpdatedEffect OnUpdatedEffectDelegate;

	UPROPERTY(BlueprintAssignable, Category = "Joystick|Force Feedback|Delegates", meta = (DisplayName = "OnDestroyedEffect"))
	FOnDestroyedEffect OnDestroyedEffectDelegate;

protected:
	SDL_HapticEffect Effect;

	virtual void CreateEffect();
	virtual void UpdateEffectData();
	virtual float GetEffectDuration();

private:
	// The last frame number we were ticked.
	// We don't want to tick multiple times per frame 
	uint32 LastFrameNumberWeTicked = INDEX_NONE;
	uint64 StartTime;

	friend class UJoystickForceFeedbackComponent;
};
