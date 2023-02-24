// JoystickPlugin is licensed under the MIT License.
// Copyright Jayden Maalouf. All Rights Reserved.

#include "ForceFeedback/Effects/ForceFeedbackEffectBase.h"
#include "JoystickHapticDeviceManager.h"
#include "JoystickLogManager.h"
#include "JoystickSubsystem.h"
#include "Runtime/Launch/Resources/Version.h"

UForceFeedbackEffectBase::UForceFeedbackEffectBase(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
	  , DeviceId(0)
	  , EffectId(-1)
	  , IsInitialised(false)
	  , AutoStartOnInitialisation(false)
	  , AutoInitialise(false)
	  , Iterations(1)
	  , InfiniteIterations(false)
{
	if (AutoInitialise)
	{
		InitialiseEffect();
	}
}

void UForceFeedbackEffectBase::BeginDestroy()
{
	DestroyEffect();

	Super::BeginDestroy();
}

void UForceFeedbackEffectBase::Tick(const float DeltaTime)
{
	ReceiveTick(DeltaTime);
}

void UForceFeedbackEffectBase::InitialiseEffect()
{
	if (IsInitialised)
	{
		return;
	}

	const UJoystickHapticDeviceManager* HapticDeviceManager = UJoystickHapticDeviceManager::GetJoystickHapticDeviceManager();
	if (!IsValid(HapticDeviceManager))
	{
		return;
	}

	CreateEffect();
	EffectId = HapticDeviceManager->CreateEffect(DeviceId, Effect);
	if (EffectId == -1)
	{
		return;
	}

	IsInitialised = true;

	//Safety check to ensure we don't try calling BP during destruction
#if ENGINE_MAJOR_VERSION < 5
	if (this->IsPendingKillOrUnreachable())
	{
		return;
	}
#else
	if (!IsValidChecked(this))
	{
		return;
	}
#endif

	OnInitialisedEffect();
	if (OnInitialisedEffectDelegate.IsBound())
	{
		OnInitialisedEffectDelegate.Broadcast(this);
	}

	if (AutoStartOnInitialisation)
	{
		StartEffect();
	}
}

void UForceFeedbackEffectBase::DestroyEffect()
{
	if (IsInitialised == false)
	{
		return;
	}

	const UJoystickHapticDeviceManager* HapticDeviceManager = UJoystickHapticDeviceManager::GetJoystickHapticDeviceManager();
	if (!IsValid(HapticDeviceManager))
	{
		return;
	}

	HapticDeviceManager->DestroyEffect(DeviceId, EffectId);

	IsInitialised = false;
	EffectId = -1;

	//Safety check to ensure we don't try calling BP during destruction
#if ENGINE_MAJOR_VERSION < 5
	if (this->IsPendingKillOrUnreachable())
	{
		return;
	}
#else
	if (!IsValidChecked(this))
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

void UForceFeedbackEffectBase::StartEffect()
{
	if (IsInitialised == false)
	{
		return;
	}

	const int Status = EffectStatus();
	if (Status == 1)
	{
		return;
	}

	const UJoystickHapticDeviceManager* HapticDeviceManager = UJoystickHapticDeviceManager::GetJoystickHapticDeviceManager();
	if (!IsValid(HapticDeviceManager))
	{
		return;
	}

	if (InfiniteIterations)
	{
		Iterations = SDL_HAPTIC_INFINITY;
	}

	const bool Result = HapticDeviceManager->RunEffect(DeviceId, EffectId, Iterations);
	if (Result == false)
	{
		return;
	}

	//Safety check to ensure we don't try calling BP during destruction
#if ENGINE_MAJOR_VERSION < 5
	if (this->IsPendingKillOrUnreachable())
	{
		return;
	}
#else
	if (!IsValidChecked(this))
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

	const UJoystickHapticDeviceManager* HapticDeviceManager = UJoystickHapticDeviceManager::GetJoystickHapticDeviceManager();
	if (!IsValid(HapticDeviceManager))
	{
		return;
	}

	const bool Result = HapticDeviceManager->StopEffect(DeviceId, EffectId);
	if (Result == false)
	{
		return;
	}

	//Safety check to ensure we don't try calling BP during destruction
#if ENGINE_MAJOR_VERSION < 5
	if (this->IsPendingKillOrUnreachable())
	{
		return;
	}
#else
	if (!IsValidChecked(this))
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

void UForceFeedbackEffectBase::UpdateEffect()
{
	const UJoystickHapticDeviceManager* HapticDeviceManager = UJoystickHapticDeviceManager::GetJoystickHapticDeviceManager();
	if (!IsValid(HapticDeviceManager))
	{
		return;
	}

	UpdateEffectData();
	const bool Result = HapticDeviceManager->UpdateEffect(DeviceId, EffectId, Effect);
	if (Result == false)
	{
		return;
	}

	//Safety check to ensure we don't try calling BP during destruction
#if ENGINE_MAJOR_VERSION < 5
	if (this->IsPendingKillOrUnreachable())
	{
		return;
	}
#else
	if (!IsValidChecked(this))
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

int UForceFeedbackEffectBase::EffectStatus() const
{
	UJoystickHapticDeviceManager* HapticDeviceManager = UJoystickHapticDeviceManager::GetJoystickHapticDeviceManager();
	if (!IsValid(HapticDeviceManager))
	{
		return -1;
	}

	return HapticDeviceManager->GetEffectStatus(DeviceId, EffectId);
}

void UForceFeedbackEffectBase::SetDeviceId(const int NewDeviceId)
{
	if (IsInitialised)
	{
		FJoystickLogManager::Get()->LogWarning(TEXT("Cannot update Effect DeviceId post initialisation."));
		return;
	}

	DeviceId = NewDeviceId;
}

void UForceFeedbackEffectBase::CreateEffect()
{
	SDL_memset(&Effect, 0, sizeof(SDL_HapticEffect));
	UpdateEffectData();
}

void UForceFeedbackEffectBase::UpdateEffectData()
{
}
