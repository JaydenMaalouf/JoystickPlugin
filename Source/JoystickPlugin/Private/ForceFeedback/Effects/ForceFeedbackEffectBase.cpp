#include "ForceFeedback/Effects/ForceFeedbackEffectBase.h"

#include "JoystickFunctionLibrary.h"

void UForceFeedbackEffectBase::BeginDestroy()
{
	DestroyEffect();

	Super::BeginDestroy();
}

void UForceFeedbackEffectBase::InitialiseEffect()
{
	if (IsInitialised) 
	{
		return;
	}

	UJoystickDeviceManager* JoystickManager = UJoystickFunctionLibrary::GetJoystickManager();
	if (JoystickManager == nullptr)
	{
		return;
	}

	FDeviceInfoSDL* DeviceInfoSDL = JoystickManager->GetDeviceInfo(DeviceId);
	if (DeviceInfoSDL == nullptr)
	{
		return;
	}
	
	SDL_Haptic* HapticDevice = DeviceInfoSDL->Haptic;
	if (HapticDevice == nullptr)
	{
		return;
	}

	SDL_HapticEffect effect = this->ToSDLEffect();

	EffectId = SDL_HapticNewEffect(HapticDevice, &effect);
	if (EffectId == -1) 
	{
		TCHAR* errorMessage = ANSI_TO_TCHAR(SDL_GetError());
		UE_LOG(JoystickPluginLog, Log, TEXT("HapticNewEffect error: %s"), errorMessage);
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

	UJoystickDeviceManager* JoystickManager = UJoystickFunctionLibrary::GetJoystickManager();
	if (JoystickManager == nullptr)
	{
		return;
	}

	FDeviceInfoSDL* DeviceInfoSDL = JoystickManager->GetDeviceInfo(DeviceId);
	if (DeviceInfoSDL == nullptr)
	{
		return;
	}
	
	SDL_Haptic* HapticDevice = DeviceInfoSDL->Haptic;
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

	int32 status = EffectStatus();
	if (status == 1)
	{
		return;
	}

	UJoystickDeviceManager* JoystickManager = UJoystickFunctionLibrary::GetJoystickManager();
	if (JoystickManager == nullptr)
	{
		return;
	}

	FDeviceInfoSDL* DeviceInfoSDL = JoystickManager->GetDeviceInfo(DeviceId);
	if (DeviceInfoSDL == nullptr)
	{
		return;
	}
	
	SDL_Haptic* HapticDevice = DeviceInfoSDL->Haptic;
	if (HapticDevice == nullptr)
	{
		return;
	}

	if (Infinite)
	{
		Iterations = SDL_HAPTIC_INFINITY;
	}

	int32 result = SDL_HapticRunEffect(HapticDevice, EffectId, Iterations);
	if (result == -1)
	{
		TCHAR* errorMessageerrorMessage = ANSI_TO_TCHAR(SDL_GetError());
		UE_LOG(JoystickPluginLog, Log, TEXT("HapticRunEffect error: %s"), errorMessageerrorMessage);
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

	UJoystickDeviceManager* JoystickManager = UJoystickFunctionLibrary::GetJoystickManager();
	if (JoystickManager == nullptr)
	{
		return;
	}

	FDeviceInfoSDL* DeviceInfoSDL = JoystickManager->GetDeviceInfo(DeviceId);
	if (DeviceInfoSDL == nullptr)
	{
		return;
	}
	
	SDL_Haptic* HapticDevice = DeviceInfoSDL->Haptic;
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
	UJoystickDeviceManager* JoystickManager = UJoystickFunctionLibrary::GetJoystickManager();
	if (JoystickManager == nullptr)
	{
		return;
	}

	FDeviceInfoSDL* DeviceInfoSDL = JoystickManager->GetDeviceInfo(DeviceId);
	if (DeviceInfoSDL == nullptr)
	{
		return;
	}
	
	SDL_Haptic* HapticDevice = DeviceInfoSDL->Haptic;
	if (HapticDevice == nullptr)
	{
		return;
	}
	
	SDL_HapticEffect effect = ToSDLEffect();

	int32 result = SDL_HapticUpdateEffect(HapticDevice, EffectId, &effect);
	if (result == -1) {
		TCHAR* errorMessage = ANSI_TO_TCHAR(SDL_GetError());
		UE_LOG(JoystickPluginLog, Log, TEXT("HapticUpdateEffect error: %s"), errorMessage);
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

int32 UForceFeedbackEffectBase::EffectStatus()
{
	UJoystickDeviceManager* JoystickManager = UJoystickFunctionLibrary::GetJoystickManager();
	if (JoystickManager == nullptr)
	{
		return -1;
	}

	FDeviceInfoSDL* DeviceInfoSDL = JoystickManager->GetDeviceInfo(DeviceId);
	if (DeviceInfoSDL == nullptr)
	{
		return -1;
	}
	
	SDL_Haptic* HapticDevice = DeviceInfoSDL->Haptic;
	if (HapticDevice == nullptr)
	{
		return -1;
	}

	int32 result = SDL_HapticGetEffectStatus(HapticDevice, EffectId);
	if (result == -1) {
		TCHAR* errorMessage = ANSI_TO_TCHAR(SDL_GetError());
		UE_LOG(JoystickPluginLog, Log, TEXT("GetEffectStatus error: %s"), errorMessage);
		return -1;
	}

	return result;
}

SDL_HapticEffect UForceFeedbackEffectBase::ToSDLEffect()
{
	SDL_HapticEffect Effect;
	SDL_memset(&Effect, 0, sizeof(SDL_HapticEffect));

	return Effect;
}