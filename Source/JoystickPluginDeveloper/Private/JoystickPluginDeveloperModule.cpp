// JoystickPlugin is licensed under the MIT License.
// Copyright Jayden Maalouf 2026. All Rights Reserved.

#include "JoystickPluginDeveloperModule.h"

#include "ForceFeedback/ForceFeedbackDebugOverlay.h"
#include "Input/JoystickInputDebugOverlay.h"
#include "Modules/ModuleManager.h"

void FJoystickPluginDeveloperModule::StartupModule()
{
	DebugDrawer.AddOverlay(MakeUnique<FJoystickInputDebugOverlay>());
	DebugDrawer.AddOverlay(MakeUnique<FForceFeedbackDebugOverlay>());
	DebugDrawer.Register();

	DebugCommands = MakeUnique<FJoystickDebugCommands>();
}

void FJoystickPluginDeveloperModule::ShutdownModule()
{
	DebugCommands.Reset();
	DebugDrawer.Unregister();
}

IMPLEMENT_MODULE(FJoystickPluginDeveloperModule, JoystickPluginDeveloper)
