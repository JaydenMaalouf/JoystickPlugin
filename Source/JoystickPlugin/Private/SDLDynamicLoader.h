// JoystickPlugin is licensed under the MIT License.
// Copyright Jayden Maalouf 2026. All Rights Reserved.

#pragma once

THIRD_PARTY_INCLUDES_START
#include "SDL_init.h"
#include "SDL_guid.h"
#include "SDL_sensor.h"
#include "SDL_haptic.h"
#include "SDL_joystick.h"
#include "SDL_gamepad.h"
#include "SDL_power.h"
#include "SDL_properties.h"
#include "SDL_error.h"
#include "SDL_timer.h"
#include "SDL_stdinc.h"
#include "SDL_hidapi.h"
THIRD_PARTY_INCLUDES_END

class FSDLDynamicLoader
{
public:
	static FSDLDynamicLoader& Get()
	{
		static FSDLDynamicLoader Instance;
		return Instance;
	}

	bool Load(const FString& LibraryPath)
	{
		if (Handle != nullptr)
		{
			return true; // Already loaded
		}

		Handle = FPlatformProcess::GetDllHandle(*LibraryPath);

		if (!Handle)
		{
			LastError = FString(UTF8_TO_TCHAR(dlerror()));
			return false;
		}

		LoadedPath = LibraryPath;
		return LoadAllFunctions();
	}

	void Unload()
	{
		if (Handle)
		{
			FPlatformProcess::FreeDllHandle(Handle);
			Handle = nullptr;
		}
	}

	FString GetLastError() const { return LastError; }
	FString GetLoadedPath() const { return LoadedPath; }
	bool IsLoaded() const { return Handle != nullptr; }

	// SDL Init functions
	decltype(&SDL_Init) pSDL_Init = nullptr;
	decltype(&SDL_InitSubSystem) pSDL_InitSubSystem = nullptr;
	decltype(&SDL_Quit) pSDL_Quit = nullptr;
	decltype(&SDL_QuitSubSystem) pSDL_QuitSubSystem = nullptr;
	decltype(&SDL_WasInit) pSDL_WasInit = nullptr;

	// SDL Event functions
	decltype(&SDL_AddEventWatch) pSDL_AddEventWatch = nullptr;
	decltype(&SDL_RemoveEventWatch) pSDL_RemoveEventWatch = nullptr;
	decltype(&SDL_PollEvent) pSDL_PollEvent = nullptr;

	// SDL Joystick functions
	decltype(&SDL_GetJoysticks) pSDL_GetJoysticks = nullptr;
	decltype(&SDL_OpenJoystick) pSDL_OpenJoystick = nullptr;
	decltype(&SDL_CloseJoystick) pSDL_CloseJoystick = nullptr;
	decltype(&SDL_GetJoystickVendor) pSDL_GetJoystickVendor = nullptr;
	decltype(&SDL_GetJoystickProduct) pSDL_GetJoystickProduct = nullptr;
	decltype(&SDL_GetJoystickProductVersion) pSDL_GetJoystickProductVersion = nullptr;
	decltype(&SDL_GetJoystickGUID) pSDL_GetJoystickGUID = nullptr;
	decltype(&SDL_GetJoystickType) pSDL_GetJoystickType = nullptr;
	decltype(&SDL_GetJoystickPowerInfo) pSDL_GetJoystickPowerInfo = nullptr;
	decltype(&SDL_GetJoystickName) pSDL_GetJoystickName = nullptr;
	decltype(&SDL_GetJoystickSerial) pSDL_GetJoystickSerial = nullptr;
	decltype(&SDL_GetJoystickProperties) pSDL_GetJoystickProperties = nullptr;
	decltype(&SDL_GetJoystickFirmwareVersion) pSDL_GetJoystickFirmwareVersion = nullptr;
	decltype(&SDL_GetJoystickPath) pSDL_GetJoystickPath = nullptr;
	decltype(&SDL_GetNumJoystickAxes) pSDL_GetNumJoystickAxes = nullptr;
	decltype(&SDL_GetNumJoystickButtons) pSDL_GetNumJoystickButtons = nullptr;
	decltype(&SDL_GetNumJoystickHats) pSDL_GetNumJoystickHats = nullptr;
	decltype(&SDL_GetNumJoystickBalls) pSDL_GetNumJoystickBalls = nullptr;
	decltype(&SDL_GetJoystickAxis) pSDL_GetJoystickAxis = nullptr;
	decltype(&SDL_GetJoystickAxisInitialState) pSDL_GetJoystickAxisInitialState = nullptr;
	decltype(&SDL_SetJoystickLED) pSDL_SetJoystickLED = nullptr;
	decltype(&SDL_IsJoystickHaptic) pSDL_IsJoystickHaptic = nullptr;

