// JoystickPlugin is licensed under the MIT License.
// Copyright Jayden Maalouf 2026. All Rights Reserved.

#include "Widgets/HapticWaveformSelector.h"

#include "Styling/AppStyle.h"
#include "Styling/CoreStyle.h"
#include "Widgets/HapticEditorStyle.h"
#include "Widgets/HapticWaveformIcon.h"
#include "Widgets/SBoxPanel.h"
#include "Widgets/Input/SButton.h"
#include "Widgets/Layout/SBorder.h"

#define LOCTEXT_NAMESPACE "HapticEffectEditor"

void SHapticWaveformSelector::Construct(const FArguments& InArgs)
{
	SelectedWaveform = InArgs._SelectedWaveform;
	OnWaveformSelected = InArgs._OnWaveformSelected;
	IsWaveformSupported = InArgs._IsWaveformSupported;

	ChildSlot
	[
		SNew(SHorizontalBox)
		+ SHorizontalBox::Slot()
		.AutoWidth()
		.Padding(0, 0, 2, 0)
		[
			MakeWaveformButton(EForceFeedbackPeriodicEffectType::Sine, LOCTEXT("Sine", "Sine"))
		]
		+ SHorizontalBox::Slot()
		.AutoWidth()
		.Padding(0, 0, 2, 0)
		[
			MakeWaveformButton(EForceFeedbackPeriodicEffectType::Triangle, LOCTEXT("Triangle", "Triangle"))
		]
		+ SHorizontalBox::Slot()
		.AutoWidth()
		.Padding(0, 0, 2, 0)
		[
			MakeWaveformButton(EForceFeedbackPeriodicEffectType::SawtoothUp, LOCTEXT("SawtoothUp", "Sawtooth Up"))
		]
		+ SHorizontalBox::Slot()
		.AutoWidth()
		.Padding(0, 0, 2, 0)
		[
			MakeWaveformButton(EForceFeedbackPeriodicEffectType::SawtoothDown, LOCTEXT("SawtoothDown", "Sawtooth Down"))
		]
		+ SHorizontalBox::Slot()
		.AutoWidth()
		[
			MakeWaveformButton(EForceFeedbackPeriodicEffectType::Square, LOCTEXT("Square", "Square"))
		]
	];
}

TSharedRef<SWidget> SHapticWaveformSelector::MakeWaveformButton(const EForceFeedbackPeriodicEffectType WaveformType, const FText& Tooltip)
{
	return SNew(SButton)
		.ButtonStyle(FAppStyle::Get(), "NoBorder")
		.ContentPadding(0.0f)
		.IsFocusable(false)
		.IsEnabled_Lambda([this, WaveformType] { return IsSupported(WaveformType); })
		.ToolTipText_Lambda([this, WaveformType, Tooltip]
		{
			return IsSupported(WaveformType) ? Tooltip : FText::Format(LOCTEXT("UnsupportedWaveformTooltip", "{0} (not supported by the selected device)"), Tooltip);
		})
		.OnClicked_Lambda([this, WaveformType]
		{
			OnWaveformSelected.ExecuteIfBound(WaveformType);
			return FReply::Handled();
		})
		[
			SNew(SBorder)
			.BorderImage(FCoreStyle::Get().GetBrush("WhiteBrush"))
			.Padding(FMargin(6.0f, 3.0f))
			.BorderBackgroundColor_Lambda([this, WaveformType]
			{
				return FHapticEffectEditorColors::GetToggleBackgroundColor(IsWaveformSelected(WaveformType));
			})
			[
				SNew(SHapticWaveformIcon)
				.WaveformType(WaveformType)
				.Color_Lambda([this, WaveformType]
				{
					if (!IsSupported(WaveformType))
					{
						return FHapticEffectEditorColors::GetUnsupportedForegroundColor().GetSpecifiedColor();
					}

					return IsWaveformSelected(WaveformType) ? FLinearColor::White : FLinearColor(0.6f, 0.6f, 0.6f, 1.0f);
				})
			]
		];
}

bool SHapticWaveformSelector::IsWaveformSelected(const EForceFeedbackPeriodicEffectType WaveformType) const
{
	const TOptional<EForceFeedbackPeriodicEffectType> Selected = SelectedWaveform.Get();
	return Selected.IsSet() && Selected.GetValue() == WaveformType;
}

bool SHapticWaveformSelector::IsSupported(const EForceFeedbackPeriodicEffectType WaveformType) const
{
	return !IsWaveformSupported.IsBound() || IsWaveformSupported.Execute(WaveformType);
}

#undef LOCTEXT_NAMESPACE
