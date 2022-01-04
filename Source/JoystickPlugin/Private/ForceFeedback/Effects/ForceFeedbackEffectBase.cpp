#include "ForceFeedback/Effects/ForceFeedbackEffectBase.h"

#include "JoystickHapticDeviceManager.h"
#include "JoystickSubsystem.h"

UForceFeedbackEffectBase::UForceFeedbackEffectBase()
	: IsInitialised(false)
	, AutoStartOnInit(false)
	, Iterations(1)
	, Infinite(false)
{
}

void UForceFeedbackEffectBase::BeginDestroy()
{
	DestroyEffect();

	Super::BeginDestroy();
}

void UForceFeedbackEffectBase::Tick(float DeltaTime)
{
	ReceiveTick(DeltaTime);
}

void UForceFeedbackEffectBase::InitialiseEffect()
{
	if (IsInitialised)
	{
		return;
	}

	UJoystickHapticDeviceManager* HapticDeviceManager = UJoystickHapticDeviceManager::GetJoystickHapticDeviceManager();
	if (HapticDeviceManager == nullptr)
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
		UE_LOG(LogTemp, Log, TEXT("HapticNewEffect error: %s"), *ErrorMessage);
		return;
	}

	IsInitialised = true;

	//Safety check to ensure we don't try calling BP during destruction
	if (this->IsPendingKillOrUnreachable())
	{
		return;
	}

	OnInitialisedEffect();
	if (OnInitialisedEffectDelegate.IsBound())
	{
		OnInitialisedEffectDelegate.Broadcast(this);
	}

	if (AutoStartOnInit)
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

	StopEffect();

	UJoystickHapticDeviceManager* HapticDeviceManager = UJoystickHapticDeviceManager::GetJoystickHapticDeviceManager();
	if (HapticDeviceManager == nullptr)
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

	//Safety check to ensure we don't try calling BP during destruction
	if (this->IsPendingKillOrUnreachable())
	{
		return;
	}

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

	const int32 Status = EffectStatus();
	if (Status == 1)
	{
		return;
	}

	UJoystickHapticDeviceManager* HapticDeviceManager = UJoystickHapticDeviceManager::GetJoystickHapticDeviceManager();
	if (HapticDeviceManager == nullptr)
	{
		return;
	}
	
	SDL_Haptic* HapticDevice = HapticDeviceManager->GetHapticDevice(DeviceId);
	if (HapticDevice == nullptr)
	{
		return;
	}

	if (Infinite)
	{
		Iterations = SDL_HAPTIC_INFINITY;
	}

	const int32 Result = SDL_HapticRunEffect(HapticDevice, EffectId, Iterations);
	if (Result == -1)
	{
		const FString ErrorMessage = FString(SDL_GetError());
		UE_LOG(LogTemp, Log, TEXT("HapticRunEffect error: %s"), *ErrorMessage);
		return;
	}

	//Safety check to ensure we don't try calling BP during destruction
	if (this->IsPendingKillOrUnreachable())
	{
		return;
	}

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

	UJoystickHapticDeviceManager* HapticDeviceManager = UJoystickHapticDeviceManager::GetJoystickHapticDeviceManager();
	if (HapticDeviceManager == nullptr)
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
	if (IsPendingKillOrUnreachable()) 
	{
		return;
	}

	OnStoppedEffect();
	if (OnStoppedEffectDelegate.IsBound())
	{
		OnStoppedEffectDelegate.Broadcast(this);
	}
}

void UForceFeedbackEffectBase::UpdateEffect()
{
	UJoystickHapticDeviceManager* HapticDeviceManager = UJoystickHapticDeviceManager::GetJoystickHapticDeviceManager();
	if (HapticDeviceManager == nullptr)
	{
		return;
	}
	
	SDL_Haptic* HapticDevice = HapticDeviceManager->GetHapticDevice(DeviceId);
	if (HapticDevice == nullptr)
	{
		return;
	}

	UpdateEffectData();
	const int32 Result = SDL_HapticUpdateEffect(HapticDevice, EffectId, &Effect);
	if (Result == -1)
	{
		const FString ErrorMessage = FString(SDL_GetError());
		UE_LOG(LogTemp, Log, TEXT("HapticUpdateEffect error: %s"), *ErrorMessage);
		return;
	}

	//Safety check to ensure we don't try calling BP during destruction
	if (this->IsPendingKillOrUnreachable())
	{
		return;
	}

	OnUpdatedEffect();
	if (OnUpdatedEffectDelegate.IsBound())
	{
		OnUpdatedEffectDelegate.Broadcast(this);
	}
}

int32 UForceFeedbackEffectBase::EffectStatus() const
{
	UJoystickHapticDeviceManager* HapticDeviceManager = UJoystickHapticDeviceManager::GetJoystickHapticDeviceManager();
	if (HapticDeviceManager == nullptr)
	{
		return -1;
	}

	return HapticDeviceManager->GetEffectStatus(DeviceId, EffectId);
}

void UForceFeedbackEffectBase::CreateEffect()
{	
	SDL_memset(&Effect, 0, sizeof(SDL_HapticEffect));
	UpdateEffectData();
}

void UForceFeedbackEffectBase::UpdateEffectData()
{
}
