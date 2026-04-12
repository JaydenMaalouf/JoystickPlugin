// JoystickPlugin is licensed under the MIT License.
// Copyright Jayden Maalouf 2026. All Rights Reserved.

#include "SDLDynamicLoader.h"

#include "Managers/JoystickLogManager.h"

bool FSDLDynamicLoader::LoadAllFunctions()
{
	bool bSuccess = true;

	// SDL Init functions
	bSuccess &= LoadFunction(pSDL_Init, TEXT("SDL_Init"));
	bSuccess &= LoadFunction(pSDL_InitSubSystem, TEXT("SDL_InitSubSystem"));
	bSuccess &= LoadFunction(pSDL_Quit, TEXT("SDL_Quit"));
	bSuccess &= LoadFunction(pSDL_QuitSubSystem, TEXT("SDL_QuitSubSystem"));
	bSuccess &= LoadFunction(pSDL_WasInit, TEXT("SDL_WasInit"));

	// SDL Event functions
	bSuccess &= LoadFunction(pSDL_AddEventWatch, TEXT("SDL_AddEventWatch"));
	bSuccess &= LoadFunction(pSDL_RemoveEventWatch, TEXT("SDL_RemoveEventWatch"));
	bSuccess &= LoadFunction(pSDL_PollEvent, TEXT("SDL_PollEvent"));

	// SDL Joystick functions
	bSuccess &= LoadFunction(pSDL_GetJoysticks, TEXT("SDL_GetJoysticks"));
	bSuccess &= LoadFunction(pSDL_OpenJoystick, TEXT("SDL_OpenJoystick"));
	bSuccess &= LoadFunction(pSDL_CloseJoystick, TEXT("SDL_CloseJoystick"));
	bSuccess &= LoadFunction(pSDL_GetJoystickVendor, TEXT("SDL_GetJoystickVendor"));
	bSuccess &= LoadFunction(pSDL_GetJoystickProduct, TEXT("SDL_GetJoystickProduct"));
	bSuccess &= LoadFunction(pSDL_GetJoystickProductVersion, TEXT("SDL_GetJoystickProductVersion"));
	bSuccess &= LoadFunction(pSDL_GetJoystickGUID, TEXT("SDL_GetJoystickGUID"));
	bSuccess &= LoadFunction(pSDL_GetJoystickType, TEXT("SDL_GetJoystickType"));
	bSuccess &= LoadFunction(pSDL_GetJoystickPowerInfo, TEXT("SDL_GetJoystickPowerInfo"));
	bSuccess &= LoadFunction(pSDL_GetJoystickName, TEXT("SDL_GetJoystickName"));
	bSuccess &= LoadFunction(pSDL_GetJoystickSerial, TEXT("SDL_GetJoystickSerial"));
	bSuccess &= LoadFunction(pSDL_GetJoystickProperties, TEXT("SDL_GetJoystickProperties"));
	bSuccess &= LoadFunction(pSDL_GetJoystickFirmwareVersion, TEXT("SDL_GetJoystickFirmwareVersion"));
	bSuccess &= LoadFunction(pSDL_GetJoystickPath, TEXT("SDL_GetJoystickPath"));
	bSuccess &= LoadFunction(pSDL_GetNumJoystickAxes, TEXT("SDL_GetNumJoystickAxes"));
	bSuccess &= LoadFunction(pSDL_GetNumJoystickButtons, TEXT("SDL_GetNumJoystickButtons"));
	bSuccess &= LoadFunction(pSDL_GetNumJoystickHats, TEXT("SDL_GetNumJoystickHats"));
	bSuccess &= LoadFunction(pSDL_GetNumJoystickBalls, TEXT("SDL_GetNumJoystickBalls"));
	bSuccess &= LoadFunction(pSDL_GetJoystickAxis, TEXT("SDL_GetJoystickAxis"));
	bSuccess &= LoadFunction(pSDL_GetJoystickAxisInitialState, TEXT("SDL_GetJoystickAxisInitialState"));
	bSuccess &= LoadFunction(pSDL_SetJoystickLED, TEXT("SDL_SetJoystickLED"));
	bSuccess &= LoadFunction(pSDL_IsJoystickHaptic, TEXT("SDL_IsJoystickHaptic"));

	// SDL Gamepad functions
	bSuccess &= LoadFunction(pSDL_IsGamepad, TEXT("SDL_IsGamepad"));
	bSuccess &= LoadFunction(pSDL_OpenGamepad, TEXT("SDL_OpenGamepad"));
	bSuccess &= LoadFunction(pSDL_CloseGamepad, TEXT("SDL_CloseGamepad"));
	bSuccess &= LoadFunction(pSDL_GamepadHasSensor, TEXT("SDL_GamepadHasSensor"));
	bSuccess &= LoadFunction(pSDL_SetGamepadSensorEnabled, TEXT("SDL_SetGamepadSensorEnabled"));
	bSuccess &= LoadFunction(pSDL_AddGamepadMappingsFromFile, TEXT("SDL_AddGamepadMappingsFromFile"));

	// SDL Haptic functions
	bSuccess &= LoadFunction(pSDL_OpenHapticFromJoystick, TEXT("SDL_OpenHapticFromJoystick"));
	bSuccess &= LoadFunction(pSDL_CloseHaptic, TEXT("SDL_CloseHaptic"));
	bSuccess &= LoadFunction(pSDL_GetNumHapticAxes, TEXT("SDL_GetNumHapticAxes"));
	bSuccess &= LoadFunction(pSDL_HapticRumbleSupported, TEXT("SDL_HapticRumbleSupported"));
	bSuccess &= LoadFunction(pSDL_InitHapticRumble, TEXT("SDL_InitHapticRumble"));
	bSuccess &= LoadFunction(pSDL_GetHapticFeatures, TEXT("SDL_GetHapticFeatures"));
	bSuccess &= LoadFunction(pSDL_SetHapticAutocenter, TEXT("SDL_SetHapticAutocenter"));
	bSuccess &= LoadFunction(pSDL_SetHapticGain, TEXT("SDL_SetHapticGain"));
	bSuccess &= LoadFunction(pSDL_PauseHaptic, TEXT("SDL_PauseHaptic"));
	bSuccess &= LoadFunction(pSDL_ResumeHaptic, TEXT("SDL_ResumeHaptic"));
	bSuccess &= LoadFunction(pSDL_StopHapticEffects, TEXT("SDL_StopHapticEffects"));
	bSuccess &= LoadFunction(pSDL_GetMaxHapticEffects, TEXT("SDL_GetMaxHapticEffects"));
	bSuccess &= LoadFunction(pSDL_GetMaxHapticEffectsPlaying, TEXT("SDL_GetMaxHapticEffectsPlaying"));
	bSuccess &= LoadFunction(pSDL_GetHapticEffectStatus, TEXT("SDL_GetHapticEffectStatus"));
	bSuccess &= LoadFunction(pSDL_CreateHapticEffect, TEXT("SDL_CreateHapticEffect"));
	bSuccess &= LoadFunction(pSDL_UpdateHapticEffect, TEXT("SDL_UpdateHapticEffect"));
	bSuccess &= LoadFunction(pSDL_RunHapticEffect, TEXT("SDL_RunHapticEffect"));
	bSuccess &= LoadFunction(pSDL_StopHapticEffect, TEXT("SDL_StopHapticEffect"));
	bSuccess &= LoadFunction(pSDL_DestroyHapticEffect, TEXT("SDL_DestroyHapticEffect"));
	bSuccess &= LoadFunction(pSDL_PlayHapticRumble, TEXT("SDL_PlayHapticRumble"));
	bSuccess &= LoadFunction(pSDL_StopHapticRumble, TEXT("SDL_StopHapticRumble"));

	// SDL Rumble functions
	bSuccess &= LoadFunction(pSDL_RumbleJoystick, TEXT("SDL_RumbleJoystick"));
	bSuccess &= LoadFunction(pSDL_RumbleJoystickTriggers, TEXT("SDL_RumbleJoystickTriggers"));

	// SDL Properties functions
	bSuccess &= LoadFunction(pSDL_GetBooleanProperty, TEXT("SDL_GetBooleanProperty"));

	// SDL Error functions
	bSuccess &= LoadFunction(pSDL_GetError, TEXT("SDL_GetError"));

	// SDL Timer functions
	bSuccess &= LoadFunction(pSDL_GetTicks, TEXT("SDL_GetTicks"));

	// SDL Memory functions
	bSuccess &= LoadFunction(pSDL_memset, TEXT("SDL_memset"));

	// SDL HID functions
	bSuccess &= LoadFunction(pSDL_hid_enumerate, TEXT("SDL_hid_enumerate"));
	bSuccess &= LoadFunction(pSDL_hid_free_enumeration, TEXT("SDL_hid_free_enumeration"));
	bSuccess &= LoadFunction(pSDL_hid_open, TEXT("SDL_hid_open"));
	bSuccess &= LoadFunction(pSDL_hid_open_path, TEXT("SDL_hid_open_path"));
	bSuccess &= LoadFunction(pSDL_hid_close, TEXT("SDL_hid_close"));
	bSuccess &= LoadFunction(pSDL_hid_read, TEXT("SDL_hid_read"));
	bSuccess &= LoadFunction(pSDL_hid_read_timeout, TEXT("SDL_hid_read_timeout"));
	bSuccess &= LoadFunction(pSDL_hid_get_feature_report, TEXT("SDL_hid_get_feature_report"));
	bSuccess &= LoadFunction(pSDL_hid_write, TEXT("SDL_hid_write"));
	bSuccess &= LoadFunction(pSDL_hid_send_feature_report, TEXT("SDL_hid_send_feature_report"));

	if (!bSuccess)
	{
		UE_LOG(LogJoystickPlugin, Error, TEXT("Failed to load one or more SDL3 functions"));
	}

	return bSuccess;
}