	// SDL Gamepad functions
	decltype(&SDL_IsGamepad) pSDL_IsGamepad = nullptr;
	decltype(&SDL_OpenGamepad) pSDL_OpenGamepad = nullptr;
	decltype(&SDL_CloseGamepad) pSDL_CloseGamepad = nullptr;
	decltype(&SDL_GamepadHasSensor) pSDL_GamepadHasSensor = nullptr;
	decltype(&SDL_SetGamepadSensorEnabled) pSDL_SetGamepadSensorEnabled = nullptr;
	decltype(&SDL_AddGamepadMappingsFromFile) pSDL_AddGamepadMappingsFromFile = nullptr;

	// SDL Haptic functions
	decltype(&SDL_OpenHapticFromJoystick) pSDL_OpenHapticFromJoystick = nullptr;
	decltype(&SDL_CloseHaptic) pSDL_CloseHaptic = nullptr;
	decltype(&SDL_GetNumHapticAxes) pSDL_GetNumHapticAxes = nullptr;
	decltype(&SDL_HapticRumbleSupported) pSDL_HapticRumbleSupported = nullptr;
	decltype(&SDL_InitHapticRumble) pSDL_InitHapticRumble = nullptr;
	decltype(&SDL_GetHapticFeatures) pSDL_GetHapticFeatures = nullptr;
	decltype(&SDL_SetHapticAutocenter) pSDL_SetHapticAutocenter = nullptr;
	decltype(&SDL_SetHapticGain) pSDL_SetHapticGain = nullptr;
	decltype(&SDL_PauseHaptic) pSDL_PauseHaptic = nullptr;
	decltype(&SDL_ResumeHaptic) pSDL_ResumeHaptic = nullptr;
	decltype(&SDL_StopHapticEffects) pSDL_StopHapticEffects = nullptr;
	decltype(&SDL_GetMaxHapticEffects) pSDL_GetMaxHapticEffects = nullptr;
	decltype(&SDL_GetMaxHapticEffectsPlaying) pSDL_GetMaxHapticEffectsPlaying = nullptr;
	decltype(&SDL_GetHapticEffectStatus) pSDL_GetHapticEffectStatus = nullptr;
	decltype(&SDL_CreateHapticEffect) pSDL_CreateHapticEffect = nullptr;
	decltype(&SDL_UpdateHapticEffect) pSDL_UpdateHapticEffect = nullptr;
	decltype(&SDL_RunHapticEffect) pSDL_RunHapticEffect = nullptr;
	decltype(&SDL_StopHapticEffect) pSDL_StopHapticEffect = nullptr;
	decltype(&SDL_DestroyHapticEffect) pSDL_DestroyHapticEffect = nullptr;
	decltype(&SDL_PlayHapticRumble) pSDL_PlayHapticRumble = nullptr;
	decltype(&SDL_StopHapticRumble) pSDL_StopHapticRumble = nullptr;

	// SDL Rumble functions
	decltype(&SDL_RumbleJoystick) pSDL_RumbleJoystick = nullptr;
	decltype(&SDL_RumbleJoystickTriggers) pSDL_RumbleJoystickTriggers = nullptr;

