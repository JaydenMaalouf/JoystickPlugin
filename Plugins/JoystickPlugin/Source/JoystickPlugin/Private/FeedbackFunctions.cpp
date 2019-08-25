#include "FeedbackFunctions.h"

#include "Engine/Engine.h"

#include "JoystickDevice.h"
#include "IJoystickPlugin.h"
#include "JoystickInterface.h"
#include "JoystickPlugin.h"
#include "DeviceSDL.h"

// @third party code - BEGIN SDL
#include "Windows/AllowWindowsPlatformTypes.h"

#include "Windows/MinWindows.h"
#include "SDL.h"
#include "SDL_joystick.h"
#include "SDL_gamecontroller.h"

#include "Windows/HideWindowsPlatformTypes.h"
// @third party code - END SDL

UFeedbackFunctions::UFeedbackFunctions(const class FObjectInitializer& PCIP)
	: Super(PCIP)
{

}

SDL_Haptic* GetSDLHapticFromDeviceId(int32 DeviceId) {
	if (!IJoystickPlugin::IsAvailable()) return NULL;

	TSharedPtr<FJoystickDevice> JoystickDevice = static_cast<FJoystickPlugin&>(IJoystickPlugin::Get()).JoystickDevice;
	auto* DeviceSDL = JoystickDevice->DeviceSDL->GetDevice(FDeviceId(DeviceId));
	if (!DeviceSDL) {
		UE_LOG(JoystickPluginLog, Log, TEXT("Invalid device"));
		return NULL;
	}

	SDL_Haptic* Haptic = DeviceSDL->Haptic;
	if (!Haptic) {
		UE_LOG(JoystickPluginLog, Log, TEXT("Device doesn't support force feedback"));
		return NULL;
	}

	return Haptic;
}

