// JoystickPlugin is licensed under the MIT License.
// Copyright Jayden Maalouf 2026. All Rights Reserved.

#include "ForceFeedback/JoystickForceFeedbackComponent.h"

#include "JoystickSubsystem.h"
#include "PBDRigidsSolver.h"
#include "Engine/Engine.h"
#include "ForceFeedback/JoystickForceFeedbackSubstepCallback.h"
#include "ForceFeedback/Data/ForceFeedbackEffectGroup.h"
#include "ForceFeedback/Effects/ForceFeedbackEffectBase.h"
#include "Managers/JoystickHapticDeviceManager.h"
#include "Managers/JoystickLogManager.h"
#include "PhysicsEngine/PhysicsSettings.h"

UJoystickForceFeedbackComponent::UJoystickForceFeedbackComponent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
	  , InstanceId(-1)
	  , Running(false)
	  , Tickable(true)
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
		Configuration.UseNativeAsyncPhysicsTick = PhysicsSettings->bTickPhysicsAsync;
	}

	SetAsyncPhysicsTickEnabled(Configuration.UseNativeAsyncPhysicsTick);
#endif
}

void UJoystickForceFeedbackComponent::BeginPlay()
{
	Super::BeginPlay();

#if ENGINE_MAJOR_VERSION == 5 && ENGINE_MINOR_VERSION >= 3
	SetAsyncPhysicsTickEnabled(Configuration.UseNativeAsyncPhysicsTick);
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
		OnSubsystemReady();
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

	DestroyAllEffects();

	Super::EndPlay(EndPlayReason);
}

void UJoystickForceFeedbackComponent::TickComponent(const float DeltaTime, const ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (Configuration.UseNativeAsyncPhysicsTick || SubstepCallback != nullptr)
	{
		return;
	}

	TickEffects(DeltaTime);
}

void UJoystickForceFeedbackComponent::SetComponentTickEnabled(const bool bEnabled)
{
	Super::SetComponentTickEnabled(bEnabled);

	SetTickable(bEnabled);
}

#if ENGINE_MAJOR_VERSION == 5 && ENGINE_MINOR_VERSION >= 3
void UJoystickForceFeedbackComponent::AsyncPhysicsTickComponent(const float DeltaTime, const float SimTime)
{
	Super::AsyncPhysicsTickComponent(DeltaTime, SimTime);

	if (!Configuration.UseNativeAsyncPhysicsTick)
	{
		return;
	}

	TickEffects(DeltaTime);
}
#endif

void UJoystickForceFeedbackComponent::SetTickable(const bool bTickable)
{
	Tickable = bTickable;
	if (Running)
	{
		if (Tickable)
		{
			StartAutoStartEffects();
		}
		else
		{
			ActionOnAllEffects([](UForceFeedbackEffectBase* Effect)
			{
				Effect->StopEffect();
			});
		}
	}
}

void UJoystickForceFeedbackComponent::OnSubsystemReady()
{
	if (Configuration.AutoStartOnInitialisation)
	{
		Running = true;
	}

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

	if (JoystickSubsystem->JoystickSubsystemReady.IsBound())
	{
		JoystickSubsystem->JoystickSubsystemReady.RemoveDynamic(this, &UJoystickForceFeedbackComponent::OnSubsystemReady);
	}
}

