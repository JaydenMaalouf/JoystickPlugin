// JoystickPlugin is licensed under the MIT License.
// Copyright Jayden Maalouf 2026. All Rights Reserved.

#pragma once

#include "CircleWidget.h"
#include "Data/Input/HatDirection.h"
#include "Widgets/SCompoundWidget.h"
#include "Widgets/Layout/SConstraintCanvas.h"

class SHatSwitch : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SHatSwitch)
		{
		}

		SLATE_ATTRIBUTE(EHatDirection, Value)
		SLATE_ATTRIBUTE(int, HatIndex)
		SLATE_ATTRIBUTE(FText, DisplayName)
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs);

	void SetValue(const EHatDirection InValue);
	void UpdateBallPosition() const;

private:
	TAttribute<EHatDirection> Value;
	TAttribute<int> HatIndex;
	TAttribute<FText> DisplayName;
	TSharedPtr<SConstraintCanvas> Canvas;
	SConstraintCanvas::FSlot* BallSlot = nullptr;
	TSharedPtr<SCircleWidget> BallWidget;
	TSharedPtr<SOverlay> OverlayContainer;

	FText HatText;

	FLinearColor GetBallColor() const;
};
