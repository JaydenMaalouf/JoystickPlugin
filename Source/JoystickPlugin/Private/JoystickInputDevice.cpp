// JoystickPlugin is licensed under the MIT License.
// Copyright Jayden Maalouf. All Rights Reserved.

#include "JoystickInputDevice.h"
#include "JoystickFunctionLibrary.h"
#include "JoystickHapticDeviceManager.h"
#include "JoystickInputSettings.h"
#include "JoystickLogManager.h"
#include "JoystickPluginModule.h"
#include "JoystickSubsystem.h"
#include "GameFramework/InputSettings.h"
#include "GenericPlatform/IInputInterface.h"
#include "Runtime/Launch/Resources/Version.h"
#include "Engine/Engine.h"

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

void FJoystickInputDevice::InitialiseAxis(const FJoystickInstanceId& InstanceId, const FString& BaseKeyName, const FString& BaseDisplayName)
{
	DeviceAxisKeys.Emplace(InstanceId);

	const FJoystickDeviceState* JoystickState = JoystickDeviceState.Find(InstanceId);
	if (JoystickState == nullptr)
	{
		return;
	}

	const int AxisCount = JoystickState->Axes.Num();
	DeviceAxisKeys[InstanceId].SetNum(AxisCount);
	for (int AxisKeyIndex = 0; AxisKeyIndex < AxisCount; AxisKeyIndex++)
	{
		FString AxisKeyName = FString::Printf(TEXT("%s_Axis%d"), *BaseKeyName, AxisKeyIndex);
		FString AxisDisplayName = FString::Printf(TEXT("%s: Axis %d"), *BaseDisplayName, AxisKeyIndex);

		FKey AxisKey = FKey(FName(*AxisKeyName));
#if (ENGINE_MAJOR_VERSION == 4 && ENGINE_MINOR_VERSION >= 26 || ENGINE_MAJOR_VERSION == 5)
		FKeyDetails AxisKeyDetails = FKeyDetails(AxisKey, FText::FromString(AxisDisplayName), FKeyDetails::GamepadKey | FKeyDetails::Axis1D, JoystickCategory);
#else
		FKeyDetails AxisKeyDetails = FKeyDetails(AxisKey, FText::FromString(AxisDisplayName), FKeyDetails::GamepadKey | FKeyDetails::FloatAxis, JoystickCategory);
#endif

		if (!EKeys::GetKeyDetails(AxisKey).IsValid())
		{
			EKeys::AddKey(AxisKeyDetails);
			FJoystickLogManager::Get()->LogDebug(TEXT("Added Axis %s (%s) %d"), *AxisKeyName, *AxisDisplayName, &InstanceId);
		}

		const FKey& MappedKey = AxisKeyDetails.GetKey();
		DeviceAxisKeys[InstanceId][AxisKeyIndex] = MappedKey;
		DeviceKeys[InstanceId].Add(MappedKey);
	}
}

void FJoystickInputDevice::InitialiseButtons(const FJoystickInstanceId& InstanceId, const FString& BaseKeyName, const FString& BaseDisplayName)
{
	DeviceButtonKeys.Emplace(InstanceId);

	const FJoystickDeviceState* JoystickState = JoystickDeviceState.Find(InstanceId);
	if (JoystickState == nullptr)
	{
		return;
	}

	const int ButtonCount = JoystickState->Buttons.Num();
	DeviceButtonKeys[InstanceId].SetNum(ButtonCount);
	for (int ButtonKeyIndex = 0; ButtonKeyIndex < ButtonCount; ButtonKeyIndex++)
	{
		FString ButtonKeyName = FString::Printf(TEXT("%s_Button%d"), *BaseKeyName, ButtonKeyIndex);
		FString ButtonDisplayName = FString::Printf(TEXT("%s: Button %d"), *BaseDisplayName, ButtonKeyIndex);

		const FKey ButtonKey = FKey(FName(*ButtonKeyName));
		FKeyDetails ButtonKeyDetails = FKeyDetails(ButtonKey, FText::FromString(ButtonDisplayName), FKeyDetails::GamepadKey, JoystickCategory);

		if (!EKeys::GetKeyDetails(ButtonKey).IsValid())
		{
			EKeys::AddKey(ButtonKeyDetails);
			FJoystickLogManager::Get()->LogDebug(TEXT("Added Button %s (%s) %d"), *ButtonKeyName, *ButtonDisplayName, &InstanceId);
		}

		const FKey& MappedKey = ButtonKeyDetails.GetKey();
		DeviceButtonKeys[InstanceId][ButtonKeyIndex] = MappedKey;
		DeviceKeys[InstanceId].Add(MappedKey);
	}
}

