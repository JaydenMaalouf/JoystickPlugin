/*
*
* Copyright (C) <2014> samiljan <Sam Persson>, tsky <thomas.kollakowksy@w-hs.de>
* All rights reserved.
*
* This software may be modified and distributed under the terms
* of the BSD license.  See the LICENSE file for details.
*/

#include "JoystickInputDevice.h"

#include "JoystickFunctionLibrary.h"
#include "JoystickInputSettings.h"
#include "JoystickSubsystem.h"
#include "GameFramework/InputSettings.h"

#include "Runtime/Launch/Resources/Version.h"

FJoystickInputDevice::FJoystickInputDevice(const TSharedRef<FGenericApplicationMessageHandler>& InMessageHandler) : MessageHandler(InMessageHandler)
{
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
}

void FJoystickInputDevice::SetMessageHandler(const TSharedRef<FGenericApplicationMessageHandler>& InMessageHandler)
{
	MessageHandler = InMessageHandler;
}

#define LOCTEXT_NAMESPACE "JoystickNamespace"

void FJoystickInputDevice::InitInputDevice(const FDeviceInfoSDL& Device)
{
	UJoystickSubsystem* JoystickSubsystem = GEngine->GetEngineSubsystem<UJoystickSubsystem>();
	if (!IsValid(JoystickSubsystem))
	{
		return;
	}

	int DeviceId = Device.DeviceId;
	FJoystickInfo DeviceInfo;

	DeviceInfo.Connected = true;
	DeviceInfo.DeviceId = DeviceId;
	DeviceInfo.Player = 0;

	JoystickSubsystem->GetDeviceIndexGuid(Device.DeviceIndex, DeviceInfo.ProductId);
	DeviceInfo.ProductName = Device.DeviceName.Replace(TEXT("."), TEXT("")).Replace(TEXT(","), TEXT(""));
	DeviceInfo.DeviceName = DeviceInfo.ProductName.Replace(TEXT(" "), TEXT(""));

	UE_LOG(LogJoystickPlugin, Log, TEXT("add device %s %i"), *DeviceInfo.DeviceName, DeviceId);
	JoystickDeviceInfo.Emplace(DeviceId, DeviceInfo);

	FJoystickDeviceData InitialState;
	const bool InitialDeviceStateResult = JoystickSubsystem->GetInitialDeviceState(DeviceId, InitialState);
	if (!InitialDeviceStateResult)
	{
		return;
	}

	FJoystickDeviceData& JoystickState = JoystickDeviceData.Emplace(DeviceId, InitialState);

	FString BaseKeyName = FString::Printf(TEXT("Joystick_%s_%d"), *DeviceInfo.DeviceName, DeviceInfo.DeviceId);
	FString BaseDisplayName = FString::Printf(TEXT("%s %d"), *DeviceInfo.ProductName, DeviceInfo.DeviceId);

	// create FKeyDetails for axis
	DeviceAxisKeys.Emplace(DeviceId);

	int AxisCount = InitialState.Axes.Num();
	DeviceAxisKeys[DeviceId].SetNum(AxisCount);
	for (int AxisKeyIndex = 0; AxisKeyIndex < AxisCount; AxisKeyIndex++)
	{
		FString AxisKeyName = FString::Printf(TEXT("%s_Axis%d"), *BaseKeyName, AxisKeyIndex);
		FString AxisDisplayName = FString::Printf(TEXT("%s Axis %d"), *BaseDisplayName, AxisKeyIndex);

		FKey AxisKey = FKey(FName(*AxisKeyName));
#if (ENGINE_MAJOR_VERSION == 4 && ENGINE_MINOR_VERSION >= 26 || ENGINE_MAJOR_VERSION > 4)
		FKeyDetails AxisKeyDetails = FKeyDetails(AxisKey, FText::FromString(AxisDisplayName), FKeyDetails::GamepadKey | FKeyDetails::Axis1D);
#else
		FKeyDetails AxisKeyDetails = FKeyDetails(AxisKey, FText::FromString(AxisDisplayName), FKeyDetails::GamepadKey | FKeyDetails::FloatAxis);
#endif

		if (!EKeys::GetKeyDetails(AxisKey).IsValid())
		{
			EKeys::AddKey(AxisKeyDetails);
			UE_LOG(LogJoystickPlugin, Log, TEXT("add key %s (%s) %i"), *AxisKeyName, *AxisDisplayName, DeviceId);
		}

		const FKey& MappedKey = AxisKeyDetails.GetKey();
		DeviceAxisKeys[DeviceId][AxisKeyIndex] = MappedKey;
		JoystickState.Axes[AxisKeyIndex].Key = MappedKey;
	}

	// create FKeyDetails for buttons
	DeviceButtonKeys.Emplace(DeviceId);

	int ButtonCount = InitialState.Buttons.Num();
	DeviceButtonKeys[DeviceId].SetNum(ButtonCount);
	for (int ButtonKeyIndex = 0; ButtonKeyIndex < ButtonCount; ButtonKeyIndex++)
	{
		FString ButtonKeyName = FString::Printf(TEXT("%s_Button%d"), *BaseKeyName, ButtonKeyIndex);
		FString ButtonDisplayName = FString::Printf(TEXT("%s Button %d"), *BaseDisplayName, ButtonKeyIndex);

		FKey ButtonKey = FKey(FName(*ButtonKeyName));
		FKeyDetails ButtonKeyDetails = FKeyDetails(ButtonKey, FText::FromString(ButtonDisplayName), FKeyDetails::GamepadKey);

		if (!EKeys::GetKeyDetails(ButtonKey).IsValid())
		{
			EKeys::AddKey(ButtonKeyDetails);
			UE_LOG(LogJoystickPlugin, Log, TEXT("add button %s (%s) %i"), *ButtonKeyName, *ButtonDisplayName, DeviceId);
		}
		DeviceButtonKeys[DeviceId][ButtonKeyIndex] = ButtonKeyDetails.GetKey();
	}

	FString AxisNames[] = {TEXT("X"), TEXT("Y")};

	// create FKeyDetails for hats
	for (int HatIndex = 0; HatIndex < 2; HatIndex++)
	{
		FString HatAxisName = *AxisNames[HatIndex];
		DeviceHatKeys[HatIndex].Emplace(DeviceId);

		int HatCount = InitialState.Hats.Num();
		DeviceHatKeys[HatIndex][DeviceId].SetNum(HatCount);
		for (int HatKeyIndex = 0; HatKeyIndex < HatCount; HatKeyIndex++)
		{
			FString HatKeyName = FString::Printf(TEXT("%s_Hat%d_%s"), *BaseKeyName, HatKeyIndex, *HatAxisName);
			FString HatDisplayName = FString::Printf(TEXT("%s Hat %d %s"), *BaseDisplayName, HatIndex, *HatAxisName);

			FKey HatKey = FKey(FName(*HatKeyName));
#if (ENGINE_MAJOR_VERSION == 4 && ENGINE_MINOR_VERSION >= 26 || ENGINE_MAJOR_VERSION > 4)
			FKeyDetails HatKeyDetails = FKeyDetails(HatKey, FText::FromString(HatDisplayName), FKeyDetails::GamepadKey | FKeyDetails::Axis1D);
#else
			FKeyDetails HatKeyDetails = FKeyDetails(HatKey, FText::FromString(HatDisplayName), FKeyDetails::GamepadKey | FKeyDetails::FloatAxis);
#endif

			if (!EKeys::GetKeyDetails(HatKey).IsValid())
			{
				EKeys::AddKey(HatKeyDetails);
				UE_LOG(LogJoystickPlugin, Log, TEXT("add hat %s (%s) %i"), *HatKeyName, *HatDisplayName, DeviceId);
			}
			DeviceHatKeys[HatIndex][DeviceId][HatKeyIndex] = HatKeyDetails.GetKey();
		}
	}

	// create FKeyDetails for balls
	for (int BallIndex = 0; BallIndex < 2; BallIndex++)
	{
		FString BallAxisName = *AxisNames[BallIndex];
		DeviceBallKeys[BallIndex].Emplace(DeviceId);

		int BallCount = InitialState.Balls.Num();
		DeviceBallKeys[BallIndex][DeviceId].SetNum(BallCount);
		for (int BallKeyIndex = 0; BallKeyIndex < BallCount; BallKeyIndex++)
		{
			FString BallKeyName = FString::Printf(TEXT("%s_Ball%d_%s"), *BaseKeyName, BallKeyIndex, *BallAxisName);
			FString BallDisplayName = FString::Printf(TEXT("%s Ball %d %s"), *BaseDisplayName, BallIndex, *BallAxisName);

			FKey BallKey = FKey(FName(*BallKeyName));
#if (ENGINE_MAJOR_VERSION == 4 && ENGINE_MINOR_VERSION >= 26 || ENGINE_MAJOR_VERSION > 4)
			FKeyDetails BallKeyDetails = FKeyDetails(BallKey, FText::FromString(BallDisplayName), FKeyDetails::GamepadKey | FKeyDetails::Axis1D);
#else
			FKeyDetails BallKeyDetails = FKeyDetails(BallKey, FText::FromString(BallDisplayName), FKeyDetails::GamepadKey | FKeyDetails::FloatAxis);
#endif

			if (!EKeys::GetKeyDetails(BallKey).IsValid())
			{
				EKeys::AddKey(BallKeyDetails);
				UE_LOG(LogJoystickPlugin, Log, TEXT("add ball %s (%s) %i"), *BallKeyName, *BallDisplayName, DeviceId);
			}
			DeviceBallKeys[BallIndex][DeviceId][BallKeyIndex] = BallKeyDetails.GetKey();
		}
	}

	UJoystickInputSettings* JoystickInputSettings = GetMutableDefault<UJoystickInputSettings>();
	if (IsValid(JoystickInputSettings))
	{
		JoystickInputSettings->DeviceAdded(FJoystickInputDeviceInformation(DeviceInfo));
	}

	UInputSettings* InputSettings = UInputSettings::GetInputSettings();
	if (IsValid(InputSettings))
	{
		InputSettings->PostInitProperties();
	}

	UpdateAxisProperties();
}

