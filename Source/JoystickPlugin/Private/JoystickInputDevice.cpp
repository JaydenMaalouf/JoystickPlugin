// JoystickPlugin is licensed under the MIT License.
// Copyright Jayden Maalouf. All Rights Reserved.

#include "JoystickInputDevice.h"

#include "Async/Async.h"
#include "Data/DeviceInfoSDL.h"
#include "Data/Input/AxisData.h"
#include "Data/Input/BallData.h"
#include "Data/Input/HatData.h"
#include "Data/Input/KeyPair.h"
#include "Data/JoystickDeviceState.h"
#include "Data/JoystickInstanceId.h"
#include "Data/Settings/JoystickInputDeviceAxisProperties.h"
#include "Data/Settings/JoystickInputDeviceConfiguration.h"
#include "Data/Settings/JoystickInputKeyConfiguration.h"
#include "Engine/Engine.h"
#include "Framework/Application/NavigationConfig.h"
#include "Framework/Application/SlateApplication.h"
#include "GameFramework/InputSettings.h"
#include "GenericPlatform/IInputInterface.h"
#include "JoystickFunctionLibrary.h"
#include "JoystickHapticDeviceManager.h"
#include "JoystickLogManager.h"
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
#if ENGINE_MAJOR_VERSION == 5
	UJoystickHapticDeviceManager* HapticDeviceManager = UJoystickHapticDeviceManager::GetJoystickHapticDeviceManager();
	if (!IsValid(HapticDeviceManager))
	{
		return;
	}

	const float LargeValue = (Values.LeftLarge > Values.RightLarge ? Values.LeftLarge : Values.RightLarge);
	const float SmallValue = (Values.LeftSmall > Values.RightSmall ? Values.LeftSmall : Values.RightSmall);
	for (const TTuple<FJoystickInstanceId, FJoystickDeviceState>& Joystick : JoystickDeviceState)
	{
		if (SmallValue == 0 && LargeValue == 0)
		{
			continue;
		}

		HapticDeviceManager->PlayRumble(Joystick.Key, SmallValue, LargeValue, -1);
	}
#endif
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

#define LOCTEXT_NAMESPACE "JoystickNamespace"

void FJoystickInputDevice::InitialiseAxis(const FJoystickInstanceId& InstanceId, const FJoystickInputDeviceConfiguration* DeviceConfiguration, const FString& BaseKeyName, const FString& BaseDisplayName)
{
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
		FString AxisKeyName = FString::Printf(TEXT("%s_Axis_%d"), *BaseKeyName, AxisKeyIndex);
		FString AxisDisplayName = FString::Printf(TEXT("%s: Axis %d"), *BaseDisplayName, AxisKeyIndex);

		if (DeviceConfiguration)
		{
			const FJoystickInputDeviceAxisProperties* AxisProperties = DeviceConfiguration->GetAxisProperties(AxisKeyIndex);
			if (AxisProperties && AxisProperties->OverrideDisplayName)
			{
				AxisDisplayName = AxisProperties->DisplayName;
			}
		}

		const FKey AxisKey = FKey(FName(*AxisKeyName));

#if (ENGINE_MAJOR_VERSION == 4 && ENGINE_MINOR_VERSION >= 26 || ENGINE_MAJOR_VERSION == 5)
		FKeyDetails AxisKeyDetails = FKeyDetails(AxisKey, FText::FromString(AxisDisplayName), FKeyDetails::GamepadKey | FKeyDetails::Axis1D, JoystickCategory);
#else
		FKeyDetails AxisKeyDetails = FKeyDetails(AxisKey, FText::FromString(AxisDisplayName), FKeyDetails::GamepadKey | FKeyDetails::FloatAxis, JoystickCategory);
#endif

		if (!EKeys::GetKeyDetails(AxisKey).IsValid())
		{
			EKeys::AddKey(AxisKeyDetails);
			FJoystickLogManager::Get()->LogDebug(TEXT("Added Axis %s (%s) %d"), *AxisKeyName, *AxisDisplayName, InstanceId.Value);
		}

		const FKey& MappedKey = AxisKeyDetails.GetKey();
		DeviceAxisKeys[InstanceId][AxisKeyIndex] = MappedKey;
		DeviceKeys[InstanceId].Add(MappedKey);
	}
}

