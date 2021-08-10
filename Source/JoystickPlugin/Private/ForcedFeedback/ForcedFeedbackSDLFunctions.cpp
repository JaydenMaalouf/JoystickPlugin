#include "ForcedFeedbackSDLFunctions.h"

SDL_Haptic* SDLFunctions::GetSDLHapticFromDeviceId(int32 DeviceId)
{
	if (!IJoystickPlugin::IsAvailable()) 
	{
		return NULL;
	}

	TSharedPtr<JoystickDeviceManager> JoystickDevice = static_cast<FJoystickPlugin&>(IJoystickPlugin::Get()).JoystickDevice;
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