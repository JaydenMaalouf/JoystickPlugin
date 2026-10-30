// JoystickPlugin is licensed under the MIT License.
// Copyright Jayden Maalouf 2026. All Rights Reserved.

#pragma once

#include "GameplayTagContainer.h"
#include "Tickable.h"
#include "Data/JoystickInstanceId.h"
#include "ForceFeedback/JoystickForceFeedbackComponent.h"
#include "ForceFeedback/Data/Configuration/ForceFeedbackEffectConfiguration.h"
#include "ForceFeedback/Debug/ForceFeedbackEffectDebugInfo.h"

THIRD_PARTY_INCLUDES_START
#include "JoystickSDL.h"
THIRD_PARTY_INCLUDES_END

#include "ForceFeedbackEffectBase.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnInitialisedEffect, const UForceFeedbackEffectBase*, Effect);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnStartedEffect, const UForceFeedbackEffectBase*, Effect);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnStoppedEffect, const UForceFeedbackEffectBase*, Effect);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnUpdatedEffect, const UForceFeedbackEffectBase*, Effect);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDestroyedEffect, const UForceFeedbackEffectBase*, Effect);

struct FJoystickHapticInformation;

UCLASS(BlueprintType, EditInlineNew)
class JOYSTICKPLUGIN_API UForceFeedbackEffectBase : public UObject, public FTickableGameObject
{
	GENERATED_BODY()

public:
	UForceFeedbackEffectBase(const FObjectInitializer& ObjectInitializer);

	virtual void BeginDestroy() override;

#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif

	// Begin FTickableGameObject Interface.
	virtual void Tick(float DeltaTime) final override;
	virtual bool IsTickable() const override { return Tickable && IsInitialised; }
	virtual bool IsTickableInEditor() const override { return TickableInEditor; }
	virtual bool IsTickableWhenPaused() const override { return TickableWhenPaused; }
	virtual TStatId GetStatId() const override { RETURN_QUICK_DECLARE_CYCLE_STAT(UForceFeedbackEffectBase, STATGROUP_Tickables); }
	// End FTickableGameObject Interface.

	UFUNCTION(BlueprintCallable, Category="Force Feedback")
	bool InitialiseEffect();

	UFUNCTION(BlueprintCallable, Category="Force Feedback")
	bool StartEffect();

	UFUNCTION(BlueprintCallable, Category="Force Feedback")
	void StopEffect();

	UFUNCTION(BlueprintCallable, Category="Force Feedback")
	void DestroyEffect();

	UFUNCTION(BlueprintCallable, Category="Force Feedback")
	void UpdateEffect();

	UFUNCTION(BlueprintNativeEvent, Category="Force Feedback|Events")
	void OnInitialisedEffect();
	virtual void OnInitialisedEffect_Implementation();

	UFUNCTION(BlueprintNativeEvent, Category="Force Feedback|Events")
	void OnStartedEffect();
	virtual void OnStartedEffect_Implementation();

	UFUNCTION(BlueprintNativeEvent, Category="Force Feedback|Events")
	void OnStoppedEffect();
	virtual void OnStoppedEffect_Implementation();

	UFUNCTION(BlueprintNativeEvent, Category="Force Feedback|Events")
	void OnUpdatedEffect();
	virtual void OnUpdatedEffect_Implementation();

	UFUNCTION(BlueprintNativeEvent, Category="Force Feedback|Events")
	void OnDestroyedEffect();
	virtual void OnDestroyedEffect_Implementation();

	UFUNCTION(BlueprintCallable, Category="Force Feedback")
	void TickEffect(const float DeltaTime);

