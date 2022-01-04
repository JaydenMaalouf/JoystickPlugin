
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

void FJoystickInputDevice::SetChannelValue(int32 ControllerId, FForceFeedbackChannelType ChannelType, float Value)
{
}

void FJoystickInputDevice::SetChannelValues(int32 ControllerId, const FForceFeedbackValues& Values)
{
}

void FJoystickInputDevice::SetMessageHandler(const TSharedRef<FGenericApplicationMessageHandler>& InMessageHandler)
{
	MessageHandler = InMessageHandler;
}

#define LOCTEXT_NAMESPACE "JoystickNamespace"

void FJoystickInputDevice::InitInputDevice(const FDeviceInfoSDL &Device)
{
	UJoystickSubsystem* JoystickSubsystem = GEngine->GetEngineSubsystem<UJoystickSubsystem>();
	if (JoystickSubsystem == nullptr)
	{
		return;
	}
	
	int32 DeviceId = Device.DeviceId;
	FJoystickInfo DeviceInfo;

	DeviceInfo.Connected = true;
	DeviceInfo.DeviceId = DeviceId;
	DeviceInfo.Player = 0;

	DeviceInfo.ProductId = JoystickSubsystem->GetDeviceIndexGuid(Device.DeviceIndex);
	DeviceInfo.ProductName = Device.DeviceName.Replace(TEXT("."), TEXT("")).Replace(TEXT(","), TEXT(""));
	DeviceInfo.DeviceName = DeviceInfo.ProductName.Replace(TEXT(" "), TEXT(""));

	UE_LOG(LogJoystickPlugin, Log, TEXT("add device %s %i"), *DeviceInfo.DeviceName, DeviceId);
	JoystickDeviceInfo.Emplace(DeviceId, DeviceInfo);

	FJoystickDeviceData InitialState = JoystickSubsystem->GetInitialDeviceState(DeviceId);
	JoystickDeviceData.Emplace(DeviceId, InitialState);

	FString BaseKeyName = FString::Printf(TEXT("Joystick_%s_%d"), *DeviceInfo.DeviceName, DeviceInfo.DeviceId);
	FString BaseDisplayName = FString::Printf(TEXT("%s %d"), *DeviceInfo.ProductName, DeviceInfo.DeviceId);
	
	// create FKeyDetails for axis
	DeviceAxisKeys.Emplace(DeviceId);
	
	int32 AxisCount = InitialState.Axes.Num();
	DeviceAxisKeys[DeviceId].SetNum(AxisCount);
	for (int32 AxisKeyIndex = 0; AxisKeyIndex < AxisCount; AxisKeyIndex++)
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
		DeviceAxisKeys[DeviceId][AxisKeyIndex] = AxisKeyDetails.GetKey();
	}

	// create FKeyDetails for buttons
	DeviceButtonKeys.Emplace(DeviceId);

	int32 ButtonCount = InitialState.Buttons.Num();
	DeviceButtonKeys[DeviceId].SetNum(ButtonCount);
	for (int32 ButtonKeyIndex = 0; ButtonKeyIndex < ButtonCount; ButtonKeyIndex++)
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

	FString AxisNames[] = { TEXT("X"), TEXT("Y") };

	// create FKeyDetails for hats
	for (int32 HatIndex = 0; HatIndex < 2; HatIndex++)
	{
		FString HatAxisName = *AxisNames[HatIndex];
		DeviceHatKeys[HatIndex].Emplace(DeviceId);

		int32 HatCount = InitialState.Hats.Num();
		DeviceHatKeys[HatIndex][DeviceId].SetNum(HatCount);
		for (int32 HatKeyIndex = 0; HatKeyIndex < HatCount; HatKeyIndex++)
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
	for (int32 BallIndex = 0; BallIndex < 2; BallIndex++)
	{
		FString BallAxisName = *AxisNames[BallIndex];
		DeviceBallKeys[BallIndex].Emplace(DeviceId);
		
		int32 BallCount = InitialState.Balls.Num();
		DeviceBallKeys[BallIndex][DeviceId].SetNum(BallCount);
		for (int32 BallKeyIndex = 0; BallKeyIndex < BallCount; BallKeyIndex++)
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
	if (JoystickInputSettings != nullptr)
	{
		JoystickInputSettings->DeviceAdded(FJoystickInputDeviceInformation(DeviceInfo.DeviceName, DeviceInfo.ProductName, DeviceInfo.ProductId));
	}
	
	UInputSettings* InputSettings = UInputSettings::GetInputSettings();
	if (InputSettings != nullptr)
	{
		InputSettings->PostInitProperties();
	}

}

#undef LOCTEXT_NAMESPACE

//Public API Implementation

void FJoystickInputDevice::JoystickPluggedIn(const FDeviceInfoSDL &DeviceInfoSDL)
{
	UE_LOG(LogJoystickPlugin, Log, TEXT("FJoystickPlugin::JoystickPluggedIn() %i"), DeviceInfoSDL.DeviceId);

	InitInputDevice(DeviceInfoSDL);
}

void FJoystickInputDevice::JoystickUnplugged(const int32 DeviceId)
{
	FJoystickInfo& InputDevice = JoystickDeviceInfo[DeviceId];
	InputDevice.Connected = false;	
	
	UJoystickInputSettings* JoystickInputSettings = GetMutableDefault<UJoystickInputSettings>();
	if (JoystickInputSettings == nullptr)
	{
		return;
	}
	
	JoystickInputSettings->DeviceRemoved(InputDevice.ProductId);
}

void FJoystickInputDevice::JoystickButton(const int32 DeviceId, const int32 Button, const bool Pressed)
{
	FButtonData& State = JoystickDeviceData[DeviceId].Buttons[Button];
	State.PreviousButtonState = State.ButtonState;
	State.ButtonState = Pressed;
}

void FJoystickInputDevice::JoystickAxis(const int32 DeviceId, const int32 Axis, const float Value)
{
	FAxisData& State = JoystickDeviceData[DeviceId].Axes[Axis];
	State.PreviousValue = State.Value;
	State.Value = Value;
}

void FJoystickInputDevice::JoystickHat(const int32 DeviceId, const int32 Hat, const EJoystickPOVDirection Value)
{
	FHatData& State = JoystickDeviceData[DeviceId].Hats[Hat];
	State.PreviousDirection = State.Direction;
	State.Direction = Value;
}

void FJoystickInputDevice::JoystickBall(const int32 DeviceId, const int32 Ball, const FVector2D Value)
{
	FBallData& State = JoystickDeviceData[DeviceId].Balls[Ball];
	State.PreviousDirection = State.Direction;
	State.Direction = Value;
}

FJoystickDeviceData FJoystickInputDevice::GetDeviceData(const int32 DeviceId)
{
	if (!JoystickDeviceData.Contains(DeviceId))
	{
		return FJoystickDeviceData();
	}

	return JoystickDeviceData[DeviceId];
}

FJoystickInfo FJoystickInputDevice::GetDeviceInfo(const int32 DeviceId)
{
	if (!JoystickDeviceInfo.Contains(DeviceId))
	{
		return FJoystickInfo();
	}

	return JoystickDeviceInfo[DeviceId];	
}

int32 FJoystickInputDevice::GetDeviceCount() const
{
	return JoystickDeviceInfo.Num();
}

static FName JoystickInputInterfaceName = FName("JoystickPluginInput");
void FJoystickInputDevice::SendControllerEvents()
{
	UJoystickSubsystem* JoystickSubsystem = GEngine->GetEngineSubsystem<UJoystickSubsystem>();
	if (JoystickSubsystem == nullptr)
	{
		return;
	}
	
	//UE_LOG(JoystickPluginLog, Log, TEXT("FJoystickDevice::SendControllerEvents()"));
	
	for (const TPair<int32, FJoystickInfo>& Device : JoystickDeviceInfo)
	{
		const int32 DeviceId = Device.Key;
		if (JoystickDeviceInfo.Contains(DeviceId) && JoystickDeviceData.Contains(DeviceId)) 
		{
			const FJoystickInfo& CurrentDevice = JoystickDeviceInfo[DeviceId];
			const FJoystickDeviceData& CurrentDeviceData = JoystickDeviceData[DeviceId];
			
			if (CurrentDevice.Connected)
			{
				const int32 PlayerId = CurrentDevice.Player;
				FInputDeviceScope InputScope(this, JoystickInputInterfaceName, DeviceId, CurrentDevice.DeviceName);

				//Axis
				if (DeviceAxisKeys.Contains(DeviceId))
				{
					for (int32 AxisIndex = 0; AxisIndex < CurrentDeviceData.Axes.Num(); AxisIndex++)
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
					for (int32 HatIndex = 0; HatIndex < CurrentDeviceData.Hats.Num(); HatIndex++)
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
					for (int32 BallIndex = 0; BallIndex < CurrentDeviceData.Balls.Num(); BallIndex++)
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
					for (int32 ButtonIndex = 0; ButtonIndex < CurrentDeviceData.Buttons.Num(); ButtonIndex++)
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
		}
	}

	JoystickSubsystem->Update();
}

void FJoystickInputDevice::ReinitialiseJoystickData(const int32 DeviceId, FJoystickDeviceData InitialState)
{
	JoystickDeviceData[DeviceId] = InitialState;
}

void FJoystickInputDevice::GetDeviceIds(TArray<int32>& DeviceIds) const
{
	JoystickDeviceInfo.GenerateKeyArray(DeviceIds);
}

void FJoystickInputDevice::SetPlayerOwnership(const int32 DeviceId, const int32 PlayerId)
{
	if (!JoystickDeviceInfo.Contains(DeviceId))
	{
		return;
	}
	
	JoystickDeviceInfo[DeviceId].Player = PlayerId;
}