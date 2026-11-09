// JoystickPlugin is licensed under the MIT License.
// Copyright Jayden Maalouf 2026. All Rights Reserved.

#pragma once

#include "Widgets/SCompoundWidget.h"
#include "Widgets/Input/SCheckBox.h"

// The "Never show again" check box and the actions of a configuration notification. Actions without a bound delegate are left out.
class SConfigurationNotificationFooter : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SConfigurationNotificationFooter)
		{
		}

		SLATE_ARGUMENT(FText, FixLabel)
		SLATE_EVENT(FSimpleDelegate, OnApplyFix)
		SLATE_EVENT(FSimpleDelegate, OnOpenSettings)
		SLATE_EVENT(FSimpleDelegate, OnDismiss)
		SLATE_EVENT(FOnCheckStateChanged, OnNeverShowAgainChanged)
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs);

private:
	TSharedRef<SWidget> MakeNeverShowAgainCheckBox(const FOnCheckStateChanged& OnCheckStateChanged) const;
	TSharedRef<SWidget> MakeActionButton(const FText& Label, const FText& ToolTip, const FName ButtonStyle, const FSimpleDelegate& OnClicked);
	TSharedRef<SWidget> MakeDismissButton(const FSimpleDelegate& OnClicked);

	FReply OnActionClicked(const FSimpleDelegate OnClicked) const;
};
