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
		TArray<UForceFeedbackEffectBase*> GetEffects();

	UFUNCTION(BlueprintCallable)
		UForceFeedbackEffectBase* GetEffectByType(TSubclassOf<class UForceFeedbackEffectBase> effectType);

	UFUNCTION(BlueprintCallable)
		void StartEffect(UForceFeedbackEffectBase* effect);

	UFUNCTION(BlueprintCallable)
		void StopEffect(UForceFeedbackEffectBase* effect);

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
		TMap<TSubclassOf<class UForceFeedbackEffectBase>, FForceFeedbackComponentData> EffectTypes;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
		TArray<UForceFeedbackEffectBase*> Effects;

};