// JoystickPlugin is licensed under the MIT License.
// Copyright Jayden Maalouf 2026. All Rights Reserved.

#pragma once

#include "Data/JoystickDeviceState.h"
#include "Input/Data/JoystickInputDebugTextRow.h"

struct FJoystickInputDebugDeviceBlock
{
	FString Name;
	FJoystickDeviceState State;
	TArray<FJoystickInputDebugTextRow> TextRows;
};
