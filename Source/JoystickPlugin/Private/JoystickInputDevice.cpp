// JoystickPlugin is licensed under the MIT License.
// Copyright Jayden Maalouf 2026. All Rights Reserved.

#include "JoystickInputDevice.h"

#include "Async/Async.h"
#include "Data/DeviceInfoSDL.h"
#include "Data/Input/AxisData.h"
#include "Data/Input/BallData.h"
#include "Data/Input/HatData.h"
#include "Data/Input/KeyPair.h"
#include "Data/JoystickDeviceState.h"
#include "Data/JoystickInstanceId.h"
#include "Data/PlatformTypes.h"
#include "Data/Settings/JoystickInputDeviceAxisProperties.h"
#include "Data/Settings/JoystickInputDeviceConfiguration.h"
#include "Data/Settings/JoystickInputKeyConfiguration.h"
#include "Engine/Engine.h"
#include "Framework/Application/NavigationConfig.h"
#include "Framework/Application/SlateApplication.h"
#include "GameFramework/InputSettings.h"
#include "GenericPlatform/IInputInterface.h"
#include "JoystickFunctionLibrary.h"
#include "Managers/JoystickHapticDeviceManager.h"
#include "Managers/JoystickLogManager.h"
#include "Managers/JoystickProfileManager.h"
#include "JoystickSubsystem.h"
#include "Misc/CoreMiscDefines.h"
#include "Runtime/Launch/Resources/Version.h"

const static FName JoystickCategory = "Joystick";

FJoystickInputDevice::FJoystickInputDevice(const TSharedRef<FGenericApplicationMessageHandler>& InMessageHandler) : MessageHandler(InMessageHandler)
{
	EKeys::AddMenuCategoryDisplayInfo(JoystickCategory, FText::FromString("Joystick"), TEXT("GraphEditor.PadEvent_16x"));
}

void FJoystickInputDevice::Tick(float DeltaTime)
{
}

bool FJoystickInputDevice::Exec(UWorld* InWorld, const TCHAR* Cmd, FOutputDevice& Ar)
{
	return false;
}

void FJoystickInputDevice::SetChannelValue(int ControllerId, FForceFeedbackChannelType ChannelType, float Value)
{
}

void FJoystickInputDevice::SetChannelValues(int ControllerId, const FForceFeedbackValues& Values)
{
	UJoystickHapticDeviceManager* HapticDeviceManager = GetMutableDefault<UJoystickHapticDeviceManager>();
	if (!IsValid(HapticDeviceManager))
	{
		return;
	}

	const float LargeValue = Values.LeftLarge > Values.RightLarge ? Values.LeftLarge : Values.RightLarge;
	const float SmallValue = Values.LeftSmall > Values.RightSmall ? Values.LeftSmall : Values.RightSmall;
	for (const TTuple<FJoystickInstanceId, FJoystickDeviceState>& Joystick : JoystickDeviceState)
	{
		if (SmallValue == 0 && LargeValue == 0)
		{
			continue;
		}

		HapticDeviceManager->PlayRumble(Joystick.Key, SmallValue, LargeValue, -1);
	}
}

bool FJoystickInputDevice::IsGamepadAttached() const
{
	if (!IsValid(GEngine))
	{
		return false;
	}

	const UJoystickSubsystem* JoystickSubsystem = GEngine->GetEngineSubsystem<UJoystickSubsystem>();
	if (!IsValid(JoystickSubsystem))
	{
		return false;
	}

	return JoystickSubsystem->HasRumbleDevice();
}

void FJoystickInputDevice::SetMessageHandler(const TSharedRef<FGenericApplicationMessageHandler>& InMessageHandler)
{
	MessageHandler = InMessageHandler;
}

#define LOCTEXT_NAMESPACE "JoystickPlugin"

void FJoystickInputDevice::InitialiseAxis(const FJoystickInstanceId& InstanceId, FJoystickInputDeviceConfiguration* DeviceConfiguration, const FString& BaseKeyName, const FString& BaseDisplayName)
{
	const UJoystickInputSettings* JoystickInputSettings = GetMutableDefault<UJoystickInputSettings>();
	if (!IsValid(JoystickInputSettings))
	{
		return;
	}

	const FJoystickDeviceState* JoystickState = JoystickDeviceState.Find(InstanceId);
	if (JoystickState == nullptr)
	{
		return;
	}

	DeviceAxisKeys.Emplace(InstanceId);

	const int AxisCount = JoystickState->Axes.Num();
	DeviceAxisKeys[InstanceId].SetNum(AxisCount);
	for (int AxisKeyIndex = 0; AxisKeyIndex < AxisCount; AxisKeyIndex++)
	{
		const int AxisKeyIndexName = JoystickInputSettings->ZeroBasedIndexing ? AxisKeyIndex : AxisKeyIndex + 1;
		FString AxisKeyName = FString::Printf(TEXT("%s_Axis_%d"), *BaseKeyName, AxisKeyIndexName);
		FString AxisDisplayName = FString::Printf(TEXT("%s: Axis %d"), *BaseDisplayName, AxisKeyIndexName);

		if (DeviceConfiguration)
		{
			const FJoystickInputDeviceAxisProperties* AxisProperties = DeviceConfiguration->GetAxisProperties(AxisKeyIndex);
			if (AxisProperties && AxisProperties->OverrideDisplayName)
			{
				AxisDisplayName = AxisProperties->DisplayName;
			}
		}

		const FKey AxisKey = FKey(FName(*AxisKeyName));
		TSharedPtr<FKeyDetails> ExistingKeyDetails = EKeys::GetKeyDetails(AxisKey);
		if (!ExistingKeyDetails)
		{
#if (ENGINE_MAJOR_VERSION == 4 && ENGINE_MINOR_VERSION >= 26 || ENGINE_MAJOR_VERSION == 5)
			FKeyDetails AxisKeyDetails = FKeyDetails(AxisKey, FText::FromString(AxisDisplayName), FKeyDetails::GamepadKey | FKeyDetails::Axis1D, JoystickCategory);
#else
			FKeyDetails AxisKeyDetails = FKeyDetails(AxisKey, FText::FromString(AxisDisplayName), FKeyDetails::GamepadKey | FKeyDetails::FloatAxis, JoystickCategory);
#endif
			EKeys::AddKey(AxisKeyDetails);
			FJoystickLogManager::Get()->LogDebug(TEXT("Added Axis %s (%s) %d"), *AxisKeyName, *AxisDisplayName, InstanceId.Value);
		}

		DeviceAxisKeys[InstanceId][AxisKeyIndex] = AxisKey;
		DeviceKeys[InstanceId].Add(AxisKey);
	}
}