	UFUNCTION(BlueprintNativeEvent)
	void ReceivedTick(const float DeltaTime);
	virtual void ReceivedTick_Implementation(const float DeltaTime);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category="Force Feedback")
	bool GetEffectStatus() const;

	// Whether the effect is running. Finite completion is reconciled by TickEffect; this doesn't query the device.
	UFUNCTION(BlueprintCallable, BlueprintPure, Category="Force Feedback")
	bool IsEffectRunning() const;

	// Seconds since the effect was last started, across all of its iterations. Zero while it isn't running.
	UFUNCTION(BlueprintCallable, BlueprintPure, Category="Force Feedback")
	float GetElapsedTime() const;

	// GetElapsedTime in whole milliseconds, read straight off the SDL tick counter the effect is timed against.
	uint64 GetElapsedTimeMilliseconds() const;

	UFUNCTION(BlueprintCallable, Category="Force Feedback")
	void SetInstanceId(const FJoystickInstanceId& NewInstanceId);

	UFUNCTION(BlueprintCallable, Category="Force Feedback")
	const FJoystickInstanceId& GetInstanceId() const;

	UFUNCTION(BlueprintCallable, Category="Force Feedback")
	void SetTickable(const bool NewTickable);

	UFUNCTION(BlueprintCallable, Category="Force Feedback")
	void SetTickableInEditor(const bool NewTickableInEditor);

	UFUNCTION(BlueprintCallable, Category="Force Feedback")
	void SetTickableWhenPaused(const bool NewTickableWhenPaused);

	UFUNCTION(BlueprintCallable, Category="Force Feedback")
	AActor* GetOwningActor() const;

	// Builds the effect data to work out its type, then checks it against the effect types the device supports.
	bool IsSupportedByDevice(const FJoystickHapticInformation& HapticInformation);

	virtual bool IsEffectValid();

	// Fills in what the debug overlay shows for this effect. Called by the overlay under the owning component's effects lock.
	virtual void DescribeEffect(FForceFeedbackEffectDebugInfo& OutInfo) const;

	// Shows a value on this effect's row in the debug overlay, replacing any earlier value with the same name.
	UFUNCTION(BlueprintCallable, Category="Force Feedback|Debug", meta=(DevelopmentOnly))
	void AddDebugValue(const FName Name, const float Value);

	// Shows a piece of text, such as a surface name, on this effect's row in the debug overlay.
	UFUNCTION(BlueprintCallable, Category="Force Feedback|Debug", meta=(DevelopmentOnly))
	void AddDebugText(const FName Name, const FName Value);

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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Force Feedback", meta=(ExposeOnSpawn=true))
	FJoystickInstanceId InstanceId;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadonly, Category="Force Feedback")
	int EffectId;

	UPROPERTY(VisibleAnywhere, BlueprintReadonly, Category="Force Feedback")
	bool IsInitialised;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Force Feedback", meta=(ExposeOnSpawn=true))
	FForceFeedbackEffectConfiguration Configuration;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Force Feedback", meta=(ExposeOnSpawn=true, EditCondition="!InfiniteIterations", UIMin="1", UIMax = "4294967295", ClampMin="1", ClampMax = "4294967295"))
	int64 Iterations;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Force Feedback", meta=(ExposeOnSpawn=true))
	bool InfiniteIterations;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Force Feedback", meta=(ToolTip="Name given to this effect by the effect group entry that spawned it."))
	FName EffectName;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Force Feedback", meta=(ToolTip="Tag given to this effect by the effect group entry that spawned it."))
	FGameplayTag EffectTag;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Force Feedback", meta=(ToolTip="Tag of the effect group that spawned this effect."))
	FGameplayTag GroupTag;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Force Feedback|Tick")
	bool Tickable;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Force Feedback|Tick")
	bool TickableInEditor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Force Feedback|Tick")
	bool TickableWhenPaused;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Force Feedback", AdvancedDisplay, meta=(ExposeOnSpawn=true, EditCondition="Tickable"))
	bool ForceStopAfterDurationLapsed;

	UPROPERTY(BlueprintAssignable, Category="Force Feedback|Delegates", meta=(DisplayName="OnInitialisedEffect"))
	FOnInitialisedEffect OnInitialisedEffectDelegate;

	UPROPERTY(BlueprintAssignable, Category="Force Feedback|Delegates", meta=(DisplayName="OnStartedEffect"))
	FOnStartedEffect OnStartedEffectDelegate;

	UPROPERTY(BlueprintAssignable, Category="Force Feedback|Delegates", meta=(DisplayName="OnStoppedEffect"))
	FOnStoppedEffect OnStoppedEffectDelegate;

	UPROPERTY(BlueprintAssignable, Category="Force Feedback|Delegates", meta=(DisplayName="OnUpdatedEffect"))
	FOnUpdatedEffect OnUpdatedEffectDelegate;

	UPROPERTY(BlueprintAssignable, Category="Force Feedback|Delegates", meta=(DisplayName="OnDestroyedEffect"))
	FOnDestroyedEffect OnDestroyedEffectDelegate;

	// The length of a single iteration in milliseconds, or SDL_HAPTIC_INFINITY.
	virtual uint32 GetEffectDuration() const;

	// The wait in milliseconds between starting the effect and its first iteration playing. Zero for effects with no replay data.
	virtual uint32 GetEffectDelay() const;

	// The number of iterations the device is asked to run, or SDL_HAPTIC_INFINITY. Lower than Iterations where the platform limits the total play time.
	uint32 GetRunIterations() const;

	// The most iterations the platform can play at this effect's duration, or SDL_HAPTIC_INFINITY where it sets no limit.
	uint32 GetPlatformMaxIterations() const;

	// The most iterations worth setting. Only one at an infinite duration, since its first iteration never ends.
	uint32 GetMaxIterations() const;

protected:
	SDL_HapticEffect Effect;

	virtual void CreateEffect();
	virtual void UpdateEffectData();

	// Seconds into the current iteration, past the start delay, for the FForceFeedbackEffectEvaluation functions. Negative while the effect isn't playing.
	float GetEvaluationTime() const;

private:
	// Clears playback state and sends the stopped event once, including natural completion.
	void FinishEffect();

	// The last frame number we were ticked.
	// We don't want to tick multiple times per frame 
	uint32 LastFrameNumber = INDEX_NONE;
	uint64 StartTime;
	float TimeAccumulator;

	bool EffectRunning;

	// Set by the owning component from the effect group entry's AutoStart. When false the component leaves this effect stopped when it starts, so it is only started by name or tag.
	bool ShouldStartWithComponent;

	friend class UJoystickForceFeedbackComponent;

#if !UE_BUILD_SHIPPING
	template <typename ListType, typename ValueType>
	static void SetDebugValue(ListType& List, const FName Name, const ValueType& Value);

	void UpdateDebugWriteRate(const float DeltaTime);

	static constexpr float DebugWriteRateWindow = 0.5f;

	FForceFeedbackEffectDebugInfo::FValueList DebugValues;
	FForceFeedbackEffectDebugInfo::FTextValueList DebugTextValues;
	uint32 DebugWriteCount;
	float DebugWriteTime;
	float DebugWritesPerSecond;
#endif
};
