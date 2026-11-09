// JoystickPlugin is licensed under the MIT License.
// Copyright Jayden Maalouf 2026. All Rights Reserved.

#include "Diagnostics/JoystickConfigurationRule.h"

FName UJoystickConfigurationRule::GetRuleId() const
{
	return NAME_None;
}

bool UJoystickConfigurationRule::HasIssue() const
{
	return false;
}

FText UJoystickConfigurationRule::GetText() const
{
	return FText::GetEmpty();
}

FText UJoystickConfigurationRule::GetSubText() const
{
	return FText::GetEmpty();
}

bool UJoystickConfigurationRule::IsAffectedBy(const UObject& Object) const
{
	return false;
}

bool UJoystickConfigurationRule::GetSettingsLocation(FName& CategoryName, FName& SectionName) const
{
	return false;
}

bool UJoystickConfigurationRule::GetFixLabel(FText& Label) const
{
	return false;
}

void UJoystickConfigurationRule::ApplyFix() const
{
}
