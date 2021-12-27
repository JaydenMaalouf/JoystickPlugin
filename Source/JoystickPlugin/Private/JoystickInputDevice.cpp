
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
#include "JoystickSubsystem.h"

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

	DeviceInfo.ProductId = JoystickSubsystem->DeviceIndexToGUID(Device.DeviceIndex);
	DeviceInfo.ProductName = Device.DeviceName.Replace(TEXT(" "), TEXT("")).Replace(TEXT("."), TEXT(""));
	DeviceInfo.DeviceName = DeviceInfo.ProductName;

	UE_LOG(LogJoystickPlugin, Log, TEXT("add device %s %i"), *DeviceInfo.DeviceName, DeviceId);
	JoystickDeviceInfo.Emplace(DeviceId, DeviceInfo);

	FJoystickDeviceData InitialState = JoystickSubsystem->GetInitialDeviceState(DeviceId);
	JoystickDeviceData.Emplace(DeviceId, InitialState);

	// create FKeyDetails for axis
	DeviceAxisKeys.Emplace(DeviceId);
	for (int32 iAxis = 0; iAxis < InitialState.Axes.Num(); iAxis++)
	{
		FString strName = FString::Printf(TEXT("Joystick_%s_%d_Axis%d"), *DeviceInfo.DeviceName, DeviceInfo.DeviceId, iAxis);
		UE_LOG(LogJoystickPlugin, Log, TEXT("add %s %i"), *strName, DeviceId);
		DeviceAxisKeys[DeviceId].Add(FKey(FName(*strName)));

		if (!EKeys::GetKeyDetails(DeviceAxisKeys[DeviceId][iAxis]).IsValid())
		{
			FText textValue = FText::Format(LOCTEXT("Joystick_{0}_{1}_Axis{2}", "{0} {1} Axis {2}"), FText::FromString(DeviceInfo.ProductName), DeviceInfo.DeviceId, FText::AsNumber(iAxis));
			EKeys::AddKey(FKeyDetails(DeviceAxisKeys[DeviceId][iAxis], textValue, FKeyDetails::GamepadKey | FKeyDetails::Axis1D));
		}
	}

	// create FKeyDetails for buttons
	DeviceButtonKeys.Emplace(DeviceId);
	for (int32 iButton = 0; iButton < InitialState.Buttons.Num(); iButton++)
	{
		FString strName = FString::Printf(TEXT("Joystick_%s_%d_Button%d"), *DeviceInfo.DeviceName, DeviceInfo.DeviceId, iButton);
		UE_LOG(LogJoystickPlugin, Log, TEXT("add %s %i"), *strName, DeviceId);
		DeviceButtonKeys[DeviceId].Add(FKey(FName(*strName)));

		if (!EKeys::GetKeyDetails(DeviceButtonKeys[DeviceId][iButton]).IsValid())
		{
			FText textValue = FText::Format(LOCTEXT("Joystick_{0}_{1}_Button{2}", "{0} {1} Button {2}"), FText::FromString(DeviceInfo.ProductName), DeviceInfo.DeviceId, FText::AsNumber(iButton));
			EKeys::AddKey(FKeyDetails(DeviceButtonKeys[DeviceId][iButton], textValue, FKeyDetails::GamepadKey));
		}
	}

	FString _2DaxisNames[] = { TEXT("X"), TEXT("Y") };

	// create FKeyDetails for hats
	for (int32 iAxis = 0; iAxis < 2; iAxis++)
	{
		DeviceHatKeys[iAxis].Emplace(DeviceId);
		for (int32 iHat = 0; iHat < InitialState.Hats.Num(); iHat++)
		{
			FString strName = FString::Printf(TEXT("Joystick_%s_%d_Hat%d_%s"), *DeviceInfo.DeviceName, DeviceInfo.DeviceId, iHat, *_2DaxisNames[iAxis]);
			UE_LOG(LogJoystickPlugin, Log, TEXT("add %s %i"), *strName, DeviceId);
			FKey key{ *strName };
			DeviceHatKeys[iAxis][DeviceId].Add(key);

			if (!EKeys::GetKeyDetails(key).IsValid())
			{
				FText textValue = FText::Format(LOCTEXT("Joystick_{0}_{1}_Hat{2}_{3}", "{0} {1} Hat {2} {3}"), FText::FromString(DeviceInfo.ProductName), DeviceInfo.DeviceId, FText::AsNumber(iHat), FText::FromString(_2DaxisNames[iAxis]));
				EKeys::AddKey(FKeyDetails(key, textValue, FKeyDetails::GamepadKey | FKeyDetails::Axis1D));
			}
		}
	}

	// create FKeyDetails for balls
	for (int32 iAxis = 0; iAxis < 2; iAxis++)
	{
		DeviceBallKeys[iAxis].Emplace(DeviceId);
		for (int32 iBall = 0; iBall < InitialState.Balls.Num(); iBall++)
		{
			FString strName = FString::Printf(TEXT("Joystick_%s_%d_Ball%d_%s"), *DeviceInfo.DeviceName, DeviceInfo.DeviceId, iBall, *_2DaxisNames[iAxis]);
			UE_LOG(LogJoystickPlugin, Log, TEXT("add %s %i"), *strName, DeviceId);
			FKey key{ *strName };
			DeviceBallKeys[iAxis][DeviceId].Add(key);

			if (!EKeys::GetKeyDetails(key).IsValid())
			{
				FText textValue = FText::Format(LOCTEXT("Joystick_{0}_{1}_Ball{2}_{3}", "{0} {1} Ball {2} {3}"), FText::FromString(DeviceInfo.ProductName), DeviceInfo.DeviceId, FText::AsNumber(iBall), FText::FromString(_2DaxisNames[iAxis]));
				EKeys::AddKey(FKeyDetails(key, textValue, FKeyDetails::GamepadKey | FKeyDetails::Axis1D));
			}
		}
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
}