const TArray<FString> AxisNames = {TEXT("X"), TEXT("Y")};

void FJoystickInputDevice::InitialiseHatsAxis(const FJoystickInstanceId& InstanceId, const FString& BaseKeyName, const FString& BaseDisplayName)
{
	const FJoystickDeviceState* JoystickState = JoystickDeviceState.Find(InstanceId);
	if (JoystickState == nullptr)
	{
		return;
	}

	for (int HatIndex = 0; HatIndex < 2; HatIndex++)
	{
		FString HatAxisName = *AxisNames[HatIndex];
		DeviceHatAxisKeys[HatIndex].Emplace(InstanceId);

		int HatCount = JoystickState->Hats.Num();
		DeviceHatAxisKeys[HatIndex][InstanceId].SetNum(HatCount);
		for (int HatKeyIndex = 0; HatKeyIndex < HatCount; HatKeyIndex++)
		{
			FString HatKeyName = FString::Printf(TEXT("%s_Hat%d_%s"), *BaseKeyName, HatKeyIndex, *HatAxisName);
			FString HatDisplayName = FString::Printf(TEXT("%s: Hat %d %s"), *BaseDisplayName, HatIndex, *HatAxisName);

			FKey HatKey = FKey(FName(*HatKeyName));
#if (ENGINE_MAJOR_VERSION == 4 && ENGINE_MINOR_VERSION >= 26 || ENGINE_MAJOR_VERSION == 5)
			FKeyDetails HatKeyDetails = FKeyDetails(HatKey, FText::FromString(HatDisplayName), FKeyDetails::GamepadKey | FKeyDetails::Axis1D, JoystickCategory);
#else
			FKeyDetails HatKeyDetails = FKeyDetails(HatKey, FText::FromString(HatDisplayName), FKeyDetails::GamepadKey | FKeyDetails::FloatAxis, JoystickCategory);
#endif

			if (!EKeys::GetKeyDetails(HatKey).IsValid())
			{
				EKeys::AddKey(HatKeyDetails);
				FJoystickLogManager::Get()->LogDebug(TEXT("Added Hat %s (%s) %d"), *HatKeyName, *HatDisplayName, &InstanceId);
			}

			const FKey& MappedKey = HatKeyDetails.GetKey();
			DeviceHatAxisKeys[HatIndex][InstanceId][HatKeyIndex] = HatKeyDetails.GetKey();
			DeviceKeys[InstanceId].Add(MappedKey);
		}
	}
}

