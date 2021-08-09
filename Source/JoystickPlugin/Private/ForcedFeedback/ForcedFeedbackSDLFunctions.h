#pragma once

#include "JoystickDevice.h"
#include "Interfaces/JoystickPluginInterface.h"
#include "Interfaces/JoystickInterface.h"
#include "JoystickPlugin.h"
#include "DeviceSDL.h"

THIRD_PARTY_INCLUDES_START

#include "SDL.h"
#include "SDL_joystick.h"
#include "SDL_gamecontroller.h"

THIRD_PARTY_INCLUDES_END

SDL_Haptic* GetSDLHapticFromDeviceId(int32 DeviceId) 
{
	if (!IJoystickPlugin::IsAvailable()) 
	{
		return NULL;
	}

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

SDL_HapticEffect FeedbackDataToSDLHapticEffect(const FForcedFeedbackData FeedbackData) 
{
	SDL_HapticEffect Effect;
	SDL_memset(&Effect, 0, sizeof(SDL_HapticEffect));

	switch (FeedbackData.Type) {

			//Periodic
		case(EForcedFeedbackType::SINE):
		case(EForcedFeedbackType::TRIANGLE):
		case(EForcedFeedbackType::SAWTOOTHDOWN):
		case(EForcedFeedbackType::SAWTOOTHUP):
		{
			switch (FeedbackData.Type) {
				case(EForcedFeedbackType::SINE): {
					Effect.type = SDL_HAPTIC_SINE;
					break;
				}
				case(EForcedFeedbackType::TRIANGLE): {
					Effect.type = SDL_HAPTIC_TRIANGLE;
					break;
				}
				case(EForcedFeedbackType::SAWTOOTHUP): {
					Effect.type = SDL_HAPTIC_SAWTOOTHUP;
					break;
				}
				case(EForcedFeedbackType::SAWTOOTHDOWN): {
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
		case(EForcedFeedbackType::CONSTANT):
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
		case(EForcedFeedbackType::SPRING):
		case(EForcedFeedbackType::DAMPER):
		case(EForcedFeedbackType::FRICTION):
		case(EForcedFeedbackType::INERTIA):
		{
			switch (FeedbackData.Type) {
				case(EForcedFeedbackType::SPRING): {
					Effect.type = SDL_HAPTIC_SPRING;
					break;
				}
				case(EForcedFeedbackType::DAMPER): {
					Effect.type = SDL_HAPTIC_DAMPER;
					break;
				}
				case(EForcedFeedbackType::FRICTION): {
					Effect.type = SDL_HAPTIC_FRICTION;
					break;
				}
				case(EForcedFeedbackType::INERTIA): {
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
		case(EForcedFeedbackType::RAMP):
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