void FJoystickInputDevice::InitialiseButtons(const FJoystickInstanceId& InstanceId, FJoystickInputDeviceConfiguration* DeviceConfiguration, const FString& BaseKeyName, const FString& BaseDisplayName)
{
	const UJoystickInputSettings* JoystickInputSettings = GetMutableDefault<UJoystickInputSettings>();
	if (!IsValid(JoystickInputSettings))
	{
		return;
	}

	const FJoystickDeviceState* JoystickState = JoystickDeviceState.Find(InstanceId);
	if (JoystickState == nullptr)
	{
		return;
	}

	DeviceButtonKeys.Emplace(InstanceId);

	const int ButtonCount = JoystickState->Buttons.Num();
	DeviceButtonKeys[InstanceId].SetNum(ButtonCount);
	for (int ButtonKeyIndex = 0; ButtonKeyIndex < ButtonCount; ButtonKeyIndex++)
	{
		const int ButtonKeyIndexName = JoystickInputSettings->ZeroBasedIndexing ? ButtonKeyIndex : ButtonKeyIndex + 1;
		FString ButtonKeyName = FString::Printf(TEXT("%s_Button_%d"), *BaseKeyName, ButtonKeyIndexName);
		FString ButtonDisplayName = FString::Printf(TEXT("%s: Button %d"), *BaseDisplayName, ButtonKeyIndexName);

		if (DeviceConfiguration)
		{
			const FJoystickInputDeviceButtonProperties* ButtonProperties = DeviceConfiguration->GetButtonProperties(ButtonKeyIndex);
			if (ButtonProperties && ButtonProperties->OverrideDisplayName)
			{
				ButtonDisplayName = ButtonProperties->DisplayName;
			}
		}

		const FKey ButtonKey = FKey(FName(*ButtonKeyName));
		TSharedPtr<FKeyDetails> ExistingKeyDetails = EKeys::GetKeyDetails(ButtonKey);
		if (!ExistingKeyDetails)
		{
			FKeyDetails ButtonKeyDetails = FKeyDetails(ButtonKey, FText::FromString(ButtonDisplayName), FKeyDetails::GamepadKey, JoystickCategory);
			EKeys::AddKey(ButtonKeyDetails);
			FJoystickLogManager::Get()->LogDebug(TEXT("Added Button %s (%s) %d"), *ButtonKeyName, *ButtonDisplayName, InstanceId.Value);
		}

		DeviceButtonKeys[InstanceId][ButtonKeyIndex] = ButtonKey;
		DeviceKeys[InstanceId].Add(ButtonKey);

		TryAddWidgetNavigation(ButtonKey);
	}
}

const TArray<FString> AxisNames = {TEXT("X"), TEXT("Y")};

void FJoystickInputDevice::InitialiseHatAxis(const FJoystickInstanceId& InstanceId, const FString& BaseKeyName, const FString& BaseDisplayName, const bool PairedKey)
{
	const UJoystickInputSettings* JoystickInputSettings = GetMutableDefault<UJoystickInputSettings>();
	if (!IsValid(JoystickInputSettings))
	{
		return;
	}

	const FJoystickDeviceState* JoystickState = JoystickDeviceState.Find(InstanceId);
	if (JoystickState == nullptr)
	{
		return;
	}

	const int HatCount = JoystickState->Hats.Num();

	DeviceHatAxisKeys.Emplace(InstanceId);
	DeviceHatAxisKeys[InstanceId].SetNum(HatCount);

	if (PairedKey)
	{
		DeviceHatAxisPairedKeys.Emplace(InstanceId);
		DeviceHatAxisPairedKeys[InstanceId].SetNum(HatCount);
	}

	for (int HatKeyIndex = 0; HatKeyIndex < HatCount; HatKeyIndex++)
	{
		for (int HatIndex = 0; HatIndex < 2; HatIndex++)
		{
			FString HatAxisName = *AxisNames[HatIndex];
			const int HatKeyIndexName = JoystickInputSettings->ZeroBasedIndexing ? HatKeyIndex : HatKeyIndex + 1;
			FString HatKeyName = FString::Printf(TEXT("%s_Hat_%d_%s"), *BaseKeyName, HatKeyIndexName, *HatAxisName);
			FString HatDisplayName = FString::Printf(TEXT("%s: Hat %d %s"), *BaseDisplayName, HatKeyIndexName, *HatAxisName);

			const FKey HatKey = FKey(FName(*HatKeyName));
			TSharedPtr<FKeyDetails> ExistingKeyDetails = EKeys::GetKeyDetails(HatKey);
			if (!ExistingKeyDetails)
			{
#if (ENGINE_MAJOR_VERSION == 4 && ENGINE_MINOR_VERSION >= 26 || ENGINE_MAJOR_VERSION == 5)
				FKeyDetails HatKeyDetails = FKeyDetails(HatKey, FText::FromString(HatDisplayName), FKeyDetails::GamepadKey | FKeyDetails::Axis1D, JoystickCategory);
#else
				FKeyDetails HatKeyDetails = FKeyDetails(HatKey, FText::FromString(HatDisplayName), FKeyDetails::GamepadKey | FKeyDetails::FloatAxis, JoystickCategory);
#endif
				EKeys::AddKey(HatKeyDetails);
				FJoystickLogManager::Get()->LogDebug(TEXT("Added Hat %s (%s) %d"), *HatKeyName, *HatDisplayName, InstanceId.Value);
			}

			DeviceHatAxisKeys[InstanceId][HatKeyIndex][HatIndex] = HatKey;
			DeviceKeys[InstanceId].Add(HatKey);
		}

#if (ENGINE_MAJOR_VERSION == 4 && ENGINE_MINOR_VERSION >= 26 || ENGINE_MAJOR_VERSION == 5)
		if (PairedKey)
		{
			const int HatKeyIndexName = JoystickInputSettings->ZeroBasedIndexing ? HatKeyIndex : HatKeyIndex + 1;
			FString HatAxisKeyName = FString::Printf(TEXT("%s_Hat_%d_2D"), *BaseKeyName, HatKeyIndexName);
			FString HatAxisDisplayName = FString::Printf(TEXT("%s: Hat %d 2D"), *BaseDisplayName, HatKeyIndexName);

			const FKey HatAxisKey = FKey(FName(*HatAxisKeyName));
			TSharedPtr<FKeyDetails> ExistingKeyDetails = EKeys::GetKeyDetails(HatAxisKey);
			if (!ExistingKeyDetails)
			{
				FKeyDetails HatAxisKeyDetails = FKeyDetails(HatAxisKey, FText::FromString(HatAxisDisplayName), FKeyDetails::GamepadKey | FKeyDetails::Axis2D, JoystickCategory);
				const FKeyPair& KeyPair = DeviceHatAxisKeys[InstanceId][HatKeyIndex];
				EKeys::AddPairedKey(HatAxisKeyDetails, KeyPair.X, KeyPair.Y);
				FJoystickLogManager::Get()->LogDebug(TEXT("Added 2D Hat %s (%s) %d"), *HatAxisKeyName, *HatAxisDisplayName, InstanceId.Value);
			}

			DeviceHatAxisPairedKeys[InstanceId][HatKeyIndex] = HatAxisKey;
			DeviceKeys[InstanceId].Add(HatAxisKey);
		}
#endif
	}
}