void FJoystickInputDevice::InitialiseHatsButtons(const FJoystickInstanceId& InstanceId, const FString& BaseKeyName, const FString& BaseDisplayName)
{
	DeviceHatButtonKeys.Emplace(InstanceId);

	const FJoystickDeviceState* JoystickState = JoystickDeviceState.Find(InstanceId);
	if (JoystickState == nullptr)
	{
		return;
	}

	const int HatCount = JoystickState->Hats.Num();
	DeviceHatButtonKeys[InstanceId].SetNum(HatCount * 8);
	for (int HatKeyIndex = 0; HatKeyIndex < HatCount; HatKeyIndex++)
	{
		FString ButtonUpKeyName = FString::Printf(TEXT("%s_Hat_%d_Button_Up"), *BaseKeyName, HatKeyIndex);
		FString ButtonUpDisplayName = FString::Printf(TEXT("%s Hat %d Button Up"), *BaseDisplayName, HatKeyIndex);
		
		FString ButtonUpRightKeyName = FString::Printf(TEXT("%s_Hat_%d_Button_Up_Right"), *BaseKeyName, HatKeyIndex);
		FString ButtonUpRightDisplayName = FString::Printf(TEXT("%s Hat %d Button Up Right"), *BaseDisplayName, HatKeyIndex);

		FString ButtonRightKeyName = FString::Printf(TEXT("%s_Hat_%d_Button_Right"), *BaseKeyName, HatKeyIndex);
		FString ButtonRightDisplayName = FString::Printf(TEXT("%s Hat %d Button Right"), *BaseDisplayName, HatKeyIndex);

		FString ButtonDownRightKeyName = FString::Printf(TEXT("%s_Hat_%d_Button_Down_Right"), *BaseKeyName, HatKeyIndex);
		FString ButtonDownRightDisplayName = FString::Printf(TEXT("%s Hat %d Button Down Right"), *BaseDisplayName, HatKeyIndex);

		FString ButtonDownKeyName = FString::Printf(TEXT("%s_Hat_%d_Button_Down"), *BaseKeyName, HatKeyIndex);
		FString ButtonDownDisplayName = FString::Printf(TEXT("%s Hat %d Button Down"), *BaseDisplayName, HatKeyIndex);

		FString ButtonDownLeftKeyName = FString::Printf(TEXT("%s_Hat_%d_Button_Down_Left"), *BaseKeyName, HatKeyIndex);
		FString ButtonDownLeftDisplayName = FString::Printf(TEXT("%s Hat %d Button Down Left"), *BaseDisplayName, HatKeyIndex);

		FString ButtonLeftKeyName = FString::Printf(TEXT("%s_Hat_%d_Button_Left"), *BaseKeyName, HatKeyIndex);
		FString ButtonLeftDisplayName = FString::Printf(TEXT("%s Hat %d Button Left"), *BaseDisplayName, HatKeyIndex);

		FString ButtonUpLeftKeyName = FString::Printf(TEXT("%s_Hat_%d_Button_Up_Left"), *BaseKeyName, HatKeyIndex);
		FString ButtonUpLeftDisplayName = FString::Printf(TEXT("%s Hat %d Button Up Left"), *BaseDisplayName, HatKeyIndex);

		const FKey ButtonUpKey = FKey(FName(ButtonUpKeyName));
		FKeyDetails ButtonUpKeyDetails = FKeyDetails(ButtonUpKey, FText::FromString(ButtonUpDisplayName), FKeyDetails::GamepadKey, JoystickCategory);

		const FKey ButtonUpRightKey = FKey(FName(ButtonUpRightKeyName));
		FKeyDetails ButtonUpRightKeyDetails = FKeyDetails(ButtonUpRightKey, FText::FromString(ButtonUpRightDisplayName), FKeyDetails::GamepadKey, JoystickCategory);

		const FKey ButtonRightKey = FKey(FName(ButtonRightKeyName));
		FKeyDetails ButtonRightKeyDetails = FKeyDetails(ButtonRightKey, FText::FromString(ButtonRightDisplayName), FKeyDetails::GamepadKey, JoystickCategory);

		const FKey ButtonDownRightKey = FKey(FName(ButtonDownRightKeyName));
		FKeyDetails ButtonDownRightKeyDetails = FKeyDetails(ButtonDownRightKey, FText::FromString(ButtonDownRightDisplayName), FKeyDetails::GamepadKey, JoystickCategory);

		const FKey ButtonDownKey = FKey(FName(ButtonDownKeyName));
		FKeyDetails ButtonDownKeyDetails = FKeyDetails(ButtonDownKey, FText::FromString(ButtonDownDisplayName), FKeyDetails::GamepadKey, JoystickCategory);

		const FKey ButtonDownLeftKey = FKey(FName(ButtonDownLeftKeyName));
		FKeyDetails ButtonDownLeftKeyDetails = FKeyDetails(ButtonDownLeftKey, FText::FromString(ButtonDownLeftDisplayName), FKeyDetails::GamepadKey, JoystickCategory);

		const FKey ButtonLeftKey = FKey(FName(ButtonLeftKeyName));
		FKeyDetails ButtonLeftKeyDetails = FKeyDetails(ButtonLeftKey, FText::FromString(ButtonLeftDisplayName), FKeyDetails::GamepadKey, JoystickCategory);

		const FKey ButtonUpLeftKey = FKey(FName(ButtonUpLeftKeyName));
		FKeyDetails ButtonUpLeftKeyDetails = FKeyDetails(ButtonUpLeftKey, FText::FromString(ButtonUpLeftDisplayName), FKeyDetails::GamepadKey, JoystickCategory);

		if (!EKeys::GetKeyDetails(ButtonUpKey).IsValid())
		{
			EKeys::AddKey(ButtonUpKeyDetails);
			FJoystickLogManager::Get()->LogDebug(TEXT("Added Button %s (%s) %d"), *ButtonUpKeyName, *ButtonUpDisplayName, &InstanceId);
		}

		if (!EKeys::GetKeyDetails(ButtonUpRightKey).IsValid())
		{
			EKeys::AddKey(ButtonUpRightKeyDetails);
			FJoystickLogManager::Get()->LogDebug(TEXT("Added Button %s (%s) %d"), *ButtonUpRightKeyName, *ButtonUpRightDisplayName, &InstanceId);
		}

		if (!EKeys::GetKeyDetails(ButtonRightKey).IsValid())
		{
			EKeys::AddKey(ButtonRightKeyDetails);
			FJoystickLogManager::Get()->LogDebug(TEXT("Added Button %s (%s) %d"), *ButtonRightKeyName, *ButtonRightDisplayName, &InstanceId);
		}

		if (!EKeys::GetKeyDetails(ButtonDownRightKey).IsValid())
		{
			EKeys::AddKey(ButtonDownRightKeyDetails);
			FJoystickLogManager::Get()->LogDebug(TEXT("Added Button %s (%s) %d"), *ButtonDownRightKeyName, *ButtonDownRightDisplayName, &InstanceId);
		}

		if (!EKeys::GetKeyDetails(ButtonDownKey).IsValid())
		{
			EKeys::AddKey(ButtonDownKeyDetails);
			FJoystickLogManager::Get()->LogDebug(TEXT("Added Button %s (%s) %d"), *ButtonDownKeyName, *ButtonDownDisplayName, &InstanceId);
		}

		if (!EKeys::GetKeyDetails(ButtonDownLeftKey).IsValid())
		{
			EKeys::AddKey(ButtonDownLeftKeyDetails);
			FJoystickLogManager::Get()->LogDebug(TEXT("Added Button %s (%s) %d"), *ButtonDownLeftKeyName, *ButtonDownLeftDisplayName, &InstanceId);
		}

		if (!EKeys::GetKeyDetails(ButtonLeftKey).IsValid())
		{
			EKeys::AddKey(ButtonLeftKeyDetails);
			FJoystickLogManager::Get()->LogDebug(TEXT("Added Button %s (%s) %d"), *ButtonLeftKeyName, *ButtonLeftDisplayName, &InstanceId);
		}
		
		if (!EKeys::GetKeyDetails(ButtonUpLeftKey).IsValid())
		{
			EKeys::AddKey(ButtonUpLeftKeyDetails);
			FJoystickLogManager::Get()->LogDebug(TEXT("Added Button %s (%s) %d"), *ButtonUpLeftKeyName, *ButtonUpLeftDisplayName, &InstanceId);
		}

		const FKey& MappedUpKey = ButtonUpKeyDetails.GetKey();
		const FKey& MappedUpRightKey = ButtonUpRightKeyDetails.GetKey();
		const FKey& MappedRightKey = ButtonRightKeyDetails.GetKey();
		const FKey& MappedDownRightKey = ButtonDownRightKeyDetails.GetKey();
		const FKey& MappedDownKey = ButtonDownKeyDetails.GetKey();
		const FKey& MappedDownLeftKey = ButtonDownLeftKeyDetails.GetKey();
		const FKey& MappedLeftKey = ButtonLeftKeyDetails.GetKey();
		const FKey& MappedUpLeftKey = ButtonUpLeftKeyDetails.GetKey();
		
		DeviceHatButtonKeys[InstanceId][HatKeyIndex * 8 + 0] = MappedUpKey;
		DeviceHatButtonKeys[InstanceId][HatKeyIndex * 8 + 1] = MappedUpRightKey;
		DeviceHatButtonKeys[InstanceId][HatKeyIndex * 8 + 2] = MappedRightKey;
		DeviceHatButtonKeys[InstanceId][HatKeyIndex * 8 + 3] = MappedDownRightKey;
		DeviceHatButtonKeys[InstanceId][HatKeyIndex * 8 + 4] = MappedDownKey;
		DeviceHatButtonKeys[InstanceId][HatKeyIndex * 8 + 5] = MappedDownLeftKey;
		DeviceHatButtonKeys[InstanceId][HatKeyIndex * 8 + 6] = MappedLeftKey;
		DeviceHatButtonKeys[InstanceId][HatKeyIndex * 8 + 7] = MappedUpLeftKey;
		
		DeviceKeys[InstanceId].Add(MappedUpKey);
		DeviceKeys[InstanceId].Add(MappedUpRightKey);
		DeviceKeys[InstanceId].Add(MappedRightKey);
		DeviceKeys[InstanceId].Add(MappedDownRightKey);
		DeviceKeys[InstanceId].Add(MappedDownKey);
		DeviceKeys[InstanceId].Add(MappedDownLeftKey);
		DeviceKeys[InstanceId].Add(MappedLeftKey);
		DeviceKeys[InstanceId].Add(MappedUpLeftKey);
	}
}

