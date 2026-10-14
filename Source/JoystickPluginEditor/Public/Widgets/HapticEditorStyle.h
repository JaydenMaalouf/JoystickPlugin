// JoystickPlugin is licensed under the MIT License.
// Copyright Jayden Maalouf 2026. All Rights Reserved.

#pragma once

#include "Math/Color.h"
#include "Styling/SlateColor.h"

// Active/inactive toggle tints, matching AxisBar/ButtonBox/HatSwitch/BallSwitch.
struct FHapticEffectEditorColors
{
	static FSlateColor GetToggleBackgroundColor(const bool bIsActive)
	{
		return bIsActive ? FSlateColor(FLinearColor(0.2f, 1.0f, 0.2f, 1.0f)) : FSlateColor(FLinearColor::Transparent);
	}

	static FSlateColor GetToggleForegroundColor(const bool bIsActive)
	{
		return bIsActive ? FSlateColor(FLinearColor::White) : FSlateColor(FLinearColor(0.7f, 0.7f, 0.7f, 1.0f));
	}
};
