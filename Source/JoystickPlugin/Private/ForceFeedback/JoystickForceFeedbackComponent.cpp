// JoystickPlugin is licensed under the MIT License.
// Copyright Jayden Maalouf. All Rights Reserved.

#include "ForceFeedback/JoystickForceFeedbackComponent.h"
#include "ForceFeedback/Effects/ForceFeedbackEffectBase.h"

UJoystickForceFeedbackComponent::UJoystickForceFeedbackComponent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
	  , InstanceId(0)
{
}

void UJoystickForceFeedbackComponent::BeginPlay()
{
	Super::BeginPlay();

	if (!IsValid(EffectType))
	{
		return;
	}

	ForcedFeedbackEffect = NewObject<UForceFeedbackEffectBase>(this, EffectType);
	if (!IsValid(ForcedFeedbackEffect))
	{
		return;
	}

	ForcedFeedbackEffect->SetInstanceId(InstanceId);

	ForcedFeedbackEffect->OnInitialisedEffectDelegate.AddDynamic(this, &UJoystickForceFeedbackComponent::OnInitialisedEffect);
	ForcedFeedbackEffect->OnStartedEffectDelegate.AddDynamic(this, &UJoystickForceFeedbackComponent::OnStartedEffect);
	ForcedFeedbackEffect->OnStoppedEffectDelegate.AddDynamic(this, &UJoystickForceFeedbackComponent::OnStoppedEffect);
	ForcedFeedbackEffect->OnUpdatedEffectDelegate.AddDynamic(this, &UJoystickForceFeedbackComponent::OnUpdatedEffect);
	ForcedFeedbackEffect->OnDestroyedEffectDelegate.AddDynamic(this, &UJoystickForceFeedbackComponent::OnDestroyedEffect);

	ForcedFeedbackEffect->AutoStartOnInitialisation = ComponentData.AutoStartOnInit;

	if (ComponentData.AutoInit)
	{
		ForcedFeedbackEffect->InitialiseEffect();
	}
}

void UJoystickForceFeedbackComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if (!IsValid(ForcedFeedbackEffect))
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

void UJoystickForceFeedbackComponent::OnInitialisedEffect_Implementation(const UForceFeedbackEffectBase* Effect)
{
}

void UJoystickForceFeedbackComponent::OnStartedEffect_Implementation(const UForceFeedbackEffectBase* Effect)
{
}

void UJoystickForceFeedbackComponent::OnStoppedEffect_Implementation(const UForceFeedbackEffectBase* Effect)
{
}

void UJoystickForceFeedbackComponent::OnUpdatedEffect_Implementation(const UForceFeedbackEffectBase* Effect)
{
}

void UJoystickForceFeedbackComponent::OnDestroyedEffect_Implementation(const UForceFeedbackEffectBase* Effect)
{
}

UForceFeedbackEffectBase* UJoystickForceFeedbackComponent::GetEffect() const
{
	return ForcedFeedbackEffect;
}

void UJoystickForceFeedbackComponent::StartEffect() const
{
	if (!IsValid(ForcedFeedbackEffect))
	{
		return;
	}

	ForcedFeedbackEffect->StartEffect();
}

void UJoystickForceFeedbackComponent::StopEffect() const
{
	if (!IsValid(ForcedFeedbackEffect))
	{
		return;
	}

	ForcedFeedbackEffect->StopEffect();
}
