// JoystickPlugin is licensed under the MIT License.
// Copyright Jayden Maalouf 2026. All Rights Reserved.

#include "Widgets/HapticEffectEditor/HapticEffectTypeSelector.h"

#include "Styling/AppStyle.h"
#include "Styling/CoreStyle.h"
#include "Widgets/SBoxPanel.h"
#include "Widgets/Input/SButton.h"
#include "Widgets/Layout/SBorder.h"
#include "Widgets/Text/STextBlock.h"

#define LOCTEXT_NAMESPACE "HapticEffectEditor"

void SHapticEffectTypeSelector::Construct(const FArguments& InArgs)
{
	Style = InArgs._Style;
	EffectKind = InArgs._EffectKind;
	OnEffectKindSelected = InArgs._OnEffectKindSelected;
	IsEffectKindSupported = InArgs._IsEffectKindSupported;

	ChildSlot
	[
		SNew(SHorizontalBox)
		+ SHorizontalBox::Slot()
		.AutoWidth()
		.Padding(0, 0, 4, 0)
		[
			MakeTypeButton(LOCTEXT("PeriodicType", "Periodic"), EHapticPreviewEffectKind::Periodic)
		]
		+ SHorizontalBox::Slot()
		.AutoWidth()
		.Padding(0, 0, 4, 0)
		[
			MakeTypeButton(LOCTEXT("ConstantType", "Constant"), EHapticPreviewEffectKind::Constant)
		]
		+ SHorizontalBox::Slot()
		.AutoWidth()
		.Padding(0, 0, 4, 0)
		[
			MakeTypeButton(LOCTEXT("RampType", "Ramp"), EHapticPreviewEffectKind::Ramp)
		]
		+ SHorizontalBox::Slot()
		.AutoWidth()
		.Padding(0, 0, 4, 0)
		[
			MakeTypeButton(LOCTEXT("ConditionType", "Condition"), EHapticPreviewEffectKind::Condition)
		]
		+ SHorizontalBox::Slot()
		.AutoWidth()
		[
			MakeTypeButton(LOCTEXT("LeftRightType", "Left/Right"), EHapticPreviewEffectKind::LeftRight)
		]
	];
}

TSharedRef<SWidget> SHapticEffectTypeSelector::MakeTypeButton(const FText& Label, const EHapticPreviewEffectKind Kind)
{
	return SNew(SButton)
		.ButtonStyle(FAppStyle::Get(), "NoBorder")
		.ContentPadding(0.0f)
		.IsFocusable(false)
		.IsEnabled_Lambda([this, Kind] { return IsKindSupported(Kind); })
		.ToolTipText_Lambda([this, Kind]
		{
			return IsKindSupported(Kind) ? FText::GetEmpty() : LOCTEXT("UnsupportedTypeTooltip", "The selected device doesn't support this effect type.");
		})
		.OnClicked_Lambda([this, Kind]
		{
			OnEffectKindSelected.ExecuteIfBound(Kind);
			return FReply::Handled();
		})
		[
			SNew(SBorder)
			.BorderImage(FCoreStyle::Get().GetBrush("WhiteBrush"))
			.Padding(FMargin(10.0f, 4.0f))
			.BorderBackgroundColor_Lambda([this, Kind]
			{
				return FSlateColor(Style->GetBackgroundColor(EffectKind.Get() == Kind));
			})
			[
				SNew(STextBlock)
				.Text(Label)
				.TextStyle(FAppStyle::Get(), "NormalText")
				.ColorAndOpacity_Lambda([this, Kind]
				{
					return FSlateColor(IsKindSupported(Kind) ? Style->GetForegroundColor(EffectKind.Get() == Kind) : Style->UnsupportedForegroundColor);
				})
			]
		];
}

bool SHapticEffectTypeSelector::IsKindSupported(const EHapticPreviewEffectKind Kind) const
{
	return !IsEffectKindSupported.IsBound() || IsEffectKindSupported.Execute(Kind);
}

#undef LOCTEXT_NAMESPACE
