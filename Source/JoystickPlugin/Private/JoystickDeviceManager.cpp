
/*
*
* Copyright (C) <2014> samiljan <Sam Persson>, tsky <thomas.kollakowksy@w-hs.de>
* All rights reserved.
*
* This software may be modified and distributed under the terms
* of the BSD license.  See the LICENSE file for details.
*/

#include "JoystickDeviceManager.h"

#include "DeviceSDL.h"
#include "JoystickFunctionLibrary.h"
#include "ForcedFeedback/ForcedFeedbackFunctionLibrary.h"

JoystickDeviceManager::JoystickDeviceManager(const TSharedRef<FGenericApplicationMessageHandler>& InMessageHandler) : MessageHandler(InMessageHandler)
{
	UE_LOG(JoystickPluginLog, Log, TEXT("FJoystickPlugin::StartupModule() creating Device SDL"));

	DeviceSDL = MakeShareable(new FDeviceSDL(this));
	DeviceSDL->Init();
}

JoystickDeviceManager::~JoystickDeviceManager()
{
	DeviceSDL = nullptr;
}

void JoystickDeviceManager::Tick(float DeltaTime)
{

}

bool JoystickDeviceManager::Exec(UWorld* InWorld, const TCHAR* Cmd, FOutputDevice& Ar)
{
	return false;
}

void JoystickDeviceManager::SetChannelValue(int32 ControllerId, FForceFeedbackChannelType ChannelType, float Value)
{

}

void JoystickDeviceManager::SetChannelValues(int32 ControllerId, const FForceFeedbackValues& Values)
{

}

void JoystickDeviceManager::SetMessageHandler(const TSharedRef<FGenericApplicationMessageHandler>& InMessageHandler)
{
	MessageHandler = InMessageHandler;
}

