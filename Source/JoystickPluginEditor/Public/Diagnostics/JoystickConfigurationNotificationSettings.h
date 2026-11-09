// JoystickPlugin is licensed under the MIT License.
// Copyright Jayden Maalouf 2026. All Rights Reserved.

#pragma once

#include "Engine/DeveloperSettings.h"

#include "JoystickConfigurationNotificationSettings.generated.h"

UCLASS(Config=EditorPerProjectUserSettings, meta=(DisplayName="Joystick Configuration Notifications"))
class JOYSTICKPLUGINEDITOR_API UJoystickConfigurationNotificationSettings : public UDeveloperSettings
{
	GENERATED_BODY()

public:
	UPROPERTY(Config, EditAnywhere, Category="Notifications", meta=(ToolTip="Show a notification when the project configuration may conflict with the Joystick Plugin."))
	bool EnableNotifications = true;

	UPROPERTY(Config, EditAnywhere, Category="Notifications", meta=(ToolTip="Notifications dismissed with 'Don't Show Again'. Remove an entry to see that notification again."))
	TArray<FName> DismissedRules;

	virtual FName GetContainerName() const override;
	virtual FName GetCategoryName() const override;

	bool IsRuleDismissed(const FName RuleId) const;
	void DismissRule(const FName RuleId);
};