void FJoystickInputDevice::InitialiseHatButtons(const FJoystickInstanceId& InstanceId, const FString& BaseKeyName, const FString& BaseDisplayName)
{
	const UJoystickInputSettings* JoystickInputSettings = GetDefault<UJoystickInputSettings>();
	if (!IsValid(JoystickInputSettings))
	{
		return;
	}

	const FJoystickDeviceState* JoystickState = JoystickDeviceState.Find(InstanceId);
	if (JoystickState == nullptr)
	{
		return;
	}

	DeviceHatButtonKeys.Emplace(InstanceId);

	const int HatCount = JoystickState->Hats.Num();
	DeviceHatButtonKeys[InstanceId].SetNum(HatCount * 8);

	for (int HatKeyIndex = 0; HatKeyIndex < HatCount; HatKeyIndex++)
	{
		const UEnum* HatDirectionEnum = StaticEnum<EHatDirection>();
		for (int i = 1; i < HatDirectionEnum->NumEnums() - 1; i++)
		{
			const bool ShouldMapHatKey = JoystickInputSettings->MapHatAxisToKeys & (1 << (i - 1));
			if (!ShouldMapHatKey)
			{
				continue;
			}

			FString DirectionName = HatDirectionEnum->GetNameStringByIndex(i);
			FString DirectionDisplayName = DirectionName.Replace(TEXT("_"), TEXT(" "));

			const int HatKeyIndexName = JoystickInputSettings->ZeroBasedIndexing ? HatKeyIndex : HatKeyIndex + 1;
			FString ButtonKeyName = FString::Printf(TEXT("%s_Hat_%d_Button_%s"), *BaseKeyName, HatKeyIndexName, *DirectionName);
			FString ButtonDisplayName = FString::Printf(TEXT("%s: Hat %d Button %s"), *BaseDisplayName, HatKeyIndexName, *DirectionDisplayName);

			const FKey ButtonKey = FKey(FName(ButtonKeyName));
			TSharedPtr<FKeyDetails> ExistingKeyDetails = EKeys::GetKeyDetails(ButtonKey);
			if (!ExistingKeyDetails)
			{
				FKeyDetails ButtonKeyDetails = FKeyDetails(ButtonKey, FText::FromString(ButtonDisplayName), FKeyDetails::GamepadKey, JoystickCategory);
				EKeys::AddKey(ButtonKeyDetails);
				FJoystickLogManager::Get()->LogDebug(TEXT("Added Hat Button %s (%s) %d"), *ButtonKeyName, *ButtonDisplayName, InstanceId.Value);
			}

			const int HatButtonIndex = HatKeyIndex * 8 + (i - 1);
			DeviceHatButtonKeys[InstanceId][HatButtonIndex] = ButtonKey;
			DeviceKeys[InstanceId].Add(ButtonKey);

			TryAddWidgetNavigation(ButtonKey);
		}
	}
}

