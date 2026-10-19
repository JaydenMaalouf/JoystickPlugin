// JoystickPlugin is licensed under the MIT License.
// Copyright Jayden Maalouf 2026. All Rights Reserved.

#include "ForceFeedback/Debug/ForceFeedbackDebug.h"

bool FForceFeedbackDebug::IsEnabled()
{
#if UE_BUILD_SHIPPING
	return false;
#else
	return CVarEnabled.GetValueOnAnyThread();
#endif
}

#if !UE_BUILD_SHIPPING
TAutoConsoleVariable<bool> FForceFeedbackDebug::CVarEnabled(TEXT("Joystick.FFB.Debug"), false, TEXT("Shows what every force feedback effect is sending to each device."), ECVF_Default);
#endif
