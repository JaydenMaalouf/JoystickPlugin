// JoystickPlugin is licensed under the MIT License.
// Copyright Jayden Maalouf 2026. All Rights Reserved.

#pragma once

#include "Modules/ModuleInterface.h"
#include "Runtime/Launch/Resources/Version.h"
#include "Notifications/JoystickDevicePairingNotifier.h"

class FJoystickPluginEditorModule : public IModuleInterface
{
public:
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;

private:
	void RegisterMenus() const;
	void RegisterSettings() const;
	void RegisterPropertyLayout() const;
	void UnregisterSettings() const;
	void UnregisterPropertyLayout() const;

#if ENGINE_MAJOR_VERSION == 5 && ENGINE_MINOR_VERSION >= 1
	FJoystickDevicePairingNotifier DevicePairingNotifier;
#endif
};