void FJoystickInputDevice::InitialiseBalls(const FJoystickInstanceId& InstanceId, const FString& BaseKeyName, const FString& BaseDisplayName, const bool PairedKey)
{
	const UJoystickInputSettings* JoystickInputSettings = GetMutableDefault<UJoystickInputSettings>();
	if (!IsValid(JoystickInputSettings))
	{
		return;
	}

	const FJoystickDeviceState* JoystickState = JoystickDeviceState.Find(InstanceId);
	if (JoystickState == nullptr)
	{
		return;
	}

	const int BallCount = JoystickState->Balls.Num();

	DeviceBallKeys.Emplace(InstanceId);
	DeviceBallKeys[InstanceId].SetNum(BallCount);

	if (PairedKey)
	{
		DeviceBallPairedKeys.Emplace(InstanceId);
		DeviceBallPairedKeys[InstanceId].SetNum(BallCount);
	}

	for (int BallKeyIndex = 0; BallKeyIndex < BallCount; BallKeyIndex++)
	{
		for (int BallIndex = 0; BallIndex < 2; BallIndex++)
		{
			FString BallAxisName = *AxisNames[BallIndex];
			const int BallKeyIndexName = JoystickInputSettings->ZeroBasedIndexing ? BallKeyIndex : BallKeyIndex + 1;
			FString BallKeyName = FString::Printf(TEXT("%s_Ball_%d_%s"), *BaseKeyName, BallKeyIndexName, *BallAxisName);
			FString BallDisplayName = FString::Printf(TEXT("%s: Ball %d %s"), *BaseDisplayName, BallKeyIndexName, *BallAxisName);

			const FKey BallKey = FKey(FName(*BallKeyName));
			TSharedPtr<FKeyDetails> ExistingKeyDetails = EKeys::GetKeyDetails(BallKey);
			if (!ExistingKeyDetails)
			{
#if (ENGINE_MAJOR_VERSION == 4 && ENGINE_MINOR_VERSION >= 26 || ENGINE_MAJOR_VERSION == 5)
				FKeyDetails BallKeyDetails = FKeyDetails(BallKey, FText::FromString(BallDisplayName), FKeyDetails::GamepadKey | FKeyDetails::Axis1D, JoystickCategory);
#else
				FKeyDetails BallKeyDetails = FKeyDetails(BallKey, FText::FromString(BallDisplayName), FKeyDetails::GamepadKey | FKeyDetails::FloatAxis, JoystickCategory);
#endif
				EKeys::AddKey(BallKeyDetails);
				FJoystickLogManager::Get()->LogDebug(TEXT("Added Ball %s (%s) %d"), *BallKeyName, *BallDisplayName, InstanceId.Value);
			}

			DeviceBallKeys[InstanceId][BallKeyIndex][BallIndex] = BallKey;
			DeviceKeys[InstanceId].Add(BallKey);
		}

#if (ENGINE_MAJOR_VERSION == 4 && ENGINE_MINOR_VERSION >= 26 || ENGINE_MAJOR_VERSION == 5)
		if (PairedKey)
		{
			const int BallKeyIndexName = JoystickInputSettings->ZeroBasedIndexing ? BallKeyIndex : BallKeyIndex + 1;
			FString BallKeyName = FString::Printf(TEXT("%s_Ball_%d_2D"), *BaseKeyName, BallKeyIndexName);
			FString BallDisplayName = FString::Printf(TEXT("%s: Ball %d 2D"), *BaseDisplayName, BallKeyIndexName);

			const FKey BallKey = FKey(FName(*BallKeyName));
			TSharedPtr<FKeyDetails> ExistingKeyDetails = EKeys::GetKeyDetails(BallKey);
			if (!ExistingKeyDetails)
			{
				FKeyDetails BallKeyDetails = FKeyDetails(BallKey, FText::FromString(BallDisplayName), FKeyDetails::GamepadKey | FKeyDetails::Axis2D, JoystickCategory);
				const FKeyPair& KeyPair = DeviceBallKeys[InstanceId][BallKeyIndex];
				EKeys::AddPairedKey(BallKeyDetails, KeyPair.X, KeyPair.Y);
				FJoystickLogManager::Get()->LogDebug(TEXT("Added 2D Ball %s (%s) %d"), *BallKeyName, *BallDisplayName, InstanceId.Value);
			}

			DeviceBallPairedKeys[InstanceId][BallKeyIndex] = BallKey;
			DeviceKeys[InstanceId].Add(BallKey);
		}
#endif
	}
}

#undef LOCTEXT_NAMESPACE

#if ENGINE_MAJOR_VERSION == 5 && ENGINE_MINOR_VERSION >= 1
void FJoystickInputDevice::NotifyDeviceState(const FInputDeviceId& InputDeviceId, const FPlatformUserId& PlatformUserId, const EInputDeviceConnectionState State) const
{
	IPlatformInputDeviceMapper& DeviceMapper = IPlatformInputDeviceMapper::Get();
	DeviceMapper.Internal_MapInputDeviceToUser(InputDeviceId, PlatformUserId, State);
}
#endif

void FJoystickInputDevice::JoystickPluggedIn(const FDeviceInfoSDL& Device)
{
	if (!IsValid(GEngine))
	{
		return;
	}

	UJoystickSubsystem* JoystickSubsystem = GEngine->GetEngineSubsystem<UJoystickSubsystem>();
	if (!IsValid(JoystickSubsystem))
	{
		return;
	}

	const FJoystickDeviceState InitialDeviceState = JoystickSubsystem->CreateInitialDeviceState(Device.InstanceId);
	JoystickDeviceState.Emplace(Device.InstanceId, InitialDeviceState);

	UJoystickInputSettings* JoystickInputSettings = GetMutableDefault<UJoystickInputSettings>();
	if (!IsValid(JoystickInputSettings))
	{
		return;
	}

	UJoystickProfileManager* JoystickProfileManager = GetMutableDefault<UJoystickProfileManager>();
	if (!IsValid(JoystickProfileManager))
	{
		return;
	}

	FJoystickInputDeviceConfiguration* DeviceConfig = JoystickProfileManager->GetInputDeviceConfiguration(Device);

	FString BaseKeyName("Joystick");
	FString BaseDisplayName("Joystick");
	if (JoystickInputSettings->UseDeviceName)
	{
		FString DeviceName = Device.SafeDeviceName;
		if (DeviceConfig && DeviceConfig->OverrideDeviceName)
		{
			DeviceName = UJoystickFunctionLibrary::SanitiseDeviceName(DeviceConfig->DeviceName);
		}

		BaseKeyName = FString::Printf(TEXT("Joystick_%s"), *DeviceName);
		BaseDisplayName = DeviceName;
	}

	if (JoystickInputSettings->IncludeDeviceIndex)
	{
		const int InputDeviceId = Device.GetInputDeviceId().GetId();
		BaseKeyName = FString::Printf(TEXT("%s_%d"), *BaseKeyName, InputDeviceId);
		BaseDisplayName = FString::Printf(TEXT("%s %d"), *BaseDisplayName, InputDeviceId);
	}

	DeviceKeys.Emplace(Device.InstanceId);

	// create FKeyDetails for inputs
	const bool EnablePairedKeys = JoystickInputSettings->EnablePairedKeys;
	InitialiseAxis(Device.InstanceId, DeviceConfig, BaseKeyName, BaseDisplayName);
	InitialiseButtons(Device.InstanceId, DeviceConfig, BaseKeyName, BaseDisplayName);
	InitialiseHatAxis(Device.InstanceId, BaseKeyName, BaseDisplayName, EnablePairedKeys);
	InitialiseHatButtons(Device.InstanceId, BaseKeyName, BaseDisplayName);
	InitialiseBalls(Device.InstanceId, BaseKeyName, BaseDisplayName, EnablePairedKeys);

	JoystickInputSettings->DeviceAdded(static_cast<FJoystickInformation>(Device));

	if (UInputSettings* InputSettings = UInputSettings::GetInputSettings())
	{
		InputSettings->PostInitProperties();
	}

	UpdateDeviceProperties();

#if ENGINE_MAJOR_VERSION == 5 && ENGINE_MINOR_VERSION >= 1
	NotifyDeviceState(Device.GetInputDeviceId(), Device.GetPlatformUserId(), EInputDeviceConnectionState::Connected);
#endif

	FJoystickLogManager::Get()->LogInformation(TEXT("Device Ready: %s (Device Id: %d) - Instance Id: %d"), *Device.DeviceName, Device.GetInputDeviceId().GetId(), Device.InstanceId.Value);
}

