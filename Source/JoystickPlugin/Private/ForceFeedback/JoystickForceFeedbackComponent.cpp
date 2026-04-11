// JoystickPlugin is licensed under the MIT License.
// Copyright Jayden Maalouf 2026. All Rights Reserved.

#include "ForceFeedback/JoystickForceFeedbackComponent.h"

#include "Chaos/Framework/PhysicsSolverBase.h"
#include "Chaos/SimCallbackObject.h"
#include "Engine/Engine.h"
#include "ForceFeedback/Effects/ForceFeedbackEffectBase.h"
#include "JoystickSubsystem.h"
#include "Physics/Experimental/PhysScene_Chaos.h"
#include "PhysicsEngine/PhysicsSettings.h"
#include "PBDRigidsSolver.h"

class FJoystickForceFeedbackSubstepCallback final
	: public Chaos::TSimCallbackObject<
		Chaos::FSimCallbackNoInput,
		Chaos::FSimCallbackNoOutput>
{
public:
	explicit FJoystickForceFeedbackSubstepCallback(UJoystickForceFeedbackComponent* InComponent)
		: Component(InComponent)
	{
	}

private:
	virtual void OnPreSimulate_Internal() override
	{
		if (UJoystickForceFeedbackComponent* JoystickComponent = Component.Get())
		{
			JoystickComponent->TickEffects(static_cast<float>(GetDeltaTime_Internal()));
		}
	}

	virtual FName GetFNameForStatId() const override
	{
		static const FLazyName StaticName(TEXT("FJoystickForceFeedbackSubstepCallback"));
		return StaticName;
	}

	TWeakObjectPtr<UJoystickForceFeedbackComponent> Component;
};

UJoystickForceFeedbackComponent::UJoystickForceFeedbackComponent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
	  , InstanceId(-1)
	  , RegisteredSolver(nullptr)
	  , SubstepCallback(nullptr)
{
	bAutoActivate = true;
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.bStartWithTickEnabled = true;
	PrimaryComponentTick.TickGroup = TG_PostPhysics;

#if ENGINE_MAJOR_VERSION == 5 && ENGINE_MINOR_VERSION >= 3
	if (const UPhysicsSettings* PhysicsSettings = GetDefault<UPhysicsSettings>())
	{
		Configuration.UseAsyncPhysicsTick = PhysicsSettings->bTickPhysicsAsync;
	}

	SetAsyncPhysicsTickEnabled(Configuration.UseAsyncPhysicsTick);
#endif
}

void UJoystickForceFeedbackComponent::BeginPlay()
{
	Super::BeginPlay();

#if ENGINE_MAJOR_VERSION == 5 && ENGINE_MINOR_VERSION >= 3
	SetAsyncPhysicsTickEnabled(Configuration.UseAsyncPhysicsTick);
#endif

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

	RegisterPhysicsSubstepCallback();
}

void UJoystickForceFeedbackComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	UnregisterPhysicsSubstepCallback();

	ActionOnAllEffects([&](UForceFeedbackEffectBase* Effect)
	{
		DestroyEffect(Effect);
	});

	{
		FScopeLock EffectsLock(&EffectsCriticalSection);
		Effects.Empty();
	}

	Super::EndPlay(EndPlayReason);
}

void UJoystickForceFeedbackComponent::TickComponent(const float DeltaTime, const ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (Configuration.UseAsyncPhysicsTick || SubstepCallback != nullptr)
	{
		return;
	}

	TickEffects(DeltaTime);
}

#if (ENGINE_MAJOR_VERSION > 5) || (ENGINE_MAJOR_VERSION == 5 && ENGINE_MINOR_VERSION >= 3)
void UJoystickForceFeedbackComponent::AsyncPhysicsTickComponent(const float DeltaTime, const float SimTime)
{
	Super::AsyncPhysicsTickComponent(DeltaTime, SimTime);

	if (!Configuration.UseAsyncPhysicsTick)
	{
		return;
	}

	TickEffects(DeltaTime);
}
#endif

