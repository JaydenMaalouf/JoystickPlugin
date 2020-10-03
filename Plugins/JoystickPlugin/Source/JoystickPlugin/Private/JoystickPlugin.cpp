/*
*
* Copyright (C) <2014> samiljan <Sam Persson>, tsky <thomas.kollakowksy@w-hs.de>
* All rights reserved.
*
* This software may be modified and distributed under the terms
* of the BSD license.  See the LICENSE file for details.
*/

#include "JoystickPlugin.h"

#include "Core.h"
#include <Engine.h>

#if WITH_EDITOR
	#include "InputSettingsCustomization.h"
#endif

IMPLEMENT_MODULE(FJoystickPlugin, JoystickPlugin)

#define LOCTEXT_NAMESPACE "JoystickPlugin"


void FJoystickPlugin::StartupModule()
{
	IJoystickPlugin::StartupModule();

	// Get the base directory of this plugin
	FString BaseDir = "../../Plugins/JoystickPlugin/";
	// Add on the relative location of the third party dll and load it
	FString LibraryPath;
#if PLATFORM_WINDOWS
	LibraryPath = FPaths::Combine(*BaseDir, TEXT("Binaries/Win64/SDL2.dll"));	
#endif // PLATFORM_WINDOWS

	SDL2LibraryHandle = !LibraryPath.IsEmpty() ? FPlatformProcess::GetDllHandle(*LibraryPath) : nullptr;

	if (SDL2LibraryHandle == nullptr)
	{
		UE_LOG(JoystickPluginLog, Error, TEXT("ThirdPartyLibraryError - Failed load sdl2.dll - %s"), *LibraryPath);
		//FMessageDialog::Open(EAppMsgType::Ok, LOCTEXT("ThirdPartyLibraryError", "Failed load sdl2.dll"));
#if PLATFORM_WINDOWS
		LibraryPath = FPaths::Combine(*BaseDir, TEXT("SDL2.dll"));
#endif // PLATFORM_WINDOWS
		SDL2LibraryHandle = !LibraryPath.IsEmpty() ? FPlatformProcess::GetDllHandle(*LibraryPath) : nullptr;
		if (SDL2LibraryHandle == nullptr) {
			UE_LOG(JoystickPluginLog, Error, TEXT("ThirdPartyLibraryError - Failed load sdl2.dll - %s"), *LibraryPath);
			FMessageDialog::Open(EAppMsgType::Ok, LOCTEXT("ThirdPartyLibraryError", "Failed load sdl2.dll"));
			exit(99);
		}
	}

	JoystickDevice = MakeShareable(new ::FJoystickDevice());
	//return;

#if WITH_EDITOR
	// Replace parts of the input settings widget to make them wide enough to fit long joystick names
	FInputActionMappingCustomizationExtended::Register();
	FInputAxisMappingCustomizationExtended::Register();
#endif
}

#undef LOCTEXT_NAMESPACE
