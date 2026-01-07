// JoystickPlugin is licensed under the MIT License.
// Copyright Jayden Maalouf. All Rights Reserved.

#include "Widgets/BallSwitch.h"

#include "Widgets/CircleWidget.h"
#include "Styling/AppStyle.h"
#include "Styling/CoreStyle.h"
#include "Widgets/Layout/SBorder.h"
#include "Widgets/Text/STextBlock.h"

void SBallSwitch::Construct(const FArguments& InArgs)
{
	Value = InArgs._Value;
	BallIndex = InArgs._BallIndex;
	DisplayName = InArgs._DisplayName;

	BallText = FText::Format(
		FText::FromString(TEXT("Ball {0}\n{1}")),
		BallIndex.Get(),
		DisplayName.Get()
	);

	SetToolTipText(BallText);

	ChildSlot
	[
		SNew(SVerticalBox)
		+ SVerticalBox::Slot()
		.AutoHeight()
		.HAlign(HAlign_Center)
		.Padding(0, 0, 0, 4)
		[
			SNew(STextBlock)
			.Text_Lambda([this]()
			{
				const FText Name = DisplayName.Get();
				return Name.IsEmpty() 
					? FText::Format(FText::FromString("Ball {0}"), BallIndex.Get())
					: Name;
			})
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
					SNew(SOverlay)
					
					// Background
					+ SOverlay::Slot()
					[
						SNew(SBorder)
						.BorderImage(FCoreStyle::Get().GetBrush("WhiteBrush"))
						.BorderBackgroundColor(FLinearColor(0.15f, 0.15f, 0.15f, 1.0f))
					]
					
					// Center Indicator
					+ SOverlay::Slot()
					.HAlign(HAlign_Center)
					.VAlign(VAlign_Center)
					[
						SNew(SCircleWidget)
						.Radius(2.0f)
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
							.Radius(8.0f)
							.Color(GetBallColor())
							.Filled(true)
						]
					]
				]
			]
		]
	];
}

void SBallSwitch::SetValue(const FVector2D& InValue)
{
	Value.Set(InValue);
	UpdateBallPosition();
	
	// Update ball color
	if (BallWidget.IsValid())
	{
		BallWidget->SetColor(TAttribute<FLinearColor>(this, &SBallSwitch::GetBallColor));
		Invalidate(EInvalidateWidget::Paint);
	}
}

FLinearColor SBallSwitch::GetBallColor() const
{
	const FVector2D Val = Value.Get();
	const float Magnitude = Val.Size();
	return Magnitude > 0.01f
		? FLinearColor(0.2f, 0.9f, 0.3f, 1.0f)  // Bright green when active
		: FLinearColor(0.4f, 0.4f, 0.4f, 1.0f);  // Gray when centered
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
