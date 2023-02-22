// JoystickPlugin is licensed under the MIT License.
// Copyright Jayden Maalouf. All Rights Reserved.

#include "ForceFeedback/Effects/ForceFeedbackEffectBase.h"
#include "JoystickFunctionLibrary.h"
#include "JoystickHapticDeviceManager.h"
#include "JoystickSubsystem.h"

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

	SDL_Haptic* HapticDevice = HapticDeviceManager->GetHapticDevice(DeviceId);
	if (HapticDevice == nullptr)
	{
		return;
	}

	CreateEffect();

	EffectId = SDL_HapticNewEffect(HapticDevice, &Effect);
	if (EffectId == -1)
	{
		const FString ErrorMessage = FString(SDL_GetError());
		UJoystickFunctionLibrary::Log(TEXT("HapticNewEffect error: %s"), *ErrorMessage);
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

	SDL_Haptic* HapticDevice = HapticDeviceManager->GetHapticDevice(DeviceId);
	if (HapticDevice == nullptr)
	{
		return;
	}

	SDL_HapticDestroyEffect(HapticDevice, EffectId);

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

	SDL_Haptic* HapticDevice = HapticDeviceManager->GetHapticDevice(DeviceId);
	if (HapticDevice == nullptr)
	{
		return;
	}

	if (InfiniteIterations)
	{
		Iterations = SDL_HAPTIC_INFINITY;
	}

	const int Result = SDL_HapticRunEffect(HapticDevice, EffectId, Iterations);
	if (Result == -1)
	{
		const FString ErrorMessage = FString(SDL_GetError());
		UJoystickFunctionLibrary::Log(TEXT("HapticRunEffect error: %s"), *ErrorMessage);
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

	SDL_Haptic* HapticDevice = HapticDeviceManager->GetHapticDevice(DeviceId);
	if (HapticDevice == nullptr)
	{
		return;
	}

	SDL_HapticStopEffect(HapticDevice, EffectId);

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

	SDL_Haptic* HapticDevice = HapticDeviceManager->GetHapticDevice(DeviceId);
	if (HapticDevice == nullptr)
	{
		return;
	}

	UpdateEffectData();
	const int Result = SDL_HapticUpdateEffect(HapticDevice, EffectId, &Effect);
	if (Result == -1)
	{
		const FString ErrorMessage = FString(SDL_GetError());
		UJoystickFunctionLibrary::Log(TEXT("HapticUpdateEffect error: %s"), *ErrorMessage);
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
		UJoystickFunctionLibrary::Log(TEXT("Cannot update Effect DeviceId post initialisation."));
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
