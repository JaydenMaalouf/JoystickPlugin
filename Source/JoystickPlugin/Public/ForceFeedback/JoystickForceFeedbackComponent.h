#pragma once

#include "Components/ActorComponent.h"
#include "Effects/ForceFeedbackEffectBase.h"

#include "Templates/SubclassOf.h"

#include "JoystickForceFeedbackComponent.generated.h"

UCLASS(Blueprintable, ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class JOYSTICKPLUGIN_API UJoystickForceFeedbackComponent : public UActorComponent
{

	GENERATED_BODY()

public:
	
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	UFUNCTION(BlueprintImplementableEvent)
		void OnInitialisedEffect();

	UFUNCTION(BlueprintImplementableEvent)
		void OnStartedEffect();

	UFUNCTION(BlueprintImplementableEvent)
		void OnStoppedEffect();

	UFUNCTION(BlueprintImplementableEvent)
		void OnUpdatedEffect();

	UFUNCTION(BlueprintImplementableEvent)
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