// JoystickPlugin is licensed under the MIT License.
// Copyright Jayden Maalouf. All Rights Reserved.

#include "Widgets/ButtonBox.h"

#include "Widgets/Layout/SScaleBox.h"

void SButtonBox::Construct(const FArguments& InArgs)
{
	Value = InArgs._Value;
	ButtonIndex = InArgs._ButtonIndex;
	DisplayName = InArgs._DisplayName;

	ButtonText = FText::FromString(FString::Format(TEXT("Button {0}\n{1}"), {ButtonIndex.Get(), DisplayName.Get().ToString()}).TrimStartAndEnd());

	SetToolTipText(ButtonText);

	ChildSlot
	[
		SNew(SBorder)
		.BorderBackgroundColor(this, &SButtonBox::GetBackgroundColor)
		.ForegroundColor(this, &SButtonBox::GetForegroundColor)
		[
			SNew(SBox)
			.Padding(FMargin(5, 0, 5, 0)) // <-- Padding outside ScaleBox now
			[
				SNew(SScaleBox)
				.HAlign(HAlign_Fill)
				.VAlign(VAlign_Center)
				.Stretch(EStretch::ScaleToFit)
				.StretchDirection(EStretchDirection::Both)
				[
					SNew(STextBlock)
					.Text(ButtonText)
					.Justification(ETextJustify::Center)
					.MinDesiredWidth(0)
				]
			]
		]
	];
}

void SButtonBox::SetValue(const float InValue)
{
	Value = InValue;
}

FLinearColor SButtonBox::GetColor() const
{
	return Value.Get() ? FLinearColor::Green : FLinearColor::Gray;
}

FSlateColor SButtonBox::GetBackgroundColor() const
{
	return FSlateColor(GetColor());
}

FSlateColor SButtonBox::GetForegroundColor() const
{
	return GetBackgroundColor();
}
