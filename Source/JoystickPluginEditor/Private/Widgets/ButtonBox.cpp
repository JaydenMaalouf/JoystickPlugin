// JoystickPlugin is licensed under the MIT License.
// Copyright Jayden Maalouf 2026. All Rights Reserved.

#include "Widgets/ButtonBox.h"

#include "Widgets/Layout/SScaleBox.h"
#include "Styling/AppStyle.h"
#include "Styling/CoreStyle.h"
#include "Widgets/Layout/SBorder.h"
#include "Widgets/Input/SButton.h"

void SButtonBox::Construct(const FArguments& InArgs)
{
	Value = InArgs._Value;
	ButtonIndex = InArgs._ButtonIndex;
	DisplayName = InArgs._DisplayName;
	OnClickedDelegate = InArgs._OnClicked;

	ButtonText = FText::Format(
		FText::FromString(TEXT("Button {0}\n{1}{2}")),
		ButtonIndex.Get(),
		DisplayName.Get(),
		OnClickedDelegate.IsBound() ? FText::FromString("\n(Click to configure)") : FText::GetEmpty()
	);

	SetToolTipText(ButtonText);

	const EMouseCursor::Type HandCursor = OnClickedDelegate.IsBound() ? EMouseCursor::Hand : EMouseCursor::None;

	ChildSlot
	[
		SNew(SButton)
		.ButtonStyle(FAppStyle::Get(), "NoBorder")
		.OnClicked(this, &SButtonBox::HandleClicked)
		.ContentPadding(0.0f)
		.Cursor(HandCursor)
		[
			SNew(SBox)
			.MinDesiredWidth(80.0f)
			.MinDesiredHeight(60.0f)
			[
				SNew(SBorder)
				.BorderImage_Lambda([this]()
				{
					return Value.Get()
						       ? FAppStyle::GetBrush("ToolPanel.GroupBorder")
						       : FAppStyle::GetBrush("ToolPanel.DarkGroupBorder");
				})
				.BorderBackgroundColor(this, &SButtonBox::GetBackgroundColor)
				.Padding(2.0f)
				[
					SNew(SBorder)
					.BorderImage(FCoreStyle::Get().GetBrush("WhiteBrush"))
					.BorderBackgroundColor(this, &SButtonBox::GetBackgroundColor)
					.Padding(6.0f)
					[
						SNew(SScaleBox)
						.HAlign(HAlign_Center)
						.VAlign(VAlign_Center)
						.Stretch(EStretch::ScaleToFit)
						.StretchDirection(EStretchDirection::Both)
						[
							SNew(STextBlock)
							.Text(ButtonText)
							.TextStyle(FAppStyle::Get(), "SmallText")
							.Justification(ETextJustify::Center)
							.AutoWrapText(true)
							.ColorAndOpacity_Lambda([this]()
							{
								return Value.Get()
									       ? FSlateColor(FLinearColor::White)
									       : FSlateColor(FLinearColor(0.7f, 0.7f, 0.7f, 1.0f));
							})
						]
					]
				]
			]
		]
	];
}

FSlateColor SButtonBox::GetForegroundColor() const
{
	return GetBackgroundColor();
}

void SButtonBox::SetValue(const bool InValue)
{
	Value = InValue;
}

FLinearColor SButtonBox::GetColor() const
{
	if (Value.Get())
	{
		// Bright green when pressed
		return FLinearColor(0.2f, 0.9f, 0.3f, 1.0f);
	}
	// Dark gray when not pressed
	return FLinearColor(0.25f, 0.25f, 0.25f, 1.0f);
}

FSlateColor SButtonBox::GetBackgroundColor() const
{
	return FSlateColor(GetColor());
}

FReply SButtonBox::HandleClicked()
{
	if (OnClickedDelegate.IsBound())
	{
		OnClickedDelegate.ExecuteIfBound();
	}
	return FReply::Handled();
}
