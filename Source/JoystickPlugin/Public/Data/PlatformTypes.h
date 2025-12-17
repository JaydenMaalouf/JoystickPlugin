// JoystickPlugin is licensed under the MIT License.
// Copyright Jayden Maalouf. All Rights Reserved.

#pragma once

#include "Misc/CoreMiscDefines.h"

#if ENGINE_MAJOR_VERSION == 4 || ENGINE_MAJOR_VERSION == 5 && ENGINE_MINOR_VERSION <= 1
using FInputDeviceId = int;
#endif