	// SDL Properties functions
	decltype(&SDL_GetBooleanProperty) pSDL_GetBooleanProperty = nullptr;

	// SDL Error functions
	decltype(&SDL_GetError) pSDL_GetError = nullptr;

	// SDL Timer functions
	decltype(&SDL_GetTicks) pSDL_GetTicks = nullptr;

	// SDL Memory functions
	decltype(&SDL_memset) pSDL_memset = nullptr;

	// SDL HID functions
	decltype(&SDL_hid_enumerate) pSDL_hid_enumerate = nullptr;
	decltype(&SDL_hid_free_enumeration) pSDL_hid_free_enumeration = nullptr;
	decltype(&SDL_hid_open) pSDL_hid_open = nullptr;
	decltype(&SDL_hid_open_path) pSDL_hid_open_path = nullptr;
	decltype(&SDL_hid_close) pSDL_hid_close = nullptr;
	decltype(&SDL_hid_read) pSDL_hid_read = nullptr;
	decltype(&SDL_hid_read_timeout) pSDL_hid_read_timeout = nullptr;
	decltype(&SDL_hid_get_feature_report) pSDL_hid_get_feature_report = nullptr;
	decltype(&SDL_hid_write) pSDL_hid_write = nullptr;
	decltype(&SDL_hid_send_feature_report) pSDL_hid_send_feature_report = nullptr;

private:
	void* Handle = nullptr;
	FString LastError;
	FString LoadedPath;

	bool LoadAllFunctions();

	template<typename T>
	bool LoadFunction(T*& FuncPtr, const TCHAR* FuncName)
	{
		FuncPtr = (T*)FPlatformProcess::GetDllExport(Handle, FuncName);
		if (!FuncPtr)
		{
			UE_LOG(LogTemp, Error, TEXT("Failed to load SDL function: %s"), UTF8_TO_TCHAR(FuncName));
			return false;
		}
		return true;
	}

	FSDLDynamicLoader() = default;
	~FSDLDynamicLoader()
	{
		Unload();
	}

	FSDLDynamicLoader(const FSDLDynamicLoader&) = delete;
	FSDLDynamicLoader& operator=(const FSDLDynamicLoader&) = delete;
};

// Undefine any existing SDL macros before redefining them
#undef SDL_Init
#undef SDL_InitSubSystem
#undef SDL_Quit
#undef SDL_QuitSubSystem
#undef SDL_WasInit
#undef SDL_AddEventWatch
#undef SDL_RemoveEventWatch
#undef SDL_PollEvent
#undef SDL_GetJoysticks
#undef SDL_OpenJoystick
#undef SDL_CloseJoystick
#undef SDL_GetJoystickVendor
#undef SDL_GetJoystickProduct
#undef SDL_GetJoystickProductVersion
#undef SDL_GetJoystickGUID
#undef SDL_GetJoystickType
#undef SDL_GetJoystickPowerInfo
#undef SDL_GetJoystickName
#undef SDL_GetJoystickSerial
#undef SDL_GetJoystickProperties
#undef SDL_GetJoystickFirmwareVersion
#undef SDL_GetJoystickPath
#undef SDL_GetNumJoystickAxes
#undef SDL_GetNumJoystickButtons
#undef SDL_GetNumJoystickHats
#undef SDL_GetNumJoystickBalls
#undef SDL_GetJoystickAxis
#undef SDL_GetJoystickAxisInitialState
#undef SDL_SetJoystickLED
#undef SDL_IsJoystickHaptic
#undef SDL_IsGamepad
#undef SDL_OpenGamepad
#undef SDL_CloseGamepad
#undef SDL_GamepadHasSensor
#undef SDL_SetGamepadSensorEnabled
#undef SDL_AddGamepadMappingsFromFile
#undef SDL_OpenHapticFromJoystick
#undef SDL_CloseHaptic
#undef SDL_GetNumHapticAxes
#undef SDL_HapticRumbleSupported
#undef SDL_InitHapticRumble
#undef SDL_GetHapticFeatures
#undef SDL_SetHapticAutocenter
#undef SDL_SetHapticGain
#undef SDL_PauseHaptic
#undef SDL_ResumeHaptic
#undef SDL_StopHapticEffects
#undef SDL_GetMaxHapticEffects
#undef SDL_GetMaxHapticEffectsPlaying
#undef SDL_GetHapticEffectStatus
#undef SDL_CreateHapticEffect
#undef SDL_UpdateHapticEffect
#undef SDL_RunHapticEffect
#undef SDL_StopHapticEffect
#undef SDL_DestroyHapticEffect
#undef SDL_PlayHapticRumble
#undef SDL_StopHapticRumble
#undef SDL_RumbleJoystick
#undef SDL_RumbleJoystickTriggers
#undef SDL_GetBooleanProperty
#undef SDL_GetError
#undef SDL_GetTicks
#undef SDL_memset
#undef SDL_hid_enumerate
#undef SDL_hid_free_enumeration
#undef SDL_hid_open
#undef SDL_hid_open_path
#undef SDL_hid_close
#undef SDL_hid_read
#undef SDL_hid_read_timeout
#undef SDL_hid_get_feature_report
#undef SDL_hid_write
#undef SDL_hid_send_feature_report