void FJoystickInputDevice::InitialiseButtons(const FJoystickInstanceId& InstanceId, const FJoystickInputDeviceConfiguration* DeviceConfiguration, const FString& BaseKeyName, const FString& BaseDisplayName)
{
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
		FString ButtonKeyName = FString::Printf(TEXT("%s_Button_%d"), *BaseKeyName, ButtonKeyIndex);
		FString ButtonDisplayName = FString::Printf(TEXT("%s: Button %d"), *BaseDisplayName, ButtonKeyIndex);

		if (DeviceConfiguration)
		{
			const FJoystickInputDeviceButtonProperties* ButtonProperties = DeviceConfiguration->GetButtonProperties(ButtonKeyIndex);
			if (ButtonProperties && ButtonProperties->OverrideDisplayName)
			{
				ButtonDisplayName = ButtonProperties->DisplayName;
			}
		}

		const FKey ButtonKey = FKey(FName(*ButtonKeyName));

		FKeyDetails ButtonKeyDetails = FKeyDetails(ButtonKey, FText::FromString(ButtonDisplayName), FKeyDetails::GamepadKey, JoystickCategory);

		if (!EKeys::GetKeyDetails(ButtonKey).IsValid())
		{
			EKeys::AddKey(ButtonKeyDetails);
			FJoystickLogManager::Get()->LogDebug(TEXT("Added Button %s (%s) %d"), *ButtonKeyName, *ButtonDisplayName, InstanceId.Value);
		}

		const FKey& MappedKey = ButtonKeyDetails.GetKey();
		DeviceButtonKeys[InstanceId][ButtonKeyIndex] = MappedKey;
		DeviceKeys[InstanceId].Add(MappedKey);

		TryAddWidgetNavigation(MappedKey);
	}
}

const TArray<FString> AxisNames = {TEXT("X"), TEXT("Y")};

