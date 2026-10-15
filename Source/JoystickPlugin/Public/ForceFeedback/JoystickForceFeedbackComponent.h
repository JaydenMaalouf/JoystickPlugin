// JoystickPlugin is licensed under the MIT License.
// Copyright Jayden Maalouf 2026. All Rights Reserved.

#pragma once

#include "Components/ActorComponent.h"
#include "GameplayTagContainer.h"
#include "Data/JoystickInstanceId.h"
#include "ForceFeedback/Data/Configuration/ForceFeedbackComponentConfiguration.h"
#include "Runtime/Launch/Resources/Version.h"
#include "PhysicsEngine/BodyInstance.h"
#include "Physics/Experimental/PhysScene_Chaos.h"
#include "Chaos/PhysicsObjectInternalInterface.h"

#include "JoystickForceFeedbackComponent.generated.h"

class UForceFeedbackEffectBase;
class UForceFeedbackEffectGroup;
class FJoystickForceFeedbackSubstepCallback;
struct FForceFeedbackEffectDebugInfo;
struct FForceFeedbackEffectGroupEntry;
struct FJoystickHapticInformation;

UCLASS(Blueprintable, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class JOYSTICKPLUGIN_API UJoystickForceFeedbackComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UJoystickForceFeedbackComponent(const FObjectInitializer& ObjectInitializer);

	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	virtual void SetComponentTickEnabled(bool bEnabled) override;

#if ENGINE_MAJOR_VERSION == 5 && ENGINE_MINOR_VERSION >= 3
	virtual void AsyncPhysicsTickComponent(float DeltaTime, float SimTime) override;
#endif

	UFUNCTION(BlueprintCallable, Category="Joystick|Force Feedback Component")
	void SetTickable(const bool bTickable);

	UFUNCTION(BlueprintNativeEvent, Category="Joystick|Force Feedback Component|Events")
	void OnInitialisedEffect(const UForceFeedbackEffectBase* Effect);

	UFUNCTION(BlueprintNativeEvent, Category="Joystick|Force Feedback Component|Events")
	void OnStartedEffect(const UForceFeedbackEffectBase* Effect);

	UFUNCTION(BlueprintNativeEvent, Category="Joystick|Force Feedback Component|Events")
	void OnStoppedEffect(const UForceFeedbackEffectBase* Effect);

	UFUNCTION(BlueprintNativeEvent, Category="Joystick|Force Feedback Component|Events")
	void OnUpdatedEffect(const UForceFeedbackEffectBase* Effect);

	UFUNCTION(BlueprintNativeEvent, Category="Joystick|Force Feedback Component|Events")
	void OnDestroyedEffect(const UForceFeedbackEffectBase* Effect);

	UFUNCTION(BlueprintCallable, Category="Joystick|Force Feedback Component")
	TArray<UForceFeedbackEffectBase*> GetEffects() const;

	UFUNCTION(BlueprintCallable, Category="Joystick|Force Feedback Component")
	void StartEffect();

	UFUNCTION(BlueprintCallable, Category="Joystick|Force Feedback Component")
	void StopEffect();

	UFUNCTION(BlueprintCallable, Category="Joystick|Force Feedback Component",
		meta=(ToolTip="Swaps the effect groups. If the component has begun play, the existing effects are destroyed and the new set is created, and started if the component is running."))
	void SetEffectGroups(const TArray<UForceFeedbackEffectGroup*>& NewEffectGroups);

	UFUNCTION(BlueprintCallable, Category="Joystick|Force Feedback Component", meta=(ToolTip="Returns the effects spawned from the group entry with this name, one per device."))
	TArray<UForceFeedbackEffectBase*> GetEffectsByName(const FName EffectName) const;

	UFUNCTION(BlueprintCallable, Category="Joystick|Force Feedback Component")
	void StartEffectByName(const FName EffectName);

	UFUNCTION(BlueprintCallable, Category="Joystick|Force Feedback Component")
	void StopEffectByName(const FName EffectName);

	UFUNCTION(BlueprintCallable, Category="Joystick|Force Feedback Component",
		meta=(ToolTip="Returns the effects whose group entry tag or group tag is this tag, or a child of it, across every group and device. Passing a group's tag returns every effect in that group."))
	TArray<UForceFeedbackEffectBase*> GetEffectsByTag(const FGameplayTag EffectTag) const;

	UFUNCTION(BlueprintCallable, Category="Joystick|Force Feedback Component")
	void StartEffectByTag(const FGameplayTag EffectTag);

	UFUNCTION(BlueprintCallable, Category="Joystick|Force Feedback Component")
	void StopEffectByTag(const FGameplayTag EffectTag);

	// Describes every initialised effect under the effects lock, so it is safe while the effects tick on the physics thread.
	void DescribeEffects(TArray<FForceFeedbackEffectDebugInfo>& OutInfos) const;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Force Feedback", meta=(ToolTip="InstanceId of the Joystick to apply the effect to. Set to -1 to apply to all devices that support this effect type"))
	FJoystickInstanceId InstanceId;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Force Feedback",
		meta=(ToolTip="Single effect type to spawn with its class defaults. Spawned in addition to the EffectGroups, ahead of the groups' entries. For anything more than one effect, use an EffectGroup instead."))
	TSubclassOf<UForceFeedbackEffectBase> EffectType;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Force Feedback",
		meta=(ToolTip="Groups of effects to spawn, such as one for always active effects and one for effects triggered by tag. The groups share each device's effect slots. Use SetEffectGroups to swap these at runtime."))
	TArray<UForceFeedbackEffectGroup*> EffectGroups;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Force Feedback")
	FForceFeedbackComponentConfiguration Configuration;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Data")
	TArray<UForceFeedbackEffectBase*> Effects;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Data")
	bool Running;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Data")
	bool Tickable;

