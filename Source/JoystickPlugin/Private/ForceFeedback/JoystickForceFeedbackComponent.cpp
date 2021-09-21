#include "ForceFeedback/JoystickForceFeedbackComponent.h"

void UJoystickForceFeedbackComponent::BeginPlay()
{
	Super::BeginPlay();

	if (EffectType == nullptr) 
	{
		return;
	}

	Effect = NewObject<UForceFeedbackEffectBase>(this, EffectType);
	if (Effect == nullptr || Effect->IsPendingKill())
	{
		return;
	}

	Effect->OnInitialisedEffectDelegate.AddDynamic(this, &UJoystickForceFeedbackComponent::OnInitialisedEffect);
	Effect->OnStartedEffectDelegate.AddDynamic(this, &UJoystickForceFeedbackComponent::OnStartedEffect);
	Effect->OnStoppedEffectDelegate.AddDynamic(this, &UJoystickForceFeedbackComponent::OnStoppedEffect);
	Effect->OnUpdatedEffectDelegate.AddDynamic(this, &UJoystickForceFeedbackComponent::OnUpdatedEffect);
	Effect->OnDestroyedEffectDelegate.AddDynamic(this, &UJoystickForceFeedbackComponent::OnDestroyedEffect);

	Effect->AutoStartOnInit = ComponentData.AutoStartOnInit;

	if (ComponentData.AutoInit) 
	{
		Effect->InitialiseEffect();
	}
}

void UJoystickForceFeedbackComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if (Effect == nullptr || Effect->IsPendingKill())
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

void UJoystickForceFeedbackComponent::OnInitialisedEffect_Implementation(UForceFeedbackEffectBase* effect)
{

}

void UJoystickForceFeedbackComponent::OnStartedEffect_Implementation(UForceFeedbackEffectBase* effect)
{

}

void UJoystickForceFeedbackComponent::OnStoppedEffect_Implementation(UForceFeedbackEffectBase* effect)
{

}

void UJoystickForceFeedbackComponent::OnUpdatedEffect_Implementation(UForceFeedbackEffectBase* effect)
{

}

void UJoystickForceFeedbackComponent::OnDestroyedEffect_Implementation(UForceFeedbackEffectBase* effect)
{

}

UForceFeedbackEffectBase* UJoystickForceFeedbackComponent::GetEffect()
{
	return Effect;
}

void UJoystickForceFeedbackComponent::StartEffect()
{
	if (Effect == nullptr)
	{
		return;
	}

	Effect->StartEffect();
}

void UJoystickForceFeedbackComponent::StopEffect()
{
	if (Effect == nullptr)
	{
		return;
	}

	Effect->StopEffect();
}
