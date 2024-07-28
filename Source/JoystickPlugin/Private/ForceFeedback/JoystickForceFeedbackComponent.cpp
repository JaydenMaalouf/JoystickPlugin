// JoystickPlugin is licensed under the MIT License.
// Copyright Jayden Maalouf. All Rights Reserved.

#include "ForceFeedback/JoystickForceFeedbackComponent.h"

#include "Engine/Engine.h"
#include "JoystickSubsystem.h"
#include "ForceFeedback/Effects/ForceFeedbackEffectBase.h"

UJoystickForceFeedbackComponent::UJoystickForceFeedbackComponent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
	  , InstanceId(-1)
{
	bAutoActivate = true;
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.bStartWithTickEnabled = true;
}

void UJoystickForceFeedbackComponent::BeginPlay()
{
	Super::BeginPlay();

	if (!IsValid(GEngine))
	{
		return;
	}

	UJoystickSubsystem* JoystickSubsystem = GEngine->GetEngineSubsystem<UJoystickSubsystem>();
	if (!IsValid(JoystickSubsystem))
	{
		return;
	}

	JoystickSubsystem->JoystickPluggedInDelegate.AddDynamic(this, &UJoystickForceFeedbackComponent::JoystickPluggedIn);
	JoystickSubsystem->JoystickUnpluggedDelegate.AddDynamic(this, &UJoystickForceFeedbackComponent::JoystickUnplugged);

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

	if (!IsValid(GEngine))
	{
		return;
	}

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
		if (!IsValid(GEngine))
		{
			return;
		}

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
	if (InstanceId != -1 && InstanceId != JoystickInstanceId)
	{
		return;
	}
	
	UForceFeedbackEffectBase* ForcedFeedbackEffect = NewObject<UForceFeedbackEffectBase>(this, EffectType);
	if (!IsValid(ForcedFeedbackEffect))
	{
		return;
	}

	if (Configuration.OverrideEffectTick)
	{
		//Disable Effect Tick as this will be driven by the component instead
		ForcedFeedbackEffect->SetTickable(false);
	}

	ForcedFeedbackEffect->SetInstanceId(JoystickInstanceId);
	ForcedFeedbackEffect->OnInitialisedEffectDelegate.AddDynamic(this, &UJoystickForceFeedbackComponent::OnInitialisedEffect);
	ForcedFeedbackEffect->OnStartedEffectDelegate.AddDynamic(this, &UJoystickForceFeedbackComponent::OnStartedEffect);
	ForcedFeedbackEffect->OnStoppedEffectDelegate.AddDynamic(this, &UJoystickForceFeedbackComponent::OnStoppedEffect);
	ForcedFeedbackEffect->OnUpdatedEffectDelegate.AddDynamic(this, &UJoystickForceFeedbackComponent::OnUpdatedEffect);
	ForcedFeedbackEffect->OnDestroyedEffectDelegate.AddDynamic(this, &UJoystickForceFeedbackComponent::OnDestroyedEffect);

	ForcedFeedbackEffect->Configuration.Update(Configuration);
	if (ForcedFeedbackEffect->Configuration.AutoInitialise)
	{
		ForcedFeedbackEffect->InitialiseEffect();
	}

	Effects.Add(ForcedFeedbackEffect);
}

void UJoystickForceFeedbackComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	ActionOnAllEffects([&](UForceFeedbackEffectBase* Effect)
	{
		DestroyEffect(Effect);
	});

	Effects.Empty();

	Super::EndPlay(EndPlayReason);
}

void UJoystickForceFeedbackComponent::DestroyEffect(UForceFeedbackEffectBase* ForcedFeedbackEffect)
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

	ForcedFeedbackEffect->ConditionalBeginDestroy();
}

void UJoystickForceFeedbackComponent::DestroyInstanceEffects(const FJoystickInstanceId& JoystickInstanceId)
{
	ActionOnJoystickEffects(InstanceId, [&](UForceFeedbackEffectBase* Effect)
	{
		DestroyEffect(Effect);
	});

	Effects.RemoveAll([&](const UForceFeedbackEffectBase* Effect)
	{
		return Effect->GetInstanceId() == JoystickInstanceId;
	});
}

void UJoystickForceFeedbackComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (!Configuration.OverrideEffectTick)
	{
		return;
	}

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

		if (ForcedFeedbackEffect->Tickable == false)
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

void UJoystickForceFeedbackComponent::StartEffect()
{
	ActionOnAllEffects([](UForceFeedbackEffectBase* Effect)
	{
		Effect->StartEffect();
	});
}

void UJoystickForceFeedbackComponent::StopEffect()
{
	ActionOnAllEffects([](UForceFeedbackEffectBase* Effect)
	{
		Effect->StopEffect();
	});
}

void UJoystickForceFeedbackComponent::JoystickPluggedIn(const FJoystickInstanceId& JoystickInstanceId)
{
	CreateInstanceEffect(JoystickInstanceId);
}

void UJoystickForceFeedbackComponent::JoystickUnplugged(const FJoystickInstanceId& JoystickInstanceId)
{
	DestroyInstanceEffects(JoystickInstanceId);
}

void UJoystickForceFeedbackComponent::ActionOnAllEffects(const TFunctionRef<void(UForceFeedbackEffectBase* Effect)>& CustomInitializer)
{
	if (Effects.Num() == 0)
	{
		return;
	}

	for (UForceFeedbackEffectBase* Effect : Effects)
	{
		if (!IsValid(Effect))
		{
			continue;
		}

		CustomInitializer(Effect);
	}
}

void UJoystickForceFeedbackComponent::ActionOnJoystickEffects(const FJoystickInstanceId& JoystickInstanceId, const TFunctionRef<void(UForceFeedbackEffectBase* Effect)>& CustomInitializer)
{
	if (Effects.Num() == 0)
	{
		return;
	}
	
	for (UForceFeedbackEffectBase* Effect : Effects)
	{
		if (!IsValid(Effect))
		{
			continue;
		}

		if (Effect->GetInstanceId() != JoystickInstanceId)
		{
			continue;
		}

		CustomInitializer(Effect);
	}
}
