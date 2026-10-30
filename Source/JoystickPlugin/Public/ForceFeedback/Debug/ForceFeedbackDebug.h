// JoystickPlugin is licensed under the MIT License.
// Copyright Jayden Maalouf 2026. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

#if !UE_BUILD_SHIPPING
#include "HAL/IConsoleManager.h"
#endif

// Toggled with Joystick.FFB.Debug. Effects only record debug values while it is on; the overlay that draws them lives in the JoystickPluginDeveloper module.
class JOYSTICKPLUGIN_API FForceFeedbackDebug
{
public:
	static bool IsEnabled();

private:
#if !UE_BUILD_SHIPPING
	static TAutoConsoleVariable<bool> CVarEnabled;
#endif
};
