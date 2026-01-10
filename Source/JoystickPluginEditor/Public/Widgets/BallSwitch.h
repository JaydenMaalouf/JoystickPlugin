// JoystickPlugin is licensed under the MIT License.
// Copyright Jayden Maalouf 2026. All Rights Reserved.

#pragma once

#include "CircleWidget.h"
#include "Widgets/SCompoundWidget.h"
#include "Widgets/Layout/SConstraintCanvas.h"

class SBallSwitch : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SBallSwitch)
		{
		}

		SLATE_ATTRIBUTE(FVector2D, Value)
		SLATE_ATTRIBUTE(int, BallIndex)
		SLATE_ATTRIBUTE(FText, DisplayName)
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs);

	void SetValue(const FVector2D& InValue);
	void UpdateBallPosition() const;

private:
	TAttribute<FVector2D> Value;
	TAttribute<int> BallIndex;
	TAttribute<FText> DisplayName;
	TSharedPtr<SConstraintCanvas> Canvas;
	SConstraintCanvas::FSlot* BallSlot = nullptr;
	TSharedPtr<SCircleWidget> BallWidget;

	FText BallText;

	FLinearColor GetBallColor() const;
};
