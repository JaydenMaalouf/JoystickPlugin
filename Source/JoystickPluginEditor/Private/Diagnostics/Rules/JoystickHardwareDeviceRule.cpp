// JoystickPlugin is licensed under the MIT License.
// Copyright Jayden Maalouf 2026. All Rights Reserved.

#include "Diagnostics/Rules/JoystickHardwareDeviceRule.h"

#include "JoystickInputDevice.h"
#include "JoystickInputSettings.h"
#include "Runtime/Launch/Resources/Version.h"

#if ENGINE_MAJOR_VERSION == 5 && ENGINE_MINOR_VERSION >= 3
#include "GameFramework/InputSettings.h"
#endif

#define LOCTEXT_NAMESPACE "JoystickPluginEditor"

FName UJoystickHardwareDeviceRule::GetRuleId() const
{
	return TEXT("JoystickHardwareDevice");
}

bool UJoystickHardwareDeviceRule::HasIssue() const
{
#if ENGINE_MAJOR_VERSION == 5 && ENGINE_MINOR_VERSION >= 3
	if (UsesDefaultHardwareDeviceIdentifier() == false)
	{
		return false;
	}

	const FHardwareDeviceIdentifier* HardwareDevice = FindJoystickHardwareDevice();
	if (HardwareDevice == nullptr)
	{
		return true;
	}

	return HardwareDevice->PrimaryDeviceType == EHardwareDevicePrimaryType::Unspecified;
#else
	return false;
#endif
}

FText UJoystickHardwareDeviceRule::GetText() const
{
	return LOCTEXT("JoystickHardwareDeviceText", "Enhanced Input can't identify joysticks");
}

FText UJoystickHardwareDeviceRule::GetSubText() const
{
	return FText::Format(
		LOCTEXT("JoystickHardwareDeviceSubText",
		        "The {0} Input Platform Settings have no Hardware Device for the Joystick Plugin with a Primary Device Type set. Enhanced Input uses the device type to filter player mappable keys and key profiles, so joystick mappings won't be told apart from other devices.\n\nIn Project Settings > Input > Platforms > {0}, add a Hardware Device with Input Class Name '{1}', Hardware Device Identifier '{2}' and a Primary Device Type such as 'Racing Wheel' or 'Flight Stick'. This can be ignored if the project doesn't use Enhanced Input's user settings."),
		FText::FromString(ANSI_TO_TCHAR(FPlatformProperties::IniPlatformName())),
		FText::FromName(FJoystickInputDevice::InputInterfaceName),
		FText::FromName(FJoystickInputDevice::DefaultHardwareDeviceIdentifier));
}

bool UJoystickHardwareDeviceRule::IsAffectedBy(const UObject& Object) const
{
#if ENGINE_MAJOR_VERSION == 5 && ENGINE_MINOR_VERSION >= 3
	return Object.IsA<UInputPlatformSettings>() || Object.IsA<UJoystickInputSettings>();
#else
	return false;
#endif
}

bool UJoystickHardwareDeviceRule::GetSettingsLocation(FName& CategoryName, FName& SectionName) const
{
	CategoryName = TEXT("Engine");
	SectionName = TEXT("Input");
	return true;
}

bool UJoystickHardwareDeviceRule::GetFixLabel(FText& Label) const
{
#if ENGINE_MAJOR_VERSION == 5 && ENGINE_MINOR_VERSION >= 3
	Label = FindJoystickHardwareDevice() == nullptr ? LOCTEXT("AddJoystickHardwareDevice", "Add Hardware Device") : LOCTEXT("SetJoystickHardwareDeviceType", "Set To Racing Wheel");
	return true;
#else
	return false;
#endif
}

void UJoystickHardwareDeviceRule::ApplyFix() const
{
#if ENGINE_MAJOR_VERSION == 5 && ENGINE_MINOR_VERSION >= 3
	UInputPlatformSettings* InputPlatformSettings = UInputPlatformSettings::Get();
	if (IsValid(InputPlatformSettings) == false)
	{
		return;
	}

	// HardwareDevices is protected and its public accessor can only append, so an existing entry without a type is corrected through reflection.
	FProperty* HardwareDevicesProperty = FindFProperty<FProperty>(UInputPlatformSettings::StaticClass(), TEXT("HardwareDevices"));
	if (HardwareDevicesProperty == nullptr)
	{
		return;
	}

	TArray<FHardwareDeviceIdentifier>* HardwareDevices = HardwareDevicesProperty->ContainerPtrToValuePtr<TArray<FHardwareDeviceIdentifier>>(InputPlatformSettings);
	FHardwareDeviceIdentifier* HardwareDevice = HardwareDevices->FindByPredicate(&UJoystickHardwareDeviceRule::IsJoystickHardwareDevice);
	if (HardwareDevice == nullptr)
	{
		HardwareDevice = &HardwareDevices->Emplace_GetRef(FJoystickInputDevice::InputInterfaceName, FJoystickInputDevice::DefaultHardwareDeviceIdentifier);
		HardwareDevice->SupportedFeaturesMask = EHardwareDeviceSupportedFeatures::Keypress
			| EHardwareDeviceSupportedFeatures::Gamepad
			| EHardwareDeviceSupportedFeatures::ForceFeedback
			| EHardwareDeviceSupportedFeatures::Acceleration
			| EHardwareDeviceSupportedFeatures::Orientation;
	}

	HardwareDevice->PrimaryDeviceType = EHardwareDevicePrimaryType::RacingWheel;

	FPropertyChangedEvent PropertyChangedEvent(HardwareDevicesProperty);
	InputPlatformSettings->PostEditChangeProperty(PropertyChangedEvent);
	InputPlatformSettings->TryUpdateDefaultConfigFile();
#endif
}

bool UJoystickHardwareDeviceRule::UsesDefaultHardwareDeviceIdentifier()
{
	const UJoystickInputSettings* JoystickInputSettings = GetDefault<UJoystickInputSettings>();
	if (IsValid(JoystickInputSettings) == false)
	{
		return false;
	}

	return JoystickInputSettings->UseDeviceNameAsHardwareDeviceIdentifier == false;
}

bool UJoystickHardwareDeviceRule::IsJoystickHardwareDevice(const FHardwareDeviceIdentifier& HardwareDevice)
{
#if ENGINE_MAJOR_VERSION == 5 && ENGINE_MINOR_VERSION >= 3
	return HardwareDevice.InputClassName == FJoystickInputDevice::InputInterfaceName && HardwareDevice.HardwareDeviceIdentifier == FJoystickInputDevice::DefaultHardwareDeviceIdentifier;
#else
	return false;
#endif
}

const FHardwareDeviceIdentifier* UJoystickHardwareDeviceRule::FindJoystickHardwareDevice()
{
#if ENGINE_MAJOR_VERSION == 5 && ENGINE_MINOR_VERSION >= 3
	const UInputPlatformSettings* InputPlatformSettings = UInputPlatformSettings::Get();
	if (IsValid(InputPlatformSettings) == false)
	{
		return nullptr;
	}

	return InputPlatformSettings->GetHardwareDevices().FindByPredicate(&UJoystickHardwareDeviceRule::IsJoystickHardwareDevice);
#else
	return nullptr;
#endif
}

#undef LOCTEXT_NAMESPACE
