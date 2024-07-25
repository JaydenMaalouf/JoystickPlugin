// JoystickPlugin is licensed under the MIT License.
// Copyright Jayden Maalouf. All Rights Reserved.

#include "ForceFeedback/JoystickForceFeedbackComponent.h"

#include "Engine/Engine.h"
#include "JoystickSubsystem.h"
#include "ForceFeedback/Effects/ForceFeedbackEffectBase.h"

UJoystickForceFeedbackComponent::UJoystickForceFeedbackComponent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
	  , InstanceId(0)
{
}

void UJoystickForceFeedbackComponent::BeginPlay()
{
	Super::BeginPlay();

	UJoystickSubsystem* JoystickSubsystem = GEngine->GetEngineSubsystem<UJoystickSubsystem>();
	if (!IsValid(JoystickSubsystem))
	{
		return;
	}

	if (JoystickSubsystem->IsReady())
	{
		CreateEffects();
	}
	else
	{
		JoystickSubsystem->JoystickSubsystemReady.AddDynamic(this, &UJoystickForceFeedbackComponent::OnSubsystemReady);
	}
}

void UJoystickForceFeedbackComponent::OnSubsystemReady()
{
	CreateEffects();

	UJoystickSubsystem* JoystickSubsystem = GEngine->GetEngineSubsystem<UJoystickSubsystem>();
	if (!IsValid(JoystickSubsystem))
	{
		return;
	}

	JoystickSubsystem->JoystickSubsystemReady.RemoveDynamic(this, &UJoystickForceFeedbackComponent::OnSubsystemReady);
}

void UJoystickForceFeedbackComponent::CreateEffects()
{
	if (!IsValid(EffectType))
	{
		return;
	}

	if (InstanceId != -1)
	{
		CreateInstanceEffect(InstanceId);
	}
	else
	{
		const UJoystickSubsystem* JoystickSubsystem = GEngine->GetEngineSubsystem<UJoystickSubsystem>();
		if (!IsValid(JoystickSubsystem))
		{
			return;
		}

		TArray<FJoystickInstanceId> InstanceIds;
		JoystickSubsystem->GetInstanceIds(InstanceIds);

		if (InstanceIds.Num() == 0)
		{
			return;
		}

		for (const FJoystickInstanceId& JoystickInstanceId : InstanceIds)
		{
			CreateInstanceEffect(JoystickInstanceId);
		}
	}
}

void UJoystickForceFeedbackComponent::CreateInstanceEffect(const FJoystickInstanceId& JoystickInstanceId)
{
	UForceFeedbackEffectBase* ForcedFeedbackEffect = NewObject<UForceFeedbackEffectBase>(this, EffectType);
	if (!IsValid(ForcedFeedbackEffect))
	{
		return;
	}

	ForcedFeedbackEffect->SetInstanceId(JoystickInstanceId);
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

	Effects.Add(ForcedFeedbackEffect);
}

void UJoystickForceFeedbackComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if (Effects.Num() == 0)
	{
		Super::EndPlay(EndPlayReason);
		return;
	}

	for (UForceFeedbackEffectBase* ForcedFeedbackEffect : Effects)
	{
		if (!IsValid(ForcedFeedbackEffect))
		{
			continue;
		}

		InternalDestroyEffect(ForcedFeedbackEffect, false);
	}
	Effects.Empty();

	Super::EndPlay(EndPlayReason);
}

void UJoystickForceFeedbackComponent::InternalDestroyEffect(UForceFeedbackEffectBase* ForcedFeedbackEffect, const bool RemoveEffect = false)
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

	if (RemoveEffect == true && Effects.Contains(ForcedFeedbackEffect))
	{
		Effects.Remove(ForcedFeedbackEffect);
	}

	ForcedFeedbackEffect->ConditionalBeginDestroy();
}

void UJoystickForceFeedbackComponent::DestroyEffect(UForceFeedbackEffectBase* ForcedFeedbackEffect)
{
	InternalDestroyEffect(ForcedFeedbackEffect, true);
}

void UJoystickForceFeedbackComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (Effects.Num() == 0)
	{
		return;
	}

	for (UForceFeedbackEffectBase* ForcedFeedbackEffect : Effects)
	{
		if (!IsValid(ForcedFeedbackEffect))
		{
			continue;
		}

		ForcedFeedbackEffect->Tick(DeltaTime);
	}
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

TArray<UForceFeedbackEffectBase*> UJoystickForceFeedbackComponent::GetEffects() const
{
	return Effects;
}

void UJoystickForceFeedbackComponent::StartEffect() const
{
	if (Effects.Num() == 0)
	{
		return;
	}

	for (UForceFeedbackEffectBase* ForcedFeedbackEffect : Effects)
	{
		if (!IsValid(ForcedFeedbackEffect))
		{
			continue;
		}

		ForcedFeedbackEffect->StartEffect();
	}
}

void UJoystickForceFeedbackComponent::StopEffect() const
{
	if (Effects.Num() == 0)
	{
		return;
	}

	for (UForceFeedbackEffectBase* ForcedFeedbackEffect : Effects)
	{
		if (!IsValid(ForcedFeedbackEffect))
		{
			continue;
		}

		ForcedFeedbackEffect->StopEffect();
	}
}
