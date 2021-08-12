#pragma once

#include "Components/ActorComponent.h"
#include "Effects/ForcedFeedbackEffectBase.h"

#include "ForcedFeedbackComponent.generated.h"

UCLASS(Blueprintable, ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class UForcedFeedbackComponent : public UActorComponent
{

	GENERATED_BODY()

public:
	
	void BeginPlay() override;
	void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	UFUNCTION(BlueprintNativeEvent)
		void OnInitialisedEffect();

	UFUNCTION(BlueprintNativeEvent)
		void OnStartedEffect();

	UFUNCTION(BlueprintNativeEvent)
		void OnStoppedEffect();

	UFUNCTION(BlueprintNativeEvent)
		void OnUpdatedEffect();

	UFUNCTION(BlueprintNativeEvent)
		void OnDestroyedEffect();

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
		TSubclassOf<class UForcedFeedbackEffectBase> EffectType;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly)
		UForcedFeedbackEffectBase* Effect;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool AutoInit = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool AutoStartOnInit = false;

};