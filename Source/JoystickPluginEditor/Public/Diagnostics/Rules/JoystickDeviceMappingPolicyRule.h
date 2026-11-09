// JoystickPlugin is licensed under the MIT License.
// Copyright Jayden Maalouf 2026. All Rights Reserved.

#pragma once

#include "Diagnostics/JoystickConfigurationRule.h"

#include "JoystickDeviceMappingPolicyRule.generated.h"

// With PrimaryUserSharesKeyboardAndFirstGamepad, joysticks after the first can be handed to other local players (ie. a shifter alongside a wheel).
UCLASS()
class JOYSTICKPLUGINEDITOR_API UJoystickDeviceMappingPolicyRule : public UJoystickConfigurationRule
{
	GENERATED_BODY()

public:
	virtual FName GetRuleId() const override;
	virtual bool HasIssue() const override;
	virtual FText GetText() const override;
	virtual FText GetSubText() const override;
	virtual bool IsAffectedBy(const UObject& Object) const override;
	virtual bool GetSettingsLocation(FName& CategoryName, FName& SectionName) const override;
	virtual bool GetFixLabel(FText& Label) const override;
	virtual void ApplyFix() const override;

private:
	static int GetConnectedJoystickCount();
};
