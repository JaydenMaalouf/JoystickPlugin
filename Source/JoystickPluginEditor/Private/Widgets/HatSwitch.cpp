// JoystickPlugin is licensed under the MIT License.
// Copyright Jayden Maalouf. All Rights Reserved.

#include "Widgets/HatSwitch.h"

#include "Widgets/CircleWidget.h"

void SHatSwitch::Construct(const FArguments& InArgs)
{
	Value = InArgs._Value;
	HatIndex = InArgs._HatIndex;
	DisplayName = InArgs._DisplayName;

	HatText = FText::FromString(FString::Format(TEXT("Hat {0}\n{1}"), {HatIndex.Get(), DisplayName.Get().ToString()}).TrimStartAndEnd());

	SetToolTipText(HatText);

	ChildSlot
	[
		SNew(SOverlay)

		+ SOverlay::Slot()
		[
			SNew(SBox)
			.WidthOverride(100.f)
			.HeightOverride(100.f)
			[
				SNew(SCircleWidget)
				.Radius(50.0f)
				.Color(FLinearColor::Black)
				.Filled(true)
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

void SHatSwitch::SetValue(const EHatDirection InValue)
{
	Value.Set(InValue);
	UpdateBallPosition();
}

void SHatSwitch::UpdateBallPosition()
{
	if (!BallSlot)
	{
		return;
	}


	FVector2D BallOffset = FVector2D::ZeroVector;

	const EHatDirection Direction = Value.Get();

	if (EnumHasAnyFlags(Direction, EHatDirection::Up)) { BallOffset.Y -= 1; }
	if (EnumHasAnyFlags(Direction, EHatDirection::Right_Up))
	{
		BallOffset.X += 1;
		BallOffset.Y -= 1;
	}
	if (EnumHasAnyFlags(Direction, EHatDirection::Right)) { BallOffset.X += 1; }
	if (EnumHasAnyFlags(Direction, EHatDirection::Right_Down))
	{
		BallOffset.X += 1;
		BallOffset.Y += 1;
	}
	if (EnumHasAnyFlags(Direction, EHatDirection::Down)) { BallOffset.Y += 1; }
	if (EnumHasAnyFlags(Direction, EHatDirection::Left_Down))
	{
		BallOffset.X -= 1;
		BallOffset.Y += 1;
	}
	if (EnumHasAnyFlags(Direction, EHatDirection::Left)) { BallOffset.X -= 1; }
	if (EnumHasAnyFlags(Direction, EHatDirection::Left_Up))
	{
		BallOffset.X -= 1;
		BallOffset.Y -= 1;
	}

	if (!BallOffset.IsZero())
	{
		BallOffset.Normalize();
	}

	const FVector2D BallSize(20.f, 20.f);
	const FVector2D CanvasSize = GetCachedGeometry().GetLocalSize();
	const FVector2D Radius = CanvasSize * 0.5f;
	const FVector2D FinalBallPosition = BallOffset * (Radius - (BallSize * 0.5f));

	BallSlot->SetOffset(FMargin(
		FinalBallPosition.X,
		FinalBallPosition.Y,
		BallSize.X,
		BallSize.Y
	));
}
