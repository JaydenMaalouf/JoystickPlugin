// JoystickPlugin is licensed under the MIT License.
// Copyright Jayden Maalouf 2026. All Rights Reserved.

#pragma once

#include "Commands/JoystickDebugCommands.h"
#include "Debug/JoystickDebugDrawer.h"
#include "Modules/ModuleInterface.h"

class FJoystickPluginDeveloperModule : public IModuleInterface
{
public:
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;

private:
	FJoystickDebugDrawer DebugDrawer;
	TUniquePtr<FJoystickDebugCommands> DebugCommands;
};
