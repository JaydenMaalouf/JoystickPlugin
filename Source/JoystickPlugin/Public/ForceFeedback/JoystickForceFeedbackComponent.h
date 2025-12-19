// JoystickPlugin is licensed under the MIT License.
// Copyright Jayden Maalouf. All Rights Reserved.

#pragma once

#include "Components/ActorComponent.h"
#include "Data/JoystickInstanceId.h"
#include "ForceFeedback/Data/Configuration/ForceFeedbackComponentConfiguration.h"

#include "JoystickForceFeedbackComponent.generated.h"

class UForceFeedbackEffectBase;

UCLASS(Blueprintable, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class JOYSTICKPLUGIN_API UJoystickForceFeedbackComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UJoystickForceFeedbackComponent(const FObjectInitializer& ObjectInitializer);

	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

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

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Joystick|Force Feedback Component")
	FJoystickInstanceId InstanceId;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Joystick|Force Feedback Component")
	TSubclassOf<UForceFeedbackEffectBase> EffectType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Joystick|Force Feedback Component")
	FForceFeedbackComponentConfiguration Configuration;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Joystick|Force Feedback Component")
	TArray<UForceFeedbackEffectBase*> Effects;

private:
	UFUNCTION()
	void OnSubsystemReady();
	UFUNCTION()
	void JoystickPluggedIn(const FJoystickInstanceId& JoystickInstanceId);
	UFUNCTION()
	void JoystickUnplugged(const FJoystickInstanceId& JoystickInstanceId);

	void CreateEffects();
	void CreateInstanceEffect(const FJoystickInstanceId& JoystickInstanceId);
	void DestroyEffect(UForceFeedbackEffectBase* ForcedFeedbackEffect);
	void DestroyInstanceEffects(const FJoystickInstanceId& JoystickInstanceId);

	void ActionOnAllEffects(const TFunctionRef<void(UForceFeedbackEffectBase* Effect)>& CustomInitializer);
	void ActionOnJoystickEffects(const FJoystickInstanceId& JoystickInstanceId, const TFunctionRef<void(UForceFeedbackEffectBase* Effect)>& CustomInitializer);
};