void FJoystickInputDevice::InitialiseHatAxis(const FJoystickInstanceId& InstanceId, const FString& BaseKeyName, const FString& BaseDisplayName, const bool PairedKey)
{
	const FJoystickDeviceState* JoystickState = JoystickDeviceState.Find(InstanceId);
	if (JoystickState == nullptr)
	{
		return;
	}

	DeviceHatAxisKeys.Emplace(InstanceId);
	DeviceHatAxisPairedKeys.Emplace(InstanceId);

	int HatCount = JoystickState->Hats.Num();
	DeviceHatAxisKeys[InstanceId].SetNum(HatCount);
	DeviceHatAxisPairedKeys[InstanceId].SetNum(HatCount);

	for (int HatKeyIndex = 0; HatKeyIndex < HatCount; HatKeyIndex++)
	{
		for (int HatIndex = 0; HatIndex < 2; HatIndex++)
		{
			FString HatAxisName = *AxisNames[HatIndex];
			FString HatKeyName = FString::Printf(TEXT("%s_Hat_%d_%s"), *BaseKeyName, HatKeyIndex, *HatAxisName);
			FString HatDisplayName = FString::Printf(TEXT("%s: Hat %d %s"), *BaseDisplayName, HatKeyIndex, *HatAxisName);

			FKey HatKey = FKey(FName(*HatKeyName));

#if (ENGINE_MAJOR_VERSION == 4 && ENGINE_MINOR_VERSION >= 26 || ENGINE_MAJOR_VERSION == 5)
			FKeyDetails HatKeyDetails = FKeyDetails(HatKey, FText::FromString(HatDisplayName), FKeyDetails::GamepadKey | FKeyDetails::Axis1D, JoystickCategory);
#else
			FKeyDetails HatKeyDetails = FKeyDetails(HatKey, FText::FromString(HatDisplayName), FKeyDetails::GamepadKey | FKeyDetails::FloatAxis, JoystickCategory);
#endif

			if (!EKeys::GetKeyDetails(HatKey).IsValid())
			{
				EKeys::AddKey(HatKeyDetails);
				FJoystickLogManager::Get()->LogDebug(TEXT("Added Hat %s (%s) %d"), *HatKeyName, *HatDisplayName, InstanceId.Value);
			}

			const FKey& MappedKey = HatKeyDetails.GetKey();
			DeviceHatAxisKeys[InstanceId][HatKeyIndex][HatIndex] = MappedKey;
			DeviceKeys[InstanceId].Add(MappedKey);
		}

#if (ENGINE_MAJOR_VERSION == 4 && ENGINE_MINOR_VERSION >= 26 || ENGINE_MAJOR_VERSION == 5)
		if (PairedKey)
		{
			const FKeyPair& KeyPair = DeviceHatAxisKeys[InstanceId][HatKeyIndex];
			FString HatAxisKeyName = FString::Printf(TEXT("%s_Hat_%d_2D"), *BaseKeyName, HatKeyIndex);
			FString HatAxisDisplayName = FString::Printf(TEXT("%s: Hat %d 2D"), *BaseDisplayName, HatKeyIndex);

			FKey HatAxisKey = FKey(FName(*HatAxisKeyName));

			FKeyDetails HatAxisKeyDetails = FKeyDetails(HatAxisKey, FText::FromString(HatAxisDisplayName), FKeyDetails::GamepadKey | FKeyDetails::Axis2D, JoystickCategory);

			if (!EKeys::GetKeyDetails(HatAxisKey).IsValid())
			{
				EKeys::AddPairedKey(HatAxisKeyDetails, KeyPair.X, KeyPair.Y);
				FJoystickLogManager::Get()->LogDebug(TEXT("Added 2D Hat %s (%s) %d"), *HatAxisKeyName, *HatAxisDisplayName, InstanceId.Value);
			}

			const FKey& MappedKey = HatAxisKeyDetails.GetKey();
			DeviceHatAxisPairedKeys[InstanceId][HatKeyIndex] = MappedKey;
			DeviceKeys[InstanceId].Add(MappedKey);
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

			FString ButtonKeyName = FString::Printf(TEXT("%s_Hat_%d_Button_%s"), *BaseKeyName, HatKeyIndex, *DirectionName);
			FString ButtonDisplayName = FString::Printf(TEXT("%s: Hat %d Button %s"), *BaseDisplayName, HatKeyIndex, *DirectionDisplayName);

			const FKey ButtonKey = FKey(FName(ButtonKeyName));

			FKeyDetails ButtonKeyDetails = FKeyDetails(ButtonKey, FText::FromString(ButtonDisplayName), FKeyDetails::GamepadKey, JoystickCategory);

			if (!EKeys::GetKeyDetails(ButtonKey).IsValid())
			{
				EKeys::AddKey(ButtonKeyDetails);
				FJoystickLogManager::Get()->LogDebug(TEXT("Added Hat Button %s (%s) %d"), *ButtonKeyName, *ButtonDisplayName, InstanceId.Value);
			}

			const FKey& MappedKey = ButtonKeyDetails.GetKey();
			int HatButtonIndex = HatKeyIndex * 8 + (i - 1);
			DeviceHatButtonKeys[InstanceId][HatButtonIndex] = MappedKey;
			DeviceKeys[InstanceId].Add(MappedKey);

			TryAddWidgetNavigation(MappedKey);
		}
	}
}

