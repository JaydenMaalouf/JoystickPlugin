// JoystickPlugin is licensed under the MIT License.
// Copyright Jayden Maalouf 2026. All Rights Reserved.

#pragma once

#include "Diagnostics/JoystickConfigurationRule.h"

#include "JoystickHardwareDeviceRule.generated.h"

struct FHardwareDeviceIdentifier;

// Without a "Joystick" hardware device entry with a primary device type on the current platform, Enhanced Input can't tell the plugin's input apart from other devices.
UCLASS()
class JOYSTICKPLUGINEDITOR_API UJoystickHardwareDeviceRule : public UJoystickConfigurationRule
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
	static bool UsesDefaultHardwareDeviceIdentifier();
	static bool IsJoystickHardwareDevice(const FHardwareDeviceIdentifier& HardwareDevice);
	static const FHardwareDeviceIdentifier* FindJoystickHardwareDevice();
};