void FJoystickInputDevice::InitialiseBalls(const FJoystickInstanceId& InstanceId, const FString& BaseKeyName, const FString& BaseDisplayName)
{
	const FJoystickDeviceState* JoystickState = JoystickDeviceState.Find(InstanceId);
	if (JoystickState == nullptr)
	{
		return;
	}

	for (int BallIndex = 0; BallIndex < 2; BallIndex++)
	{
		FString BallAxisName = *AxisNames[BallIndex];
		DeviceBallKeys[BallIndex].Emplace(InstanceId);

		int BallCount = JoystickState->Balls.Num();
		DeviceBallKeys[BallIndex][InstanceId].SetNum(BallCount);
		for (int BallKeyIndex = 0; BallKeyIndex < BallCount; BallKeyIndex++)
		{
			FString BallKeyName = FString::Printf(TEXT("%s_Ball%d_%s"), *BaseKeyName, BallKeyIndex, *BallAxisName);
			FString BallDisplayName = FString::Printf(TEXT("%s: Ball %d %s"), *BaseDisplayName, BallIndex, *BallAxisName);

			FKey BallKey = FKey(FName(*BallKeyName));
#if (ENGINE_MAJOR_VERSION == 4 && ENGINE_MINOR_VERSION >= 26 || ENGINE_MAJOR_VERSION == 5)
			FKeyDetails BallKeyDetails = FKeyDetails(BallKey, FText::FromString(BallDisplayName), FKeyDetails::GamepadKey | FKeyDetails::Axis1D, JoystickCategory);
#else
			FKeyDetails BallKeyDetails = FKeyDetails(BallKey, FText::FromString(BallDisplayName), FKeyDetails::GamepadKey | FKeyDetails::FloatAxis, JoystickCategory);
#endif

			if (!EKeys::GetKeyDetails(BallKey).IsValid())
			{
				EKeys::AddKey(BallKeyDetails);
				FJoystickLogManager::Get()->LogDebug(TEXT("Added Ball %s (%s) %d"), *BallKeyName, *BallDisplayName, &InstanceId);
			}

			const FKey& MappedKey = BallKeyDetails.GetKey();
			DeviceBallKeys[BallIndex][InstanceId][BallKeyIndex] = BallKeyDetails.GetKey();
			DeviceKeys[InstanceId].Add(MappedKey);
		}
	}
}