void FJoystickInputDevice::InitialiseBalls(const FJoystickInstanceId& InstanceId, const FString& BaseKeyName, const FString& BaseDisplayName, const bool PairedKey)
{
	const FJoystickDeviceState* JoystickState = JoystickDeviceState.Find(InstanceId);
	if (JoystickState == nullptr)
	{
		return;
	}

	DeviceBallKeys.Emplace(InstanceId);
	DeviceBallPairedKeys.Emplace(InstanceId);

	int BallCount = JoystickState->Balls.Num();
	DeviceBallKeys[InstanceId].SetNum(BallCount);
	DeviceBallPairedKeys[InstanceId].SetNum(BallCount);

	for (int BallKeyIndex = 0; BallKeyIndex < BallCount; BallKeyIndex++)
	{
		for (int BallIndex = 0; BallIndex < 2; BallIndex++)
		{
			FString BallAxisName = *AxisNames[BallIndex];
			FString BallKeyName = FString::Printf(TEXT("%s_Ball_%d_%s"), *BaseKeyName, BallKeyIndex, *BallAxisName);
			FString BallDisplayName = FString::Printf(TEXT("%s: Ball %d %s"), *BaseDisplayName, BallKeyIndex, *BallAxisName);

			FKey BallKey = FKey(FName(*BallKeyName));

#if (ENGINE_MAJOR_VERSION == 4 && ENGINE_MINOR_VERSION >= 26 || ENGINE_MAJOR_VERSION == 5)
			FKeyDetails BallKeyDetails = FKeyDetails(BallKey, FText::FromString(BallDisplayName), FKeyDetails::GamepadKey | FKeyDetails::Axis1D, JoystickCategory);
#else
			FKeyDetails BallKeyDetails = FKeyDetails(BallKey, FText::FromString(BallDisplayName), FKeyDetails::GamepadKey | FKeyDetails::FloatAxis, JoystickCategory);
#endif

			if (!EKeys::GetKeyDetails(BallKey).IsValid())
			{
				EKeys::AddKey(BallKeyDetails);
				FJoystickLogManager::Get()->LogDebug(TEXT("Added Ball %s (%s) %d"), *BallKeyName, *BallDisplayName, InstanceId.Value);
			}

			const FKey& MappedKey = BallKeyDetails.GetKey();
			DeviceBallKeys[InstanceId][BallKeyIndex][BallIndex] = MappedKey;
			DeviceKeys[InstanceId].Add(MappedKey);
		}

#if (ENGINE_MAJOR_VERSION == 4 && ENGINE_MINOR_VERSION >= 26 || ENGINE_MAJOR_VERSION == 5)
		if (PairedKey)
		{
			const FKeyPair& KeyPair = DeviceBallKeys[InstanceId][BallKeyIndex];
			FString BallKeyName = FString::Printf(TEXT("%s_Ball_%d_2D"), *BaseKeyName, BallKeyIndex);
			FString BallDisplayName = FString::Printf(TEXT("%s: Ball %d 2D"), *BaseDisplayName, BallKeyIndex);

			FKey BallKey = FKey(FName(*BallKeyName));

			FKeyDetails BallKeyDetails = FKeyDetails(BallKey, FText::FromString(BallDisplayName), FKeyDetails::GamepadKey | FKeyDetails::Axis2D, JoystickCategory);

			if (!EKeys::GetKeyDetails(BallKey).IsValid())
			{
				EKeys::AddPairedKey(BallKeyDetails, KeyPair.X, KeyPair.Y);
				FJoystickLogManager::Get()->LogDebug(TEXT("Added 2D Ball %s (%s) %d"), *BallKeyName, *BallDisplayName, InstanceId.Value);
			}

			const FKey& MappedKey = BallKeyDetails.GetKey();
			DeviceBallPairedKeys[InstanceId][BallKeyIndex] = MappedKey;
			DeviceKeys[InstanceId].Add(MappedKey);
		}
#endif
	}
}

#undef LOCTEXT_NAMESPACE

