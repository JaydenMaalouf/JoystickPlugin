#include "ForcedFeedback/ForcedFeedbackEffect.h"
#include "ForcedFeedback/ForcedFeedbackData.h"
#include "Interfaces/JoystickPluginInterface.h"
#include "ForcedFeedback/ForcedFeedbackSDLFunctions.h"

void UForcedFeedbackEffect::Init()
{
	if (IsReady) 
	{
		return;
	}

	SDL_Haptic* haptic = GetSDLHapticFromDeviceId(DeviceId);
	if (haptic == nullptr)
	{
		return;
	}

	SDL_HapticEffect effect = this->ToSDLEffect();

	EffectId = SDL_HapticNewEffect(haptic, &effect);
	if (EffectId == -1) {
		TCHAR* Error = ANSI_TO_TCHAR(SDL_GetError());
		UE_LOG(JoystickPluginLog, Log, TEXT("HapticNewEffect error: %s"), Error);
		return;
	}

	IsReady = true;
	if (AutoStart) 
	{
		StartEffect();
	}
}

FForcedFeedbackData UForcedFeedbackEffect::GetEffect_Implementation()
{
	return EffectData;
}

void UForcedFeedbackEffect::PostInitProperties()
{
	Super::PostInitProperties();
	
	if (AutoInit && GetWorld())
	{
		Init();
	}
}

void UForcedFeedbackEffect::BeginDestroy()
{
	DestroyEffect();
	Super::BeginDestroy();
}

void UForcedFeedbackEffect::DestroyEffect()
{
	if (!IsReady) 
	{
		return;
	}

	StopEffect();

	SDL_Haptic* haptic = GetSDLHapticFromDeviceId(DeviceId);
	if (haptic == nullptr)
	{
		return;
	}

	SDL_HapticDestroyEffect(haptic, EffectId);

	IsReady = false;
}

int32 UForcedFeedbackEffect::EffectStatus()
{
	SDL_Haptic* haptic = GetSDLHapticFromDeviceId(DeviceId);
	if (haptic == nullptr)
	{
		return -1;
	}

	int32 result = SDL_HapticGetEffectStatus(haptic, EffectId);
	if (result == -1) {
		TCHAR* Error = ANSI_TO_TCHAR(SDL_GetError());
		UE_LOG(JoystickPluginLog, Log, TEXT("GetEffectStatus error: %s"), Error);
		return -1;
	}

	return result;
}

void UForcedFeedbackEffect::StartEffect()
{
	SDL_Haptic* haptic = GetSDLHapticFromDeviceId(DeviceId);
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
		TCHAR* Error = ANSI_TO_TCHAR(SDL_GetError());
		UE_LOG(JoystickPluginLog, Log, TEXT("HapticRunEffect error: %s"), Error);
	}
}

void UForcedFeedbackEffect::StopEffect()
{
	if (IsReady == false) 
	{
		return;
	}

	SDL_Haptic* haptic = GetSDLHapticFromDeviceId(DeviceId);
	if (haptic == nullptr)
	{
		return;
	}

	SDL_HapticStopEffect(haptic, EffectId);
}

void UForcedFeedbackEffect::UpdateEffect()
{
	FForcedFeedbackData data = GetEffect();

	SDL_Haptic* haptic = GetSDLHapticFromDeviceId(DeviceId);
	if (haptic == nullptr) 
	{
		return;
	}

	SDL_HapticEffect effect = this->ToSDLEffect();

	int32 result = SDL_HapticUpdateEffect(haptic, EffectId, &effect);
	if (result == -1) {
		TCHAR* Error = ANSI_TO_TCHAR(SDL_GetError());
		UE_LOG(JoystickPluginLog, Log, TEXT("HapticUpdateEffect error: %s"), Error);
	}
}

SDL_HapticEffect UForcedFeedbackEffect::ToSDLEffect()
{
	SDL_HapticEffect Effect;
	SDL_memset(&Effect, 0, sizeof(SDL_HapticEffect));

	return Effect;
}

UWorld* UForcedFeedbackEffect::GetWorld() const
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