#undef LOCTEXT_NAMESPACE

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

	const FJoystickDeviceState InitialState = JoystickSubsystem->CreateInitialDeviceState(Device.InstanceId);
	JoystickDeviceState.Emplace(Device.InstanceId, InitialState);

	UJoystickInputSettings* JoystickInputSettings = GetMutableDefault<UJoystickInputSettings>();
	if (!IsValid(JoystickInputSettings))
	{
		return;
	}

	FString BaseKeyName = FString::Printf(TEXT("Joystick_%d"), Device.InternalDeviceIndex);
	FString BaseDisplayName = FString::Printf(TEXT("Joystick %d"), Device.InternalDeviceIndex);
	if (JoystickInputSettings->UseDeviceName)
	{
		const FJoystickInputDeviceConfiguration* DeviceConfig = JoystickInputSettings->GetInputDeviceConfiguration(Device.ProductGuid);
		if (DeviceConfig != nullptr && DeviceConfig->OverrideDeviceName)
		{
			const FString DeviceName = DeviceConfig->DeviceName.Replace(TEXT(" "), TEXT("_"));
			BaseKeyName = FString::Printf(TEXT("Joystick_%s_%d"), *DeviceName, Device.InternalDeviceIndex);
			BaseDisplayName = FString::Printf(TEXT("%s %d"), *DeviceName, Device.InternalDeviceIndex);
		}
		else
		{
			const FString DeviceName = Device.SafeDeviceName.Replace(TEXT(" "), TEXT("_"));
			BaseKeyName = FString::Printf(TEXT("Joystick_%s_%d"), *DeviceName, Device.InternalDeviceIndex);
			BaseDisplayName = FString::Printf(TEXT("%s %d"), *Device.ProductName, Device.InternalDeviceIndex);
		}
	}

	DeviceKeys.Emplace(Device.InstanceId);

	// create FKeyDetails for inputs
	InitialiseAxis(Device.InstanceId, BaseKeyName, BaseDisplayName);
	InitialiseButtons(Device.InstanceId, BaseKeyName, BaseDisplayName);
	InitialiseHatsAxis(Device.InstanceId, BaseKeyName, BaseDisplayName);
	InitialiseHatsButtons(Device.InstanceId, BaseKeyName, BaseDisplayName);
	InitialiseBalls(Device.InstanceId, BaseKeyName, BaseDisplayName);

	JoystickInputSettings->DeviceAdded(static_cast<FJoystickInformation>(Device));

	UInputSettings* InputSettings = UInputSettings::GetInputSettings();
	if (IsValid(InputSettings))
	{
		InputSettings->PostInitProperties();
	}

	UpdateAxisProperties();

	FJoystickLogManager::Get()->LogInformation(TEXT("Device Ready: %s (%d) - Instance Id: %d"), *Device.DeviceName, Device.InternalDeviceIndex, &Device.InstanceId);
}

