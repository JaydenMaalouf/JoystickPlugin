// JoystickPlugin is licensed under the MIT License.
// Copyright Jayden Maalouf 2026. All Rights Reserved.

#include "ForceFeedback/Effects/ForceFeedbackEffectBase.h"

#include "Data/JoystickHapticInformation.h"
#include "Data/JoystickInstanceId.h"
#include "GameFramework/Actor.h"
#include "Managers/JoystickHapticDeviceManager.h"
#include "Managers/JoystickLogManager.h"
#include "Runtime/Launch/Resources/Version.h"

THIRD_PARTY_INCLUDES_START

#include "JoystickSDL.h"

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
	  , StartTime(UINT64_MAX)
	  , TimeAccumulator(0)
	  , EffectRunning(false)
	  , ShouldStartWithComponent(true)
{
}

void UForceFeedbackEffectBase::BeginDestroy()
{
	DestroyEffect();

	Super::BeginDestroy();
}

void UForceFeedbackEffectBase::Tick(const float DeltaTime)
{
	if (Configuration.UseEffectRate)
	{
		const float FixedTimeStep = 1.0f / Configuration.EffectHz;

		TimeAccumulator += DeltaTime;

		int SubTickCount = 0;
		while (TimeAccumulator >= FixedTimeStep && SubTickCount < Configuration.MaxSubticks)
		{
			TickEffect(FixedTimeStep);

			TimeAccumulator -= FixedTimeStep;
			SubTickCount++;
		}

		// Clamp accumulator to prevent spiral of death
		if (TimeAccumulator > FixedTimeStep * Configuration.MaxSubticks)
		{
			TimeAccumulator = FixedTimeStep;
		}
	}
	else
	{
		TickEffect(DeltaTime);
	}
}

bool UForceFeedbackEffectBase::InitialiseEffect()
{
	if (IsInitialised)
	{
		FJoystickLogManager::Get()->LogWarning(TEXT("UForceFeedbackEffectBase::InitialiseEffect: Effect is already initialised"));
		return true;
	}

	if (InstanceId == -1)
	{
		FJoystickLogManager::Get()->LogError(TEXT("UForceFeedbackEffectBase::InitialiseEffect: Effect InstanceId is not set"));
		return false;
	}

	const UJoystickHapticDeviceManager* HapticDeviceManager = GetDefault<UJoystickHapticDeviceManager>();
	if (!IsValid(HapticDeviceManager))
	{
		FJoystickLogManager::Get()->LogError(TEXT("UForceFeedbackEffectBase::InitialiseEffect: HapticDeviceManager is invalid"));
		return false;
	}

	CreateEffect();
	EffectId = HapticDeviceManager->CreateEffect(InstanceId, Effect);
	if (EffectId == -1)
	{
		return false;
	}

	IsInitialised = true;

	//Safety check to ensure we don't try calling BP during destruction
	if (IsValidChecked(this) == false || this->IsUnreachable())
	{
		return true;
	}

	OnInitialisedEffect();
	if (OnInitialisedEffectDelegate.IsBound())
	{
		OnInitialisedEffectDelegate.Broadcast(this);
	}

	if (Configuration.AutoStartOnInitialisation)
	{
		StartEffect();
	}

	return true;
}