void UJoystickForceFeedbackComponent::TickEffects(const float DeltaTime)
{
	if (!Running || !Tickable)
	{
		return;
	}

	if (!Configuration.OverrideEffectTick)
	{
		return;
	}

	FScopeLock EffectsLock(&EffectsCriticalSection);

	if (Effects.Num() == 0)
	{
		return;
	}

	// Iterate a copy, as the effect callbacks can re-enter the component on this thread and change Effects.
	// The lock stays held for the whole pass so an effect cannot be destroyed or started/stopped while it ticks.
	const TArray<UForceFeedbackEffectBase*, TInlineAllocator<16>> EffectsSnapshot(Effects);
	for (UForceFeedbackEffectBase* ForcedFeedbackEffect : EffectsSnapshot)
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
	if (!Configuration.UsePhysicsCallbackTick || Configuration.UseNativeAsyncPhysicsTick || SubstepCallback != nullptr)
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
	SubstepCallback = Solver->CreateAndRegisterSimCallbackObject_External<FJoystickForceFeedbackSubstepCallback>();
	SubstepCallback->Init(this);
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

void UJoystickForceFeedbackComponent::CreateEffects()
{
	if (!EffectType && EffectGroups.Num() == 0)
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

	if (!EffectType && EffectGroups.Num() == 0)
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

	// Budget the device's effect slots, so the lowest priority effects are skipped instead of failing to create
	int AvailableSlots = GetAvailableEffectSlots(JoystickInstanceId);
	if (EffectType && AvailableSlots > 0)
	{
		// Single effect type, spawned with its class defaults
		if (CreateInstanceEffect(JoystickInstanceId, JoystickInfo.Haptic, EffectType, FForceFeedbackEffectGroupEntry(), nullptr))
		{
			AvailableSlots--;
		}
	}

	TArray<TPair<FForceFeedbackEffectGroupEntry, const UForceFeedbackEffectGroup*>> Entries;
	GetActiveGroupEntries(Entries);

	for (int i = 0; i < Entries.Num(); i++)
	{
		if (AvailableSlots <= 0)
		{
			FJoystickLogManager::Get()->LogWarning(TEXT("Device %d has no free effect slots, skipping the %d lowest priority effect(s)"), JoystickInstanceId.Value, Entries.Num() - i);
			break;
		}

		const FForceFeedbackEffectGroupEntry& Entry = Entries[i].Key;
		if (CreateInstanceEffect(JoystickInstanceId, JoystickInfo.Haptic, Entry.Effect->GetClass(), Entry, Entries[i].Value))
		{
			AvailableSlots--;
		}
	}
}

bool UJoystickForceFeedbackComponent::CreateInstanceEffect(const FJoystickInstanceId& JoystickInstanceId, const FJoystickHapticInformation& HapticInformation, const TSubclassOf<UForceFeedbackEffectBase> EffectClass,
                                                           const FForceFeedbackEffectGroupEntry& Entry, const UForceFeedbackEffectGroup* Group)
{
	UForceFeedbackEffectBase* ForcedFeedbackEffect = NewObject<UForceFeedbackEffectBase>(this, EffectClass, NAME_None, RF_NoFlags, Entry.Effect);
	if (!IsValid(ForcedFeedbackEffect))
	{
		return false;
	}

	ForcedFeedbackEffect->SetInstanceId(JoystickInstanceId);
	if (!ForcedFeedbackEffect->IsSupportedByDevice(HapticInformation))
	{
		FJoystickLogManager::Get()->LogDebug(TEXT("Device %d does not support effect %s, skipping"), JoystickInstanceId.Value, *EffectClass->GetName());
		return false;
	}

	if (Configuration.OverrideEffectTick)
	{
		//Disable Effect Tick as this will be driven by the component instead
		ForcedFeedbackEffect->SetTickable(false);
	}

	ForcedFeedbackEffect->EffectName = Entry.Name;
	ForcedFeedbackEffect->EffectTag = Entry.Tag;
	if (IsValid(Group))
	{
		ForcedFeedbackEffect->GroupTag = Group->GroupTag;
	}
	ForcedFeedbackEffect->ShouldStartWithComponent = Entry.AutoStart;

	ForcedFeedbackEffect->OnInitialisedEffectDelegate.AddDynamic(this, &UJoystickForceFeedbackComponent::OnInitialisedEffect);
	ForcedFeedbackEffect->OnStartedEffectDelegate.AddDynamic(this, &UJoystickForceFeedbackComponent::OnStartedEffect);
	ForcedFeedbackEffect->OnStoppedEffectDelegate.AddDynamic(this, &UJoystickForceFeedbackComponent::OnStoppedEffect);
	ForcedFeedbackEffect->OnUpdatedEffectDelegate.AddDynamic(this, &UJoystickForceFeedbackComponent::OnUpdatedEffect);
	ForcedFeedbackEffect->OnDestroyedEffectDelegate.AddDynamic(this, &UJoystickForceFeedbackComponent::OnDestroyedEffect);

	ForcedFeedbackEffect->Configuration.Update(Configuration);

	if (Running == false || Entry.AutoStart == false)
	{
		// Force effect not to auto start if the component isn't "running", or the effect is started manually
		ForcedFeedbackEffect->Configuration.AutoStartOnInitialisation = false;
	}

	if (ForcedFeedbackEffect->Configuration.AutoInitialise)
	{
		ForcedFeedbackEffect->InitialiseEffect();
	}

	if (Running && Entry.AutoStart)
	{
		// Effect component is already running, we should start the effect
		ForcedFeedbackEffect->StartEffect();
	}

	{
		FScopeLock EffectsLock(&EffectsCriticalSection);
		Effects.Add(ForcedFeedbackEffect);
	}

	return true;
}

int UJoystickForceFeedbackComponent::GetAvailableEffectSlots(const FJoystickInstanceId& JoystickInstanceId) const
{
	const UJoystickHapticDeviceManager* HapticDeviceManager = GetDefault<UJoystickHapticDeviceManager>();
	if (!IsValid(HapticDeviceManager))
	{
		return INT_MAX;
	}

	const int MaxEffects = HapticDeviceManager->GetMaxNumEffects(JoystickInstanceId);
	if (MaxEffects <= 0)
	{
		return INT_MAX;
	}

	// Only this component's effects are counted, the device does not report slots used elsewhere
	int ExistingEffects = 0;
	FScopeLock EffectsLock(&EffectsCriticalSection);
	for (const UForceFeedbackEffectBase* Effect : Effects)
	{
		if (IsValid(Effect) && Effect->GetInstanceId() == JoystickInstanceId)
		{
			ExistingEffects++;
		}
	}

	return FMath::Max(MaxEffects - ExistingEffects, 0);
}

void UJoystickForceFeedbackComponent::GetActiveGroupEntries(TArray<TPair<FForceFeedbackEffectGroupEntry, const UForceFeedbackEffectGroup*>>& OutEntries) const
{
	for (const UForceFeedbackEffectGroup* Group : EffectGroups)
	{
		if (!IsValid(Group))
		{
			continue;
		}

		TArray<FForceFeedbackEffectGroupEntry> GroupEntries;
		Group->GetActiveEntries(GroupEntries);

		for (const FForceFeedbackEffectGroupEntry& Entry : GroupEntries)
		{
			OutEntries.Emplace(Entry, Group);
		}
	}

	// The groups share the device's effect slots, so priority is compared across all of them
	OutEntries.StableSort([](const TPair<FForceFeedbackEffectGroupEntry, const UForceFeedbackEffectGroup*>& A, const TPair<FForceFeedbackEffectGroupEntry, const UForceFeedbackEffectGroup*>& B)
	{
		return A.Key.Priority > B.Key.Priority;
	});
}

void UJoystickForceFeedbackComponent::DestroyEffect(UForceFeedbackEffectBase* ForcedFeedbackEffect)
{
	if (!IsValid(ForcedFeedbackEffect))
	{
		return;
	}

	if (ForcedFeedbackEffect->OnInitialisedEffectDelegate.IsBound())
	{
		ForcedFeedbackEffect->OnInitialisedEffectDelegate.RemoveDynamic(this, &UJoystickForceFeedbackComponent::OnInitialisedEffect);
	}
	if (ForcedFeedbackEffect->OnStartedEffectDelegate.IsBound())
	{
		ForcedFeedbackEffect->OnStartedEffectDelegate.RemoveDynamic(this, &UJoystickForceFeedbackComponent::OnStartedEffect);
	}
	if (ForcedFeedbackEffect->OnStoppedEffectDelegate.IsBound())
	{
		ForcedFeedbackEffect->OnStoppedEffectDelegate.RemoveDynamic(this, &UJoystickForceFeedbackComponent::OnStoppedEffect);
	}
	if (ForcedFeedbackEffect->OnUpdatedEffectDelegate.IsBound())
	{
		ForcedFeedbackEffect->OnUpdatedEffectDelegate.RemoveDynamic(this, &UJoystickForceFeedbackComponent::OnUpdatedEffect);
	}
	if (ForcedFeedbackEffect->OnDestroyedEffectDelegate.IsBound())
	{
		ForcedFeedbackEffect->OnDestroyedEffectDelegate.RemoveDynamic(this, &UJoystickForceFeedbackComponent::OnDestroyedEffect);
	}

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

void UJoystickForceFeedbackComponent::DestroyAllEffects()
{
	TArray<UForceFeedbackEffectBase*> EffectsToDestroy;

	{
		FScopeLock EffectsLock(&EffectsCriticalSection);
		EffectsToDestroy = Effects;
		Effects.Empty();
	}

	for (UForceFeedbackEffectBase* Effect : EffectsToDestroy)
	{
		DestroyEffect(Effect);
	}
}

void UJoystickForceFeedbackComponent::StartAutoStartEffects()
{
	// Held while the effects are started, so they cannot be ticked on the physics thread at the same time
	FScopeLock EffectsLock(&EffectsCriticalSection);

	TArray<UForceFeedbackEffectBase*> EffectsToStart;
	for (UForceFeedbackEffectBase* Effect : Effects)
	{
		if (IsValid(Effect) && Effect->ShouldStartWithComponent)
		{
			EffectsToStart.Add(Effect);
		}
	}

	for (UForceFeedbackEffectBase* Effect : EffectsToStart)
	{
		Effect->StartEffect();
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
	Running = true;
	StartAutoStartEffects();
}

void UJoystickForceFeedbackComponent::StopEffect()
{
	Running = false;
	ActionOnAllEffects([](UForceFeedbackEffectBase* Effect)
	{
		Effect->StopEffect();
	});
}

void UJoystickForceFeedbackComponent::SetEffectGroups(const TArray<UForceFeedbackEffectGroup*>& NewEffectGroups)
{
	if (EffectGroups == NewEffectGroups)
	{
		return;
	}

	EffectGroups = NewEffectGroups;

	if (!HasBegunPlay())
	{
		return;
	}

	DestroyAllEffects();
	CreateEffects();
}

TArray<UForceFeedbackEffectBase*> UJoystickForceFeedbackComponent::GetEffectsByName(const FName EffectName) const
{
	TArray<UForceFeedbackEffectBase*> NamedEffects;
	if (EffectName.IsNone())
	{
		return NamedEffects;
	}

	FScopeLock EffectsLock(&EffectsCriticalSection);

	for (UForceFeedbackEffectBase* Effect : Effects)
	{
		if (IsValid(Effect) && Effect->EffectName == EffectName)
		{
			NamedEffects.Add(Effect);
		}
	}

	return NamedEffects;
}

void UJoystickForceFeedbackComponent::StartEffectByName(const FName EffectName)
{
	FScopeLock EffectsLock(&EffectsCriticalSection);
	for (UForceFeedbackEffectBase* Effect : GetEffectsByName(EffectName))
	{
		Effect->StartEffect();
	}
}

void UJoystickForceFeedbackComponent::StopEffectByName(const FName EffectName)
{
	FScopeLock EffectsLock(&EffectsCriticalSection);
	for (UForceFeedbackEffectBase* Effect : GetEffectsByName(EffectName))
	{
		Effect->StopEffect();
	}
}

TArray<UForceFeedbackEffectBase*> UJoystickForceFeedbackComponent::GetEffectsByTag(const FGameplayTag EffectTag) const
{
	TArray<UForceFeedbackEffectBase*> TaggedEffects;
	if (!EffectTag.IsValid())
	{
		return TaggedEffects;
	}

	FScopeLock EffectsLock(&EffectsCriticalSection);

	for (UForceFeedbackEffectBase* Effect : Effects)
	{
		if (IsValid(Effect) && (Effect->EffectTag.MatchesTag(EffectTag) || Effect->GroupTag.MatchesTag(EffectTag)))
		{
			TaggedEffects.Add(Effect);
		}
	}

	return TaggedEffects;
}

void UJoystickForceFeedbackComponent::StartEffectByTag(const FGameplayTag EffectTag)
{
	FScopeLock EffectsLock(&EffectsCriticalSection);
	for (UForceFeedbackEffectBase* Effect : GetEffectsByTag(EffectTag))
	{
		Effect->StartEffect();
	}
}

void UJoystickForceFeedbackComponent::StopEffectByTag(const FGameplayTag EffectTag)
{
	FScopeLock EffectsLock(&EffectsCriticalSection);
	for (UForceFeedbackEffectBase* Effect : GetEffectsByTag(EffectTag))
	{
		Effect->StopEffect();
	}
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
	// Held while the action runs, so the effects cannot be ticked on the physics thread at the same time
	FScopeLock EffectsLock(&EffectsCriticalSection);

	if (Effects.Num() == 0)
	{
		return;
	}

	TArray<UForceFeedbackEffectBase*> EffectsSnapshot;
	for (UForceFeedbackEffectBase* Effect : Effects)
	{
		if (IsValid(Effect))
		{
			EffectsSnapshot.Add(Effect);
		}
	}

	for (UForceFeedbackEffectBase* Effect : EffectsSnapshot)
	{
		CustomInitializer(Effect);
	}
}

void UJoystickForceFeedbackComponent::ActionOnJoystickEffects(const FJoystickInstanceId& JoystickInstanceId, const TFunctionRef<void(UForceFeedbackEffectBase* Effect)>& CustomInitializer)
{
	// Held while the action runs, so the effects cannot be ticked on the physics thread at the same time
	FScopeLock EffectsLock(&EffectsCriticalSection);

	if (Effects.Num() == 0)
	{
		return;
	}

	TArray<UForceFeedbackEffectBase*> EffectsSnapshot;
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

	for (UForceFeedbackEffectBase* Effect : EffectsSnapshot)
	{
		CustomInitializer(Effect);
	}
}