// Macros to redirect SDL calls to our function pointers
#define SDL_Init(...) FSDLDynamicLoader::Get().pSDL_Init(__VA_ARGS__)
#define SDL_InitSubSystem(...) FSDLDynamicLoader::Get().pSDL_InitSubSystem(__VA_ARGS__)
#define SDL_Quit(...) FSDLDynamicLoader::Get().pSDL_Quit(__VA_ARGS__)
#define SDL_QuitSubSystem(...) FSDLDynamicLoader::Get().pSDL_QuitSubSystem(__VA_ARGS__)
#define SDL_WasInit(...) FSDLDynamicLoader::Get().pSDL_WasInit(__VA_ARGS__)

#define SDL_AddEventWatch(...) FSDLDynamicLoader::Get().pSDL_AddEventWatch(__VA_ARGS__)
#define SDL_RemoveEventWatch(...) FSDLDynamicLoader::Get().pSDL_RemoveEventWatch(__VA_ARGS__)
#define SDL_PollEvent(...) FSDLDynamicLoader::Get().pSDL_PollEvent(__VA_ARGS__)

#define SDL_GetJoysticks(...) FSDLDynamicLoader::Get().pSDL_GetJoysticks(__VA_ARGS__)
#define SDL_OpenJoystick(...) FSDLDynamicLoader::Get().pSDL_OpenJoystick(__VA_ARGS__)
#define SDL_CloseJoystick(...) FSDLDynamicLoader::Get().pSDL_CloseJoystick(__VA_ARGS__)
#define SDL_GetJoystickVendor(...) FSDLDynamicLoader::Get().pSDL_GetJoystickVendor(__VA_ARGS__)
#define SDL_GetJoystickProduct(...) FSDLDynamicLoader::Get().pSDL_GetJoystickProduct(__VA_ARGS__)
#define SDL_GetJoystickProductVersion(...) FSDLDynamicLoader::Get().pSDL_GetJoystickProductVersion(__VA_ARGS__)
#define SDL_GetJoystickGUID(...) FSDLDynamicLoader::Get().pSDL_GetJoystickGUID(__VA_ARGS__)
#define SDL_GetJoystickType(...) FSDLDynamicLoader::Get().pSDL_GetJoystickType(__VA_ARGS__)
#define SDL_GetJoystickPowerInfo(...) FSDLDynamicLoader::Get().pSDL_GetJoystickPowerInfo(__VA_ARGS__)
#define SDL_GetJoystickName(...) FSDLDynamicLoader::Get().pSDL_GetJoystickName(__VA_ARGS__)
#define SDL_GetJoystickSerial(...) FSDLDynamicLoader::Get().pSDL_GetJoystickSerial(__VA_ARGS__)
#define SDL_GetJoystickProperties(...) FSDLDynamicLoader::Get().pSDL_GetJoystickProperties(__VA_ARGS__)
#define SDL_GetJoystickFirmwareVersion(...) FSDLDynamicLoader::Get().pSDL_GetJoystickFirmwareVersion(__VA_ARGS__)
#define SDL_GetJoystickPath(...) FSDLDynamicLoader::Get().pSDL_GetJoystickPath(__VA_ARGS__)
#define SDL_GetNumJoystickAxes(...) FSDLDynamicLoader::Get().pSDL_GetNumJoystickAxes(__VA_ARGS__)
#define SDL_GetNumJoystickButtons(...) FSDLDynamicLoader::Get().pSDL_GetNumJoystickButtons(__VA_ARGS__)
#define SDL_GetNumJoystickHats(...) FSDLDynamicLoader::Get().pSDL_GetNumJoystickHats(__VA_ARGS__)
#define SDL_GetNumJoystickBalls(...) FSDLDynamicLoader::Get().pSDL_GetNumJoystickBalls(__VA_ARGS__)
#define SDL_GetJoystickAxis(...) FSDLDynamicLoader::Get().pSDL_GetJoystickAxis(__VA_ARGS__)
#define SDL_GetJoystickAxisInitialState(...) FSDLDynamicLoader::Get().pSDL_GetJoystickAxisInitialState(__VA_ARGS__)
#define SDL_SetJoystickLED(...) FSDLDynamicLoader::Get().pSDL_SetJoystickLED(__VA_ARGS__)
#define SDL_IsJoystickHaptic(...) FSDLDynamicLoader::Get().pSDL_IsJoystickHaptic(__VA_ARGS__)

