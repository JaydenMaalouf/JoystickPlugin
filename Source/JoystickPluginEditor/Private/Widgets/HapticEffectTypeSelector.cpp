// JoystickPlugin is licensed under the MIT License.
// Copyright Jayden Maalouf 2026. All Rights Reserved.

#include "Widgets/HapticEffectTypeSelector.h"

#include "Styling/AppStyle.h"
#include "Styling/CoreStyle.h"
#include "Widgets/HapticEditorStyle.h"
#include "Widgets/SBoxPanel.h"
#include "Widgets/Input/SButton.h"
#include "Widgets/Layout/SBorder.h"
#include "Widgets/Text/STextBlock.h"

#define LOCTEXT_NAMESPACE "HapticEffectEditor"

void SHapticEffectTypeSelector::Construct(const FArguments& InArgs)
{
	EffectKind = InArgs._EffectKind;
	OnEffectKindSelected = InArgs._OnEffectKindSelected;

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
				return FHapticEffectEditorColors::GetToggleBackgroundColor(EffectKind.Get() == Kind);
			})
			[
				SNew(STextBlock)
				.Text(Label)
				.TextStyle(FAppStyle::Get(), "NormalText")
				.ColorAndOpacity_Lambda([this, Kind]
				{
					return FHapticEffectEditorColors::GetToggleForegroundColor(EffectKind.Get() == Kind);
				})
			]
		];
}

#undef LOCTEXT_NAMESPACE