#undef LOCTEXT_NAMESPACE

//Public API Implementation

void FJoystickInputDevice::JoystickPluggedIn(const FDeviceInfoSDL& Device)
{
	UE_LOG(LogJoystickPlugin, Log, TEXT("FJoystickPlugin::JoystickPluggedIn() %i"), Device.DeviceId);

	InitInputDevice(Device);
}

void FJoystickInputDevice::JoystickUnplugged(const int DeviceId)
{
	FJoystickInfo& InputDevice = JoystickDeviceInfo[DeviceId];
	InputDevice.Connected = false;

	UJoystickInputSettings* JoystickInputSettings = GetMutableDefault<UJoystickInputSettings>();
	if (!IsValid(JoystickInputSettings))
	{
		return;
	}

	JoystickInputSettings->DeviceRemoved(InputDevice.ProductId);
}

void FJoystickInputDevice::JoystickButton(const int DeviceId, const int Button, const bool Pressed)
{
	if (!JoystickDeviceData.Contains(DeviceId))
	{
		return;
	}

	FJoystickDeviceData& DeviceData = JoystickDeviceData[DeviceId];
	if (!DeviceData.Buttons.IsValidIndex(Button))
	{
		return;
	}

	FButtonData& State = DeviceData.Buttons[Button];
	State.PreviousButtonState = State.ButtonState;
	State.ButtonState = Pressed;
}