private:
	friend class FJoystickForceFeedbackSubstepCallback;

	UFUNCTION()
	void OnSubsystemReady();
	UFUNCTION()
	void JoystickPluggedIn(const FJoystickInstanceId& JoystickInstanceId);
	UFUNCTION()
	void JoystickUnplugged(const FJoystickInstanceId& JoystickInstanceId);

	void TickEffects(float DeltaTime);
	void RegisterPhysicsSubstepCallback();
	void UnregisterPhysicsSubstepCallback();

	void CreateEffects();
	void CreateInstanceEffect(const FJoystickInstanceId& JoystickInstanceId);
	bool CreateInstanceEffect(const FJoystickInstanceId& JoystickInstanceId, const FJoystickHapticInformation& HapticInformation, const TSubclassOf<UForceFeedbackEffectBase> EffectClass, const FForceFeedbackEffectGroupEntry& Entry,
	                          const UForceFeedbackEffectGroup* Group);
	int GetAvailableEffectSlots(const FJoystickInstanceId& JoystickInstanceId) const;
	// Returns the active entries of every group paired with their group, highest priority first.
	void GetActiveGroupEntries(TArray<TPair<FForceFeedbackEffectGroupEntry, const UForceFeedbackEffectGroup*>>& OutEntries) const;
	void DestroyEffect(UForceFeedbackEffectBase* ForcedFeedbackEffect);
	void DestroyInstanceEffects(const FJoystickInstanceId& JoystickInstanceId);
	void DestroyAllEffects();

	void StartAutoStartEffects();

	void ActionOnAllEffects(const TFunctionRef<void(UForceFeedbackEffectBase* Effect)>& CustomInitializer);
	void ActionOnJoystickEffects(const FJoystickInstanceId& JoystickInstanceId, const TFunctionRef<void(UForceFeedbackEffectBase* Effect)>& CustomInitializer);

	Chaos::FPhysicsSolverBase* RegisteredSolver;
	FJoystickForceFeedbackSubstepCallback* SubstepCallback;
	mutable FCriticalSection EffectsCriticalSection;
};