void FJoystickInputDevice::JoystickUnplugged(const FJoystickInstanceId& InstanceId) const
{
	UJoystickInputSettings* JoystickInputSettings = GetMutableDefault<UJoystickInputSettings>();
	if (!IsValid(JoystickInputSettings))
	{
		return;
	}

	JoystickInputSettings->DeviceRemoved(InstanceId);
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

	FJoystickLogManager::Get()->LogDebug(TEXT("Event JoystickButton Device=%d Button=%d State=%d"), &InstanceId, Button, Pressed);
}

void FJoystickInputDevice::JoystickAxis(const FJoystickInstanceId& InstanceId, const int Axis, const float Value)
{
	if (!JoystickDeviceState.Contains(InstanceId))
	{
		return;
	}

	FJoystickDeviceState& DeviceData = JoystickDeviceState[InstanceId];
	if (!DeviceData.Axes.IsValidIndex(Axis))
	{
		return;
	}

	FAxisData& State = DeviceData.Axes[Axis];
	State.Update(Value);
}

void FJoystickInputDevice::JoystickHat(const FJoystickInstanceId& InstanceId, const int Hat, const EJoystickPointOfViewDirection Value)
{
	if (!JoystickDeviceState.Contains(InstanceId))
	{
		return;
	}

	FJoystickDeviceState& DeviceData = JoystickDeviceState[InstanceId];
	if (!DeviceData.Hats.IsValidIndex(Hat))
	{
		return;
	}

	FHatData& State = DeviceData.Hats[Hat];
	State.Update(Value);
}

void FJoystickInputDevice::JoystickBall(const FJoystickInstanceId& InstanceId, const int Ball, const FVector2D& Value)
{
	if (!JoystickDeviceState.Contains(InstanceId))
	{
		return;
	}

	FJoystickDeviceState& DeviceData = JoystickDeviceState[InstanceId];
	if (!DeviceData.Balls.IsValidIndex(Ball))
	{
		return;
	}

	FBallData& State = DeviceData.Balls[Ball];
	State.Update(Value);
}

void FJoystickInputDevice::JoystickGyro(const FJoystickInstanceId& InstanceId, const int Timestamp, const FVector& Value)
{
	if (!JoystickDeviceState.Contains(InstanceId))
	{
		return;
	}

	FJoystickDeviceState& DeviceData = JoystickDeviceState[InstanceId];
	DeviceData.Motion.UpdateGyro(Value, Timestamp);
}