SDL_HapticEffect FeedbackDataToSDLHapticEffect(const FFeedbackData FeedbackData) {
	SDL_HapticEffect Effect;
	SDL_memset(&Effect, 0, sizeof(SDL_HapticEffect));


	switch (FeedbackData.Type) {

			//Periodic
		case(EFeedbackType::SINE):
		case(EFeedbackType::TRIANGLE):
		case(EFeedbackType::SAWTOOTHDOWN):
		case(EFeedbackType::SAWTOOTHUP):
		{
			switch (FeedbackData.Type) {
				case(EFeedbackType::SINE): {
					Effect.type = SDL_HAPTIC_SINE;
					break;
				}
				case(EFeedbackType::TRIANGLE): {
					Effect.type = SDL_HAPTIC_TRIANGLE;
					break;
				}
				case(EFeedbackType::SAWTOOTHUP): {
					Effect.type = SDL_HAPTIC_SAWTOOTHUP;
					break;
				}
				case(EFeedbackType::SAWTOOTHDOWN): {
					Effect.type = SDL_HAPTIC_SAWTOOTHDOWN;
					break;
				}
				default: {}
			}
			Effect.periodic.direction.type = SDL_HAPTIC_CARTESIAN;
			Effect.periodic.direction.dir[0] = Sint32(FeedbackData.Direction.X*INT_MAX);
			Effect.periodic.direction.dir[1] = Sint32(FeedbackData.Direction.Y*INT_MAX);
			Effect.periodic.direction.dir[2] = Sint32(FeedbackData.Direction.Z*INT_MAX);
			Effect.periodic.period = Uint16(FeedbackData.Period * 1000.0f);
			Effect.periodic.magnitude = FMath::Clamp(Uint16(FeedbackData.Magnitude * float(INT16_MAX)), Uint16(0), Uint16(INT16_MAX));
			Effect.periodic.offset = Sint16(FeedbackData.Offset * 32767.0f);
			Effect.periodic.phase = Uint16(FeedbackData.Phase * 100.0f);
			Effect.periodic.delay = Uint16(FeedbackData.Delay * 1000.0f);
			Effect.periodic.attack_length = Uint16(FeedbackData.AttackLength * 1000.0f);
			Effect.periodic.attack_level = FMath::Clamp(Uint16(FeedbackData.AttackLevel * float(INT16_MAX)), Uint16(0), Uint16(INT16_MAX));
			Effect.periodic.fade_length = Uint16(FeedbackData.FadeLength * 1000.0f);
			Effect.periodic.fade_level = FMath::Clamp(Uint16(FeedbackData.FadeLevel * float(INT16_MAX)), Uint16(0), Uint16(INT16_MAX));

			if (FeedbackData.InfiniteLength) {
				Effect.periodic.length = SDL_HAPTIC_INFINITY;
			}
			else {
				Effect.periodic.length = Uint32(FeedbackData.Length * 1000.0f);
			}

			break;
		}

		//constant
		case(EFeedbackType::CONSTANT):
		{
			Effect.type = SDL_HAPTIC_CONSTANT;
			Effect.constant.direction.type = SDL_HAPTIC_CARTESIAN;
			Effect.constant.direction.dir[0] = Sint32(FeedbackData.Direction.X*INT_MAX);
			Effect.constant.direction.dir[1] = Sint32(FeedbackData.Direction.Y*INT_MAX);
			Effect.constant.direction.dir[2] = Sint32(FeedbackData.Direction.Z*INT_MAX);
			Effect.constant.level = FMath::Clamp(Sint16(FeedbackData.Level * float(INT16_MAX)), Sint16(INT16_MIN), Sint16(INT16_MAX));
			Effect.constant.delay = Uint16(FeedbackData.Delay * 1000.0f);
			Effect.constant.attack_length = Uint16(FeedbackData.AttackLength * 1000.0f);
			Effect.constant.attack_level = FMath::Clamp(Uint16(FeedbackData.AttackLevel * float(INT16_MAX)), Uint16(0), Uint16(INT16_MAX));
			Effect.constant.fade_length = Uint16(FeedbackData.FadeLength * 1000.0f);
			Effect.constant.fade_level = FMath::Clamp(Uint16(FeedbackData.FadeLevel * float(INT16_MAX)), Uint16(0), Uint16(INT16_MAX));

			if (FeedbackData.InfiniteLength) {
				Effect.constant.length = SDL_HAPTIC_INFINITY;
			}
			else {
				Effect.constant.length = Uint32(FeedbackData.Length * 1000.0f);
			}

			break;
		}
		
		//condition
		case(EFeedbackType::SPRING):
		case(EFeedbackType::DAMPER):
		case(EFeedbackType::FRICTION):
		case(EFeedbackType::INERTIA):
		{
			switch (FeedbackData.Type) {
				case(EFeedbackType::SPRING): {
					Effect.type = SDL_HAPTIC_SPRING;
					break;
				}
				case(EFeedbackType::DAMPER): {
					Effect.type = SDL_HAPTIC_DAMPER;
					break;
				}
				case(EFeedbackType::FRICTION): {
					Effect.type = SDL_HAPTIC_FRICTION;
					break;
				}
				case(EFeedbackType::INERTIA): {
					Effect.type = SDL_HAPTIC_INERTIA;
					break;
				}
				default: {}
			}
			Effect.condition.direction.type = SDL_HAPTIC_CARTESIAN;
			Effect.condition.direction.dir[0] = Sint32(FeedbackData.Direction.X*INT_MAX);
			Effect.condition.direction.dir[1] = Sint32(FeedbackData.Direction.Y*INT_MAX);
			Effect.condition.direction.dir[2] = Sint32(FeedbackData.Direction.Z*INT_MAX);
			Effect.condition.delay = Uint16(FeedbackData.Delay * 1000.0f);
			Effect.condition.center[0] = Sint16(FeedbackData.Center.X*INT16_MAX);
			Effect.condition.center[1] = Sint16(FeedbackData.Center.Y*INT16_MAX);
			Effect.condition.center[2] = Sint16(FeedbackData.Center.Z*INT16_MAX);
			Effect.condition.deadband[0] = FMath::Clamp(Uint16(FeedbackData.Deadband.X * float(UINT16_MAX)), Uint16(0), Uint16(UINT16_MAX));
			Effect.condition.deadband[1] = FMath::Clamp(Uint16(FeedbackData.Deadband.Y * float(UINT16_MAX)), Uint16(0), Uint16(UINT16_MAX));
			Effect.condition.deadband[2] = FMath::Clamp(Uint16(FeedbackData.Deadband.Z * float(UINT16_MAX)), Uint16(0), Uint16(UINT16_MAX));
			Effect.condition.left_coeff[0] = FMath::Clamp(Sint16(FeedbackData.LeftCoeff.X * float(INT16_MAX)), Sint16(INT16_MIN), Sint16(INT16_MAX));
			Effect.condition.left_coeff[1] = FMath::Clamp(Sint16(FeedbackData.LeftCoeff.Y * float(INT16_MAX)), Sint16(INT16_MIN), Sint16(INT16_MAX));
			Effect.condition.left_coeff[2] = FMath::Clamp(Sint16(FeedbackData.LeftCoeff.Z * float(INT16_MAX)), Sint16(INT16_MIN), Sint16(INT16_MAX));
			Effect.condition.left_sat[0] = FMath::Clamp(Uint16(FeedbackData.LeftSat.X * float(UINT16_MAX)), Uint16(0), Uint16(UINT16_MAX));
			Effect.condition.left_sat[1] = FMath::Clamp(Uint16(FeedbackData.LeftSat.Y * float(UINT16_MAX)), Uint16(0), Uint16(UINT16_MAX));
			Effect.condition.left_sat[2] = FMath::Clamp(Uint16(FeedbackData.LeftSat.Z * float(UINT16_MAX)), Uint16(0), Uint16(UINT16_MAX));
			Effect.condition.right_coeff[0] = FMath::Clamp(Sint16(FeedbackData.RightCoeff.X * float(INT16_MAX)), Sint16(INT16_MIN), Sint16(INT16_MAX));
			Effect.condition.right_coeff[1] = FMath::Clamp(Sint16(FeedbackData.RightCoeff.Y * float(INT16_MAX)), Sint16(INT16_MIN), Sint16(INT16_MAX));
			Effect.condition.right_coeff[2] = FMath::Clamp(Sint16(FeedbackData.RightCoeff.Z * float(INT16_MAX)), Sint16(INT16_MIN), Sint16(INT16_MAX));
			Effect.condition.right_sat[0] = FMath::Clamp(Uint16(FeedbackData.RightSat.X * float(UINT16_MAX)), Uint16(0), Uint16(UINT16_MAX));
			Effect.condition.right_sat[1] = FMath::Clamp(Uint16(FeedbackData.RightSat.Y * float(UINT16_MAX)), Uint16(0), Uint16(UINT16_MAX));
			Effect.condition.right_sat[2] = FMath::Clamp(Uint16(FeedbackData.RightSat.Z * float(UINT16_MAX)), Uint16(0), Uint16(UINT16_MAX));

			if (FeedbackData.InfiniteLength) {
				Effect.condition.length = SDL_HAPTIC_INFINITY;
			}
			else {
				Effect.condition.length = Uint32(FeedbackData.Length * 1000.0f);
			}

			break;
		}

		//ramp
		case(EFeedbackType::RAMP): 
		{
			Effect.type = SDL_HAPTIC_RAMP;
			Effect.ramp.direction.type = SDL_HAPTIC_CARTESIAN;
			Effect.ramp.direction.dir[0] = Sint32(FeedbackData.Direction.X*INT_MAX);
			Effect.ramp.direction.dir[1] = Sint32(FeedbackData.Direction.Y*INT_MAX);
			Effect.ramp.direction.dir[2] = Sint32(FeedbackData.Direction.Z*INT_MAX);
			Effect.ramp.start = FMath::Clamp(Sint16(FeedbackData.RampStart * float(INT16_MAX)), Sint16(INT16_MIN), Sint16(INT16_MAX));
			Effect.ramp.end = FMath::Clamp(Sint16(FeedbackData.RampEnd * float(INT16_MAX)), Sint16(INT16_MIN), Sint16(INT16_MAX));
			Effect.ramp.delay = Uint16(FeedbackData.Delay * 1000.0f);
			Effect.ramp.length = Uint16(FeedbackData.Length * 1000.0f);
			Effect.ramp.attack_length = Uint16(FeedbackData.AttackLength * 1000.0f);
			Effect.ramp.attack_level = FMath::Clamp(Uint16(FeedbackData.AttackLevel * float(INT16_MAX)), Uint16(0), Uint16(INT16_MAX));
			Effect.ramp.fade_length = Uint16(FeedbackData.FadeLength * 1000.0f);
			Effect.ramp.fade_level = FMath::Clamp(Uint16(FeedbackData.FadeLevel * float(INT16_MAX)), Uint16(0), Uint16(INT16_MAX));
			break;
		}

		default: {}
		}

	return Effect;
}

