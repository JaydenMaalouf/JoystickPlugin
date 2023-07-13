// JoystickPlugin is licensed under the MIT License.
// Copyright Jayden Maalouf. All Rights Reserved.

#pragma once

#include "Components/ActorComponent.h"
#include "Data/ForceFeedbackComponentData.h"
#include "Data/JoystickInstanceId.h"

#include "JoystickMultiForceFeedbackComponent.generated.h"

class UForceFeedbackEffectBase;

UCLASS(Blueprintable, ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class JOYSTICKPLUGIN_API UJoystickMultiForceFeedbackComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UJoystickMultiForceFeedbackComponent(const FObjectInitializer& ObjectInitializer);

	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	UFUNCTION(BlueprintNativeEvent, Category = "Force Feedback|Component|Events")
	void OnInitialisedEffect(const UForceFeedbackEffectBase* Effect);

	UFUNCTION(BlueprintNativeEvent, Category = "Force Feedback|Component|Events")
	void OnStartedEffect(const UForceFeedbackEffectBase* Effect);

	UFUNCTION(BlueprintNativeEvent, Category = "Force Feedback|Component|Events")
	void OnStoppedEffect(const UForceFeedbackEffectBase* Effect);

	UFUNCTION(BlueprintNativeEvent, Category = "Force Feedback|Component|Events")
	void OnUpdatedEffect(const UForceFeedbackEffectBase* Effect);

	UFUNCTION(BlueprintNativeEvent, Category = "Force Feedback|Component|Events")
	void OnDestroyedEffect(const UForceFeedbackEffectBase* Effect);

	UFUNCTION(BlueprintCallable, Category = "Force Feedback|Component|Functions")
	TArray<UForceFeedbackEffectBase*> GetEffects();

	UFUNCTION(BlueprintCallable, Category = "Force Feedback|Component|Functions")
	UForceFeedbackEffectBase* GetEffectByType(TSubclassOf<UForceFeedbackEffectBase> EffectType);

	UFUNCTION(BlueprintCallable, Category = "Force Feedback|Component|Functions")
	void StartEffect(UForceFeedbackEffectBase* Effect);

	UFUNCTION(BlueprintCallable, Category = "Force Feedback|Component|Functions")
	void StopEffect(UForceFeedbackEffectBase* Effect);

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Force Feedback|Component")
	FJoystickInstanceId InstanceId;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Force Feedback|Component")
	TMap<TSubclassOf<UForceFeedbackEffectBase>, FForceFeedbackComponentData> EffectTypes;

	UPROPERTY(VisibleAnywhere, Category = "Force Feedback|Component")
	TArray<UForceFeedbackEffectBase*> Effects;
};