void FJoystickInputDevice::JoystickAxis(const int DeviceId, const int Axis, const float Value)
{
	if (!JoystickDeviceData.Contains(DeviceId))
	{
		return;
	}

	FJoystickDeviceData& DeviceData = JoystickDeviceData[DeviceId];
	if (!DeviceData.Axes.IsValidIndex(Axis))
	{
		return;
	}

	FAxisData& State = DeviceData.Axes[Axis];
	State.PreviousValue = State.Value;
	State.Value = Value;
}

void FJoystickInputDevice::JoystickHat(const int DeviceId, const int Hat, const EJoystickPOVDirection Value)
{
	if (!JoystickDeviceData.Contains(DeviceId))
	{
		return;
	}

	FJoystickDeviceData& DeviceData = JoystickDeviceData[DeviceId];
	if (!DeviceData.Hats.IsValidIndex(Hat))
	{
		return;
	}

	FHatData& State = DeviceData.Hats[Hat];
	State.PreviousDirection = State.Direction;
	State.Direction = Value;
}

void FJoystickInputDevice::JoystickBall(const int DeviceId, const int Ball, const FVector2D Value)
{
	if (!JoystickDeviceData.Contains(DeviceId))
	{
		return;
	}

	FJoystickDeviceData& DeviceData = JoystickDeviceData[DeviceId];
	if (!DeviceData.Balls.IsValidIndex(Ball))
	{
		return;
	}

	FBallData& State = DeviceData.Balls[Ball];
	State.PreviousDirection = State.Direction;
	State.Direction = Value;
}

