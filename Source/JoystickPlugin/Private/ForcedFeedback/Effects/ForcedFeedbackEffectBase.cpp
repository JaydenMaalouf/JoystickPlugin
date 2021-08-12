#include "ForcedFeedback/Effects/ForcedFeedbackEffectBase.h"

#include "ForcedFeedback/SDLForcedFeedbackFunctions.h"

void UForcedFeedbackEffectBase::BeginDestroy()
{
	DestroyEffect();

	Super::BeginDestroy();
}

void UForcedFeedbackEffectBase::InitialiseEffect()
{
	if (IsInitialised) 
	{
		return;
	}

	SDL_Haptic* haptic = SDLForcedFeedbackFunctions::GetSDLHapticFromDeviceId(DeviceId);
	if (haptic == nullptr)
	{
		return;
	}

	SDL_HapticEffect effect = this->ToSDLEffect();

	EffectId = SDL_HapticNewEffect(haptic, &effect);
	if (EffectId == -1) {
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
		OnInitialisedEffectDelegate.Broadcast();
	}

	if (AutoStartOnInit)
	{
		StartEffect();
	}
}

void UForcedFeedbackEffectBase::DestroyEffect()
{
	if (IsInitialised == false) 
	{
		return;
	}

	StopEffect();

	SDL_Haptic* haptic = SDLForcedFeedbackFunctions::GetSDLHapticFromDeviceId(DeviceId);
	if (haptic == nullptr)
	{
		return;
	}

	SDL_HapticDestroyEffect(haptic, EffectId);

	IsInitialised = false;

	//Safety check to ensure we don't try calling BP during destruction
	if (this->IsPendingKillOrUnreachable())
	{
		return;
	}

	OnDestroyedEffect();
	if (OnDestroyedEffectDelegate.IsBound())
	{
		OnDestroyedEffectDelegate.Broadcast();
	}
}

void UForcedFeedbackEffectBase::StartEffect()
{

	if (IsInitialised == false)
	{
		return;
	}

	SDL_Haptic* haptic = SDLForcedFeedbackFunctions::GetSDLHapticFromDeviceId(DeviceId);
	if (haptic == nullptr)
	{
		return;
	}

	if (Infinite)
	{
		Iterations = SDL_HAPTIC_INFINITY;
	}

	int32 result = SDL_HapticRunEffect(haptic, EffectId, Iterations);
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
		OnStartedEffectDelegate.Broadcast();
	}
}

void UForcedFeedbackEffectBase::StopEffect()
{
	if (IsInitialised == false) 
	{
		return;
	}

	SDL_Haptic* haptic = SDLForcedFeedbackFunctions::GetSDLHapticFromDeviceId(DeviceId);
	if (haptic == nullptr)
	{
		return;
	}

	SDL_HapticStopEffect(haptic, EffectId);

	//Safety check to ensure we don't try calling BP during destruction
	if (this->IsPendingKillOrUnreachable()) 
	{
		return;
	}

	OnStoppedEffect();
	if (OnStoppedEffectDelegate.IsBound())
	{
		OnStoppedEffectDelegate.Broadcast();
	}
}

void UForcedFeedbackEffectBase::UpdateEffect()
{
	SDL_Haptic* haptic = SDLForcedFeedbackFunctions::GetSDLHapticFromDeviceId(DeviceId);
	if (haptic == nullptr) 
	{
		return;
	}

	SDL_HapticEffect effect = this->ToSDLEffect();

	int32 result = SDL_HapticUpdateEffect(haptic, EffectId, &effect);
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
		OnUpdatedEffectDelegate.Broadcast();
	}
}

int32 UForcedFeedbackEffectBase::EffectStatus()
{
	SDL_Haptic* haptic = SDLForcedFeedbackFunctions::GetSDLHapticFromDeviceId(DeviceId);
	if (haptic == nullptr)
	{
		return -1;
	}

	int32 result = SDL_HapticGetEffectStatus(haptic, EffectId);
	if (result == -1) {
		TCHAR* errorMessage = ANSI_TO_TCHAR(SDL_GetError());
		UE_LOG(JoystickPluginLog, Log, TEXT("GetEffectStatus error: %s"), errorMessage);
		return -1;
	}

	return result;
}

SDL_HapticEffect UForcedFeedbackEffectBase::ToSDLEffect()
{
	SDL_HapticEffect Effect;
	SDL_memset(&Effect, 0, sizeof(SDL_HapticEffect));

	return Effect;
}