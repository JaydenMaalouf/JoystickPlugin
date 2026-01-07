// JoystickPlugin is licensed under the MIT License.
// Copyright Jayden Maalouf. All Rights Reserved.

#include "Widgets/AxisBar.h"

#include "Rendering/DrawElements.h"
#include "Widgets/Layout/SScaleBox.h"
#include "Styling/AppStyle.h"
#include "Styling/CoreStyle.h"
#include "Widgets/Layout/SBorder.h"

void SAxisBar::Construct(const FArguments& InArgs)
{
	Value = FMath::Clamp(InArgs._Value, -1.f, 1.f);
	AxisIndex = InArgs._AxisIndex;
	DisplayName = InArgs._DisplayName;

	AxisText = FText::Format(
		FText::FromString(TEXT("Axis {0}\n{1}")),
		AxisIndex.Get(),
		DisplayName.Get()
	);

	SetToolTipText(AxisText);

	ChildSlot
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
				.BorderBackgroundColor(FLinearColor(0.1f, 0.1f, 0.1f, 1.0f))
				.Padding(2.0f)
			]

			// Background Track
			+ SOverlay::Slot()
			.Padding(2.0f)
			[
				SNew(SBorder)
				.BorderImage(FCoreStyle::Get().GetBrush("WhiteBrush"))
				.BorderBackgroundColor(FLinearColor(0.15f, 0.15f, 0.15f, 1.0f))
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
					.BorderBackgroundColor(FLinearColor(0.4f, 0.4f, 0.4f, 1.0f))
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
						const float Val = FMath::Abs(Value.Get());
						// Gradient from green to bright green based on value
						return FLinearColor(0.2f, 0.8f + Val * 0.2f, 0.2f, 1.0f);
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
						const float Val = FMath::Abs(Value.Get());
						// Gradient from red to bright red based on value
						return FLinearColor(0.8f + Val * 0.2f, 0.2f, 0.2f, 1.0f);
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
					.ColorAndOpacity(FLinearColor::White)
				]
			]
		]
	];
}

void SAxisBar::SetValue(const float InValue)
{
	Value.Set(FMath::Clamp(InValue, -1.f, 1.f));
}
