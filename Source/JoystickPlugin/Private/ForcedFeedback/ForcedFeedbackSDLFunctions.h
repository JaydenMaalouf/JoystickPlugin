#pragma once

#include "JoystickDevice.h"
#include "Interfaces/JoystickPluginInterface.h"
#include "Interfaces/JoystickInterface.h"
#include "JoystickPlugin.h"
#include "DeviceSDL.h"

THIRD_PARTY_INCLUDES_START

#include "SDL.h"

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

//SDL_HapticEffect FeedbackDataToSDLHapticEffect(const FForcedFeedbackData FeedbackData) 
//{
//	SDL_HapticEffect Effect;
//	SDL_memset(&Effect, 0, sizeof(SDL_HapticEffect));
//
//	switch (FeedbackData.Type) {
//
//			//Periodic
//		case(EForcedFeedbackEffectType::SINE):
//		case(EForcedFeedbackEffectType::TRIANGLE):
//		case(EForcedFeedbackEffectType::SAWTOOTHDOWN):
//		case(EForcedFeedbackEffectType::SAWTOOTHUP):
//		{
//
//			break;
//		}
//
//		//constant
//		case(EForcedFeedbackEffectType::CONSTANT):
//		{
//			break;
//		}
//		
//		//condition
//		case(EForcedFeedbackEffectType::SPRING):
//		case(EForcedFeedbackEffectType::DAMPER):
//		case(EForcedFeedbackEffectType::FRICTION):
//		case(EForcedFeedbackEffectType::INERTIA):
//		{
//
//			break;
//		}
//
//		//ramp
//		case(EForcedFeedbackEffectType::RAMP):
//		{
//			break;
//		}
//
//		default: {}
//		}
//
//	return Effect;
//}