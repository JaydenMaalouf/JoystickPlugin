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
	Style = InArgs._Style;
	HasDevice = InArgs._HasDevice;
	IsPlaying = InArgs._IsPlaying;
	IsLooping = InArgs._IsLooping;

	// Icon-only buttons that never take keyboard focus, as a focused SButton would hijack the spacebar play/stop shortcut.
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
				FSlateColor(Style->PlayColor),
				TAttribute<bool>::CreateLambda([this] { return HasDevice.Get() && !IsPlaying.Get(); }),
				InArgs._OnPlayClicked)
		]
		+ SHorizontalBox::Slot()
		.AutoWidth()
		[
			MakeIconButton(
				"PlayWorld.StopPlaySession.Small",
				LOCTEXT("StopTooltip", "Stop"),
				FSlateColor(Style->StopColor),
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
					return FSlateColor(IsLooping.Get() ? Style->LoopOnColor : Style->LoopOffColor);
				}),
				TAttribute<bool>::CreateLambda([this] { return !IsPlaying.Get(); }),
				InArgs._OnLoopClicked)
		]
	];
}

#undef LOCTEXT_NAMESPACE
