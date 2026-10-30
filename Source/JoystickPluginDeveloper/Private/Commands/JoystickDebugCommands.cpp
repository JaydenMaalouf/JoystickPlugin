// JoystickPlugin is licensed under the MIT License.
// Copyright Jayden Maalouf 2026. All Rights Reserved.

#include "Commands/JoystickDebugCommands.h"

#include "Data/JoystickInformation.h"
#include "Engine/Engine.h"
#include "Engine/World.h"
#include "ForceFeedback/JoystickForceFeedbackComponent.h"
#include "ForceFeedback/Data/ForceFeedbackEffectGroup.h"
#include "GameFramework/PlayerController.h"
#include "JoystickSubsystem.h"
#include "Managers/JoystickHapticDeviceManager.h"
#include "Managers/JoystickProfileManager.h"
#include "Misc/OutputDevice.h"
#include "Misc/PackageName.h"
#include "UObject/UObjectIterator.h"
#include "UObject/UnrealType.h"

FJoystickDebugCommands::FJoystickDebugCommands()
	: ListCommand(TEXT("Joystick.List"), TEXT("Lists every joystick the plugin has seen, with its ids, profile, inputs and haptic support."),
	              FConsoleCommandWithWorldArgsAndOutputDeviceDelegate::CreateRaw(this, &FJoystickDebugCommands::ListDevices))
	  , PlayCommand(TEXT("Joystick.FFB.Play"),
	                TEXT("Joystick.FFB.Play <EffectGroupPath> [InstanceId]. Plays a force feedback effect group from the first player controller, on every device unless an instance id is given. Entries with AutoStart off stay stopped."),
	                FConsoleCommandWithWorldArgsAndOutputDeviceDelegate::CreateRaw(this, &FJoystickDebugCommands::PlayEffectGroup))
	  , StopAllCommand(TEXT("Joystick.FFB.StopAll"), TEXT("Stops every force feedback component, effect and rumble, and removes the components Joystick.FFB.Play added."),
	                   FConsoleCommandWithWorldArgsAndOutputDeviceDelegate::CreateRaw(this, &FJoystickDebugCommands::StopAll))
{
}

void FJoystickDebugCommands::ListDevices(const TArray<FString>& Args, UWorld* World, FOutputDevice& Ar) const
{
	UJoystickSubsystem* JoystickSubsystem = GEngine->GetEngineSubsystem<UJoystickSubsystem>();
	if (IsValid(JoystickSubsystem) == false)
	{
		Ar.Log(TEXT("The joystick subsystem is not available."));
		return;
	}

	TArray<FJoystickInstanceId> InstanceIds;
	JoystickSubsystem->GetInstanceIds(InstanceIds, true);
	if (InstanceIds.Num() == 0)
	{
		Ar.Log(TEXT("No joysticks found."));
		return;
	}

	Ar.Logf(TEXT("%d joysticks, %d connected"), InstanceIds.Num(), JoystickSubsystem->GetConnectedJoystickCount());
	for (const FJoystickInstanceId& InstanceId : InstanceIds)
	{
		FJoystickInformation Info;
		if (JoystickSubsystem->GetJoystickInfo(InstanceId, Info) == false)
		{
			continue;
		}

		ListDevice(Info, JoystickSubsystem->IsConnected(InstanceId), Ar);
	}
}

void FJoystickDebugCommands::PlayEffectGroup(const TArray<FString>& Args, UWorld* World, FOutputDevice& Ar)
{
	if (Args.Num() == 0)
	{
		Ar.Log(TEXT("Usage: Joystick.FFB.Play <EffectGroupPath> [InstanceId]"));
		return;
	}

	if (IsValid(World) == false)
	{
		Ar.Log(TEXT("Joystick.FFB.Play needs a running game."));
		return;
	}

	APlayerController* PlayerController = World->GetFirstPlayerController();
	if (IsValid(PlayerController) == false)
	{
		Ar.Log(TEXT("Joystick.FFB.Play needs a running game with a player controller."));
		return;
	}

	FString ObjectPath = Args[0];
	if (ObjectPath.Contains(TEXT(".")) == false)
	{
		ObjectPath += TEXT(".") + FPackageName::GetShortName(ObjectPath);
	}

	UForceFeedbackEffectGroup* EffectGroup = LoadObject<UForceFeedbackEffectGroup>(nullptr, *ObjectPath);
	if (IsValid(EffectGroup) == false)
	{
		Ar.Logf(TEXT("No force feedback effect group found at %s."), *ObjectPath);
		return;
	}

	UJoystickForceFeedbackComponent* Component = NewObject<UJoystickForceFeedbackComponent>(PlayerController, NAME_None, RF_Transient);
	if (Args.Num() > 1)
	{
		Component->InstanceId = FJoystickInstanceId(FCString::Atoi(*Args[1]));
	}

	Component->EffectGroups.Add(EffectGroup);
	Component->RegisterComponent();

	PlayedComponents.RemoveAll([](const TWeakObjectPtr<UJoystickForceFeedbackComponent>& Played)
	{
		return Played.IsValid() == false;
	});
	PlayedComponents.Add(Component);

	const FString Target = !Component->InstanceId.IsValid() ? TEXT("every device") : FString::Printf(TEXT("device %d"), Component->InstanceId.Value);
	Ar.Logf(TEXT("Playing %s on %s. Joystick.FFB.StopAll stops it."), *EffectGroup->GetName(), *Target);
}

