// JoystickPlugin is licensed under the MIT License.
// Copyright Jayden Maalouf. All Rights Reserved.

#include "ForceFeedback/JoystickMultiForceFeedbackComponent.h"
#include "ForceFeedback/Data/ForceFeedbackComponentData.h"
#include "ForceFeedback/Effects/ForceFeedbackEffectBase.h"

UJoystickMultiForceFeedbackComponent::UJoystickMultiForceFeedbackComponent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
	  , InstanceId(0)
{
}

void UJoystickMultiForceFeedbackComponent::BeginPlay()
{
	Super::BeginPlay();

	if (EffectTypes.Num() == 0)
	{
		return;
	}

	for (auto& EffectType : EffectTypes)
	{
		UForceFeedbackEffectBase* Effect = NewObject<UForceFeedbackEffectBase>(this, EffectType.Key);
		if (!IsValid(Effect))
		{
			continue;
		}

		Effect->SetInstanceId(InstanceId);

		Effect->OnInitialisedEffectDelegate.AddDynamic(this, &UJoystickMultiForceFeedbackComponent::OnInitialisedEffect);
		Effect->OnStartedEffectDelegate.AddDynamic(this, &UJoystickMultiForceFeedbackComponent::OnStartedEffect);
		Effect->OnStoppedEffectDelegate.AddDynamic(this, &UJoystickMultiForceFeedbackComponent::OnStoppedEffect);
		Effect->OnUpdatedEffectDelegate.AddDynamic(this, &UJoystickMultiForceFeedbackComponent::OnUpdatedEffect);
		Effect->OnDestroyedEffectDelegate.AddDynamic(this, &UJoystickMultiForceFeedbackComponent::OnDestroyedEffect);

		Effect->AutoStartOnInitialisation = EffectType.Value.AutoStartOnInit;

		if (EffectType.Value.AutoInit)
		{
			Effect->InitialiseEffect();
		}

		Effects.Add(Effect);
	}
}

void UJoystickMultiForceFeedbackComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if (Effects.Num() == 0)
	{
		return;
	}

	for (int i = 0; i < Effects.Num(); i++)
	{
		UForceFeedbackEffectBase* Effect = Effects[i];
		if (!IsValid(Effect))
		{
			continue;
		}

		Effect->OnInitialisedEffectDelegate.RemoveDynamic(this, &UJoystickMultiForceFeedbackComponent::OnInitialisedEffect);
		Effect->OnStartedEffectDelegate.RemoveDynamic(this, &UJoystickMultiForceFeedbackComponent::OnStartedEffect);
		Effect->OnStoppedEffectDelegate.RemoveDynamic(this, &UJoystickMultiForceFeedbackComponent::OnStoppedEffect);
		Effect->OnUpdatedEffectDelegate.RemoveDynamic(this, &UJoystickMultiForceFeedbackComponent::OnUpdatedEffect);
		Effect->OnDestroyedEffectDelegate.RemoveDynamic(this, &UJoystickMultiForceFeedbackComponent::OnDestroyedEffect);
	}

	Effects.Empty();
}

void UJoystickMultiForceFeedbackComponent::OnInitialisedEffect_Implementation(const UForceFeedbackEffectBase* Effect)
{
}

void UJoystickMultiForceFeedbackComponent::OnStartedEffect_Implementation(const UForceFeedbackEffectBase* Effect)
{
}

void UJoystickMultiForceFeedbackComponent::OnStoppedEffect_Implementation(const UForceFeedbackEffectBase* Effect)
{
}

void UJoystickMultiForceFeedbackComponent::OnUpdatedEffect_Implementation(const UForceFeedbackEffectBase* Effect)
{
}

void UJoystickMultiForceFeedbackComponent::OnDestroyedEffect_Implementation(const UForceFeedbackEffectBase* Effect)
{
}

TArray<UForceFeedbackEffectBase*> UJoystickMultiForceFeedbackComponent::GetEffects()
{
	return Effects;
}

UForceFeedbackEffectBase* UJoystickMultiForceFeedbackComponent::GetEffectByType(const TSubclassOf<class UForceFeedbackEffectBase> EffectType)
{
	for (int i = 0; i < Effects.Num(); i++)
	{
		UForceFeedbackEffectBase* Effect = Effects[i];
		if (Effect->GetClass() == EffectType)
		{
			return Effect;
		}
	}

	return nullptr;
}

void UJoystickMultiForceFeedbackComponent::StartEffect(UForceFeedbackEffectBase* Effect)
{
	if (!IsValid(Effect))
	{
		return;
	}

	Effect->StartEffect();
}

void UJoystickMultiForceFeedbackComponent::StopEffect(UForceFeedbackEffectBase* Effect)
{
	if (!IsValid(Effect))
	{
		return;
	}

	Effect->StopEffect();
}
