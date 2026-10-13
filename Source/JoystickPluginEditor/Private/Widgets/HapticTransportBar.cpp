// JoystickPlugin is licensed under the MIT License.
// Copyright Jayden Maalouf 2026. All Rights Reserved.

#include "Widgets/HapticTransportBar.h"

#include "Styling/AppStyle.h"
#include "Widgets/SBoxPanel.h"
#include "Widgets/Images/SImage.h"
#include "Widgets/Input/SButton.h"

#define LOCTEXT_NAMESPACE "HapticEffectEditor"

void SHapticTransportBar::Construct(const FArguments& InArgs)
{
	HasDevice = InArgs._HasDevice;
	IsPlaying = InArgs._IsPlaying;
	IsLooping = InArgs._IsLooping;

	// Icon-only buttons: Play/Stop/Loop are momentary/mode controls, not selection state, so they
	// stay visually distinct from the Periodic/Constant/Ramp and waveform "active" pill fills -
	// tinting the icon itself rather than filling a background avoids the two concepts looking
	// like the same kind of toggle.
	// None of the toolbar's buttons take keyboard focus: a focused SButton treats the spacebar as a
	// click on itself, which would hijack the play/stop shortcut (see SHapticEffectEditor::OnKeyDown)
	// after a mouse click.
	auto MakeIconButton = [](const FName IconName, const FText& Tooltip, const TAttribute<FSlateColor>& IconColor, const TAttribute<bool>& IsEnabledAttribute, const FOnClicked& OnClickedDelegate)
	{
		return SNew(SButton)
			.ButtonStyle(FAppStyle::Get(), "NoBorder")
			.ContentPadding(FMargin(6.0f, 2.0f))
			.IsFocusable(false)
			.ToolTipText(Tooltip)
			.IsEnabled(IsEnabledAttribute)
			.OnClicked(OnClickedDelegate)
			[
				SNew(SImage)
				.Image(FAppStyle::GetBrush(IconName))
				.ColorAndOpacity(IconColor)
			];
	};

	ChildSlot
	[
		SNew(SHorizontalBox)
		+ SHorizontalBox::Slot()
		.AutoWidth()
		[
			MakeIconButton(
				"PlayWorld.PlayInViewport",
				LOCTEXT("PlayTooltip", "Play"),
				FSlateColor(FLinearColor(0.2f, 1.0f, 0.2f, 1.0f)),
				TAttribute<bool>::CreateLambda([this] { return HasDevice.Get() && !IsPlaying.Get(); }),
				InArgs._OnPlayClicked)
		]
		+ SHorizontalBox::Slot()
		.AutoWidth()
		[
			MakeIconButton(
				"PlayWorld.StopPlaySession.Small",
				LOCTEXT("StopTooltip", "Stop"),
				FSlateColor(FLinearColor(0.9f, 0.3f, 0.3f, 1.0f)),
				TAttribute<bool>::CreateLambda([this] { return IsPlaying.Get(); }),
				InArgs._OnStopClicked)
		]
		+ SHorizontalBox::Slot()
		.AutoWidth()
		[
			MakeIconButton(
				"TimelineEditor.Loop",
				LOCTEXT("LoopTooltip", "Loop"),
				TAttribute<FSlateColor>::CreateLambda([this]
				{
					return IsLooping.Get() ? FSlateColor(FLinearColor(0.2f, 1.0f, 0.2f, 1.0f)) : FSlateColor(FLinearColor(0.6f, 0.6f, 0.6f, 1.0f));
				}),
				TAttribute<bool>::CreateLambda([this] { return !IsPlaying.Get(); }),
				InArgs._OnLoopClicked)
		]
	];
}

#undef LOCTEXT_NAMESPACE