void FJoystickDebugCommands::StopAll(const TArray<FString>& Args, UWorld* World, FOutputDevice& Ar)
{
	DestroyPlayedComponents();

	const int StoppedComponents = StopComponents();
	const int StoppedDevices = StopDevices();
	Ar.Logf(TEXT("Stopped %d force feedback components and %d devices."), StoppedComponents, StoppedDevices);
}

void FJoystickDebugCommands::DestroyPlayedComponents()
{
	for (const TWeakObjectPtr<UJoystickForceFeedbackComponent>& Played : PlayedComponents)
	{
		UJoystickForceFeedbackComponent* Component = Played.Get();
		if (IsValid(Component) == false)
		{
			continue;
		}

		Component->DestroyComponent();
	}

	PlayedComponents.Reset();
}

int FJoystickDebugCommands::StopComponents()
{
	int StoppedComponents = 0;
	for (UJoystickForceFeedbackComponent* Component : TObjectRange<UJoystickForceFeedbackComponent>())
	{
		if (IsValid(Component) == false)
		{
			continue;
		}

		if (Component->HasBegunPlay() == false)
		{
			continue;
		}

		Component->StopEffect();
		StoppedComponents++;
	}

	return StoppedComponents;
}

int FJoystickDebugCommands::StopDevices()
{
	UJoystickSubsystem* JoystickSubsystem = GEngine->GetEngineSubsystem<UJoystickSubsystem>();
	if (IsValid(JoystickSubsystem) == false)
	{
		return 0;
	}

	TArray<FJoystickInstanceId> InstanceIds;
	JoystickSubsystem->GetInstanceIds(InstanceIds);

	int StoppedDevices = 0;
	for (const FJoystickInstanceId& InstanceId : InstanceIds)
	{
		FJoystickInformation Info;
		if (JoystickSubsystem->GetJoystickInfo(InstanceId, Info) == false)
		{
			continue;
		}

		StopDevice(Info);
		StoppedDevices++;
	}

	return StoppedDevices;
}

void FJoystickDebugCommands::ListDevice(const FJoystickInformation& Info, const bool IsConnected, FOutputDevice& Ar)
{
	Ar.Logf(TEXT("[%d] %s (%s)"), Info.InstanceId.Value, *Info.GetDeviceDisplayName(), IsConnected ? TEXT("Connected") : TEXT("Disconnected"));
	Ar.Logf(TEXT("    Type %s   Connection %s   Gamepad %s"), *FormatEnum(Info.Type), *FormatEnum(Info.Connection), *FormatYesNo(Info.IsGamepad));
	Ar.Logf(TEXT("    Vendor 0x%04X   Product 0x%04X   Version 0x%04X   Firmware 0x%04X"), Info.VendorId, Info.ProductId, Info.ProductVersion, Info.FirmwareVersion);
	Ar.Logf(TEXT("    GUID %s   Hash %s"), *Info.ProductGuid.ToString(), *Info.DeviceHash);
	Ar.Logf(TEXT("    Profile %s"), *FormatProfile(Info));

	if (IsConnected == false)
	{
		return;
	}

	Ar.Logf(TEXT("    Inputs %s"), *FormatInputs(Info.InstanceId));
	Ar.Logf(TEXT("    Haptic %s"), *FormatHaptic(Info));
	Ar.Logf(TEXT("    Rumble %s"), *FormatRumble(Info));
	Ar.Logf(TEXT("    Sensors %s"), *FormatSensors(Info));
	Ar.Logf(TEXT("    Power %s %d%%"), *FormatEnum(Info.Power.State), Info.Power.Level);
}

void FJoystickDebugCommands::StopDevice(const FJoystickInformation& Info)
{
	const UJoystickHapticDeviceManager* HapticDeviceManager = GetDefault<UJoystickHapticDeviceManager>();
	if (IsValid(HapticDeviceManager) == false)
	{
		return;
	}

	if (Info.Haptic.Supported)
	{
		HapticDeviceManager->StopAllEffects(Info.InstanceId);
	}

	if (Info.Rumble.StandardRumble)
	{
		HapticDeviceManager->StopRumble(Info.InstanceId);
	}

	if (Info.Rumble.TriggerRumble)
	{
		HapticDeviceManager->StopRumbleTriggers(Info.InstanceId);
	}

	if (Info.HapticRumble.Supported)
	{
		HapticDeviceManager->StopHapticRumble(Info.InstanceId);
	}
}

