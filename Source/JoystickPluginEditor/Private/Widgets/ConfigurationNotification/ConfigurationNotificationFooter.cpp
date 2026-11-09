// JoystickPlugin is licensed under the MIT License.
// Copyright Jayden Maalouf 2026. All Rights Reserved.

#include "Widgets/ConfigurationNotification/ConfigurationNotificationFooter.h"

#include "Styling/AppStyle.h"
#include "Widgets/SBoxPanel.h"
#include "Widgets/Images/SImage.h"
#include "Widgets/Input/SButton.h"
#include "Widgets/Text/STextBlock.h"

#define LOCTEXT_NAMESPACE "JoystickPluginEditor"

void SConfigurationNotificationFooter::Construct(const FArguments& InArgs)
{
	const TSharedRef<SHorizontalBox> Row = SNew(SHorizontalBox);

	Row->AddSlot()
	   .FillWidth(1.0f)
	   .VAlign(VAlign_Center)
	[
		MakeNeverShowAgainCheckBox(InArgs._OnNeverShowAgainChanged)
	];

	if (InArgs._OnOpenSettings.IsBound())
	{
		Row->AddSlot()
		   .AutoWidth()
		   .VAlign(VAlign_Center)
		   .Padding(4.0f, 0.0f, 0.0f, 0.0f)
		[
			MakeActionButton(
				LOCTEXT("OpenSettings", "Settings"),
				LOCTEXT("OpenSettingsTooltip", "Open the Project Settings page that resolves this issue."),
				TEXT("Button"),
				InArgs._OnOpenSettings)
		];
	}

	if (InArgs._OnApplyFix.IsBound())
	{
		Row->AddSlot()
		   .AutoWidth()
		   .VAlign(VAlign_Center)
		   .Padding(4.0f, 0.0f, 0.0f, 0.0f)
		[
			MakeActionButton(
				InArgs._FixLabel,
				LOCTEXT("ApplyFixTooltip", "Change the project configuration to resolve this issue."),
				TEXT("PrimaryButton"),
				InArgs._OnApplyFix)
		];
	}

	Row->AddSlot()
	   .AutoWidth()
	   .VAlign(VAlign_Center)
	   .Padding(6.0f, 0.0f, 0.0f, 0.0f)
	[
		MakeDismissButton(InArgs._OnDismiss)
	];

	ChildSlot
	[
		Row
	];
}

TSharedRef<SWidget> SConfigurationNotificationFooter::MakeNeverShowAgainCheckBox(const FOnCheckStateChanged& OnCheckStateChanged) const
{
	return SNew(SCheckBox)
		.IsChecked(ECheckBoxState::Unchecked)
		.OnCheckStateChanged(OnCheckStateChanged)
		.ToolTipText(LOCTEXT("NeverShowAgainTooltip", "When dismissed, never show this notification again. Can be re-enabled in Editor Preferences > Plugins > Joystick Configuration Notifications."))
		[
			SNew(STextBlock)
			.Text(LOCTEXT("NeverShowAgain", "Never show again"))
			.TextStyle(FAppStyle::Get(), "NotificationList.WidgetText")
		];
}

TSharedRef<SWidget> SConfigurationNotificationFooter::MakeActionButton(const FText& Label, const FText& ToolTip, const FName ButtonStyle, const FSimpleDelegate& OnClicked)
{
	return SNew(SButton)
		.ButtonStyle(FAppStyle::Get(), ButtonStyle)
		.TextStyle(FAppStyle::Get(), "NotificationList.WidgetText")
		.Text(Label)
		.ToolTipText(ToolTip)
		.OnClicked(this, &SConfigurationNotificationFooter::OnActionClicked, OnClicked);
}

TSharedRef<SWidget> SConfigurationNotificationFooter::MakeDismissButton(const FSimpleDelegate& OnClicked)
{
	return SNew(SButton)
		.ButtonStyle(FAppStyle::Get(), "SimpleButton")
		.ContentPadding(4.0f)
		.ToolTipText(LOCTEXT("DismissTooltip", "Hide this notification until the issue is resolved and occurs again."))
		.OnClicked(this, &SConfigurationNotificationFooter::OnActionClicked, OnClicked)
		[
			SNew(SImage)
			.Image(FAppStyle::Get().GetBrush("Icons.X"))
			.ColorAndOpacity(FSlateColor::UseForeground())
		];
}

FReply SConfigurationNotificationFooter::OnActionClicked(const FSimpleDelegate OnClicked) const
{
	OnClicked.ExecuteIfBound();
	return FReply::Handled();
}

#undef LOCTEXT_NAMESPACE
