// JoystickPlugin is licensed under the MIT License.
// Copyright Jayden Maalouf 2026. All Rights Reserved.

#pragma once

#include "Widgets/SCompoundWidget.h"

// Wraps one of the Haptic Effect Editor's toolbar controls in a bordered "pill" container.
class SHapticToolbarPill : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SHapticToolbarPill)
		{
		}

		SLATE_DEFAULT_SLOT(FArguments, Content)
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs);
};
