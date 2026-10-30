// JoystickPlugin is licensed under the MIT License.
// Copyright Jayden Maalouf 2026. All Rights Reserved.

#pragma once

#include "HAL/Platform.h"

class UFont;

// Tracks the debug font, so layouts measured from it are only rebuilt when it changes.
struct FJoystickDebugFontKey
{
	// Returns true when the font differs from the one last seen.
	bool Update();

	const UFont* Font = nullptr;
	int32 LegacyFontSize = 0;
};
