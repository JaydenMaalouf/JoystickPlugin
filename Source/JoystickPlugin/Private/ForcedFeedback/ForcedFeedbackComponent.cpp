#include "ForcedFeedback/ForcedFeedbackComponent.h"

void UForcedFeedbackComponent::BeginPlay()
{
	Super::BeginPlay();

	if (EffectType == nullptr) 
	{
		return;
	}

	Effect = NewObject<UForcedFeedbackEffectBase>(this, EffectType);
	if (Effect == nullptr) 
	{
		return;
	}

	Effect->OnInitialisedEffectDelegate.AddDynamic(this, &UForcedFeedbackComponent::OnInitialisedEffect);
	Effect->OnStartedEffectDelegate.AddDynamic(this, &UForcedFeedbackComponent::OnStartedEffect);
	Effect->OnStoppedEffectDelegate.AddDynamic(this, &UForcedFeedbackComponent::OnStoppedEffect);
	Effect->OnUpdatedEffectDelegate.AddDynamic(this, &UForcedFeedbackComponent::OnUpdatedEffect);
	Effect->OnDestroyedEffectDelegate.AddDynamic(this, &UForcedFeedbackComponent::OnDestroyedEffect);

	Effect->AutoStartOnInit = AutoStartOnInit;

	if (AutoInit) 
	{
		Effect->InitialiseEffect();
	}
}

void UForcedFeedbackComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if (Effect == nullptr)
	{
		return;
	}

	Effect->OnInitialisedEffectDelegate.RemoveDynamic(this, &UForcedFeedbackComponent::OnInitialisedEffect);
	Effect->OnStartedEffectDelegate.RemoveDynamic(this, &UForcedFeedbackComponent::OnStartedEffect);
	Effect->OnStoppedEffectDelegate.RemoveDynamic(this, &UForcedFeedbackComponent::OnStoppedEffect);
	Effect->OnUpdatedEffectDelegate.RemoveDynamic(this, &UForcedFeedbackComponent::OnUpdatedEffect);
	Effect->OnDestroyedEffectDelegate.RemoveDynamic(this, &UForcedFeedbackComponent::OnDestroyedEffect);

	Effect = nullptr;
}

void UForcedFeedbackComponent::OnInitialisedEffect_Implementation()
{

}

void UForcedFeedbackComponent::OnStartedEffect_Implementation()
{

}

void UForcedFeedbackComponent::OnStoppedEffect_Implementation()
{

}

void UForcedFeedbackComponent::OnUpdatedEffect_Implementation()
{

}

void UForcedFeedbackComponent::OnDestroyedEffect_Implementation()
{

}
