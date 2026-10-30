// JoystickPlugin is licensed under the MIT License.
// Copyright Jayden Maalouf 2026. All Rights Reserved.

#pragma once

#include "Modules/ModuleManager.h"
#include "IInputDeviceModule.h"

/** The public interface to this module */
class IJoystickPlugin : public IInputDeviceModule
{
public:
	/** Returns the module instance, loading it on demand. Beware of calling this during shutdown. */
	static IJoystickPlugin& Get()
	{
		return FModuleManager::LoadModuleChecked<IJoystickPlugin>("JoystickPlugin");
	}

	/** Whether the module is loaded and ready. Get() is only valid if this returns true. */
	static bool IsAvailable()
	{
		return FModuleManager::Get().IsModuleLoaded("JoystickPlugin");
	}
};
