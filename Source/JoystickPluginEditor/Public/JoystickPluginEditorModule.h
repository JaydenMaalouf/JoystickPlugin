// JoystickPlugin is licensed under the MIT License.
// Copyright Jayden Maalouf. All Rights Reserved.

#pragma once

#include "Modules/ModuleInterface.h"

class FJoystickPluginEditorModule : public IModuleInterface
{
public:
	virtual void ShutdownModule() override;
	virtual void StartupModule() override;

private:
	void RegisterSettings() const;
	void UnregisterSettings() const;
};
