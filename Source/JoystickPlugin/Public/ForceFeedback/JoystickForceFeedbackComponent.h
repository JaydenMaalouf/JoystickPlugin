#pragma once

#include "Components/ActorComponent.h"
#include "Effects/ForceFeedbackEffectBase.h"

#include "Templates/SubclassOf.h"

#include "JoystickForceFeedbackComponent.generated.h"

UCLASS(Blueprintable, ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class UJoystickForceFeedbackComponent : public UActorComponent
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
		TSubclassOf<class UForceFeedbackEffectBase> EffectType;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly)
		UForceFeedbackEffectBase* Effect;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool AutoInit = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool AutoStartOnInit = false;

};