void FJoystickInputDevice::NotifyDeviceState(const FInputDeviceId& InputDeviceId, const FPlatformUserId& PlatformUserId, const EInputDeviceConnectionState State) const
{
	IPlatformInputDeviceMapper& DeviceMapper = IPlatformInputDeviceMapper::Get();
	DeviceMapper.Internal_MapInputDeviceToUser(InputDeviceId, PlatformUserId, State);
}

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

	const FPlatformUserId PlatformUser = FGenericPlatformMisc::GetPlatformUserForUserIndex(Device.PlayerId);

	auto [DeviceState, Result] = GetDeviceState(Device.InstanceId);
	if (Result.bSuccess)
	{
		NotifyDeviceState(DeviceState->InputDeviceId, PlatformUser, EInputDeviceConnectionState::Connected);
	}
	else
	{
		FJoystickDeviceState NewDeviceState = JoystickSubsystem->CreateInitialDeviceState(Device.InstanceId);
		JoystickDeviceState.Emplace(Device.InstanceId, NewDeviceState);
		NotifyDeviceState(NewDeviceState.InputDeviceId, PlatformUser, EInputDeviceConnectionState::Connected);
	}

	UJoystickInputSettings* JoystickInputSettings = GetMutableDefault<UJoystickInputSettings>();
	if (!IsValid(JoystickInputSettings))
	{
		return;
	}

	const FJoystickInputDeviceConfiguration* DeviceConfig = JoystickInputSettings->GetInputDeviceConfiguration(Device);

	FString BaseKeyName("Joystick");
	FString BaseDisplayName("Joystick");
	if (JoystickInputSettings->UseDeviceName)
	{
		if (DeviceConfig != nullptr && DeviceConfig->OverrideDeviceName)
		{
			const FString DeviceName = SanitiseDeviceName(DeviceConfig->DeviceName);
			BaseKeyName = FString::Printf(TEXT("Joystick_%s"), *DeviceName);
			BaseDisplayName = DeviceName;
		}
		else
		{
			const FString DeviceName = SanitiseDeviceName(Device.SafeDeviceName);
			BaseKeyName = FString::Printf(TEXT("Joystick_%s"), *DeviceName);
			BaseDisplayName = Device.ProductName;
		}
	}

	if (JoystickInputSettings->IncludeDeviceIndex)
	{
		BaseKeyName = FString::Printf(TEXT("%s_%d"), *BaseKeyName, Device.InternalDeviceIndex);
		BaseDisplayName = FString::Printf(TEXT("%s %d"), *BaseDisplayName, Device.InternalDeviceIndex);
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

	UpdateAxisProperties();

	FJoystickLogManager::Get()->LogInformation(TEXT("Device Ready: %s (%d) - Instance Id: %d"), *Device.DeviceName, Device.InternalDeviceIndex, Device.InstanceId.Value);
}

void FJoystickInputDevice::JoystickUnplugged(const FJoystickInstanceId& InstanceId)
{
	UJoystickInputSettings* JoystickInputSettings = GetMutableDefault<UJoystickInputSettings>();
	if (!IsValid(JoystickInputSettings))
	{
		return;
	}

	JoystickInputSettings->DeviceRemoved(InstanceId);

	auto [DeviceState, Result] = GetDeviceState(InstanceId);
	if (Result.bSuccess)
	{
		//Device managed by subsystem
		const IPlatformInputDeviceMapper& DeviceMapper = IPlatformInputDeviceMapper::Get();
		const FPlatformUserId NewUserToAssign = DeviceMapper.GetUserForUnpairedInputDevices();
		NotifyDeviceState(DeviceState->InputDeviceId, NewUserToAssign, EInputDeviceConnectionState::Disconnected);
	}
}

void FJoystickInputDevice::JoystickButton(const FJoystickInstanceId& InstanceId, const int Button, const bool Pressed)
{
	if (!JoystickDeviceState.Contains(InstanceId))
	{
		return;
	}

	FJoystickDeviceState& DeviceData = JoystickDeviceState[InstanceId];
	if (!DeviceData.Buttons.IsValidIndex(Button))
	{
		return;
	}

	FButtonData& State = DeviceData.Buttons[Button];
	State.Update(Pressed);

	FJoystickLogManager::Get()->LogDebug(TEXT("Event JoystickButton Device=%d Button=%d State=%d"), InstanceId.Value, Button, Pressed);
}