void FJoystickInputDevice::JoystickAccelerometer(const FJoystickInstanceId& InstanceId, const int Timestamp, const FVector& Value)
{
	if (!JoystickDeviceState.Contains(InstanceId))
	{
		return;
	}

	FJoystickDeviceState& DeviceData = JoystickDeviceState[InstanceId];
	DeviceData.Motion.UpdateAccelerometer(Value, Timestamp);
}

FJoystickDeviceState* FJoystickInputDevice::GetDeviceData(const FJoystickInstanceId& InstanceId)
{
	if (!JoystickDeviceState.Contains(InstanceId))
	{
		return nullptr;
	}

	return JoystickDeviceState.Find(InstanceId);
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
		IPlatformInputDeviceMapper& DeviceMapper = IPlatformInputDeviceMapper::Get();
		DeviceMapper.AllocateNewInputDeviceId();
		FPlatformUserId PlatformUser = FGenericPlatformMisc::GetPlatformUserForUserIndex(PlayerId);
		FInputDeviceId InputDeviceId = FInputDeviceId::CreateFromInternalId(InstanceId);
		DeviceMapper.RemapControllerIdToPlatformUserAndDevice(PlayerId, OUT PlatformUser, OUT InputDeviceId);
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
#if ENGINE_MAJOR_VERSION == 5 && ENGINE_MINOR_VERSION >= 1
					MessageHandler->OnControllerAnalog(AxisKey.GetFName(), PlatformUser, InputDeviceId, CurrentState.Axes[AxisIndex].GetValue());
#else
					MessageHandler->OnControllerAnalog(AxisKey.GetFName(), PlayerId, CurrentState.Axes[AxisIndex].GetValue());
#endif
				}
			}
		}

		// Hats Axis
		if (DeviceHatAxisKeys[0].Contains(InstanceId) && DeviceHatAxisKeys[1].Contains(InstanceId))
		{
			for (int HatIndex = 0; HatIndex < CurrentState.Hats.Num(); HatIndex++)
			{
				const FKey& XHatKey = DeviceHatAxisKeys[0][InstanceId][HatIndex];
				const FKey& YHatKey = DeviceHatAxisKeys[1][InstanceId][HatIndex];
				if (XHatKey.IsValid() && YHatKey.IsValid())
				{
					const FVector2D& POVAxis = UJoystickFunctionLibrary::POVAxis(CurrentState.Hats[HatIndex].GetValue());
#if ENGINE_MAJOR_VERSION == 5 && ENGINE_MINOR_VERSION >= 1
					MessageHandler->OnControllerAnalog(XHatKey.GetFName(), PlatformUser, InputDeviceId, POVAxis.X);
					MessageHandler->OnControllerAnalog(YHatKey.GetFName(), PlatformUser, InputDeviceId, POVAxis.Y);
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
			for (int HatKeyIndex = 0; HatKeyIndex < CurrentState.Hats.Num(); HatKeyIndex++)
			{
				FHatData& CurrentHatState = CurrentState.Hats[HatKeyIndex];
				const EJoystickPointOfViewDirection PreviousDirection = CurrentHatState.GetPreviousValue();
				const EJoystickPointOfViewDirection CurrentDirection = CurrentHatState.GetValue();

				if (PreviousDirection != CurrentDirection)
				{
					if (PreviousDirection != EJoystickPointOfViewDirection::None)
					{
						const uint8 DirectionIndex = static_cast<uint8>(PreviousDirection) - 1;
						const FKey& DirectionKey = DeviceHatButtonKeys[InstanceId][HatKeyIndex * 8 + DirectionIndex];
						if (DirectionKey.IsValid())
						{
#if ENGINE_MAJOR_VERSION == 5 && ENGINE_MINOR_VERSION >= 1
							MessageHandler->OnControllerButtonReleased(DirectionKey.GetFName(), PlatformUser, InputDeviceId, false);
#else
							MessageHandler->OnControllerButtonReleased(DirectionKey.GetFName(), PlayerId, false);
#endif
						}		
					}

					if (CurrentDirection != EJoystickPointOfViewDirection::None)
					{
						const uint8 DirectionIndex = static_cast<uint8>(CurrentDirection) - 1;
						const FKey& DirectionKey = DeviceHatButtonKeys[InstanceId][HatKeyIndex * 8 + DirectionIndex];
						if (DirectionKey.IsValid())
						{
#if ENGINE_MAJOR_VERSION == 5 && ENGINE_MINOR_VERSION >= 1
							MessageHandler->OnControllerButtonPressed(DirectionKey.GetFName(), PlatformUser, InputDeviceId, false);
#else
							MessageHandler->OnControllerButtonPressed(DirectionKey.GetFName(), PlayerId, false);
#endif
						}
					}

					CurrentHatState.PreviousDirection = CurrentDirection;
				}
			}
		}

		// Balls
		if (DeviceBallKeys[0].Contains(InstanceId) && DeviceBallKeys[1].Contains(InstanceId))
		{
			for (int BallIndex = 0; BallIndex < CurrentState.Balls.Num(); BallIndex++)
			{
				const FKey& XBallKey = DeviceBallKeys[0][InstanceId][BallIndex];
				const FKey& YBallKey = DeviceBallKeys[1][InstanceId][BallIndex];
				if (XBallKey.IsValid() && YBallKey.IsValid())
				{
					const FVector2D& BallAxis = CurrentState.Balls[BallIndex].GetValue();
#if ENGINE_MAJOR_VERSION == 5 && ENGINE_MINOR_VERSION >= 1
					MessageHandler->OnControllerAnalog(XBallKey.GetFName(), PlatformUser, InputDeviceId, BallAxis.X);
					MessageHandler->OnControllerAnalog(YBallKey.GetFName(), PlatformUser, InputDeviceId, BallAxis.Y);
#else
					MessageHandler->OnControllerAnalog(XBallKey.GetFName(), PlayerId, BallAxis.X);
					MessageHandler->OnControllerAnalog(YBallKey.GetFName(), PlayerId, BallAxis.Y);
#endif
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
					FButtonData& ButtonData = JoystickDeviceState[InstanceId].Buttons[ButtonIndex];
					if (ButtonData.GetValue() != ButtonData.GetPreviousValue())
					{
						if (ButtonData.GetValue())
						{
#if ENGINE_MAJOR_VERSION == 5 && ENGINE_MINOR_VERSION >= 1
							MessageHandler->OnControllerButtonPressed(ButtonKey.GetFName(), PlatformUser, InputDeviceId, false);
#else
							MessageHandler->OnControllerButtonPressed(ButtonKey.GetFName(), PlayerId, false);
#endif
						}
						else
						{
#if ENGINE_MAJOR_VERSION == 5 && ENGINE_MINOR_VERSION >= 1
							MessageHandler->OnControllerButtonReleased(ButtonKey.GetFName(), PlatformUser, InputDeviceId, false);
#else
							MessageHandler->OnControllerButtonReleased(ButtonKey.GetFName(), PlayerId, false);
#endif
						}

						ButtonData.PreviousButtonState = ButtonData.ButtonState;
					}
				}
			}
		}

		// Gyro
#if ENGINE_MAJOR_VERSION == 5 && ENGINE_MINOR_VERSION >= 1
		MessageHandler->OnMotionDetected(FVector::ZeroVector, CurrentState.Motion.Gyro, FVector::ZeroVector, CurrentState.Motion.Accelerometer, PlatformUser, InputDeviceId);
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

		const FJoystickInputDeviceConfiguration* DeviceConfig = JoystickInputSettings->GetInputDeviceConfiguration(DeviceInfo.ProductGuid);
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
			const FJoystickInputDeviceAxisProperties* AxisProperties = DeviceConfig->AxisProperties.FindByPredicate([&](const FJoystickInputDeviceAxisProperties& AxisProperty)
			{
				return AxisProperty.AxisIndex != -1 && AxisProperty.AxisIndex == i;
			});
			if (AxisProperties == nullptr)
			{
				continue;
			}

			FAxisData& AxisKeyData = CurrentState.Axes[i];
			AxisKeyData.RemappingEnabled = AxisProperties->RemappingEnabled;
			if (AxisKeyData.RemappingEnabled)
			{				
				AxisKeyData.Value = AxisProperties->InputRangeMin;
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
		const TArray<FKey> Keys = DeviceAxisKeys[i];
		if (Keys.Contains(Key))
		{
			return i;
		}
	}

	return -1;
}
