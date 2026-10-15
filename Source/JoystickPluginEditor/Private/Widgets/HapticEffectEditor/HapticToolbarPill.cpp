// JoystickPlugin is licensed under the MIT License.
// Copyright Jayden Maalouf 2026. All Rights Reserved.

#include "Widgets/HapticEffectEditor/HapticToolbarPill.h"

#include "Styling/AppStyle.h"
#include "Widgets/Layout/SBorder.h"

void SHapticToolbarPill::Construct(const FArguments& InArgs)
{
	ChildSlot
	[
		SNew(SBorder)
		.BorderImage(FAppStyle::GetBrush("ToolPanel.GroupBorder"))
		.Padding(FMargin(8.0f, 6.0f))
		[
			InArgs
			._Content
			.Widget
		]
	];
}