void FJoystickInputDevice::JoystickAxis(const FJoystickInstanceId& InstanceId, const int Axis, const float Value)
{
	auto [DeviceState, Result] = GetDeviceState(InstanceId);
	if (!DeviceState || Result.bSuccess == false)
	{
		FJoystickLogManager::Get()->LogError(Result);
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
	if (!DeviceState || Result.bSuccess == false)
	{
		FJoystickLogManager::Get()->LogError(Result);
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
	if (!DeviceState || Result.bSuccess == false)
	{
		FJoystickLogManager::Get()->LogError(Result);
		return;
	}

	if (!DeviceState->Balls.IsValidIndex(Ball))
	{
		return;
	}

	FBallData& State = DeviceState->Balls[Ball];
	State.Update(Value);
}

void FJoystickInputDevice::JoystickGyro(const FJoystickInstanceId& InstanceId, const int Timestamp, const FVector& Value)
{
	auto [DeviceState, Result] = GetDeviceState(InstanceId);
	if (!DeviceState || Result.bSuccess == false)
	{
		FJoystickLogManager::Get()->LogError(Result);
		return;
	}

	DeviceState->Motion.UpdateGyro(Value, Timestamp);
}

void FJoystickInputDevice::JoystickAccelerometer(const FJoystickInstanceId& InstanceId, const int Timestamp, const FVector& Value)
{
	auto [DeviceState, Result] = GetDeviceState(InstanceId);
	if (!DeviceState || Result.bSuccess == false)
	{
		FJoystickLogManager::Get()->LogError(Result);
		return;
	}

	DeviceState->Motion.UpdateAccelerometer(Value, Timestamp);
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

		const int PlayerId = DeviceInfo.PlayerId;
#if ENGINE_MAJOR_VERSION == 5 && ENGINE_MINOR_VERSION >= 1
		const FPlatformUserId& PlatformUser = FGenericPlatformMisc::GetPlatformUserForUserIndex(PlayerId);
#endif

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

#if ENGINE_MAJOR_VERSION == 5 && ENGINE_MINOR_VERSION >= 1
					constexpr int32 BaseKey = 10000;
					const int Key = BaseKey + (CurrentState.InputDeviceId.GetId() * CurrentState.Axes.Num()) + AxisIndex;
					GEngine->AddOnScreenDebugMessage(
						Key,
						0.3f,
						FColor::Green,
						FString::Printf(TEXT("Axis %s Raw: %.3f Mapped: %.3f"), *AxisKey.GetDisplayName().ToString(), AxisData.Value, CurrentValue)
					);
					MessageHandler->OnControllerAnalog(AxisKey.GetFName(), PlatformUser, CurrentState.InputDeviceId, CurrentValue);
#else
					MessageHandler->OnControllerAnalog(AxisKey.GetFName(), PlayerId, CurrentValue);
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
					MessageHandler->OnControllerAnalog(XHatKey.GetFName(), PlatformUser, CurrentState.InputDeviceId, POVAxis.X);
					MessageHandler->OnControllerAnalog(YHatKey.GetFName(), PlatformUser, CurrentState.InputDeviceId, POVAxis.Y);
#else
					MessageHandler->OnControllerAnalog(XHatKey.GetFName(), PlayerId, POVAxis.X);
					MessageHandler->OnControllerAnalog(YHatKey.GetFName(), PlayerId, POVAxis.Y);
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
						MessageHandler->OnControllerButtonReleased(DirectionKey.GetFName(), PlatformUser, CurrentState.InputDeviceId, false);
#else
						MessageHandler->OnControllerButtonReleased(DirectionKey.GetFName(), PlayerId, false);
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
						MessageHandler->OnControllerButtonPressed(DirectionKey.GetFName(), PlatformUser, CurrentState.InputDeviceId, false);
#else
						MessageHandler->OnControllerButtonPressed(DirectionKey.GetFName(), PlayerId, false);
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
					MessageHandler->OnControllerAnalog(XBallKey.GetFName(), PlatformUser, CurrentState.InputDeviceId, CurrentValue.X);
					MessageHandler->OnControllerAnalog(YBallKey.GetFName(), PlatformUser, CurrentState.InputDeviceId, CurrentValue.Y);
#else
					MessageHandler->OnControllerAnalog(XBallKey.GetFName(), PlayerId, CurrentValue.X);
					MessageHandler->OnControllerAnalog(YBallKey.GetFName(), PlayerId, CurrentValue.Y);
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
						MessageHandler->OnControllerButtonPressed(ButtonKey.GetFName(), PlatformUser, CurrentState.InputDeviceId, false);
#else
						MessageHandler->OnControllerButtonPressed(ButtonKey.GetFName(), PlayerId, false);
#endif
					}
					else
					{
#if ENGINE_MAJOR_VERSION == 5 && ENGINE_MINOR_VERSION >= 1
						MessageHandler->OnControllerButtonReleased(ButtonKey.GetFName(), PlatformUser, CurrentState.InputDeviceId, false);
#else
						MessageHandler->OnControllerButtonReleased(ButtonKey.GetFName(), PlayerId, false);
#endif
					}

					ButtonData.Processed();
				}
			}
		}

		// Gyro
