// JoystickPlugin is licensed under the MIT License.
// Copyright Jayden Maalouf 2026. All Rights Reserved.

#include "ForceFeedback/Effects/ForceFeedbackEffectBase.h"

#include "Data/JoystickInstanceId.h"
#include "GameFramework/Actor.h"
#include "Managers/JoystickHapticDeviceManager.h"
#include "Managers/JoystickLogManager.h"
#include "Runtime/Launch/Resources/Version.h"

THIRD_PARTY_INCLUDES_START

#include "SDL_timer.h"
#include "SDL_stdinc.h"

THIRD_PARTY_INCLUDES_END

UForceFeedbackEffectBase::UForceFeedbackEffectBase(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
	  , InstanceId(-1)
	  , EffectId(-1)
	  , IsInitialised(false)
	  , Iterations(1)
	  , InfiniteIterations(false)
	  , Tickable(true)
	  , TickableInEditor(true)
	  , TickableWhenPaused(false)
	  , ForceStopAfterDurationLapsed(false)
	  , Effect()
	  , StartTime(-1)
	  , EffectRunning(false)
{
}

void UForceFeedbackEffectBase::BeginDestroy()
{
	DestroyEffect();

	Super::BeginDestroy();
}

void UForceFeedbackEffectBase::Tick(const float DeltaTime)
{
	if (LastFrameNumber == GFrameCounter)
	{
		return;
	}

	LastFrameNumber = GFrameCounter;

	if (!IsInitialised || this->IsUnreachable())
	{
		return;
	}

	ReceiveTick(DeltaTime);

	if (Configuration.AutoUpdatePostTick)
	{
		UpdateEffect();
	}

	const uint32 Duration = GetEffectDuration();
	if (EffectRunning && InfiniteIterations == false && StartTime != -1 && Duration != -1)
	{
		const uint64 CurrentTime = SDL_GetTicks();
		if (CurrentTime - StartTime >= Duration)
		{
			// Effect should be finished but sometimes SDL doesn't handle the status correctly.
			if (GetEffectStatus() == true || ForceStopAfterDurationLapsed)
			{
				StopEffect();
			}
		}
	}
}

void UForceFeedbackEffectBase::InitialiseEffect()
{
	if (IsInitialised)
	{
		FJoystickLogManager::Get()->LogWarning(TEXT("UForceFeedbackEffectBase::InitialiseEffect: Effect is already initialised"));
		return;
	}

	if (InstanceId == -1)
	{
		FJoystickLogManager::Get()->LogError(TEXT("UForceFeedbackEffectBase::InitialiseEffect: Effect InstanceId is not set"));
		return;
	}

	const UJoystickHapticDeviceManager* HapticDeviceManager = GetDefault<UJoystickHapticDeviceManager>();
	if (!IsValid(HapticDeviceManager))
	{
		FJoystickLogManager::Get()->LogError(TEXT("UForceFeedbackEffectBase::InitialiseEffect: HapticDeviceManager is invalid"));
		return;
	}

	CreateEffect();
	EffectId = HapticDeviceManager->CreateEffect(InstanceId, Effect);
	if (EffectId == -1)
	{
		return;
	}

	IsInitialised = true;

	//Safety check to ensure we don't try calling BP during destruction
#if ENGINE_MAJOR_VERSION == 5
	if (IsValidChecked(this) == false || this->IsUnreachable())
	{
		return;
	}
#else
	if (this->IsPendingKillOrUnreachable())
	{
		return;
	}
#endif

	OnInitialisedEffect();
	if (OnInitialisedEffectDelegate.IsBound())
	{
		OnInitialisedEffectDelegate.Broadcast(this);
	}

	if (Configuration.AutoStartOnInitialisation)
	{
		StartEffect();
	}
}

void UForceFeedbackEffectBase::StartEffect()
{
	if (IsInitialised == false)
	{
		return;
	}

	const bool Status = GetEffectStatus();
	if (Status == true)
	{
		EffectRunning = true;
		return;
	}

	const UJoystickHapticDeviceManager* HapticDeviceManager = GetDefault<UJoystickHapticDeviceManager>();
	if (!IsValid(HapticDeviceManager))
	{
		return;
	}

	if (InfiniteIterations)
	{
		Iterations = SDL_HAPTIC_INFINITY;
	}

	const bool Result = HapticDeviceManager->RunEffect(InstanceId, EffectId, Iterations);
	if (Result == false)
	{
		return;
	}
	StartTime = SDL_GetTicks();
	EffectRunning = true;

	//Safety check to ensure we don't try calling BP during destruction
#if ENGINE_MAJOR_VERSION == 5
	if (IsValidChecked(this) == false || this->IsUnreachable())
	{
		return;
	}
#else
	if (this->IsPendingKillOrUnreachable())
	{
		return;
	}
#endif

	OnStartedEffect();
	if (OnStartedEffectDelegate.IsBound())
	{
		OnStartedEffectDelegate.Broadcast(this);
	}
}

void UForceFeedbackEffectBase::StopEffect()
{
	if (IsInitialised == false)
	{
		return;
	}

	const UJoystickHapticDeviceManager* HapticDeviceManager = GetDefault<UJoystickHapticDeviceManager>();
	if (!IsValid(HapticDeviceManager))
	{
		return;
	}

	const bool Result = HapticDeviceManager->StopEffect(InstanceId, EffectId);
	if (Result == false)
	{
		return;
	}
	StartTime = -1;
	EffectRunning = false;

	//Safety check to ensure we don't try calling BP during destruction
#if ENGINE_MAJOR_VERSION == 5
	if (IsValidChecked(this) == false || this->IsUnreachable())
	{
		return;
	}
#else
	if (this->IsPendingKillOrUnreachable())
	{
		return;
	}
#endif

	OnStoppedEffect();
	if (OnStoppedEffectDelegate.IsBound())
	{
		OnStoppedEffectDelegate.Broadcast(this);
	}
}

void UForceFeedbackEffectBase::DestroyEffect()
{
	if (IsInitialised == false)
	{
		return;
	}

	const UJoystickHapticDeviceManager* HapticDeviceManager = GetDefault<UJoystickHapticDeviceManager>();
	if (!IsValid(HapticDeviceManager))
	{
		return;
	}

	HapticDeviceManager->DestroyEffect(InstanceId, EffectId);

	IsInitialised = false;
	EffectId = -1;
	StartTime = -1;
	EffectRunning = false;

	//Safety check to ensure we don't try calling BP during destruction
#if ENGINE_MAJOR_VERSION == 5
	if (IsValidChecked(this) == false || this->IsUnreachable())
	{
		return;
	}
#else
	if (this->IsPendingKillOrUnreachable())
	{
		return;
	}
#endif

	OnDestroyedEffect();
	if (OnDestroyedEffectDelegate.IsBound())
	{
		OnDestroyedEffectDelegate.Broadcast(this);
	}
}

void UForceFeedbackEffectBase::UpdateEffect()
{
	const UJoystickHapticDeviceManager* HapticDeviceManager = GetDefault<UJoystickHapticDeviceManager>();
	if (!IsValid(HapticDeviceManager))
	{
		return;
	}

	UpdateEffectData();
	const bool Result = HapticDeviceManager->UpdateEffect(InstanceId, EffectId, Effect);
	if (Result == false)
	{
		return;
	}

	//Safety check to ensure we don't try calling BP during destruction
#if ENGINE_MAJOR_VERSION == 5
	if (IsValidChecked(this) == false || this->IsUnreachable())
	{
		return;
	}
#else
	if (this->IsPendingKillOrUnreachable())
	{
		return;
	}
#endif

	OnUpdatedEffect();
	if (OnUpdatedEffectDelegate.IsBound())
	{
		OnUpdatedEffectDelegate.Broadcast(this);
	}
}

void UForceFeedbackEffectBase::ReceiveTick_Implementation(const float DeltaTime)
{
}

bool UForceFeedbackEffectBase::GetEffectStatus() const
{
	const UJoystickHapticDeviceManager* HapticDeviceManager = GetMutableDefault<UJoystickHapticDeviceManager>();
	if (!IsValid(HapticDeviceManager))
	{
		return false;
	}

	return HapticDeviceManager->GetEffectStatus(InstanceId, EffectId);
}

void UForceFeedbackEffectBase::SetInstanceId(const FJoystickInstanceId& NewInstanceId)
{
	if (IsInitialised)
	{
		FJoystickLogManager::Get()->LogWarning(TEXT("Cannot update Effect InstanceId post initialisation"));
		return;
	}

	InstanceId = NewInstanceId;
}

const FJoystickInstanceId& UForceFeedbackEffectBase::GetInstanceId() const
{
	return InstanceId;
}

void UForceFeedbackEffectBase::SetTickable(const bool NewTickable)
{
	Tickable = NewTickable;
}

void UForceFeedbackEffectBase::SetTickableInEditor(const bool NewTickableInEditor)
{
	TickableInEditor = NewTickableInEditor;
}

void UForceFeedbackEffectBase::SetTickableWhenPaused(const bool NewTickableWhenPaused)
{
	TickableWhenPaused = NewTickableWhenPaused;
}

AActor* UForceFeedbackEffectBase::GetOwningActor() const
{
	return GetOwningActor<AActor>();
}

void UForceFeedbackEffectBase::CreateEffect()
{
	SDL_memset(&Effect, 0, sizeof(SDL_HapticEffect));
	UpdateEffectData();
}

void UForceFeedbackEffectBase::UpdateEffectData()
{
}

uint32 UForceFeedbackEffectBase::GetEffectDuration()
{
	return -1;
}
