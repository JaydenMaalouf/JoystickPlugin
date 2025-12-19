// JoystickPlugin is licensed under the MIT License.
// Copyright Jayden Maalouf. All Rights Reserved.

#pragma once

#include "Modules/ModuleInterface.h"

class FJoystickPluginEditorModule : public IModuleInterface
{
public:
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;

private:
	void RegisterMenus() const;
	void RegisterPropertyLayout() const;
	void RegisterSettings() const;
	void UnregisterSettings() const;
};