void FJoystickInputDevice::JoystickUnplugged(const FJoystickInstanceId& InstanceId, const FInputDeviceId& InputDeviceId) const
{
	UJoystickInputSettings* JoystickInputSettings = GetMutableDefault<UJoystickInputSettings>();
	if (!IsValid(JoystickInputSettings))
	{
		return;
	}

	JoystickInputSettings->DeviceRemoved(InstanceId);

#if ENGINE_MAJOR_VERSION == 5 && ENGINE_MINOR_VERSION >= 1
	if (JoystickDeviceState.Contains(InstanceId))
	{
		// Device managed by subsystem
		const IPlatformInputDeviceMapper& DeviceMapper = IPlatformInputDeviceMapper::Get();
		const FPlatformUserId NewUserToAssign = DeviceMapper.GetUserForUnpairedInputDevices();
		NotifyDeviceState(InputDeviceId, NewUserToAssign, EInputDeviceConnectionState::Disconnected);
	}
#endif
}

void FJoystickInputDevice::JoystickButton(const FJoystickInstanceId& InstanceId, const int Button, const bool Pressed)
{
	auto [DeviceState, Result] = GetDeviceState(InstanceId);
	if (Result.bSuccess == false || DeviceState == nullptr)
	{
		return;
	}

	if (!DeviceState->Buttons.IsValidIndex(Button))
	{
		return;
	}

	FButtonData& State = DeviceState->Buttons[Button];
	State.Update(Pressed);

	FJoystickLogManager::Get()->LogDebug(TEXT("Event JoystickButton Device=%d Button=%d State=%d"), InstanceId.Value, Button, State.GetValue());
}

void FJoystickInputDevice::JoystickAxis(const FJoystickInstanceId& InstanceId, const int Axis, const float Value)
{
	auto [DeviceState, Result] = GetDeviceState(InstanceId);
	if (Result.bSuccess == false || DeviceState == nullptr)
	{
		return;
	}

	if (!DeviceState->Axes.IsValidIndex(Axis))
	{
		return;
	}

	FAxisData& State = DeviceState->Axes[Axis];
	State.Update(Value);
}

void FJoystickInputDevice::JoystickHat(const FJoystickInstanceId& InstanceId, const int Hat, const EHatDirection Value)
{
	auto [DeviceState, Result] = GetDeviceState(InstanceId);
	if (Result.bSuccess == false || DeviceState == nullptr)
	{
		return;
	}

	if (!DeviceState->Hats.IsValidIndex(Hat))
	{
		return;
	}

	FHatData& State = DeviceState->Hats[Hat];
	State.Update(Value);
}

void FJoystickInputDevice::JoystickBall(const FJoystickInstanceId& InstanceId, const int Ball, const FVector2D& Value)
{
	auto [DeviceState, Result] = GetDeviceState(InstanceId);
	if (Result.bSuccess == false || DeviceState == nullptr)
	{
		return;
	}

	if (!DeviceState->Balls.IsValidIndex(Ball))
	{
		return;
	}

	FBallData& State = DeviceState->Balls[Ball];
	State.Update(Value);
}

void FJoystickInputDevice::JoystickGyro(const FJoystickInstanceId& InstanceId, const FVector& Value)
{
	auto [DeviceState, Result] = GetDeviceState(InstanceId);
	if (Result.bSuccess == false || DeviceState == nullptr)
	{
		return;
	}

	DeviceState->Motion.UpdateGyro(Value);
}

void FJoystickInputDevice::JoystickAccelerometer(const FJoystickInstanceId& InstanceId, const FVector& Value)
{
	auto [DeviceState, Result] = GetDeviceState(InstanceId);
	if (Result.bSuccess == false || DeviceState == nullptr)
	{
		return;
	}

	DeviceState->Motion.UpdateAccelerometer(Value);
}

TTuple<FJoystickDeviceState*, FInternalResultMessage> FJoystickInputDevice::GetDeviceState(const FJoystickInstanceId& InstanceId)
{
	if (!JoystickDeviceState.Contains(InstanceId))
	{
		return {nullptr, FInternalResultMessage(false, TEXT("Device is not a valid Joystick device"))};
	}

	return {&JoystickDeviceState[InstanceId], FInternalResultMessage(true)};
}

void FJoystickInputDevice::GetDeviceKeys(const FJoystickInstanceId& InstanceId, TArray<FKey>& Keys)
{
	if (!DeviceKeys.Contains(InstanceId))
	{
		return;
	}

	Keys = DeviceKeys[InstanceId];
}

const FKey& FJoystickInputDevice::GetDeviceAxisKey(const FJoystickInstanceId& InstanceId, const int AxisIndex)
{
	return DeviceAxisKeys[InstanceId][AxisIndex];
}

const FKey& FJoystickInputDevice::GetDeviceButtonKey(const FJoystickInstanceId& InstanceId, const int ButtonIndex)
{
	return DeviceButtonKeys[InstanceId][ButtonIndex];
}

const FKeyPair& FJoystickInputDevice::GetDeviceHatKey(const FJoystickInstanceId& InstanceId, const int HatKeyIndex)
{
	return DeviceHatAxisKeys[InstanceId][HatKeyIndex];
}

static FName JoystickInputInterfaceName = FName("JoystickPluginInput");

