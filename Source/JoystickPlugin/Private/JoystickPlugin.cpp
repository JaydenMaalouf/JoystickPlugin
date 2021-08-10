/*
*
* Copyright (C) <2014> samiljan <Sam Persson>, tsky <thomas.kollakowksy@w-hs.de>
* All rights reserved.
*
* This software may be modified and distributed under the terms
* of the BSD license.  See the LICENSE file for details.
*/

#include "JoystickPlugin.h"

#if WITH_EDITOR
	#include "InputSettingsCustomization.h"
#endif

#define LOCTEXT_NAMESPACE "JoystickPlugin"

TSharedPtr<class IInputDevice> FJoystickPlugin::CreateInputDevice(const TSharedRef<FGenericApplicationMessageHandler>& InMessageHandler)
{
	JoystickDevice = MakeShareable(new JoystickDeviceManager(InMessageHandler));
	return JoystickDevice;
}

void FJoystickPlugin::StartupModule()
{
	IJoystickPlugin::StartupModule();

#if WITH_EDITOR
	// Replace parts of the input settings widget to make them wide enough to fit long joystick names
	FInputActionMappingCustomizationExtended::Register();
	FInputAxisMappingCustomizationExtended::Register();
#endif

}

void FJoystickPlugin::ShutdownModule()
{
	IJoystickPlugin::ShutdownModule();

	JoystickDevice = nullptr;
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FJoystickPlugin, JoystickPlugin)
