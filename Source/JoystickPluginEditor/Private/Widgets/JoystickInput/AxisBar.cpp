// JoystickPlugin is licensed under the MIT License.
// Copyright Jayden Maalouf 2026. All Rights Reserved.

#include "Widgets/JoystickInput/AxisBar.h"

#include "Rendering/DrawElements.h"
#include "Widgets/Layout/SScaleBox.h"
#include "Styling/AppStyle.h"
#include "Styling/CoreStyle.h"
#include "Widgets/Layout/SBorder.h"
#include "Widgets/Input/SButton.h"

void SAxisBar::Construct(const FArguments& InArgs)
{
	Style = InArgs._Style;
	Value = FMath::Clamp(InArgs._Value, -1.f, 1.f);
	AxisIndex = InArgs._AxisIndex;
	DisplayName = InArgs._DisplayName;
	OnClickedDelegate = InArgs._OnClicked;

	AxisText = FText::Format(
		FText::FromString(TEXT("Axis {0}\n{1}{2}")),
		AxisIndex.Get(),
		DisplayName.Get(),
		OnClickedDelegate.IsBound() ? FText::FromString("\n(Click to configure)") : FText::GetEmpty()
	);

	const EMouseCursor::Type HandCursor = OnClickedDelegate.IsBound() ? EMouseCursor::Type::Hand : EMouseCursor::Type::Default;

	SetToolTipText(AxisText);

	ChildSlot
	[
		SNew(SButton)
		.ButtonStyle(FAppStyle::Get(), "NoBorder")
		.OnClicked(this, &SAxisBar::HandleClicked)
		.ContentPadding(0.0f)
		.Cursor(HandCursor)
		[
			SNew(SBox)
			.WidthOverride(60.0f)
			.HeightOverride(200.0f)
			[
				SNew(SOverlay)

				// Outer Border
				+ SOverlay::Slot()
				[
					SNew(SBorder)
					.BorderImage(FAppStyle::GetBrush("ToolPanel.GroupBorder"))
					.BorderBackgroundColor(Style->BorderColor)
					.Padding(2.0f)
				]

				// Background Track
				+ SOverlay::Slot()
				.Padding(2.0f)
				[
					SNew(SBorder)
					.BorderImage(FCoreStyle::Get().GetBrush("WhiteBrush"))
					.BorderBackgroundColor(Style->TrackColor)
				]

				// Center Line Indicator
				+ SOverlay::Slot()
				.VAlign(VAlign_Center)
				.HAlign(HAlign_Fill)
				.Padding(2.0f)
				[
					SNew(SBox)
					.HeightOverride(1.0f)
					[
						SNew(SBorder)
						.BorderImage(FCoreStyle::Get().GetBrush("WhiteBrush"))
						.BorderBackgroundColor(Style->CentreLineColor)
					]
				]

				// Positive Fill (upward from center)
				+ SOverlay::Slot()
				.VAlign(VAlign_Center)
				.Padding(2.0f)
				[
					SNew(SBox)
					.HeightOverride_Lambda([this]
					{
						if (Value.Get() <= 0.f)
						{
							return 0.f;
						}
						const float HalfHeight = (GetCachedGeometry().GetLocalSize().Y - 4.0f) / 2.f;
						return HalfHeight * Value.Get();
					})
					[
						SNew(SBorder)
						.BorderImage(FCoreStyle::Get().GetBrush("WhiteBrush"))
						.BorderBackgroundColor_Lambda([this]
						{
							return FMath::Lerp(Style->PositiveFillColor, Style->PositiveFillPeakColor, FMath::Abs(Value.Get()));
						})
						.RenderTransform_Lambda([this]
						{
							const float Offset = -(GetCachedGeometry().GetLocalSize().Y - 4.0f) * 0.25f * Value.Get();
							return FSlateRenderTransform(FVector2D(0.f, Offset));
						})
					]
				]

				// Negative Fill (downward from center)
				+ SOverlay::Slot()
				.VAlign(VAlign_Center)
				.Padding(2.0f)
				[
					SNew(SBox)
					.HeightOverride_Lambda([this]
					{
						if (Value.Get() >= 0.f)
						{
							return 0.f;
						}
						const float HalfHeight = (GetCachedGeometry().GetLocalSize().Y - 4.0f) / 2.f;
						return HalfHeight * -Value.Get();
					})
					[
						SNew(SBorder)
						.BorderImage(FCoreStyle::Get().GetBrush("WhiteBrush"))
						.BorderBackgroundColor_Lambda([this]
						{
							return FMath::Lerp(Style->NegativeFillColor, Style->NegativeFillPeakColor, FMath::Abs(Value.Get()));
						})
						.RenderTransform_Lambda([this]
						{
							const float Offset = (GetCachedGeometry().GetLocalSize().Y - 4.0f) * 0.25f * -Value.Get();
							return FSlateRenderTransform(FVector2D(0.f, Offset));
						})
					]
				]

				// Rotated + scaled label
				+ SOverlay::Slot()
				.HAlign(HAlign_Fill)
				.VAlign(VAlign_Fill)
				.Padding(2.0f)
				[
					SNew(SScaleBox)
					.HAlign(HAlign_Center)
					.Clipping(EWidgetClipping::Inherit)
					[
						SNew(STextBlock)
						.Text(AxisText)
						.TextStyle(FAppStyle::Get(), "SmallText")
						.RenderTransformPivot(FVector2D(0.5f, 0.5f))
						.RenderTransform(FSlateRenderTransform(FQuat2D(FMath::DegreesToRadians(-90.0f))))
						.Justification(ETextJustify::Center)
						.AutoWrapText(false)
						.ColorAndOpacity(Style->LabelColor)
					]
				]
			]
		]
	];
}

void SAxisBar::SetValue(const float InValue)
{
	Value.Set(FMath::Clamp(InValue, -1.f, 1.f));
}

FReply SAxisBar::HandleClicked() const
{
	if (OnClickedDelegate.IsBound())
	{
		OnClickedDelegate.ExecuteIfBound();
	}
	return FReply::Handled();
}
