// JoystickPlugin is licensed under the MIT License.
// Copyright Jayden Maalouf 2026. All Rights Reserved.

#pragma once

#include "UObject/Object.h"

#include "JoystickConfigurationRule.generated.h"

UCLASS(Abstract)
class JOYSTICKPLUGINEDITOR_API UJoystickConfigurationRule : public UObject
{
	GENERATED_BODY()

public:
	// Persisted when a rule is dismissed indefinitely, so it must stay stable across renames.
	virtual FName GetRuleId() const;

	virtual bool HasIssue() const;
	virtual FText GetText() const;
	virtual FText GetSubText() const;

	// Whether a change to this object can change the outcome of HasIssue.
	virtual bool IsAffectedBy(const UObject& Object) const;

	// The Project Settings page that resolves the issue, if there is one.
	virtual bool GetSettingsLocation(FName& CategoryName, FName& SectionName) const;

	// The label of the button that resolves the issue automatically, if the rule can.
	virtual bool GetFixLabel(FText& Label) const;
	virtual void ApplyFix() const;
};
