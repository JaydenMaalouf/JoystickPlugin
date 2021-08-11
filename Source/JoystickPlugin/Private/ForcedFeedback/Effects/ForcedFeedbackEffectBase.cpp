#include "ForcedFeedback/Effects/ForcedFeedbackEffectBase.h"

#include "ForcedFeedback/SDLForcedFeedbackFunctions.h"

void UForcedFeedbackEffectBase::Init()
{
	if (IsReady) 
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

	IsReady = true;
	if (AutoStart) 
	{
		StartEffect();
	}
}

void UForcedFeedbackEffectBase::PostInitProperties()
{
	Super::PostInitProperties();
	
	if (AutoInit && GetWorld())
	{
		Init();
	}
}

void UForcedFeedbackEffectBase::BeginDestroy()
{
	DestroyEffect();
	Super::BeginDestroy();
}

void UForcedFeedbackEffectBase::DestroyEffect()
{
	if (!IsReady) 
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

	IsReady = false;
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

void UForcedFeedbackEffectBase::StartEffect()
{
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
	}
}

void UForcedFeedbackEffectBase::StopEffect()
{
	if (IsReady == false) 
	{
		return;
	}

	SDL_Haptic* haptic = SDLForcedFeedbackFunctions::GetSDLHapticFromDeviceId(DeviceId);
	if (haptic == nullptr)
	{
		return;
	}

	SDL_HapticStopEffect(haptic, EffectId);
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
	}
}

SDL_HapticEffect UForcedFeedbackEffectBase::ToSDLEffect()
{
	SDL_HapticEffect Effect;
	SDL_memset(&Effect, 0, sizeof(SDL_HapticEffect));

	return Effect;
}

UWorld* UForcedFeedbackEffectBase::GetWorld() const
{
	if (GIsEditor && !GIsPlayInEditorWorld)
	{
		return nullptr;
	}
	else if (GetOuter())
	{
		return GetOuter()->GetWorld();
	}
	return nullptr;
}
