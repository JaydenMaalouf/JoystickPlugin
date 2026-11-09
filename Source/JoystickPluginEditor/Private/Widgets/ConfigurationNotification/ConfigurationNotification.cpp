// JoystickPlugin is licensed under the MIT License.
// Copyright Jayden Maalouf 2026. All Rights Reserved.

#include "Widgets/ConfigurationNotification/ConfigurationNotification.h"

#include "Styling/AppStyle.h"
#include "Widgets/ConfigurationNotification/ConfigurationNotificationFooter.h"
#include "Widgets/SBoxPanel.h"
#include "Widgets/Images/SImage.h"
#include "Widgets/Layout/SSeparator.h"
#include "Widgets/Text/STextBlock.h"

void SConfigurationNotification::Construct(const FArguments& InArgs)
{
	ChildSlot
	[
		SNew(SVerticalBox)
		+ SVerticalBox::Slot()
		.AutoHeight()
		[
			MakeMessage(InArgs._Title, InArgs._Body)
		]
		+ SVerticalBox::Slot()
		.AutoHeight()
		.Padding(0.0f, 10.0f, 0.0f, 6.0f)
		[
			SNew(SSeparator)
			.Orientation(Orient_Horizontal)
			.Thickness(1.0f)
		]
		+ SVerticalBox::Slot()
		.AutoHeight()
		[
			SNew(SConfigurationNotificationFooter)
			.FixLabel(InArgs._FixLabel)
			.OnApplyFix(InArgs._OnApplyFix)
			.OnOpenSettings(InArgs._OnOpenSettings)
			.OnDismiss(InArgs._OnDismiss)
			.OnNeverShowAgainChanged(InArgs._OnNeverShowAgainChanged)
		]
	];
}

void SConfigurationNotification::OnSetCompletionState(SNotificationItem::ECompletionState State)
{
}

TSharedRef<SWidget> SConfigurationNotification::AsWidget()
{
	return AsShared();
}

TSharedRef<SWidget> SConfigurationNotification::MakeMessage(const FText& Title, const FText& Body) const
{
	return SNew(SHorizontalBox)
		+ SHorizontalBox::Slot()
		.AutoWidth()
		.VAlign(VAlign_Top)
		.Padding(0.0f, 2.0f, 10.0f, 0.0f)
		[
			SNew(SImage)
			.Image(FAppStyle::Get().GetBrush("Icons.WarningWithColor"))
			.DesiredSizeOverride(FVector2D(24.0f, 24.0f))
		]
		+ SHorizontalBox::Slot()
		.FillWidth(1.0f)
		[
			SNew(SVerticalBox)
			+ SVerticalBox::Slot()
			.AutoHeight()
			[
				SNew(STextBlock)
				.Text(Title)
				.Font(FAppStyle::Get().GetFontStyle("NotificationList.FontBold"))
				.AutoWrapText(true)
			]
			+ SVerticalBox::Slot()
			.AutoHeight()
			.Padding(0.0f, 6.0f, 0.0f, 0.0f)
			[
				SNew(STextBlock)
				.Text(Body)
				.AutoWrapText(true)
			]
		];
}
