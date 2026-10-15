// JoystickPlugin is licensed under the MIT License.
// Copyright Jayden Maalouf 2026. All Rights Reserved.

#include "Widgets/JoystickInput/BallSwitch.h"

#include "Widgets/JoystickInput/CircleWidget.h"
#include "Styling/AppStyle.h"
#include "Styling/CoreStyle.h"
#include "Widgets/Layout/SBorder.h"
#include "Widgets/Text/STextBlock.h"

void SBallSwitch::Construct(const FArguments& InArgs)
{
	Style = InArgs._Style;
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
			.Text_Lambda([this]
			{
				const FText Name = DisplayName.Get();
				return Name.IsEmpty()
					       ? FText::Format(FText::FromString("Ball {0}"), BallIndex.Get())
					       : Name;
			})
			.TextStyle(FAppStyle::Get(), "SmallText")
			.Justification(ETextJustify::Center)
			.ColorAndOpacity(Style->LabelColor)
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
						.BorderBackgroundColor(Style->BackgroundColor)
					]

					// Center Indicator
					+ SOverlay::Slot()
					.HAlign(HAlign_Center)
					.VAlign(VAlign_Center)
					[
						SNew(SCircleWidget)
						.Radius(2.0f)
						.Color(Style->CentreDotColor)
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
	return Value.Get().Size() > 0.01f ? Style->ActiveBallColor : Style->CentredBallColor;
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