bool UForceFeedbackEffectBase::StartEffect()
{
	if (IsInitialised == false)
	{
		return false;
	}

	const bool Status = GetEffectStatus();
	if (Status == true)
	{
		EffectRunning = true;
		return true;
	}

	const UJoystickHapticDeviceManager* HapticDeviceManager = GetDefault<UJoystickHapticDeviceManager>();
	if (!IsValid(HapticDeviceManager))
	{
		return false;
	}

	if (InfiniteIterations)
	{
		Iterations = SDL_HAPTIC_INFINITY;
	}

	const bool Result = HapticDeviceManager->RunEffect(InstanceId, EffectId, Iterations);
	if (Result == false)
	{
		return false;
	}
	StartTime = SDL_GetTicks();
	EffectRunning = true;

	//Safety check to ensure we don't try calling BP during destruction
	if (IsValidChecked(this) == false || this->IsUnreachable())
	{
		return false;
	}

	OnStartedEffect();
	if (OnStartedEffectDelegate.IsBound())
	{
		OnStartedEffectDelegate.Broadcast(this);
	}

	return true;
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
	StartTime = UINT64_MAX;
	EffectRunning = false;

	//Safety check to ensure we don't try calling BP during destruction
	if (IsValidChecked(this) == false || this->IsUnreachable())
	{
		return;
	}

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
	StartTime = UINT64_MAX;
	EffectRunning = false;

	//Safety check to ensure we don't try calling BP during destruction
	if (IsValidChecked(this) == false || this->IsUnreachable())
	{
		return;
	}

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
	if (IsValidChecked(this) == false || this->IsUnreachable())
	{
		return;
	}

	OnUpdatedEffect();
	if (OnUpdatedEffectDelegate.IsBound())
	{
		OnUpdatedEffectDelegate.Broadcast(this);
	}
}

void UForceFeedbackEffectBase::OnInitialisedEffect_Implementation()
{
}

void UForceFeedbackEffectBase::OnStartedEffect_Implementation()
{
}

void UForceFeedbackEffectBase::OnStoppedEffect_Implementation()
{
}

void UForceFeedbackEffectBase::OnUpdatedEffect_Implementation()
{
}

void UForceFeedbackEffectBase::OnDestroyedEffect_Implementation()
{
}

void UForceFeedbackEffectBase::TickEffect(const float DeltaTime)
{
	if (!IsInitialised || this->IsUnreachable())
	{
		return;
	}

	ReceivedTick(DeltaTime);

	if (Configuration.AutoUpdatePostTick)
	{
		UpdateEffect();
	}

	// Effect should be finished but sometimes SDL2 doesn't handle the status correctly.
	if (ForceStopAfterDurationLapsed)
	{
		const uint32 Duration = GetEffectDuration();

		const bool ShouldCheck =
			EffectRunning &&
			InfiniteIterations == false &&
			StartTime != UINT64_MAX &&
			Duration != SDL_HAPTIC_INFINITY &&
			Duration != 0;

		if (ShouldCheck)
		{
			const uint64 CurrentTime = SDL_GetTicks();

			if (CurrentTime - StartTime >= Duration && GetEffectStatus())
			{
				StopEffect();
			}
		}
	}
}

void UForceFeedbackEffectBase::ReceivedTick_Implementation(const float DeltaTime)
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

bool UForceFeedbackEffectBase::IsSupportedByDevice(const FJoystickHapticInformation& HapticInformation)
{
	if (!IsInitialised)
	{
		CreateEffect();
	}

	switch (Effect.type)
	{
		case SDL_HAPTIC_CONSTANT:
			return HapticInformation.Constant;
		case SDL_HAPTIC_SINE:
			return HapticInformation.Sine;
		case SDL_HAPTIC_TRIANGLE:
			return HapticInformation.Triangle;
		case SDL_HAPTIC_SAWTOOTHUP:
			return HapticInformation.SawToothUp;
		case SDL_HAPTIC_SAWTOOTHDOWN:
			return HapticInformation.SawToothDown;
		case SDL_HAPTIC_RAMP:
			return HapticInformation.Ramp;
		case SDL_HAPTIC_SPRING:
			return HapticInformation.Spring;
		case SDL_HAPTIC_DAMPER:
			return HapticInformation.Damper;
		case SDL_HAPTIC_INERTIA:
			return HapticInformation.Inertia;
		case SDL_HAPTIC_FRICTION:
			return HapticInformation.Friction;
		case SDL_HAPTIC_LEFTRIGHT:
			return HapticInformation.LeftRight;
		case SDL_HAPTIC_CUSTOM:
			return HapticInformation.Custom;
		default:
			// Not a type we track support for, let SDL decide when the effect is created.
			return true;
	}
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
