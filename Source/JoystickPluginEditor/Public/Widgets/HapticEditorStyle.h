// JoystickPlugin is licensed under the MIT License.
// Copyright Jayden Maalouf 2026. All Rights Reserved.

#pragma once

#include "Math/Color.h"
#include "Styling/SlateColor.h"

// Reuses the same active/inactive tint convention as AxisBar/ButtonBox/HatSwitch/BallSwitch: the
// same bright green (0.2,1.0,0.2) AxisBar uses for its full-strength positive fill for the
// active/highlighted state, dimmed (0.7,0.7,0.7) text otherwise. The inactive background is
// transparent rather than a flat gray box, since segments now sit inside a shared pill container
// that already provides their background.
// Uses SButton+SBorder(WhiteBrush) rather than SCheckBox's ToggleButtonCheckbox style, since that
// style's checked/unchecked brushes have their own baked-in accent color that BorderBackgroundColor
// only tints rather than replaces.
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
