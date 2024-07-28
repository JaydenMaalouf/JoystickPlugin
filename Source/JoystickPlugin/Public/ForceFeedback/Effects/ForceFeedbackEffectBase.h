// JoystickPlugin is licensed under the MIT License.
// Copyright Jayden Maalouf. All Rights Reserved.

#pragma once

#include "Tickable.h"
#include "Data/JoystickInstanceId.h"
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

	virtual void PostInitProperties() override;
	virtual void BeginDestroy() override;

	// Begin FTickableGameObject Interface.
	virtual void Tick(float DeltaTime) override;
	virtual bool IsTickable() const override { return Tickable; }
	virtual bool IsTickableInEditor() const override { return TickableInEditor; }
	virtual bool IsTickableWhenPaused() const override { return TickableWhenPaused; }
	virtual TStatId GetStatId() const override { return TStatId(); }
	// End FTickableGameObject Interface.

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

	UFUNCTION(BlueprintImplementableEvent, meta=(DisplayName = "Tick"))
	void ReceiveTick(float DeltaSeconds);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Force Feedback|Functions")
	int EffectStatus() const;

	UFUNCTION(BlueprintCallable, Category = "Force Feedback|Functions")
	void SetInstanceId(const FJoystickInstanceId& NewInstanceId);

	UFUNCTION(BlueprintCallable, Category = "Force Feedback|Functions")
	const FJoystickInstanceId& GetInstanceId() const;

	UFUNCTION(BlueprintCallable, Category = "Force Feedback|Functions")
	void SetTickable(const bool NewTickable);

	UFUNCTION(BlueprintCallable, Category = "Force Feedback|Functions")
	void SetTickableInEditor(const bool NewTickableInEditor);

	UFUNCTION(BlueprintCallable, Category = "Force Feedback|Functions")
	void SetTickableWhenPaused(const bool NewTickableWhenPaused);

	UFUNCTION(BlueprintCallable, Category = "Force Feedback|Functions")
	AActor* GetOwningActor() const;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Force Feedback", meta = (ExposeOnSpawn = true))
	FJoystickInstanceId InstanceId;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadonly, Category = "Force Feedback")
	int EffectId;

	UPROPERTY(VisibleAnywhere, BlueprintReadonly, Category = "Force Feedback")
	bool IsInitialised;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Force Feedback")
	FForceFeedbackEffectConfiguration Configuration;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Force Feedback", meta = (ExposeOnSpawn = true, EditCondition="!InfiniteIterations", UIMin="0", ClampMin="0"))
	int Iterations;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Force Feedback", meta = (ExposeOnSpawn = true))
	bool InfiniteIterations;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Force Feedback|Tick")
	bool Tickable;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Force Feedback|Tick")
	bool TickableInEditor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Force Feedback|Tick")
	bool TickableWhenPaused;

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
	SDL_HapticEffect Effect;

	virtual void CreateEffect();
	virtual void UpdateEffectData();
};
