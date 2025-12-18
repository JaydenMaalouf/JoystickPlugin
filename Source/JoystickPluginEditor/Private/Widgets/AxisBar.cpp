// JoystickPlugin is licensed under the MIT License.
// Copyright Jayden Maalouf. All Rights Reserved.

#include "Widgets/AxisBar.h"

#include "Rendering/DrawElements.h"
#include "Widgets/Layout/SScaleBox.h"

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
		SNew(SOverlay)

		// Background
		+ SOverlay::Slot()
		[
			SNew(SBorder)
			.BorderImage(FCoreStyle::Get().GetBrush("WhiteBrush"))
			.BorderBackgroundColor(FLinearColor::Black)
		]

		// Positive Fill (upward from center)
		+ SOverlay::Slot()
		.VAlign(VAlign_Center)
		[
			SNew(SBox)
			.HeightOverride_Lambda([this]
			{
				if (Value.Get() <= 0.f)
				{
					return 0.f;
				}
				const float HalfHeight = GetCachedGeometry().GetLocalSize().Y / 2.f;
				return HalfHeight * Value.Get();
			})
			[
				SNew(SBorder)
				.BorderImage(FCoreStyle::Get().GetBrush("WhiteBrush"))
				.BorderBackgroundColor(FLinearColor::Green)
				.RenderTransform_Lambda([this]
				{
					const float Offset = -GetCachedGeometry().GetLocalSize().Y * 0.25f * Value.Get(); // Upward
					return FSlateRenderTransform(FVector2D(0.f, Offset));
				})
			]
		]

		// Negative Fill (downward from center)
		+ SOverlay::Slot()
		.VAlign(VAlign_Center)
		[
			SNew(SBox)
			.HeightOverride_Lambda([this]
			{
				if (Value.Get() >= 0.f)
				{
					return 0.f;
				}
				const float HalfHeight = GetCachedGeometry().GetLocalSize().Y / 2.f;
				return HalfHeight * -Value.Get();
			})
			[
				SNew(SBorder)
				.BorderImage(FCoreStyle::Get().GetBrush("WhiteBrush"))
				.BorderBackgroundColor(FLinearColor::Green)
				.RenderTransform_Lambda([this]
				{
					const float Offset = GetCachedGeometry().GetLocalSize().Y * 0.25f * -Value.Get(); // Downward
					return FSlateRenderTransform(FVector2D(0.f, Offset));
				})
			]
		]


		// Rotated + scaled label
		+ SOverlay::Slot()
		.HAlign(HAlign_Fill)
		.VAlign(VAlign_Fill)
		[
			SNew(SScaleBox)
			.HAlign(HAlign_Center)
			.Clipping(EWidgetClipping::Inherit)
			[
				SNew(STextBlock)
				.Text(AxisText)
				.RenderTransformPivot(FVector2D(0.5f, 0.5f))
				.RenderTransform(FSlateRenderTransform(FQuat2D(FMath::DegreesToRadians(-90.0f))))
				.Justification(ETextJustify::Center)
				.AutoWrapText(false)
			]
		]
	];
}

void SAxisBar::SetValue(const float InValue)
{
	Value.Set(FMath::Clamp(InValue, -1.f, 1.f));
}