bool UFeedbackFunctions::SetAutocenter(int32 DeviceId, int Center) {
	SDL_Haptic* Haptic = GetSDLHapticFromDeviceId(DeviceId);
	if (!Haptic) return false;

	int Success=SDL_HapticSetAutocenter(Haptic, Center);

	if (Success == -1) {
		TCHAR* Error = ANSI_TO_TCHAR(SDL_GetError());
		UE_LOG(JoystickPluginLog, Log,TEXT("Autocenter error: %s") , Error);
		return false;
	}

	return true;
}

bool UFeedbackFunctions::SetGain(int32 DeviceId, int Gain) {
	SDL_Haptic* Haptic = GetSDLHapticFromDeviceId(DeviceId);
	if (!Haptic) return false;

	int Success = SDL_HapticSetGain(Haptic, Gain);

	if (Success == -1) {
		TCHAR* Error = ANSI_TO_TCHAR(SDL_GetError());
		UE_LOG(JoystickPluginLog, Log, TEXT("Gain error: %s"), Error);
		return false;
	}

	return true;
}

void UFeedbackFunctions::DestroyEffect(int32 DeviceId, int32 EffectId) {
	SDL_Haptic* Haptic = GetSDLHapticFromDeviceId(DeviceId);
	if (!Haptic) return;
	SDL_HapticDestroyEffect(Haptic, EffectId);
}

