// JoystickPlugin is licensed under the MIT License.
// Copyright Jayden Maalouf. All Rights Reserved.

#include "Widgets/HatSwitch.h"

#include "Widgets/CircleWidget.h"
#include "Styling/AppStyle.h"
#include "Widgets/Layout/SBorder.h"
#include "Widgets/Text/STextBlock.h"

void SHatSwitch::Construct(const FArguments& InArgs)
{
	Value = InArgs._Value;
	HatIndex = InArgs._HatIndex;
	DisplayName = InArgs._DisplayName;

	HatText = FText::Format(
		FText::FromString(TEXT("Hat {0}\n{1}")),
		HatIndex.Get(),
		DisplayName.Get()
	);

	SetToolTipText(HatText);

	ChildSlot
	[
		SNew(SVerticalBox)
		+ SVerticalBox::Slot()
		.AutoHeight()
		.HAlign(HAlign_Center)
		.Padding(0, 0, 0, 4)
		[
			SNew(STextBlock)
			.Text(DisplayName.Get())
			.TextStyle(FAppStyle::Get(), "SmallText")
			.Justification(ETextJustify::Center)
			.ColorAndOpacity(FLinearColor::White)
		]
		+ SVerticalBox::Slot()
		.AutoHeight()
		.HAlign(HAlign_Center)
		[
			SNew(SBorder)
			.BorderImage(FAppStyle::GetBrush("ToolPanel.GroupBorder"))
			.Padding(8.0f)
			[
				SNew(SBox)
				.MinDesiredWidth(120.f)
				.MinDesiredHeight(120.f)
				.WidthOverride(120.f)
				.HeightOverride(120.f)
				[
					SAssignNew(OverlayContainer, SOverlay)
					
					// Outer Circle Background
					+ SOverlay::Slot()
					[
						SNew(SCircleWidget)
						.Radius(56.0f)
						.Color(FLinearColor(0.15f, 0.15f, 0.15f, 1.0f))
						.Filled(true)
					]
					
					// Inner Circle Track
					+ SOverlay::Slot()
					[
						SNew(SCircleWidget)
						.Radius(50.0f)
						.Color(FLinearColor(0.25f, 0.25f, 0.25f, 1.0f))
						.Filled(true)
					]
					
					// Center Indicator
					+ SOverlay::Slot()
					.HAlign(HAlign_Center)
					.VAlign(VAlign_Center)
					[
						SNew(SCircleWidget)
						.Radius(3.0f)
						.Color(FLinearColor(0.5f, 0.5f, 0.5f, 1.0f))
						.Filled(true)
					]

					// Ball Indicator
					+ SOverlay::Slot()
					[
						SAssignNew(Canvas, SConstraintCanvas)

						+ SConstraintCanvas::Slot()
						.Anchors(FAnchors(0.5f, 0.5f))
						.Alignment(FVector2D(0.5f, 0.5f))
						.AutoSize(true)
						.Expose(BallSlot)
						[
							SAssignNew(BallWidget, SCircleWidget)
							.Radius(12.0f)
							.Color(GetBallColor())
							.Filled(true)
						]
					]
				]
			]
		]
	];
}

void SHatSwitch::SetValue(const EHatDirection InValue)
{
	Value.Set(InValue);
	UpdateBallPosition();
	
	// Update ball color
	if (BallWidget.IsValid())
	{
		BallWidget->SetColor(TAttribute<FLinearColor>(this, &SHatSwitch::GetBallColor));
		Invalidate(EInvalidateWidget::Paint);
	}
}

FLinearColor SHatSwitch::GetBallColor() const
{
	const EHatDirection Direction = Value.Get();
	return Direction != EHatDirection::None
		? FLinearColor(0.2f, 0.9f, 0.3f, 1.0f)  // Bright green when active
		: FLinearColor(0.4f, 0.4f, 0.4f, 1.0f);  // Gray when centered
}

void SHatSwitch::UpdateBallPosition() const
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
	const FVector2D CanvasSize = OverlayContainer->GetCachedGeometry().GetLocalSize();
	const FVector2D Radius = CanvasSize * 0.5f;
	const FVector2D FinalBallPosition = BallOffset * (Radius - (BallSize * 0.5f));

	BallSlot->SetOffset(FMargin(
		FinalBallPosition.X,
		FinalBallPosition.Y,
		BallSize.X,
		BallSize.Y
	));
}