void FJoystickInputDevice::SendControllerEvents()
{
	if (!IsValid(GEngine))
	{
		return;
	}

	UJoystickSubsystem* JoystickSubsystem = GEngine->GetEngineSubsystem<UJoystickSubsystem>();
	if (!IsValid(JoystickSubsystem))
	{
		return;
	}

	const UJoystickInputSettings* JoystickInputSettings = GetDefault<UJoystickInputSettings>();

	for (const TPair<FJoystickInstanceId, FDeviceInfoSDL>& Device : JoystickSubsystem->GetDevices())
	{
		const FJoystickInstanceId& InstanceId = Device.Key;
		const FDeviceInfoSDL& DeviceInfo = Device.Value;

		if (!JoystickDeviceState.Contains(InstanceId))
		{
			continue;
		}

		FJoystickDeviceState& CurrentState = JoystickDeviceState[InstanceId];

		const bool IsConnected = JoystickSubsystem->IsConnected(InstanceId);
		if (!IsConnected)
		{
			continue;
		}

		FInputDeviceScope InputScope(this, JoystickInputInterfaceName, InstanceId, DeviceInfo.DeviceName);

		// Axis
		if (DeviceAxisKeys.Contains(InstanceId))
		{
			for (int AxisIndex = 0; AxisIndex < CurrentState.Axes.Num(); AxisIndex++)
			{
				const FKey& AxisKey = DeviceAxisKeys[InstanceId][AxisIndex];
				if (AxisKey.IsValid())
				{
					FAxisData& AxisData = CurrentState.Axes[AxisIndex];
					const float CurrentValue = AxisData.GetValue();
					const float PreviousValue = AxisData.GetPreviousValue();
					if (CurrentValue == PreviousValue)
					{
						continue;
					}

#if WITH_EDITOR
					if (IsValid(JoystickInputSettings) && JoystickInputSettings->DebugAxis)
					{
						const int Key = DeviceInfo.GetInputDeviceId().GetId() * CurrentState.Axes.Num() + AxisIndex;
						GEngine->AddOnScreenDebugMessage(
							Key,
							0.3f,
							FColor::Green,
							FString::Printf(TEXT("Axis %s Raw: %.3f Mapped: %.3f"), *AxisKey.GetDisplayName().ToString(), AxisData.Value, CurrentValue)
						);
					}
#endif

#if ENGINE_MAJOR_VERSION == 5 && ENGINE_MINOR_VERSION >= 1
					MessageHandler->OnControllerAnalog(AxisKey.GetFName(), DeviceInfo.GetPlatformUserId(), DeviceInfo.GetInputDeviceId(), CurrentValue);
#else
					MessageHandler->OnControllerAnalog(AxisKey.GetFName(), DeviceInfo.GetPlatformUserId(), CurrentValue);
#endif

					AxisData.Processed();
				}
			}
		}

		// Hats Axis
		if (DeviceHatAxisKeys.Contains(InstanceId))
		{
			for (int HatKeyIndex = 0; HatKeyIndex < CurrentState.Hats.Num(); HatKeyIndex++)
			{
				const FKey& XHatKey = DeviceHatAxisKeys[InstanceId][HatKeyIndex][0];
				const FKey& YHatKey = DeviceHatAxisKeys[InstanceId][HatKeyIndex][1];
				if (XHatKey.IsValid() && YHatKey.IsValid())
				{
					FHatData& HatData = CurrentState.Hats[HatKeyIndex];
					const EHatDirection CurrentValue = HatData.GetValue();
					const EHatDirection PreviousValue = HatData.GetPreviousValue();
					if (CurrentValue == PreviousValue)
					{
						continue;
					}

					const FVector2D& POVAxis = UJoystickFunctionLibrary::HatDirectionToFVector2D(CurrentValue);
#if ENGINE_MAJOR_VERSION == 5 && ENGINE_MINOR_VERSION >= 1
					MessageHandler->OnControllerAnalog(XHatKey.GetFName(), DeviceInfo.GetPlatformUserId(), DeviceInfo.GetInputDeviceId(), POVAxis.X);
					MessageHandler->OnControllerAnalog(YHatKey.GetFName(), DeviceInfo.GetPlatformUserId(), DeviceInfo.GetInputDeviceId(), POVAxis.Y);
#else
					MessageHandler->OnControllerAnalog(XHatKey.GetFName(), DeviceInfo.GetPlatformUserId(), POVAxis.X);
					MessageHandler->OnControllerAnalog(YHatKey.GetFName(), DeviceInfo.GetPlatformUserId(), POVAxis.Y);
#endif
				}
			}
		}

		// Hats Buttons
		if (DeviceHatButtonKeys.Contains(InstanceId))
		{
			const UEnum* HatDirectionEnum = StaticEnum<EHatDirection>();
			for (int HatKeyIndex = 0; HatKeyIndex < CurrentState.Hats.Num(); HatKeyIndex++)
			{
				FHatData& HatData = CurrentState.Hats[HatKeyIndex];
				const EHatDirection CurrentValue = HatData.GetValue();
				const EHatDirection PreviousValue = HatData.GetPreviousValue();
				if (CurrentValue == PreviousValue)
				{
					continue;
				}

				if (PreviousValue != EHatDirection::None)
				{
					const uint8 DirectionIndex = HatDirectionEnum->GetIndexByValue(static_cast<int64>(PreviousValue)) - 1;
					const FKey& DirectionKey = DeviceHatButtonKeys[InstanceId][HatKeyIndex * 8 + DirectionIndex];
					if (DirectionKey.IsValid())
					{
#if ENGINE_MAJOR_VERSION == 5 && ENGINE_MINOR_VERSION >= 1
						MessageHandler->OnControllerButtonReleased(DirectionKey.GetFName(), DeviceInfo.GetPlatformUserId(), DeviceInfo.GetInputDeviceId(), false);
#else
						MessageHandler->OnControllerButtonReleased(DirectionKey.GetFName(), DeviceInfo.GetPlatformUserId(), false);
#endif
					}
				}

				if (CurrentValue != EHatDirection::None)
				{
					const uint8 DirectionIndex = HatDirectionEnum->GetIndexByValue(static_cast<int64>(CurrentValue)) - 1;
					const FKey& DirectionKey = DeviceHatButtonKeys[InstanceId][HatKeyIndex * 8 + DirectionIndex];
					if (DirectionKey.IsValid())
					{
#if ENGINE_MAJOR_VERSION == 5 && ENGINE_MINOR_VERSION >= 1
						MessageHandler->OnControllerButtonPressed(DirectionKey.GetFName(), DeviceInfo.GetPlatformUserId(), DeviceInfo.GetInputDeviceId(), false);
#else
						MessageHandler->OnControllerButtonPressed(DirectionKey.GetFName(), DeviceInfo.GetPlatformUserId(), false);
#endif
					}
				}

				HatData.Processed();
			}
		}

		// Balls
		if (DeviceBallKeys.Contains(InstanceId))
		{
			for (int BallIndex = 0; BallIndex < CurrentState.Balls.Num(); BallIndex++)
			{
				const FKey& XBallKey = DeviceBallKeys[InstanceId][BallIndex][0];
				const FKey& YBallKey = DeviceBallKeys[InstanceId][BallIndex][1];
				if (XBallKey.IsValid() && YBallKey.IsValid())
				{
					FBallData& BallData = CurrentState.Balls[BallIndex];
					const FVector2D& CurrentValue = BallData.GetValue();
					const FVector2D& PreviousValue = BallData.GetPreviousValue();
					if (CurrentValue == PreviousValue)
					{
						continue;
					}

#if ENGINE_MAJOR_VERSION == 5 && ENGINE_MINOR_VERSION >= 1
					MessageHandler->OnControllerAnalog(XBallKey.GetFName(), DeviceInfo.GetPlatformUserId(), DeviceInfo.GetInputDeviceId(), CurrentValue.X);
					MessageHandler->OnControllerAnalog(YBallKey.GetFName(), DeviceInfo.GetPlatformUserId(), DeviceInfo.GetInputDeviceId(), CurrentValue.Y);
#else
					MessageHandler->OnControllerAnalog(XBallKey.GetFName(), DeviceInfo.GetPlatformUserId(), CurrentValue.X);
					MessageHandler->OnControllerAnalog(YBallKey.GetFName(), DeviceInfo.GetPlatformUserId(), CurrentValue.Y);
#endif

					BallData.Processed();
				}
			}
		}

		// Buttons
		if (DeviceButtonKeys.Contains(InstanceId))
		{
			for (int ButtonIndex = 0; ButtonIndex < CurrentState.Buttons.Num(); ButtonIndex++)
			{
				const FKey& ButtonKey = DeviceButtonKeys[InstanceId][ButtonIndex];
				if (ButtonKey.IsValid())
				{
					FButtonData& ButtonData = CurrentState.Buttons[ButtonIndex];
					const bool CurrentValue = ButtonData.GetValue();
					const bool PreviousValue = ButtonData.GetPreviousValue();
					if (CurrentValue == PreviousValue)
					{
						continue;
					}

					if (CurrentValue)
					{
#if ENGINE_MAJOR_VERSION == 5 && ENGINE_MINOR_VERSION >= 1
						MessageHandler->OnControllerButtonPressed(ButtonKey.GetFName(), DeviceInfo.GetPlatformUserId(), DeviceInfo.GetInputDeviceId(), false);
#else
						MessageHandler->OnControllerButtonPressed(ButtonKey.GetFName(), DeviceInfo.GetPlatformUserId(), false);
#endif
					}
					else
					{
#if ENGINE_MAJOR_VERSION == 5 && ENGINE_MINOR_VERSION >= 1
						MessageHandler->OnControllerButtonReleased(ButtonKey.GetFName(), DeviceInfo.GetPlatformUserId(), DeviceInfo.GetInputDeviceId(), false);
#else
						MessageHandler->OnControllerButtonReleased(ButtonKey.GetFName(), DeviceInfo.GetPlatformUserId(), false);
#endif
					}

					ButtonData.Processed();
				}
			}
		}

		// Gyro
#if ENGINE_MAJOR_VERSION == 5 && ENGINE_MINOR_VERSION >= 1
		MessageHandler->OnMotionDetected(FVector::ZeroVector, CurrentState.Motion.Gyro, FVector::ZeroVector, CurrentState.Motion.Accelerometer, DeviceInfo.GetPlatformUserId(), DeviceInfo.GetInputDeviceId());
#else
		MessageHandler->OnMotionDetected(FVector::ZeroVector, CurrentState.Motion.Gyro, FVector::ZeroVector, CurrentState.Motion.Accelerometer, DeviceInfo.GetPlatformUserId());
#endif
	}

	JoystickSubsystem->Update();
}

