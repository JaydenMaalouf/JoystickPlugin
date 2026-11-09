// JoystickPlugin is licensed under the MIT License.
// Copyright Jayden Maalouf 2026. All Rights Reserved.

#include "Diagnostics/JoystickConfigurationNotificationSettings.h"

FName UJoystickConfigurationNotificationSettings::GetContainerName() const
{
	return TEXT("Editor");
}

FName UJoystickConfigurationNotificationSettings::GetCategoryName() const
{
	return TEXT("Plugins");
}

bool UJoystickConfigurationNotificationSettings::IsRuleDismissed(const FName RuleId) const
{
	return DismissedRules.Contains(RuleId);
}

void UJoystickConfigurationNotificationSettings::DismissRule(const FName RuleId)
{
	DismissedRules.AddUnique(RuleId);
	SaveConfig();
}