bool FJoystickInputDevice::GetDeviceData(const int DeviceId, FJoystickDeviceData& DeviceData)
{
	if (!JoystickDeviceData.Contains(DeviceId))
	{
		return false;
	}

	DeviceData = JoystickDeviceData[DeviceId];
	return true;
}

bool FJoystickInputDevice::GetDeviceInfo(const int DeviceId, FJoystickInfo& DeviceInfo)
{
	if (!JoystickDeviceInfo.Contains(DeviceId))
	{
		return false;
	}

	DeviceInfo = JoystickDeviceInfo[DeviceId];
	return true;
}

int FJoystickInputDevice::GetDeviceCount() const
{
	return JoystickDeviceInfo.Num();
}

static FName JoystickInputInterfaceName = FName("JoystickPluginInput");

void FJoystickInputDevice::SendControllerEvents()
{
	const UJoystickSubsystem* JoystickSubsystem = GEngine->GetEngineSubsystem<UJoystickSubsystem>();
	if (!IsValid(JoystickSubsystem))
	{
		return;
	}

	//UE_LOG(JoystickPluginLog, Log, TEXT("FJoystickDevice::SendControllerEvents()"));

	for (const TPair<int, FJoystickInfo>& Device : JoystickDeviceInfo)
	{
		const int DeviceId = Device.Key;
		if (!JoystickDeviceInfo.Contains(DeviceId) || !JoystickDeviceData.Contains(DeviceId))
		{
			continue;
		}

		const FJoystickInfo& CurrentDevice = JoystickDeviceInfo[DeviceId];
		if (!CurrentDevice.Connected)
		{
			continue;
		}

		const int PlayerId = CurrentDevice.Player;
		FInputDeviceScope InputScope(this, JoystickInputInterfaceName, DeviceId, CurrentDevice.DeviceName);
		const FJoystickDeviceData& CurrentDeviceData = JoystickDeviceData[DeviceId];

		//Axis
		if (DeviceAxisKeys.Contains(DeviceId))
		{
			for (int AxisIndex = 0; AxisIndex < CurrentDeviceData.Axes.Num(); AxisIndex++)
			{
				const FKey& AxisKey = DeviceAxisKeys[DeviceId][AxisIndex];
				if (AxisKey.IsValid())
				{
					MessageHandler->OnControllerAnalog(AxisKey.GetFName(), PlayerId, CurrentDeviceData.Axes[AxisIndex].GetValue());
				}
			}
		}

		//Hats
		if (DeviceHatKeys[0].Contains(DeviceId) && DeviceHatKeys[1].Contains(DeviceId))
		{
			for (int HatIndex = 0; HatIndex < CurrentDeviceData.Hats.Num(); HatIndex++)
			{
				const FKey& XHatKey = DeviceHatKeys[0][DeviceId][HatIndex];
				const FKey& YHatKey = DeviceHatKeys[1][DeviceId][HatIndex];
				if (XHatKey.IsValid() && YHatKey.IsValid())
				{
					const FVector2D& POVAxis = UJoystickFunctionLibrary::POVAxis(CurrentDeviceData.Hats[HatIndex].Direction);
					MessageHandler->OnControllerAnalog(XHatKey.GetFName(), PlayerId, POVAxis.X);
					MessageHandler->OnControllerAnalog(YHatKey.GetFName(), PlayerId, POVAxis.Y);
				}
			}
		}

		//Balls
		if (DeviceBallKeys[0].Contains(DeviceId) && DeviceBallKeys[1].Contains(DeviceId))
		{
			for (int BallIndex = 0; BallIndex < CurrentDeviceData.Balls.Num(); BallIndex++)
			{
				const FKey& XBallKey = DeviceBallKeys[0][DeviceId][BallIndex];
				const FKey& YBallKey = DeviceBallKeys[1][DeviceId][BallIndex];
				if (XBallKey.IsValid() && YBallKey.IsValid())
				{
					const FVector2D& BallAxis = CurrentDeviceData.Balls[BallIndex].Direction;
					MessageHandler->OnControllerAnalog(XBallKey.GetFName(), PlayerId, BallAxis.X);
					MessageHandler->OnControllerAnalog(YBallKey.GetFName(), PlayerId, BallAxis.Y);
				}
			}
		}

		//Buttons
		if (DeviceButtonKeys.Contains(DeviceId))
		{
			for (int ButtonIndex = 0; ButtonIndex < CurrentDeviceData.Buttons.Num(); ButtonIndex++)
			{
				const FKey& ButtonKey = DeviceButtonKeys[DeviceId][ButtonIndex];
				if (ButtonKey.IsValid())
				{
					FButtonData& ButtonData = JoystickDeviceData[DeviceId].Buttons[ButtonIndex];
					if (ButtonData.ButtonState != ButtonData.PreviousButtonState)
					{
						if (ButtonData.ButtonState)
						{
							MessageHandler->OnControllerButtonPressed(ButtonKey.GetFName(), PlayerId, false);
						}
						else
						{
							MessageHandler->OnControllerButtonReleased(ButtonKey.GetFName(), PlayerId, false);
						}

						ButtonData.PreviousButtonState = ButtonData.ButtonState;
					}
				}
			}
		}
	}

	JoystickSubsystem->Update();
}