#define SDL_IsGamepad(...) FSDLDynamicLoader::Get().pSDL_IsGamepad(__VA_ARGS__)
#define SDL_OpenGamepad(...) FSDLDynamicLoader::Get().pSDL_OpenGamepad(__VA_ARGS__)
#define SDL_CloseGamepad(...) FSDLDynamicLoader::Get().pSDL_CloseGamepad(__VA_ARGS__)
#define SDL_GamepadHasSensor(...) FSDLDynamicLoader::Get().pSDL_GamepadHasSensor(__VA_ARGS__)
#define SDL_SetGamepadSensorEnabled(...) FSDLDynamicLoader::Get().pSDL_SetGamepadSensorEnabled(__VA_ARGS__)
#define SDL_AddGamepadMappingsFromFile(...) FSDLDynamicLoader::Get().pSDL_AddGamepadMappingsFromFile(__VA_ARGS__)

#define SDL_OpenHapticFromJoystick(...) FSDLDynamicLoader::Get().pSDL_OpenHapticFromJoystick(__VA_ARGS__)
#define SDL_CloseHaptic(...) FSDLDynamicLoader::Get().pSDL_CloseHaptic(__VA_ARGS__)
#define SDL_GetNumHapticAxes(...) FSDLDynamicLoader::Get().pSDL_GetNumHapticAxes(__VA_ARGS__)
#define SDL_HapticRumbleSupported(...) FSDLDynamicLoader::Get().pSDL_HapticRumbleSupported(__VA_ARGS__)
#define SDL_InitHapticRumble(...) FSDLDynamicLoader::Get().pSDL_InitHapticRumble(__VA_ARGS__)
#define SDL_GetHapticFeatures(...) FSDLDynamicLoader::Get().pSDL_GetHapticFeatures(__VA_ARGS__)
#define SDL_SetHapticAutocenter(...) FSDLDynamicLoader::Get().pSDL_SetHapticAutocenter(__VA_ARGS__)
#define SDL_SetHapticGain(...) FSDLDynamicLoader::Get().pSDL_SetHapticGain(__VA_ARGS__)
#define SDL_PauseHaptic(...) FSDLDynamicLoader::Get().pSDL_PauseHaptic(__VA_ARGS__)
#define SDL_ResumeHaptic(...) FSDLDynamicLoader::Get().pSDL_ResumeHaptic(__VA_ARGS__)
#define SDL_StopHapticEffects(...) FSDLDynamicLoader::Get().pSDL_StopHapticEffects(__VA_ARGS__)
#define SDL_GetMaxHapticEffects(...) FSDLDynamicLoader::Get().pSDL_GetMaxHapticEffects(__VA_ARGS__)
#define SDL_GetMaxHapticEffectsPlaying(...) FSDLDynamicLoader::Get().pSDL_GetMaxHapticEffectsPlaying(__VA_ARGS__)
#define SDL_GetHapticEffectStatus(...) FSDLDynamicLoader::Get().pSDL_GetHapticEffectStatus(__VA_ARGS__)
#define SDL_CreateHapticEffect(...) FSDLDynamicLoader::Get().pSDL_CreateHapticEffect(__VA_ARGS__)
#define SDL_UpdateHapticEffect(...) FSDLDynamicLoader::Get().pSDL_UpdateHapticEffect(__VA_ARGS__)
#define SDL_RunHapticEffect(...) FSDLDynamicLoader::Get().pSDL_RunHapticEffect(__VA_ARGS__)
#define SDL_StopHapticEffect(...) FSDLDynamicLoader::Get().pSDL_StopHapticEffect(__VA_ARGS__)
#define SDL_DestroyHapticEffect(...) FSDLDynamicLoader::Get().pSDL_DestroyHapticEffect(__VA_ARGS__)
#define SDL_PlayHapticRumble(...) FSDLDynamicLoader::Get().pSDL_PlayHapticRumble(__VA_ARGS__)
#define SDL_StopHapticRumble(...) FSDLDynamicLoader::Get().pSDL_StopHapticRumble(__VA_ARGS__)

