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

	UFUNCTION(BlueprintNativeEvent, Category = "Force Feedback|Component|Events")
		void OnInitialisedEffect(UForceFeedbackEffectBase* Effect);

	UFUNCTION(BlueprintNativeEvent, Category = "Force Feedback|Component|Events")
		void OnStartedEffect(UForceFeedbackEffectBase* Effect);

	UFUNCTION(BlueprintNativeEvent, Category = "Force Feedback|Component|Events")
		void OnStoppedEffect(UForceFeedbackEffectBase* Effect);

	UFUNCTION(BlueprintNativeEvent, Category = "Force Feedback|Component|Events")
		void OnUpdatedEffect(UForceFeedbackEffectBase* Effect);

	UFUNCTION(BlueprintNativeEvent, Category = "Force Feedback|Component|Events")
		void OnDestroyedEffect(UForceFeedbackEffectBase* Effect);

	UFUNCTION(BlueprintCallable, Category = "Force Feedback|Component|Functions")
		UForceFeedbackEffectBase* GetEffect();

	UFUNCTION(BlueprintCallable, Category = "Force Feedback|Component|Functions")
		void StartEffect();

	UFUNCTION(BlueprintCallable, Category = "Force Feedback|Component|Functions")
		void StopEffect();

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Force Feedback|Component")
		TSubclassOf<class UForceFeedbackEffectBase> EffectType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Force Feedback|Component")
		FForceFeedbackComponentData ComponentData;

	UPROPERTY(VisibleAnywhere, Category = "Force Feedback|Component")
		UForceFeedbackEffectBase* ForcedFeedbackEffect;

};