void FJoystickInputDevice::GetDeviceIds(TArray<int>& DeviceIds) const
{
	JoystickDeviceInfo.GenerateKeyArray(DeviceIds);
}

void FJoystickInputDevice::SetPlayerOwnership(const int DeviceId, const int PlayerId)
{
	if (!JoystickDeviceInfo.Contains(DeviceId))
	{
		return;
	}

	JoystickDeviceInfo[DeviceId].Player = PlayerId;
}

void FJoystickInputDevice::UpdateAxisProperties()
{
	const UJoystickInputSettings* InputSettings = GetDefault<UJoystickInputSettings>();
	if (!IsValid(InputSettings))
	{
		return;
	}

	for (const TPair<int, FJoystickInfo>& Device : JoystickDeviceInfo)
	{
		FJoystickDeviceData* DeviceData = JoystickDeviceData.Find(Device.Key);
		if (DeviceData == nullptr)
		{
			continue;
		}

		const FJoystickInputDeviceConfiguration* DeviceConfig = InputSettings->
		                                                        DeviceConfigurations.FindByPredicate([&](const FJoystickInputDeviceConfiguration& PredicateDeviceConfig)
		                                                        {
			                                                        return (PredicateDeviceConfig.DeviceName.IsEmpty() || Device.Value.ProductName == PredicateDeviceConfig.DeviceName) &&
				                                                        (!PredicateDeviceConfig.ProductId.IsValid() || Device.Value.ProductId == PredicateDeviceConfig.ProductId);
		                                                        });
		if (DeviceConfig == nullptr)
		{
			continue;
		}

		for (FAxisData& AxisKeyData : DeviceData->Axes)
		{
			const FJoystickInputDeviceAxisProperties* AxisProperties = DeviceConfig->AxisProperties.Find(AxisKeyData.Key);
			if (AxisProperties == nullptr)
			{
				continue;
			}

			AxisKeyData.bRemapRanges = AxisProperties->bEnabled;
			AxisKeyData.InputOffset = AxisProperties->InputOffset;
			AxisKeyData.bInvertInput = AxisProperties->bInvertInput;
			AxisKeyData.InputRangeMin = AxisProperties->InputRangeMin;
			AxisKeyData.InputRangeMax = AxisProperties->InputRangeMax;
			AxisKeyData.OutputRangeMin = AxisProperties->OutputRangeMin;
			AxisKeyData.OutputRangeMax = AxisProperties->OutputRangeMax;
			AxisKeyData.bInvertOutput = AxisProperties->bInvertOutput;
		}
	}
}
