#pragma once

#include "Templates/SubclassOf.h"
#include "Components/ActorComponent.h"
#include "Effects/ForceFeedbackEffectBase.h"
#include "Data/ForceFeedbackComponentData.h"

#include "JoystickMultiForceFeedbackComponent.generated.h"

UCLASS(Blueprintable, ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class JOYSTICKPLUGIN_API UJoystickMultiForceFeedbackComponent : public UActorComponent
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
		TArray<UForceFeedbackEffectBase*> GetEffects();

	UFUNCTION(BlueprintCallable, Category = "Force Feedback|Component|Functions")
		UForceFeedbackEffectBase* GetEffectByType(TSubclassOf<class UForceFeedbackEffectBase> EffectType);

	UFUNCTION(BlueprintCallable, Category = "Force Feedback|Component|Functions")
		void StartEffect(UForceFeedbackEffectBase* Effect);

	UFUNCTION(BlueprintCallable, Category = "Force Feedback|Component|Functions")
		void StopEffect(UForceFeedbackEffectBase* Effect);

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Force Feedback|Component")
		TMap<TSubclassOf<class UForceFeedbackEffectBase>, FForceFeedbackComponentData> EffectTypes;

	UPROPERTY(VisibleAnywhere, Category = "Force Feedback|Component")
		TArray<UForceFeedbackEffectBase*> Effects;

};