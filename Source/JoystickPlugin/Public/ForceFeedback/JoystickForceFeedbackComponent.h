#pragma once

#include "Templates/SubclassOf.h"
#include "Components/ActorComponent.h"
#include "Effects/ForceFeedbackEffectBase.h"
#include "Data/ForceFeedbackComponentData.h"

#include "JoystickForceFeedbackComponent.generated.h"

UCLASS(Blueprintable, ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class JOYSTICKPLUGIN_API UJoystickForceFeedbackComponent : public UActorComponent
{

	GENERATED_BODY()

public:
	
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	UFUNCTION(BlueprintNativeEvent)
		void OnInitialisedEffect(UForceFeedbackEffectBase* effect);

	UFUNCTION(BlueprintNativeEvent)
		void OnStartedEffect(UForceFeedbackEffectBase* effect);

	UFUNCTION(BlueprintNativeEvent)
		void OnStoppedEffect(UForceFeedbackEffectBase* effect);

	UFUNCTION(BlueprintNativeEvent)
		void OnUpdatedEffect(UForceFeedbackEffectBase* effect);

	UFUNCTION(BlueprintNativeEvent)
		void OnDestroyedEffect(UForceFeedbackEffectBase* effect);

	UFUNCTION(BlueprintCallable)
		UForceFeedbackEffectBase* GetEffect();

	UFUNCTION(BlueprintCallable)
		void StartEffect();

	UFUNCTION(BlueprintCallable)
		void StopEffect();

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Force Feedback|Component")
		TSubclassOf<class UForceFeedbackEffectBase> EffectType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Force Feedback|Component")
		FForceFeedbackComponentData ComponentData;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Force Feedback|Component")
		UForceFeedbackEffectBase* Effect;

};