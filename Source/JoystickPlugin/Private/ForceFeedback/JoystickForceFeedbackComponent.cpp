#include "ForceFeedback/JoystickForceFeedbackComponent.h"

void UJoystickForceFeedbackComponent::BeginPlay()
{
	Super::BeginPlay();

	if (EffectType == nullptr) 
	{
		return;
	}

	Effect = NewObject<UForceFeedbackEffectBase>(this, EffectType);
	if (Effect == nullptr) 
	{
		return;
	}

	Effect->OnInitialisedEffectDelegate.AddDynamic(this, &UJoystickForceFeedbackComponent::OnInitialisedEffect);
	Effect->OnStartedEffectDelegate.AddDynamic(this, &UJoystickForceFeedbackComponent::OnStartedEffect);
	Effect->OnStoppedEffectDelegate.AddDynamic(this, &UJoystickForceFeedbackComponent::OnStoppedEffect);
	Effect->OnUpdatedEffectDelegate.AddDynamic(this, &UJoystickForceFeedbackComponent::OnUpdatedEffect);
	Effect->OnDestroyedEffectDelegate.AddDynamic(this, &UJoystickForceFeedbackComponent::OnDestroyedEffect);

	Effect->AutoStartOnInit = AutoStartOnInit;

	if (AutoInit) 
	{
		Effect->InitialiseEffect();
	}
}

void UJoystickForceFeedbackComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if (Effect == nullptr)
	{
		return;
	}

	Effect->OnInitialisedEffectDelegate.RemoveDynamic(this, &UJoystickForceFeedbackComponent::OnInitialisedEffect);
	Effect->OnStartedEffectDelegate.RemoveDynamic(this, &UJoystickForceFeedbackComponent::OnStartedEffect);
	Effect->OnStoppedEffectDelegate.RemoveDynamic(this, &UJoystickForceFeedbackComponent::OnStoppedEffect);
	Effect->OnUpdatedEffectDelegate.RemoveDynamic(this, &UJoystickForceFeedbackComponent::OnUpdatedEffect);
	Effect->OnDestroyedEffectDelegate.RemoveDynamic(this, &UJoystickForceFeedbackComponent::OnDestroyedEffect);

	Effect = nullptr;
}

void UJoystickForceFeedbackComponent::OnInitialisedEffect_Implementation()
{

}

void UJoystickForceFeedbackComponent::OnStartedEffect_Implementation()
{

}

void UJoystickForceFeedbackComponent::OnStoppedEffect_Implementation()
{

}

void UJoystickForceFeedbackComponent::OnUpdatedEffect_Implementation()
{

}

void UJoystickForceFeedbackComponent::OnDestroyedEffect_Implementation()
{

}
