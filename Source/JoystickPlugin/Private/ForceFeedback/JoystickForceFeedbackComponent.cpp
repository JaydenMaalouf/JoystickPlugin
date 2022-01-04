#include "ForceFeedback/JoystickForceFeedbackComponent.h"

void UJoystickForceFeedbackComponent::BeginPlay()
{
	Super::BeginPlay();

	if (EffectType == nullptr) 
	{
		return;
	}

	ForcedFeedbackEffect = NewObject<UForceFeedbackEffectBase>(this, EffectType);
	if (ForcedFeedbackEffect == nullptr || ForcedFeedbackEffect->IsPendingKill())
	{
		return;
	}

	ForcedFeedbackEffect->OnInitialisedEffectDelegate.AddDynamic(this, &UJoystickForceFeedbackComponent::OnInitialisedEffect);
	ForcedFeedbackEffect->OnStartedEffectDelegate.AddDynamic(this, &UJoystickForceFeedbackComponent::OnStartedEffect);
	ForcedFeedbackEffect->OnStoppedEffectDelegate.AddDynamic(this, &UJoystickForceFeedbackComponent::OnStoppedEffect);
	ForcedFeedbackEffect->OnUpdatedEffectDelegate.AddDynamic(this, &UJoystickForceFeedbackComponent::OnUpdatedEffect);
	ForcedFeedbackEffect->OnDestroyedEffectDelegate.AddDynamic(this, &UJoystickForceFeedbackComponent::OnDestroyedEffect);

	ForcedFeedbackEffect->AutoStartOnInit = ComponentData.AutoStartOnInit;

	if (ComponentData.AutoInit) 
	{
		ForcedFeedbackEffect->InitialiseEffect();
	}
}

void UJoystickForceFeedbackComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if (ForcedFeedbackEffect == nullptr || ForcedFeedbackEffect->IsPendingKill())
	{
		return;
	}

	ForcedFeedbackEffect->OnInitialisedEffectDelegate.RemoveDynamic(this, &UJoystickForceFeedbackComponent::OnInitialisedEffect);
	ForcedFeedbackEffect->OnStartedEffectDelegate.RemoveDynamic(this, &UJoystickForceFeedbackComponent::OnStartedEffect);
	ForcedFeedbackEffect->OnStoppedEffectDelegate.RemoveDynamic(this, &UJoystickForceFeedbackComponent::OnStoppedEffect);
	ForcedFeedbackEffect->OnUpdatedEffectDelegate.RemoveDynamic(this, &UJoystickForceFeedbackComponent::OnUpdatedEffect);
	ForcedFeedbackEffect->OnDestroyedEffectDelegate.RemoveDynamic(this, &UJoystickForceFeedbackComponent::OnDestroyedEffect);

	ForcedFeedbackEffect = nullptr;
}

void UJoystickForceFeedbackComponent::OnInitialisedEffect_Implementation(UForceFeedbackEffectBase* Effect)
{

}

void UJoystickForceFeedbackComponent::OnStartedEffect_Implementation(UForceFeedbackEffectBase* Effect)
{

}

void UJoystickForceFeedbackComponent::OnStoppedEffect_Implementation(UForceFeedbackEffectBase* Effect)
{

}

void UJoystickForceFeedbackComponent::OnUpdatedEffect_Implementation(UForceFeedbackEffectBase* Effect)
{

}

void UJoystickForceFeedbackComponent::OnDestroyedEffect_Implementation(UForceFeedbackEffectBase* Effect)
{

}

UForceFeedbackEffectBase* UJoystickForceFeedbackComponent::GetEffect()
{
	return ForcedFeedbackEffect;
}

void UJoystickForceFeedbackComponent::StartEffect()
{
	if (ForcedFeedbackEffect == nullptr)
	{
		return;
	}

	ForcedFeedbackEffect->StartEffect();
}

void UJoystickForceFeedbackComponent::StopEffect()
{
	if (ForcedFeedbackEffect == nullptr)
	{
		return;
	}

	ForcedFeedbackEffect->StopEffect();
}