FJoystickInstanceId FJoystickInputDevice::GetInstanceIdByKey(const FKey& Key) const
{
	for (const TPair<FJoystickInstanceId, TArray<FKey>>& Device : DeviceKeys)
	{
		for (const FKey& DeviceKey : Device.Value)
		{
			if (DeviceKey == Key)
			{
				return Device.Key;
			}
		}
	}

	return -1;
}

int FJoystickInputDevice::GetAxisIndexFromKey(const FKey& Key) const
{
	for (int i = 0; i < DeviceAxisKeys.Num(); i++)
	{
		if (!DeviceAxisKeys.Contains(i))
		{
			continue;
		}

		const TArray<FKey>& Keys = DeviceAxisKeys[i];
		for (int AxisIndex = 0; AxisIndex < Keys.Num(); AxisIndex++)
		{
			if (Keys[AxisIndex] == Key)
			{
				return AxisIndex;
			}
		}
	}

	return -1;
}

int FJoystickInputDevice::GetButtonIndexFromKey(const FKey& Key) const
{
	for (int i = 0; i < DeviceButtonKeys.Num(); i++)
	{
		if (!DeviceButtonKeys.Contains(i))
		{
			continue;
		}

		const TArray<FKey>& Keys = DeviceButtonKeys[i];
		for (int ButtonIndex = 0; ButtonIndex < Keys.Num(); ButtonIndex++)
		{
			if (Keys[ButtonIndex] == Key)
			{
				return ButtonIndex;
			}
		}
	}

	return -1;
}

void FJoystickInputDevice::ResetAxisProperties()
{
	for (TPair<FJoystickInstanceId, FJoystickDeviceState>& DeviceData : JoystickDeviceState)
	{
		for (int i = 0; i < DeviceData.Value.Axes.Num(); i++)
		{
			const FAxisData Defaults;
			FAxisData& AxisKeyData = DeviceData.Value.Axes[i];
			AxisKeyData.RemappingEnabled = Defaults.RemappingEnabled;
			AxisKeyData.InputOffset = Defaults.InputOffset;
			AxisKeyData.InvertInput = Defaults.InvertInput;
			AxisKeyData.InputRangeMin = Defaults.InputRangeMin;
			AxisKeyData.InputRangeMax = Defaults.InputRangeMax;
			AxisKeyData.OutputRangeMin = Defaults.OutputRangeMin;
			AxisKeyData.OutputRangeMax = Defaults.OutputRangeMax;
			AxisKeyData.InvertOutput = Defaults.InvertOutput;
		}
	}
}

void FJoystickInputDevice::ResetButtonProperties()
{
	for (TPair<FJoystickInstanceId, FJoystickDeviceState>& DeviceData : JoystickDeviceState)
	{
		for (int i = 0; i < DeviceData.Value.Buttons.Num(); i++)
		{
			const FAxisData Defaults;
			FButtonData& ButtonKeyData = DeviceData.Value.Buttons[i];
			ButtonKeyData.InvertOutput = Defaults.InvertOutput;
		}
	}
}