int UFeedbackFunctions::GetEffectStatus(int32 DeviceId, int32 EffectId) {
	SDL_Haptic* Haptic = GetSDLHapticFromDeviceId(DeviceId);
	if (!Haptic) return -1;

	int Status = SDL_HapticGetEffectStatus(Haptic,EffectId);
	if (Status == -1) {
		TCHAR* Error = ANSI_TO_TCHAR(SDL_GetError());
		UE_LOG(JoystickPluginLog, Log, TEXT("GetEffectStatus error: %s"), Error);
		return -1;
	}
	else {
		return Status;
	}
}

void UFeedbackFunctions::StopEffect(int32 DeviceId, int32 EffectId) {
	SDL_Haptic* Haptic = GetSDLHapticFromDeviceId(DeviceId);
	if (!Haptic) return;

	SDL_HapticStopEffect(Haptic, EffectId);
}

void UFeedbackFunctions::PauseDevice(int32 DeviceId) {
	SDL_Haptic* Haptic = GetSDLHapticFromDeviceId(DeviceId);
	if (!Haptic) return;

	SDL_HapticPause(Haptic);
}

void UFeedbackFunctions::UnpauseDevice(int32 DeviceId) {
	SDL_Haptic* Haptic = GetSDLHapticFromDeviceId(DeviceId);
	if (!Haptic) return;

	SDL_HapticUnpause(Haptic);
}

void UFeedbackFunctions::StopAllEffects(int32 DeviceId) {
	SDL_Haptic* Haptic = GetSDLHapticFromDeviceId(DeviceId);
	if (!Haptic) return;

	SDL_HapticStopAll(Haptic);
}

int UFeedbackFunctions::GetNumEffects(int32 DeviceId) {
	SDL_Haptic* Haptic = GetSDLHapticFromDeviceId(DeviceId);
	if (!Haptic) return -1;

	return SDL_HapticNumEffects(Haptic);
}


int UFeedbackFunctions::GetNumEffectsPlaying(int32 DeviceId) {
	SDL_Haptic* Haptic = GetSDLHapticFromDeviceId(DeviceId);
	if (!Haptic) return -1;

	return SDL_HapticNumEffectsPlaying(Haptic);
}

int32 UFeedbackFunctions::CreateEffect(int32 DeviceId, FFeedbackData EffectData, int NumIterations, bool Infinity) {
	SDL_Haptic* Haptic = GetSDLHapticFromDeviceId(DeviceId);
	if (!Haptic) return -1;

	SDL_HapticEffect Effect = FeedbackDataToSDLHapticEffect(EffectData);

	int EffectId = SDL_HapticNewEffect(Haptic, &Effect);

	if (EffectId == -1) {
		TCHAR* Error = ANSI_TO_TCHAR(SDL_GetError());
		UE_LOG(JoystickPluginLog, Log, TEXT("HapticNewEffect error: %s"), Error);
		return -1;
	}

	if (Infinity) { NumIterations = SDL_HAPTIC_INFINITY; }

	int Success = SDL_HapticRunEffect(Haptic, EffectId, NumIterations);

	if (Success == -1) {
		TCHAR* Error = ANSI_TO_TCHAR(SDL_GetError());
		UE_LOG(JoystickPluginLog, Log, TEXT("HapticRunEffect error: %s"), Error);
		return -1;
	}

	return EffectId;
}

void UFeedbackFunctions::UpdateEffect(int32 DeviceId, int32 EffectId, FFeedbackData EffectData) {
	SDL_Haptic* Haptic = GetSDLHapticFromDeviceId(DeviceId);
	if (!Haptic) return;
	SDL_HapticEffect Effect = FeedbackDataToSDLHapticEffect(EffectData);

	int Success = SDL_HapticUpdateEffect(Haptic, EffectId, &Effect);

	if (Success == -1) {
		TCHAR* Error = ANSI_TO_TCHAR(SDL_GetError());
		UE_LOG(JoystickPluginLog, Log, TEXT("HapticUpdateEffect error: %s"), Error);
	}
}