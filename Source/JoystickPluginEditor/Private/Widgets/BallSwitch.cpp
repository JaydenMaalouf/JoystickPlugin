// JoystickPlugin is licensed under the MIT License.
// Copyright Jayden Maalouf. All Rights Reserved.

#include "Widgets/BallSwitch.h"

#include "Widgets/CircleWidget.h"

void SBallSwitch::Construct(const FArguments& InArgs)
{
	Value = InArgs._Value;
	BallIndex = InArgs._BallIndex;
	DisplayName = InArgs._DisplayName;

	BallText = FText::FromString(FString::Format(TEXT("Ball {0}\n{1}"), {BallIndex.Get(), DisplayName.Get().ToString()}).TrimStartAndEnd());

	SetToolTipText(BallText);

	ChildSlot
	[
		SNew(SOverlay)

		+ SOverlay::Slot()
		[
			SNew(SBox)
			.WidthOverride(100.f)
			.HeightOverride(100.f)
			[
				SNew(SBorder)
				.BorderBackgroundColor(FSlateColor(FLinearColor::Black))
			]
		]

		+ SOverlay::Slot()
		[
			SAssignNew(Canvas, SConstraintCanvas)

			+ SConstraintCanvas::Slot()
			.Anchors(FAnchors(0.5f, 0.5f))
			.Alignment(FVector2D(0.5f, 0.5f))
			.AutoSize(true)
			.Expose(BallSlot)
			[
				SNew(SCircleWidget)
				.Radius(10.0f)
				.Color(FLinearColor::Green)
			]
		]
	];
}

void SBallSwitch::SetValue(const FVector2D& InValue)
{
	Value.Set(InValue);
}

void SBallSwitch::UpdateBallPosition() const
{
	if (!BallSlot)
	{
		return;
	}

	const FVector2D BallSize(20.f, 20.f);
	BallSlot->SetOffset(FMargin(
		Value.Get().X,
		Value.Get().Y,
		BallSize.X,
		BallSize.Y
	));
}