void FJoystickInputDevice::UpdateDeviceProperties()
{
	ResetAxisProperties();
	ResetButtonProperties();

	UJoystickProfileManager* JoystickProfileManager = GetMutableDefault<UJoystickProfileManager>();
	if (!IsValid(JoystickProfileManager))
	{
		return;
	}

	if (!IsValid(GEngine))
	{
		return;
	}

	UJoystickSubsystem* JoystickSubsystem = GEngine->GetEngineSubsystem<UJoystickSubsystem>();
	if (!IsValid(JoystickSubsystem))
	{
		return;
	}

	for (const TPair<FJoystickInstanceId, FDeviceInfoSDL>& Device : JoystickSubsystem->GetDevices())
	{
		const FJoystickInstanceId& InstanceId = Device.Key;
		const FDeviceInfoSDL& DeviceInfo = Device.Value;

		FJoystickInputDeviceConfiguration* DeviceConfig = JoystickProfileManager->GetInputDeviceConfiguration(DeviceInfo);
		if (DeviceConfig == nullptr)
		{
			continue;
		}

		if (!JoystickDeviceState.Contains(InstanceId))
		{
			continue;
		}

		FJoystickDeviceState& CurrentState = JoystickDeviceState[InstanceId];
		UpdateAxisProperties(DeviceConfig, CurrentState);
		UpdateButtonProperties(DeviceConfig, CurrentState);
	}
}

void FJoystickInputDevice::UpdateAxisProperties(FJoystickInputDeviceConfiguration* DeviceConfig, FJoystickDeviceState& CurrentState) const
{
	for (int i = 0; i < CurrentState.Axes.Num(); i++)
	{
		const FJoystickInputDeviceAxisProperties* AxisProperties = DeviceConfig->GetAxisProperties(i);
		if (AxisProperties == nullptr)
		{
			continue;
		}

		FAxisData& AxisKeyData = CurrentState.Axes[i];
		AxisKeyData.RemappingEnabled = AxisProperties->RemappingEnabled;
		if (AxisKeyData.RemappingEnabled)
		{
			if ((AxisProperties->InputRangeMin == -1.0f && AxisProperties->InputRangeMax == 1.0f && AxisProperties->OutputRangeMin == 0.0f && AxisProperties->OutputRangeMax == 1.0f) ||
				(AxisProperties->InputRangeMin == 0.0f && AxisProperties->InputRangeMax == 1.0f && AxisProperties->OutputRangeMin == 0.0f && AxisProperties->OutputRangeMax == 1.0f))
			{
				AxisKeyData.Value = AxisProperties->InvertInput ? -AxisProperties->InputRangeMin : AxisProperties->InputRangeMin;
			}
			else if (AxisProperties->InputRangeMin == -1.0f && AxisProperties->InputRangeMax == 1.0f && AxisProperties->OutputRangeMin == -1.0f && AxisProperties->OutputRangeMax == 1.0f)
			{
				AxisKeyData.Value = 0.0f;
			}
			else
			{
				AxisKeyData.Value = AxisProperties->InputRangeMin;
			}
			AxisKeyData.PreviousValue = AxisKeyData.Value;
		}
		AxisKeyData.InputOffset = AxisProperties->InputOffset;
		AxisKeyData.InvertInput = AxisProperties->InvertInput;
		AxisKeyData.InputRangeMin = AxisProperties->InputRangeMin;
		AxisKeyData.InputRangeMax = AxisProperties->InputRangeMax;
		AxisKeyData.OutputRangeMin = AxisProperties->OutputRangeMin;
		AxisKeyData.OutputRangeMax = AxisProperties->OutputRangeMax;
		AxisKeyData.InvertOutput = AxisProperties->InvertOutput;
	}
}

void FJoystickInputDevice::UpdateButtonProperties(FJoystickInputDeviceConfiguration* DeviceConfig, FJoystickDeviceState& CurrentState) const
{
	for (int i = 0; i < CurrentState.Buttons.Num(); i++)
	{
		const FJoystickInputDeviceButtonProperties* ButtonProperties = DeviceConfig->GetButtonProperties(i);
		if (ButtonProperties == nullptr)
		{
			continue;
		}

		FButtonData& ButtonKeyData = CurrentState.Buttons[i];
		ButtonKeyData.InvertOutput = ButtonProperties->InvertOutput;
	}
}

void FJoystickInputDevice::TryAddWidgetNavigation(const FKey& ButtonKey) const
{
	const UJoystickInputSettings* JoystickInputSettings = GetDefault<UJoystickInputSettings>();
	if (!IsValid(JoystickInputSettings))
	{
		return;
	}

	const FJoystickInputKeyConfiguration* KeyConfiguration = JoystickInputSettings->KeyConfigurations.Find(ButtonKey);
	if (!KeyConfiguration)
	{
		return;
	}

	const FJoystickInputKeyConfiguration KeyConfig = *KeyConfiguration;
	AsyncTask(ENamedThreads::GameThread, [KeyConfig, ButtonKey]
	{
		const FSlateApplication& SlateApplication = FSlateApplication::Get();
		const TSharedPtr<FNavigationConfig> NavigationConfig = SlateApplication.GetNavigationConfig();
		if (!NavigationConfig)
		{
			return;
		}

		// Key Event (Up, Down, Left, Right, etc.)
		if (KeyConfig.Direction != EUINavigation::Invalid)
		{
			FJoystickLogManager::Get()->LogDebug(TEXT("Added FKey (%s) to Slate Navigation Events with direction: %s"), *ButtonKey.GetDisplayName().ToString(), *UEnum::GetValueAsString(KeyConfig.Direction));
			NavigationConfig->KeyEventRules.Add(ButtonKey, KeyConfig.Direction);
		}

#if ENGINE_MAJOR_VERSION >= 5 && ENGINE_MINOR_VERSION >= 4
		// Key Action (Accept, Back, etc.)
		if (KeyConfig.Action != EUINavigationAction::Invalid)
		{
			FJoystickLogManager::Get()->LogDebug(TEXT("Added FKey (%s) to Slate Navigation Actions with action: %s"), *ButtonKey.GetDisplayName().ToString(), *UEnum::GetValueAsString(KeyConfig.Action));
			NavigationConfig->KeyActionRules.Add(ButtonKey, KeyConfig.Action);
		}
#endif
	});
}