#if ENGINE_MAJOR_VERSION == 5 && ENGINE_MINOR_VERSION >= 1
		MessageHandler->OnMotionDetected(FVector::ZeroVector, CurrentState.Motion.Gyro, FVector::ZeroVector, CurrentState.Motion.Accelerometer, PlatformUser, CurrentState.InputDeviceId);
#else
		MessageHandler->OnMotionDetected(FVector::ZeroVector, CurrentState.Motion.Gyro, FVector::ZeroVector, CurrentState.Motion.Accelerometer, PlayerId);
#endif
	}

	JoystickSubsystem->Update();
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

void FJoystickInputDevice::UpdateAxisProperties()
{
	ResetAxisProperties();

	const UJoystickInputSettings* JoystickInputSettings = GetDefault<UJoystickInputSettings>();
	if (!IsValid(JoystickInputSettings))
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

		const FJoystickInputDeviceConfiguration* DeviceConfig = JoystickInputSettings->GetInputDeviceConfiguration(DeviceInfo);
		if (DeviceConfig == nullptr)
		{
			continue;
		}

		if (!JoystickDeviceState.Contains(InstanceId))
		{
			continue;
		}

		FJoystickDeviceState& CurrentState = JoystickDeviceState[InstanceId];
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
				if (AxisProperties->InputRangeMin == -1.0f && AxisProperties->InputRangeMax == 1.0f && AxisProperties->OutputRangeMin == 0.0f && AxisProperties->OutputRangeMax == 1.0f ||
					AxisProperties->InputRangeMin == 0.0f && AxisProperties->InputRangeMax == 1.0f && AxisProperties->OutputRangeMin == 0.0f && AxisProperties->OutputRangeMax == 1.0f)
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

	AsyncTask(ENamedThreads::GameThread, [KeyConfiguration, ButtonKey]()
	{
		const FSlateApplication& SlateApplication = FSlateApplication::Get();
		const TSharedPtr<FNavigationConfig> NavigationConfig = SlateApplication.GetNavigationConfig();
		if (!NavigationConfig)
		{
			return;
		}

		// Key Event (Up, Down, Left, Right, etc.)
		if (KeyConfiguration->Direction != EUINavigation::Invalid)
		{
			FJoystickLogManager::Get()->LogDebug(TEXT("Added FKey (%s) to Slate Navigation Events with direction: %s"), *ButtonKey.GetDisplayName().ToString(), *UEnum::GetValueAsString(KeyConfiguration->Direction));
			NavigationConfig->KeyEventRules.Add(ButtonKey, KeyConfiguration->Direction);
		}

#if ENGINE_MAJOR_VERSION >= 5 && ENGINE_MINOR_VERSION >= 4
		// Key Action (Accept, Back, etc.)
		if (KeyConfiguration->Action != EUINavigationAction::Invalid)
		{
			FJoystickLogManager::Get()->LogDebug(TEXT("Added FKey (%s) to Slate Navigation Actions with action: %s"), *ButtonKey.GetDisplayName().ToString(), *UEnum::GetValueAsString(KeyConfiguration->Action));
			NavigationConfig->KeyActionRules.Add(ButtonKey, KeyConfiguration->Action);
		}
#endif
	});
}

FString FJoystickInputDevice::SanitiseDeviceName(const FString& InDeviceName) const
{
	FString OutDeviceName;

	// Reserve to avoid reallocs
	OutDeviceName.Reserve(InDeviceName.Len());

	for (const TCHAR Char : InDeviceName)
	{
		// Allow alphanumeric + underscore only
		if (FChar::IsAlnum(Char))
		{
			OutDeviceName.AppendChar(Char);
		}
		else
		{
			OutDeviceName.AppendChar(TEXT('_'));
		}
	}

	// Collapse multiple underscores
	FString Collapsed;
	Collapsed.Reserve(OutDeviceName.Len());

	bool LastCharWasUnderscore = false;
	for (const TCHAR Char : OutDeviceName)
	{
		const bool IsUnderscore = (Char == TEXT('_'));
		if (IsUnderscore && LastCharWasUnderscore)
		{
			continue;
		}

		LastCharWasUnderscore = IsUnderscore;
		Collapsed.AppendChar(Char);
	}

	OutDeviceName = MoveTemp(Collapsed);

	return OutDeviceName.TrimStartAndEnd();
}
