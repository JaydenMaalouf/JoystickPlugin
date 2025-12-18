// JoystickPlugin is licensed under the MIT License.
// Copyright Jayden Maalouf. All Rights Reserved.

#pragma once

#include "Modules/ModuleInterface.h"

class FJoystickPluginEditorModule : public IModuleInterface
{
public:
	virtual void ShutdownModule() override;
	void RegisterPropertyLayout() const;
	virtual void StartupModule() override;

private:
	void RegisterMenus() const;
	void RegisterSettings() const;
	void UnregisterSettings() const;
};
