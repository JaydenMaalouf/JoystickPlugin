// JoystickPlugin is licensed under the MIT License.
// Copyright Jayden Maalouf 2026. All Rights Reserved.

#pragma once

#include "Widgets/SCompoundWidget.h"
#include "Widgets/Input/SCheckBox.h"
#include "Widgets/Notifications/INotificationWidget.h"

// The content of a configuration rule's notification: a warning with its explanation above a footer of actions.
class SConfigurationNotification : public SCompoundWidget, public INotificationWidget
{
public:
	SLATE_BEGIN_ARGS(SConfigurationNotification)
		{
		}

		SLATE_ARGUMENT(FText, Title)
		SLATE_ARGUMENT(FText, Body)
		SLATE_ARGUMENT(FText, FixLabel)
		SLATE_EVENT(FSimpleDelegate, OnApplyFix)
		SLATE_EVENT(FSimpleDelegate, OnOpenSettings)
		SLATE_EVENT(FSimpleDelegate, OnDismiss)
		SLATE_EVENT(FOnCheckStateChanged, OnNeverShowAgainChanged)
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs);

	// INotificationWidget
	virtual void OnSetCompletionState(SNotificationItem::ECompletionState State) override;
	virtual TSharedRef<SWidget> AsWidget() override;

private:
	TSharedRef<SWidget> MakeMessage(const FText& Title, const FText& Body) const;
};
