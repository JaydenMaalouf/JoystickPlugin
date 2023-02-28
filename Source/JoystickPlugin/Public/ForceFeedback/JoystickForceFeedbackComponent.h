// JoystickPlugin is licensed under the MIT License.
// Copyright Jayden Maalouf. All Rights Reserved.

#pragma once

#include "Components/ActorComponent.h"
#include "Data/ForceFeedbackComponentData.h"
#include "Data/JoystickInstanceId.h"

#include "JoystickForceFeedbackComponent.generated.h"

class UForceFeedbackEffectBase;

UCLASS(Blueprintable, ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class JOYSTICKPLUGIN_API UJoystickForceFeedbackComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UJoystickForceFeedbackComponent(const FObjectInitializer& ObjectInitializer);

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
	UForceFeedbackEffectBase* GetEffect() const;

	UFUNCTION(BlueprintCallable, Category = "Force Feedback|Component|Functions")
	void StartEffect() const;

	UFUNCTION(BlueprintCallable, Category = "Force Feedback|Component|Functions")
	void StopEffect() const;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Force Feedback|Component")
	FJoystickInstanceId InstanceId;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Force Feedback|Component")
	TSubclassOf<UForceFeedbackEffectBase> EffectType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Force Feedback|Component")
	FForceFeedbackComponentData ComponentData;

	UPROPERTY(VisibleAnywhere, Category = "Force Feedback|Component")
	UForceFeedbackEffectBase* ForcedFeedbackEffect;
};