FString FJoystickDebugCommands::FormatInputs(const FJoystickInstanceId& InstanceId)
{
	const UJoystickSubsystem* JoystickSubsystem = GEngine->GetEngineSubsystem<UJoystickSubsystem>();
	if (IsValid(JoystickSubsystem) == false)
	{
		return TEXT("Unavailable");
	}

	FJoystickDeviceState State;
	if (JoystickSubsystem->GetJoystickState(InstanceId, State) == false)
	{
		return TEXT("Unavailable");
	}

	return FString::Printf(TEXT("%d axes   %d buttons   %d hats   %d balls"), State.Axes.Num(), State.Buttons.Num(), State.Hats.Num(), State.Balls.Num());
}

FString FJoystickDebugCommands::FormatProfile(const FJoystickInformation& Info)
{
	UJoystickProfileManager* ProfileManager = GetMutableDefault<UJoystickProfileManager>();
	if (IsValid(ProfileManager) == false)
	{
		return TEXT("Unavailable");
	}

	const FJoystickInputDeviceConfiguration* Configuration = ProfileManager->GetInputDeviceConfiguration(Info);
	if (Configuration == nullptr)
	{
		return TEXT("None");
	}

	return FString::Printf(TEXT("Matched by hash   %d axis overrides   %d button overrides   Force feedback overrides %s"), Configuration->AxisProperties.Num(), Configuration->ButtonProperties.Num(),
	                       *FormatYesNo(Configuration->OverrideForceFeedbackProperties));
}

FString FJoystickDebugCommands::FormatHaptic(const FJoystickInformation& Info)
{
	if (Info.Haptic.Supported == false)
	{
		return TEXT("Not supported");
	}

	const UJoystickHapticDeviceManager* HapticDeviceManager = GetDefault<UJoystickHapticDeviceManager>();
	if (IsValid(HapticDeviceManager) == false)
	{
		return TEXT("Not supported");
	}

	return FString::Printf(TEXT("%d effect slots   %d playing at once   %d axes   %s"), HapticDeviceManager->GetMaxNumEffects(Info.InstanceId), HapticDeviceManager->GetMaxNumEffectsPlaying(Info.InstanceId), Info.Haptic.AxesCount,
	                       *FormatHapticEffects(Info.Haptic));
}

FString FJoystickDebugCommands::FormatHapticEffects(const FJoystickHapticInformation& Haptic)
{
	TArray<FString> Supported;
	for (TFieldIterator<FBoolProperty> It(FJoystickHapticInformation::StaticStruct()); It; ++It)
	{
		if (It->GetFName() == GET_MEMBER_NAME_CHECKED(FJoystickHapticInformation, Supported))
		{
			continue;
		}

		if (It->GetPropertyValue_InContainer(&Haptic) == false)
		{
			continue;
		}

		Supported.Add(It->GetName());
	}

	return Supported.Num() > 0 ? FString::Join(Supported, TEXT(", ")) : TEXT("No effects");
}

FString FJoystickDebugCommands::FormatRumble(const FJoystickInformation& Info)
{
	TArray<FString> Supported;
	if (Info.Rumble.StandardRumble)
	{
		Supported.Add(TEXT("Standard"));
	}

	if (Info.Rumble.TriggerRumble)
	{
		Supported.Add(TEXT("Triggers"));
	}

	if (Info.HapticRumble.Supported)
	{
		Supported.Add(TEXT("Haptic"));
	}

	return Supported.Num() > 0 ? FString::Join(Supported, TEXT(", ")) : TEXT("Not supported");
}

FString FJoystickDebugCommands::FormatSensors(const FJoystickInformation& Info)
{
	const TPair<const TCHAR*, const FJoystickSensorInformation*> Sensors[] = {{TEXT("Gyro"), &Info.Gyro}, {TEXT("Accelerometer"), &Info.Accelerometer}};

	TArray<FString> Supported;
	for (const TPair<const TCHAR*, const FJoystickSensorInformation*>& Sensor : Sensors)
	{
		if (Sensor.Value->Supported == false)
		{
			continue;
		}

		Supported.Add(FString::Printf(TEXT("%s (%s)"), Sensor.Key, Sensor.Value->Enabled ? TEXT("enabled") : TEXT("disabled")));
	}

	return Supported.Num() > 0 ? FString::Join(Supported, TEXT(", ")) : TEXT("None");
}

FString FJoystickDebugCommands::FormatYesNo(const bool Value)
{
	return Value ? TEXT("Yes") : TEXT("No");
}