#define SDL_RumbleJoystick(...) FSDLDynamicLoader::Get().pSDL_RumbleJoystick(__VA_ARGS__)
#define SDL_RumbleJoystickTriggers(...) FSDLDynamicLoader::Get().pSDL_RumbleJoystickTriggers(__VA_ARGS__)

#define SDL_GetBooleanProperty(...) FSDLDynamicLoader::Get().pSDL_GetBooleanProperty(__VA_ARGS__)

#define SDL_GetError(...) FSDLDynamicLoader::Get().pSDL_GetError(__VA_ARGS__)

#define SDL_GetTicks(...) FSDLDynamicLoader::Get().pSDL_GetTicks(__VA_ARGS__)

#define SDL_memset(...) FSDLDynamicLoader::Get().pSDL_memset(__VA_ARGS__)

#define SDL_hid_enumerate(...) FSDLDynamicLoader::Get().pSDL_hid_enumerate(__VA_ARGS__)
#define SDL_hid_free_enumeration(...) FSDLDynamicLoader::Get().pSDL_hid_free_enumeration(__VA_ARGS__)
#define SDL_hid_open(...) FSDLDynamicLoader::Get().pSDL_hid_open(__VA_ARGS__)
#define SDL_hid_open_path(...) FSDLDynamicLoader::Get().pSDL_hid_open_path(__VA_ARGS__)
#define SDL_hid_close(...) FSDLDynamicLoader::Get().pSDL_hid_close(__VA_ARGS__)
#define SDL_hid_read(...) FSDLDynamicLoader::Get().pSDL_hid_read(__VA_ARGS__)
#define SDL_hid_read_timeout(...) FSDLDynamicLoader::Get().pSDL_hid_read_timeout(__VA_ARGS__)
#define SDL_hid_get_feature_report(...) FSDLDynamicLoader::Get().pSDL_hid_get_feature_report(__VA_ARGS__)
#define SDL_hid_write(...) FSDLDynamicLoader::Get().pSDL_hid_write(__VA_ARGS__)
#define SDL_hid_send_feature_report(...) FSDLDynamicLoader::Get().pSDL_hid_send_feature_report(__VA_ARGS__)