void JoystickDeviceManager::InitInputDevice(const FDeviceInfoSDL &Device)
{
	FDeviceId DeviceId = Device.DeviceId;
	FJoystickInfo DeviceInfo;

	DeviceInfo.Connected = true;
	DeviceInfo.DeviceId = DeviceId.value;
	DeviceInfo.Player = 0;

	DeviceInfo.ProductId = FDeviceSDL::DeviceGUIDtoGUID(Device.DeviceIndex);
	DeviceInfo.ProductName = Device.Name;
	DeviceInfo.ProductName = DeviceInfo.ProductName.Replace(TEXT(" "), TEXT(""));
	DeviceInfo.DeviceName = DeviceInfo.ProductName.Replace(TEXT(" "), TEXT(""));
	DeviceInfo.ProductName = DeviceInfo.ProductName.Replace(TEXT("."), TEXT(""));
	DeviceInfo.DeviceName = DeviceInfo.DeviceName.Replace(TEXT("."), TEXT(""));

	UE_LOG(JoystickPluginLog, Log, TEXT("add device %s %i"), *DeviceInfo.DeviceName, DeviceId.value);
	InputDevices.Emplace(DeviceId, DeviceInfo);

	FJoystickState InitialState = DeviceSDL->InitialDeviceState(DeviceId);
	PreviousState.Emplace(DeviceId, InitialState);
	CurrentState.Emplace(DeviceId, InitialState);

	// create FKeyDetails for axis
	DeviceAxisKeys.Emplace(DeviceId);
	for (int32 iAxis = 0; iAxis < InitialState.Axes.Num(); iAxis++)
	{
		FString strName = FString::Printf(TEXT("Joystick_%s_%d_Axis%d"), *DeviceInfo.DeviceName, DeviceInfo.DeviceId, iAxis);
		UE_LOG(JoystickPluginLog, Log, TEXT("add %s %i"), *strName, DeviceId.value);
		DeviceAxisKeys[DeviceId].Add(FKey(FName(*strName)));

		if (!EKeys::GetKeyDetails(DeviceAxisKeys[DeviceId][iAxis]).IsValid())
		{
			FText textValue = FText::Format(NSLOCTEXT("JoystickPlugin", "DeviceAxis", "{0} {1} Axis {2}"), FText::FromString(DeviceInfo.ProductName), DeviceInfo.DeviceId, FText::AsNumber(iAxis));
			EKeys::AddKey(FKeyDetails(DeviceAxisKeys[DeviceId][iAxis], textValue, FKeyDetails::GamepadKey | FKeyDetails::Axis1D));
		}
	}

	// create FKeyDetails for buttons
	DeviceButtonKeys.Emplace(DeviceId);
	for (int32 iButton = 0; iButton < InitialState.Buttons.Num(); iButton++)
	{
		FString strName = FString::Printf(TEXT("Joystick_%s_%d_Button%d"), *DeviceInfo.DeviceName, DeviceInfo.DeviceId, iButton);
		UE_LOG(JoystickPluginLog, Log, TEXT("add %s %i"), *strName, DeviceId.value);
		DeviceButtonKeys[DeviceId].Add(FKey(FName(*strName)));

		if (!EKeys::GetKeyDetails(DeviceButtonKeys[DeviceId][iButton]).IsValid())
		{
			FText textValue = FText::Format(NSLOCTEXT("JoystickPlugin", "DeviceButton", "{0} {1} Button {2}"), FText::FromString(DeviceInfo.ProductName), DeviceInfo.DeviceId, FText::AsNumber(iButton));
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
			UE_LOG(JoystickPluginLog, Log, TEXT("add %s %i"), *strName, DeviceId.value);
			FKey key{ *strName };
			DeviceHatKeys[iAxis][DeviceId].Add(key);

			if (!EKeys::GetKeyDetails(key).IsValid())
			{
				FText textValue = FText::Format(NSLOCTEXT("JoystickPlugin", "DeviceHat", "{0} {1} Hat {2} {3}"), FText::FromString(DeviceInfo.ProductName), DeviceInfo.DeviceId, FText::AsNumber(iHat), FText::FromString(_2DaxisNames[iAxis]));
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
			UE_LOG(JoystickPluginLog, Log, TEXT("add %s %i"), *strName, DeviceId.value);
			FKey key{ *strName };
			DeviceBallKeys[iAxis][DeviceId].Add(key);

			if (!EKeys::GetKeyDetails(key).IsValid())
			{
				FText textValue = FText::Format(NSLOCTEXT("JoystickPlugin", "DeviceBall", "{0} {1} Ball {2} {3}"), FText::FromString(DeviceInfo.ProductName), DeviceInfo.DeviceId, FText::AsNumber(iBall), FText::FromString(_2DaxisNames[iAxis]));
				EKeys::AddKey(FKeyDetails(key, textValue, FKeyDetails::GamepadKey | FKeyDetails::Axis1D));
			}
		}
	}
}

//Public API Implementation

void JoystickDeviceManager::JoystickPluggedIn(const FDeviceInfoSDL &DeviceInfoSDL)
{
	UE_LOG(JoystickPluginLog, Log, TEXT("FJoystickPlugin::JoystickPluggedIn() %i"), DeviceInfoSDL.DeviceId.value);

	InitInputDevice(DeviceInfoSDL);
	for (auto & listener : EventListeners)
	{
		UObject * listenerObject = listener.Get();
		if (listenerObject != nullptr)
		{
			IJoystickInterface::Execute_JoystickPluggedIn(listenerObject, DeviceInfoSDL.DeviceId.value);
		}
	}
}

void JoystickDeviceManager::JoystickUnplugged(FDeviceId DeviceId)
{
	InputDevices[DeviceId].Connected = false;

	UE_LOG(JoystickPluginLog, Log, TEXT("Joystick %d disconnected"), DeviceId.value);

	for (auto & listener : EventListeners)
	{
		UObject * listenerObject = listener.Get();
		if (listenerObject != nullptr)
		{
			IJoystickInterface::Execute_JoystickUnplugged(listenerObject, DeviceId.value);
		}
	}
}

void JoystickDeviceManager::JoystickButton(FDeviceId DeviceId, int32 Button, bool Pressed)
{
	PreviousState[DeviceId].Buttons[Button] = CurrentState[DeviceId].Buttons[Button];
	CurrentState[DeviceId].Buttons[Button] = Pressed;

	for (auto & listener : EventListeners)
	{
		UObject * listenerObject = listener.Get();
		if (listenerObject != nullptr)
		{
			if (Pressed)
				IJoystickInterface::Execute_JoystickButtonPressed(listenerObject, Button, CurrentState[DeviceId]);
			else
				IJoystickInterface::Execute_JoystickButtonReleased(listenerObject, Button, CurrentState[DeviceId]);
		}
	}
}

void JoystickDeviceManager::JoystickAxis(FDeviceId DeviceId, int32 Axis, float Value)
{
	PreviousState[DeviceId].Axes[Axis] = CurrentState[DeviceId].Axes[Axis];
	CurrentState[DeviceId].Axes[Axis] = Value;

	for (auto & listener : EventListeners)
	{
		UObject * listenerObject = listener.Get();
		if (listenerObject != nullptr)
		{
			IJoystickInterface::Execute_JoystickAxisChanged(listenerObject, Axis, CurrentState[DeviceId].Axes[Axis], PreviousState[DeviceId].Axes[Axis], CurrentState[DeviceId], PreviousState[DeviceId]);
		}
	}
}

void JoystickDeviceManager::JoystickHat(FDeviceId DeviceId, int32 Hat, EJoystickPOVDirection Value)
{
	PreviousState[DeviceId].Hats[Hat] = CurrentState[DeviceId].Hats[Hat];
	CurrentState[DeviceId].Hats[Hat] = Value;

	for (auto & listener : EventListeners)
	{
		UObject * listenerObject = listener.Get();
		if (listenerObject != nullptr)
		{
			IJoystickInterface::Execute_JoystickHatChanged(listenerObject, Hat, Value, CurrentState[DeviceId]);
		}
	}
}

void JoystickDeviceManager::JoystickBall(FDeviceId DeviceId, int32 Ball, FVector2D Delta)
{
	PreviousState[DeviceId].Balls[Ball] = CurrentState[DeviceId].Balls[Ball];
	CurrentState[DeviceId].Balls[Ball] = Delta;

	for (auto & listener : EventListeners)
	{
		UObject * listenerObject = listener.Get();
		if (listenerObject != nullptr)
		{
			IJoystickInterface::Execute_JoystickBallMoved(listenerObject, Ball, Delta, CurrentState[DeviceId]);
		}
	}
}

void JoystickDeviceManager::SendControllerEvents()
{
	//UE_LOG(JoystickPluginLog, Log, TEXT("FJoystickDevice::SendControllerEvents()"));
	
	for (auto & Device : InputDevices)
	{
		FDeviceId DeviceId = Device.Key;
		if (InputDevices.Contains(DeviceId)) 
		{
			int32 playerId = InputDevices[DeviceId].Player;
			FJoystickState currentState = CurrentState[DeviceId];
			FJoystickState previousState = PreviousState[DeviceId];

			if (InputDevices[DeviceId].Connected) 
			{
				//Axis
				for (int32 axisIndex = 0; axisIndex < currentState.Axes.Num(); axisIndex++)
				{
					MessageHandler->OnControllerAnalog(DeviceAxisKeys[DeviceId][axisIndex].GetFName(), playerId, currentState.Axes[axisIndex]);
				}

				//Hats
				for (int32 hatIndex = 0; hatIndex < currentState.Hats.Num(); hatIndex++)
				{
					FVector2D povAxis = UJoystickFunctionLibrary::POVAxis(currentState.Hats[hatIndex]);
					MessageHandler->OnControllerAnalog(DeviceHatKeys[0][DeviceId][hatIndex].GetFName(), playerId, povAxis.X);
					MessageHandler->OnControllerAnalog(DeviceHatKeys[1][DeviceId][hatIndex].GetFName(), playerId, povAxis.Y);
				}

				//Balls
				for (int32 ballIndex = 0; ballIndex < currentState.Balls.Num(); ballIndex++)
				{
					FVector2D ballAxis = currentState.Balls[ballIndex];
					MessageHandler->OnControllerAnalog(DeviceBallKeys[0][DeviceId][ballIndex].GetFName(), playerId, ballAxis.X);
					MessageHandler->OnControllerAnalog(DeviceBallKeys[1][DeviceId][ballIndex].GetFName(), playerId, ballAxis.Y);

					CurrentState[DeviceId].Balls[ballIndex] = FVector2D::ZeroVector; //this was left over from the previous author, not sure what it is meant to achieve?
				}

				//Buttons
				for (int32 buttonIndex = 0; buttonIndex < currentState.Buttons.Num(); buttonIndex++)
				{
					bool currentPressed = currentState.Buttons[buttonIndex];
					bool previousPressed = previousState.Buttons[buttonIndex];

					if (currentPressed == true && previousPressed == false) 
					{
						MessageHandler->OnControllerButtonPressed(DeviceButtonKeys[DeviceId][buttonIndex].GetFName(), playerId, false);
					}
					else if (previousPressed == true && currentPressed == false)
					{
						MessageHandler->OnControllerButtonReleased(DeviceButtonKeys[DeviceId][buttonIndex].GetFName(), playerId, false);
					}
				}
			}
		}

	}

	DeviceSDL->Update();

	// Clean up weak references
	for (int32 i = 0; i < EventListeners.Num(); i++)
	{
		if (!EventListeners[i].IsValid())
		{
			EventListeners.RemoveAt(i);
			i--;
		}
	}

}

bool JoystickDeviceManager::AddEventListener(UObject* Listener)
{
	if (Listener != nullptr && Listener->GetClass()->ImplementsInterface(UJoystickInterface::StaticClass()))
	{
		EventListeners.Add(TWeakObjectPtr<>(Listener));
		return true;
	}
	return false;
}

void JoystickDeviceManager::IgnoreGameControllers(bool bIgnore)
{
	DeviceSDL->IgnoreGameControllers(bIgnore);
}
