// JoystickPlugin is licensed under the MIT License.
// Copyright Jayden Maalouf 2026. All Rights Reserved.

#include "Diagnostics/Rules/JoystickDeviceMappingPolicyRule.h"

#include "Engine/Engine.h"
#include "JoystickSubsystem.h"
#include "Runtime/Launch/Resources/Version.h"

#if ENGINE_MAJOR_VERSION == 5 && ENGINE_MINOR_VERSION >= 6
#include "GameFramework/InputSettings.h"
#include "GenericPlatform/GenericPlatformInputDeviceMapper.h"
#endif

#define LOCTEXT_NAMESPACE "JoystickPluginEditor"

FName UJoystickDeviceMappingPolicyRule::GetRuleId() const
{
	return TEXT("DeviceMappingPolicy");
}

bool UJoystickDeviceMappingPolicyRule::HasIssue() const
{
#if ENGINE_MAJOR_VERSION == 5 && ENGINE_MINOR_VERSION >= 6
	const UInputPlatformSettings* InputPlatformSettings = UInputPlatformSettings::Get();
	if (IsValid(InputPlatformSettings) == false)
	{
		return false;
	}

	if (InputPlatformSettings->DeviceMappingPolicy != EInputDeviceMappingPolicy::PrimaryUserSharesKeyboardAndFirstGamepad)
	{
		return false;
	}

	return GetConnectedJoystickCount() > 1;
#else
	return false;
#endif
}

FText UJoystickDeviceMappingPolicyRule::GetText() const
{
	return LOCTEXT("DeviceMappingPolicyText", "Joysticks may be assigned to other players");
}

FText UJoystickDeviceMappingPolicyRule::GetSubText() const
{
	return FText::Format(
		LOCTEXT("DeviceMappingPolicySubText",
		        "{0} joysticks are connected while the Input Device Mapping Policy is 'Primary User Shares Keyboard And First Gamepad'. Additional joysticks (ie. a shifter or pedals) may be assigned to other local players instead of the primary player.\n\nSet the Device Mapping Policy to 'Map All Devices To Primary User' in Project Settings > Input."),
		FText::AsNumber(GetConnectedJoystickCount()));
}

bool UJoystickDeviceMappingPolicyRule::IsAffectedBy(const UObject& Object) const
{
#if ENGINE_MAJOR_VERSION == 5 && ENGINE_MINOR_VERSION >= 6
	return Object.IsA<UInputPlatformSettings>();
#else
	return false;
#endif
}

bool UJoystickDeviceMappingPolicyRule::GetSettingsLocation(FName& CategoryName, FName& SectionName) const
{
	CategoryName = TEXT("Engine");
	SectionName = TEXT("Input");
	return true;
}

bool UJoystickDeviceMappingPolicyRule::GetFixLabel(FText& Label) const
{
#if ENGINE_MAJOR_VERSION == 5 && ENGINE_MINOR_VERSION >= 6
	Label = LOCTEXT("MapAllDevicesToPrimaryUser", "Map All To Primary User");
	return true;
#else
	return false;
#endif
}

void UJoystickDeviceMappingPolicyRule::ApplyFix() const
{
#if ENGINE_MAJOR_VERSION == 5 && ENGINE_MINOR_VERSION >= 6
	UInputPlatformSettings* InputPlatformSettings = UInputPlatformSettings::Get();
	if (IsValid(InputPlatformSettings) == false)
	{
		return;
	}

	FProperty* DeviceMappingPolicyProperty = FindFProperty<FProperty>(UInputPlatformSettings::StaticClass(), GET_MEMBER_NAME_CHECKED(UInputPlatformSettings, DeviceMappingPolicy));
	if (DeviceMappingPolicyProperty == nullptr)
	{
		return;
	}

	InputPlatformSettings->DeviceMappingPolicy = EInputDeviceMappingPolicy::MapAllDevicesToPrimaryUser;
	IPlatformInputDeviceMapper::Get().HandleInputDevicePolicyChanged(InputPlatformSettings->DeviceMappingPolicy);

	FPropertyChangedEvent PropertyChangedEvent(DeviceMappingPolicyProperty);
	InputPlatformSettings->PostEditChangeProperty(PropertyChangedEvent);
	InputPlatformSettings->TryUpdateDefaultConfigFile();
#endif
}

int UJoystickDeviceMappingPolicyRule::GetConnectedJoystickCount()
{
	if (IsValid(GEngine) == false)
	{
		return 0;
	}

	const UJoystickSubsystem* JoystickSubsystem = GEngine->GetEngineSubsystem<UJoystickSubsystem>();
	if (IsValid(JoystickSubsystem) == false)
	{
		return 0;
	}

	return JoystickSubsystem->GetConnectedJoystickCount();
}

#undef LOCTEXT_NAMESPACE