void FJoystickInputDevice::JoystickButton(const int32 DeviceId, const int32 Button, const bool Pressed)
{
	FButtonData& State = JoystickDeviceData[DeviceId].Buttons[Button];
	State.PreviousButtonState = State.ButtonState;
	State.ButtonState = Pressed;
}

void FJoystickInputDevice::JoystickAxis(const int32 DeviceId, const int32 Axis, const float Value)
{
	FAnalogData& State = JoystickDeviceData[DeviceId].Axes[Axis];
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
		if (JoystickDeviceInfo.Contains(DeviceId)) 
		{
			const FJoystickInfo& CurrentDevice = JoystickDeviceInfo[DeviceId];
			const FJoystickDeviceData& CurrentState = JoystickDeviceData[DeviceId];
			
			if (JoystickDeviceInfo[DeviceId].Connected) 
			{
				const int32 playerId = CurrentDevice.Player;
				FInputDeviceScope InputScope(this, JoystickInputInterfaceName, DeviceId, CurrentDevice.DeviceName);

				//Axis
				for (int32 AxisIndex = 0; AxisIndex < CurrentState.Axes.Num(); AxisIndex++)
				{
					const FKey& AxisKey = DeviceAxisKeys[DeviceId][AxisIndex];
					MessageHandler->OnControllerAnalog(AxisKey.GetFName(), playerId, CurrentState.Axes[AxisIndex].GetValue());
				}

				//Hats
				for (int32 HatIndex = 0; HatIndex < CurrentState.Hats.Num(); HatIndex++)
				{
					const FVector2D& POVAxis = UJoystickFunctionLibrary::POVAxis(CurrentState.Hats[HatIndex].Direction);
					MessageHandler->OnControllerAnalog(DeviceHatKeys[0][DeviceId][HatIndex].GetFName(), playerId, POVAxis.X);
					MessageHandler->OnControllerAnalog(DeviceHatKeys[1][DeviceId][HatIndex].GetFName(), playerId, POVAxis.Y);
				}

				//Balls
				for (int32 BallIndex = 0; BallIndex < CurrentState.Balls.Num(); BallIndex++)
				{
					const FVector2D& BallAxis = CurrentState.Balls[BallIndex].Direction;
					MessageHandler->OnControllerAnalog(DeviceBallKeys[0][DeviceId][BallIndex].GetFName(), playerId, BallAxis.X);
					MessageHandler->OnControllerAnalog(DeviceBallKeys[1][DeviceId][BallIndex].GetFName(), playerId, BallAxis.Y);
				}

				//Buttons
				for (int32 ButtonIndex = 0; ButtonIndex < CurrentState.Buttons.Num(); ButtonIndex++)
				{
					FButtonData& ButtonData = JoystickDeviceData[DeviceId].Buttons[ButtonIndex];

					const FKey& ButtonKey = DeviceButtonKeys[DeviceId][ButtonIndex];
					if (ButtonData.ButtonState != ButtonData.PreviousButtonState)
					{
						if (ButtonData.ButtonState)
						{
							MessageHandler->OnControllerButtonPressed(ButtonKey.GetFName(), playerId, false);
						}
						else
						{
							MessageHandler->OnControllerButtonReleased(ButtonKey.GetFName(), playerId, false);
						}
						
						ButtonData.PreviousButtonState = ButtonData.ButtonState;
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