void UJoystickForceFeedbackComponent::TickEffects(const float DeltaTime)
{
	if (!Configuration.OverrideEffectTick)
	{
		return;
	}

	FScopeLock EffectsLock(&EffectsCriticalSection);

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

		// If true, the effect will handle its own Tick.
		if (ForcedFeedbackEffect->IsTickable())
		{
			continue;
		}

		ForcedFeedbackEffect->Tick(DeltaTime);
	}
}

void UJoystickForceFeedbackComponent::RegisterPhysicsSubstepCallback()
{
	if (!Configuration.UsePhysicsSubstepTick || Configuration.UseAsyncPhysicsTick || SubstepCallback != nullptr)
	{
		return;
	}

	const UWorld* World = GetWorld();
	if (!IsValid(World))
	{
		return;
	}

	const FPhysScene* PhysScene = World->GetPhysicsScene();
	if (PhysScene == nullptr)
	{
		return;
	}

	Chaos::FPhysicsSolverBase* Solver = PhysScene->GetSolver();
	if (Solver == nullptr)
	{
		return;
	}

	RegisteredSolver = Solver;
	SubstepCallback = Solver->CreateAndRegisterSimCallbackObject_External<FJoystickForceFeedbackSubstepCallback>(this);
}

void UJoystickForceFeedbackComponent::UnregisterPhysicsSubstepCallback()
{
	if (RegisteredSolver != nullptr && SubstepCallback != nullptr)
	{
		RegisteredSolver->UnregisterAndFreeSimCallbackObject_External(SubstepCallback);
	}

	RegisteredSolver = nullptr;
	SubstepCallback = nullptr;
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
	if (!EffectType)
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

	if (!EffectType)
	{
		return;
	}

	UJoystickSubsystem* JoystickSubsystem = GEngine->GetEngineSubsystem<UJoystickSubsystem>();
	if (!IsValid(JoystickSubsystem))
	{
		return;
	}

	FJoystickInformation JoystickInfo;
	const bool Result = JoystickSubsystem->GetJoystickInfo(JoystickInstanceId, JoystickInfo);
	if (!Result)
	{
		return;
	}

	if (!JoystickInfo.Haptic.Supported)
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

	{
		FScopeLock EffectsLock(&EffectsCriticalSection);
		Effects.Add(ForcedFeedbackEffect);
	}
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
	TArray<UForceFeedbackEffectBase*> EffectsToDestroy;

	{
		FScopeLock EffectsLock(&EffectsCriticalSection);
		for (UForceFeedbackEffectBase* Effect : Effects)
		{
			if (IsValid(Effect) && Effect->GetInstanceId() == JoystickInstanceId)
			{
				EffectsToDestroy.Add(Effect);
			}
		}

		Effects.RemoveAll([JoystickInstanceId](const UForceFeedbackEffectBase* Effect)
		{
			return !IsValid(Effect) || Effect->GetInstanceId() == JoystickInstanceId;
		});
	}

	for (UForceFeedbackEffectBase* Effect : EffectsToDestroy)
	{
		DestroyEffect(Effect);
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
	FScopeLock EffectsLock(&EffectsCriticalSection);
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
	TArray<UForceFeedbackEffectBase*> EffectsSnapshot;

	{
		FScopeLock EffectsLock(&EffectsCriticalSection);

		if (Effects.Num() == 0)
		{
			return;
		}

		for (UForceFeedbackEffectBase* Effect : Effects)
		{
			if (IsValid(Effect))
			{
				EffectsSnapshot.Add(Effect);
			}
		}
	}

	for (UForceFeedbackEffectBase* Effect : EffectsSnapshot)
	{
		CustomInitializer(Effect);
	}
}

void UJoystickForceFeedbackComponent::ActionOnJoystickEffects(const FJoystickInstanceId& JoystickInstanceId, const TFunctionRef<void(UForceFeedbackEffectBase* Effect)>& CustomInitializer)
{
	TArray<UForceFeedbackEffectBase*> EffectsSnapshot;

	{
		FScopeLock EffectsLock(&EffectsCriticalSection);

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

			EffectsSnapshot.Add(Effect);
		}
	}

	for (UForceFeedbackEffectBase* Effect : EffectsSnapshot)
	{
		CustomInitializer(Effect);
	}
}
