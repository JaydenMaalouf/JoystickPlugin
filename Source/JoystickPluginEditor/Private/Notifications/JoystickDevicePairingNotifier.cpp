// JoystickPlugin is licensed under the MIT License.
// Copyright Jayden Maalouf 2026. All Rights Reserved.

#include "Notifications/JoystickDevicePairingNotifier.h"

#if ENGINE_MAJOR_VERSION == 5 && ENGINE_MINOR_VERSION >= 1

#include "Data/JoystickInstanceId.h"
#include "Engine/Engine.h"
#include "Framework/Notifications/NotificationManager.h"
#include "GameFramework/InputSettings.h"
#include "GenericPlatform/GenericPlatformInputDeviceMapper.h"
#include "GenericPlatform/InputDeviceMappingPolicy.h"
#include "ISettingsModule.h"
#include "JoystickSubsystem.h"
#include "Managers/JoystickLogManager.h"
#include "Modules/ModuleManager.h"
#include "Widgets/Notifications/SNotificationList.h"

#define LOCTEXT_NAMESPACE "JoystickPluginEditor"

void FJoystickDevicePairingNotifier::Register()
{
	UJoystickSubsystem* Subsystem = GEngine != nullptr ? GEngine->GetEngineSubsystem<UJoystickSubsystem>() : nullptr;
	if (Subsystem == nullptr)
	{
		return;
	}

	DeviceConnectedHandle = Subsystem->Internal_JoystickPluggedInDelegate.AddRaw(this, &FJoystickDevicePairingNotifier::OnDeviceConnected);
}

void FJoystickDevicePairingNotifier::Unregister()
{
	if (!DeviceConnectedHandle.IsValid())
	{
		return;
	}

	if (UJoystickSubsystem* Subsystem = GEngine != nullptr ? GEngine->GetEngineSubsystem<UJoystickSubsystem>() : nullptr)
	{
		Subsystem->Internal_JoystickPluggedInDelegate.Remove(DeviceConnectedHandle);
	}

	DeviceConnectedHandle.Reset();
}

void FJoystickDevicePairingNotifier::OnDeviceConnected(const FJoystickInstanceId& InstanceId)
{
	if (bNotified)
	{
		return;
	}

	const UInputPlatformSettings* PlatformSettings = UInputPlatformSettings::Get();
	if (PlatformSettings == nullptr)
	{
		return;
	}

	// The other two policies either never stop pairing or never start, so only this one can surprise anyone
	if (PlatformSettings->DeviceMappingPolicy != EInputDeviceMappingPolicy::PrimaryUserSharesKeyboardAndFirstGamepad)
	{
		return;
	}

	IPlatformInputDeviceMapper& DeviceMapper = IPlatformInputDeviceMapper::Get();

	TArray<FInputDeviceId> PrimaryUserDevices;
	DeviceMapper.GetAllInputDevicesForUser(DeviceMapper.GetPrimaryPlatformUser(), PrimaryUserDevices);

	// GetPlatformUserForNewlyConnectedDevice pairs to the primary user only while it owns one or fewer
	if (PrimaryUserDevices.Num() <= 1)
	{
		return;
	}

	bNotified = true;

	const FText Message = FText::Format(
		LOCTEXT("DeviceMappingPolicyWarning",
			"The Device Mapping Policy is Primary User Shares Keyboard And First Gamepad, which pairs "
			"a device to the primary user only while that user owns one or fewer. It already owns {0}.\n\n"
			"Every device from here on gets a platform user with no player behind it, so it reports "
			"nothing and looks broken. Set the policy to Map All Devices To Primary User to keep them "
			"all on one player."),
		FText::AsNumber(PrimaryUserDevices.Num()));

	FNotificationInfo Info(Message);
	Info.ExpireDuration = 15.0f;
	Info.HyperlinkText = LOCTEXT("DeviceMappingPolicySettings", "Open Input settings");
	Info.Hyperlink = FSimpleDelegate::CreateLambda([]()
	{
		if (ISettingsModule* SettingsModule = FModuleManager::GetModulePtr<ISettingsModule>("Settings"))
		{
			SettingsModule->ShowViewer("Project", "Engine", "Input");
		}
	});

	FSlateNotificationManager::Get().AddNotification(Info);

	// Logged too, so an unattended run still has something to grep for
	UE_LOG(LogJoystickPlugin, Warning, TEXT("%s"), *Message.ToString());
}

#undef LOCTEXT_NAMESPACE

#endif
