/*
*
* Copyright (C) <2014> samiljan <Sam Persson>, tsky <thomas.kollakowksy@w-hs.de>
* All rights reserved.
*
* This software may be modified and distributed under the terms
* of the BSD license.  See the LICENSE file for details.
*/

#include "DeviceSDL.h"

#include "JoystickInputSettings.h"

DEFINE_LOG_CATEGORY(JoystickPluginLog);

void FDeviceSDL::Init()
{ 
	UE_LOG(JoystickPluginLog, Log, TEXT("DeviceSDL Starting"));

	if (SDL_WasInit(0) != 0)
	{
		UE_LOG(JoystickPluginLog, Log, TEXT("SDL already loaded"));
		bOwnsSDL = false;
	}
	else
	{
		UE_LOG(JoystickPluginLog, Log, TEXT("DeviceSDL::InitSDL() SDL init 0"));
		SDL_Init(0);
		bOwnsSDL = true;
	}

	int result = SDL_InitSubSystem(SDL_INIT_HAPTIC);
	if (result == 0)
	{
		UE_LOG(JoystickPluginLog, Log, TEXT("DeviceSDL::InitSDL() SDL init subsystem haptic"));
	}

	result = SDL_InitSubSystem(SDL_INIT_GAMECONTROLLER);
	if (result == 0)
	{
		UE_LOG(JoystickPluginLog, Log, TEXT("DeviceSDL::InitSDL() SDL init subsystem joystick"));
	}

	result = SDL_InitSubSystem(SDL_INIT_JOYSTICK);
	if (result == 0)
	{
		UE_LOG(JoystickPluginLog, Log, TEXT("DeviceSDL::InitSDL() SDL init subsystem joystick"));
	}

	int Joysticks = SDL_NumJoysticks();
	for (int32 i = 0; i < Joysticks; i++)
	{
		AddDevice(i);
	}

	SDL_AddEventWatch(HandleSDLEvent, this);
}

FDeviceSDL::FDeviceSDL(IJoystickEventInterface* JoystickEventInterface) : EventInterface(JoystickEventInterface)
{
}

FDeviceSDL::~FDeviceSDL()
{
	UE_LOG(JoystickPluginLog, Log, TEXT("DeviceSDL Closing"));

	for (auto & Device : Devices)
	{
		RemoveDevice(Device.Key);
	}

	SDL_DelEventWatch(HandleSDLEvent, this);

	if (bOwnsSDL)
	{
		SDL_Quit();
	}
}

FDeviceInfoSDL * FDeviceSDL::GetDevice(int32 DeviceId)
{
	if (Devices.Contains(DeviceId))
	{
		return &Devices[DeviceId];
	}
	return nullptr;
}

void FDeviceSDL::IgnoreGameControllers(bool bIgnore)
{
	if (bIgnore && !bIgnoreGameControllers)
	{
		bIgnoreGameControllers = true;
		for (auto &Device : Devices)
		{
			if (DeviceMapping.Contains(Device.Value.InstanceId) && SDL_IsGameController(Device.Value.DeviceIndex))
			{
				RemoveDevice(Device.Key);
			}
		}
	}
	else if (!bIgnore && bIgnoreGameControllers)
	{
		bIgnoreGameControllers = false;
		int Joysticks = SDL_NumJoysticks();
		for (int32 i = 0; i < Joysticks; i++)
		{
			if (SDL_IsGameController(i))
			{
				AddDevice(i);
			}
		}
	}
}

//void FDeviceSDL::SetJoystickRumble(FDeviceId DeviceId, FForceFeedbackValues values)
//{
//	FDeviceInfoSDL* device = GetDevice(DeviceId);
//
//	uint16 lowFrequency = FMath::Clamp(Uint16(((values.LeftSmall + values.RightSmall) / 2) * float(UINT16_MAX)), Uint16(0), Uint16(UINT16_MAX));
//	uint16 highFrequency = FMath::Clamp(Uint16(((values.LeftLarge + values.RightLarge) / 2) * float(UINT16_MAX)), Uint16(0), Uint16(UINT16_MAX));
//
//	SDL_JoystickRumble(device->Joystick, lowFrequency, highFrequency, 1);
//}

FDeviceInfoSDL FDeviceSDL::AddDevice(int32 DeviceIndex)
{
	FDeviceInfoSDL Device;
	if (SDL_IsGameController(DeviceIndex) && bIgnoreGameControllers)
	{
		// Let UE handle it
		return Device;
	}

	Device.DeviceIndex = DeviceIndex;

	Device.Joystick = SDL_JoystickOpen(DeviceIndex);
	if (Device.Joystick == nullptr)
	{
		return Device;
	}
	
	Device.InstanceId = SDL_JoystickInstanceID(Device.Joystick);

	// DEBUG
	Device.DeviceName = FString(ANSI_TO_TCHAR(SDL_JoystickName(Device.Joystick)));
	UE_LOG(JoystickPluginLog, Log, TEXT("--- %s"), *Device.DeviceName);
	UE_LOG(JoystickPluginLog, Log, TEXT("--- Number of Axis %i"), SDL_JoystickNumAxes(Device.Joystick));
	UE_LOG(JoystickPluginLog, Log, TEXT("--- Number of Balls %i"), SDL_JoystickNumBalls(Device.Joystick));
	UE_LOG(JoystickPluginLog, Log, TEXT("--- Number of Buttons %i"), SDL_JoystickNumButtons(Device.Joystick));
	UE_LOG(JoystickPluginLog, Log, TEXT("--- Number of Hats %i"), SDL_JoystickNumHats(Device.Joystick));

	if (SDL_JoystickIsHaptic(Device.Joystick))
	{
		Device.Haptic = SDL_HapticOpenFromJoystick(Device.Joystick);
		if (Device.Haptic != nullptr)
		{
			UE_LOG(JoystickPluginLog, Log, TEXT("--- Haptic device detected"));

			UE_LOG(JoystickPluginLog, Log, TEXT("Number of Haptic Axis: %i"), SDL_HapticNumAxes(Device.Haptic));
			UE_LOG(JoystickPluginLog, Log, TEXT("Rumble Support: %i"), SDL_HapticRumbleSupported(Device.Haptic));

			UE_LOG(JoystickPluginLog, Log, TEXT("SDL_HAPTIC_CONSTANT support: %i"), (SDL_HapticQuery(Device.Haptic) & SDL_HAPTIC_CONSTANT));
			UE_LOG(JoystickPluginLog, Log, TEXT("SDL_HAPTIC_SINE support: %i"), (SDL_HapticQuery(Device.Haptic) & SDL_HAPTIC_SINE));
			UE_LOG(JoystickPluginLog, Log, TEXT("SDL_HAPTIC_TRIANGLE support: %i"), (SDL_HapticQuery(Device.Haptic) & SDL_HAPTIC_TRIANGLE));
			UE_LOG(JoystickPluginLog, Log, TEXT("SDL_HAPTIC_SAWTOOTHUP support: %i"), (SDL_HapticQuery(Device.Haptic) & SDL_HAPTIC_SAWTOOTHUP));
			UE_LOG(JoystickPluginLog, Log, TEXT("SDL_HAPTIC_SAWTOOTHDOWN support: %i"), (SDL_HapticQuery(Device.Haptic) & SDL_HAPTIC_SAWTOOTHDOWN));
			UE_LOG(JoystickPluginLog, Log, TEXT("SDL_HAPTIC_RAMP support: %i"), (SDL_HapticQuery(Device.Haptic) & SDL_HAPTIC_RAMP));
			UE_LOG(JoystickPluginLog, Log, TEXT("SDL_HAPTIC_SPRING support: %i"), (SDL_HapticQuery(Device.Haptic) & SDL_HAPTIC_SPRING));
			UE_LOG(JoystickPluginLog, Log, TEXT("SDL_HAPTIC_DAMPER support: %i"), (SDL_HapticQuery(Device.Haptic) &  SDL_HAPTIC_DAMPER));
			UE_LOG(JoystickPluginLog, Log, TEXT("SDL_HAPTIC_INERTIA support: %i"), (SDL_HapticQuery(Device.Haptic) &  SDL_HAPTIC_INERTIA));
			UE_LOG(JoystickPluginLog, Log, TEXT("SDL_HAPTIC_FRICTION support: %i"), (SDL_HapticQuery(Device.Haptic) &  SDL_HAPTIC_FRICTION));
			UE_LOG(JoystickPluginLog, Log, TEXT("SDL_HAPTIC_CUSTOM support: %i"), (SDL_HapticQuery(Device.Haptic) &  SDL_HAPTIC_CUSTOM));
			UE_LOG(JoystickPluginLog, Log, TEXT("SDL_HAPTIC_GAIN support: %i"), (SDL_HapticQuery(Device.Haptic) &  SDL_HAPTIC_GAIN));
			UE_LOG(JoystickPluginLog, Log, TEXT("SDL_HAPTIC_AUTOCENTER support: %i"), (SDL_HapticQuery(Device.Haptic) & SDL_HAPTIC_AUTOCENTER));
			
			if (SDL_HapticRumbleInit(Device.Haptic) != -1)
			{
				UE_LOG(JoystickPluginLog, Log, TEXT("--- init Rumble device SUCCESSFUL"));
			}
			else {
				UE_LOG(JoystickPluginLog, Log, TEXT("ERROR HapticRumbleInit FAILED"));
			}		

		}
	}	

	for (auto &ExistingDevice : Devices)
	{
		if (ExistingDevice.Value.Joystick == nullptr && ExistingDevice.Value.DeviceName == Device.DeviceName)
		{
			Device.DeviceId = ExistingDevice.Key;
			Devices[Device.DeviceId] = Device;

			DeviceMapping.Add(Device.InstanceId, Device.DeviceId);
			EventInterface->JoystickPluggedIn(Device);
			return Device;
		}
	}

	Device.DeviceId = Devices.Num();
	Devices.Add(Device.DeviceId, Device);

	DeviceMapping.Add(Device.InstanceId, Device.DeviceId);
	EventInterface->JoystickPluggedIn(Device);
	return Device;
}

void FDeviceSDL::RemoveDevice(int32 DeviceId)
{
	EventInterface->JoystickUnplugged(DeviceId);

	FDeviceInfoSDL &DeviceInfo = Devices[DeviceId];
	DeviceMapping.Remove(DeviceInfo.InstanceId);

	if (DeviceInfo.Haptic != nullptr)
	{
		SDL_HapticClose(DeviceInfo.Haptic);
		DeviceInfo.Haptic = nullptr;
	}

	if (DeviceInfo.Joystick != nullptr)
	{
		SDL_JoystickClose(DeviceInfo.Joystick);
		DeviceInfo.Joystick = nullptr;
	}
}

FString FDeviceSDL::DeviceGUIDtoString(int32 DeviceIndex)
{
	char buffer[32];
	int8 sizeBuffer = sizeof(buffer);

	SDL_JoystickGUID guid = SDL_JoystickGetDeviceGUID(DeviceIndex);
	SDL_JoystickGetGUIDString(guid, buffer, sizeBuffer);
	return ANSI_TO_TCHAR(buffer);
}

FGuid FDeviceSDL::DeviceIndexToGUID(int32 DeviceIndex)
{
	FGuid result;
	SDL_JoystickGUID guid = SDL_JoystickGetDeviceGUID(DeviceIndex);
	memcpy(&result, &guid, sizeof(FGuid));
	return result;
}

EJoystickPOVDirection SDL_hatValToDirection(int8 Value)
{
	switch (Value)
	{
	case SDL_HAT_CENTERED:  return EJoystickPOVDirection::DIRECTION_NONE;
	case SDL_HAT_UP:        return EJoystickPOVDirection::DIRECTION_UP;
	case SDL_HAT_RIGHTUP:   return EJoystickPOVDirection::DIRECTION_UP_RIGHT;
	case SDL_HAT_RIGHT:	    return EJoystickPOVDirection::DIRECTION_RIGHT;
	case SDL_HAT_RIGHTDOWN: return EJoystickPOVDirection::DIRECTION_DOWN_RIGHT;
	case SDL_HAT_DOWN:	    return EJoystickPOVDirection::DIRECTION_DOWN;
	case SDL_HAT_LEFTDOWN:  return EJoystickPOVDirection::DIRECTION_DOWN_LEFT;
	case SDL_HAT_LEFT:	    return EJoystickPOVDirection::DIRECTION_LEFT;
	case SDL_HAT_LEFTUP:    return EJoystickPOVDirection::DIRECTION_UP_LEFT;
	default:
		//UE_LOG(LogTemp, Warning, TEXT("Warning, POV unhandled case. %d"), (int32)value);
		return EJoystickPOVDirection::DIRECTION_NONE;
	}
}

FJoystickDeviceData FDeviceSDL::InitialDeviceState(int32 DeviceId)
{
	const FDeviceInfoSDL Device = Devices[DeviceId];
	FJoystickDeviceData State(DeviceId);

	if (Device.Joystick)
	{
		const int32 AxesCount = SDL_JoystickNumAxes(Device.Joystick);
		const int32 ButtonCount = SDL_JoystickNumButtons(Device.Joystick);
		const int32 HatsCount = SDL_JoystickNumHats(Device.Joystick);
		const int32 BallsCount = SDL_JoystickNumBalls(Device.Joystick);
		
		State.Axes.SetNumZeroed(AxesCount);
		State.Buttons.SetNumZeroed(ButtonCount);
		State.Hats.SetNumZeroed(HatsCount);
		State.Balls.SetNumZeroed(BallsCount);
		
		const UJoystickInputSettings* InputSettings = GetDefault<UJoystickInputSettings>();
		for (const FJoystickInputDeviceConfiguration& DeviceConfig : InputSettings->DeviceConfigurations)
		{
			const int32 DeviceGuid = FCString::Strtoi(*DeviceConfig.DeviceGUID, nullptr, 16);
			
			if ((DeviceConfig.DeviceName.IsEmpty() || Device.DeviceName == DeviceConfig.DeviceName) && (DeviceGuid == 0 || Device.DeviceId == DeviceGuid))
			{
				const int32 PropCount = DeviceConfig.AxisProperties.Num();
				for (int32 i = 0; i < FMath::Min(AxesCount, PropCount); i++)
				{
					if (!DeviceConfig.AxisProperties.IsValidIndex(i) || !State.Axes.IsValidIndex(i))
					{
						continue;
					}
					
					const FJoystickInputDeviceAxisProperties& AxisProps = DeviceConfig.AxisProperties[i];
					FAnalogData& AnalogData = State.Axes[i];
					if (AxisProps.bEnabled)
					{
						AnalogData.RangeMin = AxisProps.RangeMin;
						AnalogData.RangeMax = AxisProps.RangeMax;
						AnalogData.Offset = AxisProps.Offset;
						AnalogData.bInverted = AxisProps.bInverted;
						AnalogData.bGamepadStick = AxisProps.bGamepadStick;
					}
					else
					{
						AnalogData.KeyName = NAME_None;
					}
				}
				
				break;
			}
		}
	}

	
	//UE_LOG(JoystickPluginLog, Log, TEXT("DeviceSDL::getDeviceState() %s"), device.Name));

	return State;
}

void FDeviceSDL::Update()
{
	if (bOwnsSDL)
	{
		SDL_Event Event;
		while (SDL_PollEvent(&Event))
		{
			// The event watcher handles it
		}
	}
}

int FDeviceSDL::HandleSDLEvent(void* Userdata, SDL_Event* Event)
{
	FDeviceSDL& Self = *static_cast<FDeviceSDL*>(Userdata);

	switch (Event->type)
	{
	case SDL_JOYDEVICEADDED:
		Self.AddDevice(Event->cdevice.which);
		break;
	/*case SDL_CONTROLLERDEVICEADDED:
	{
		if (Self.bIgnoreGameControllers)
		{
			// Since JOYSTICK is inited before GAMECONTROLLER (by GAMECONTROLLER), 
			// a controller can be added as a joystick before we can check that it is a controller.
			// Remove it again and let UE handle it.
			FDeviceIndex DeviceIndex = FDeviceIndex(Event->cdevice.which);
			for (auto &Device : Self.Devices)
			{
				if (Device.Value.DeviceIndex == DeviceIndex && Self.DeviceMapping.Contains(Device.Value.InstanceId))
				{
					Self.DeviceMapping.Remove(Device.Value.InstanceId);
					Self.EventInterface->JoystickUnplugged(Device.Value.DeviceId);
				}
			}
		}
		break;
	}*/
	case SDL_JOYDEVICEREMOVED:
	{
		const int32 InstanceId = Event->cdevice.which;
		if (Self.DeviceMapping.Contains(InstanceId))
		{
			const int32 DeviceId = Self.DeviceMapping[InstanceId];
			Self.RemoveDevice(DeviceId);
		}
		break;
	}
	case SDL_JOYBUTTONDOWN:
	case SDL_JOYBUTTONUP:
		if (Self.DeviceMapping.Contains(Event->jbutton.which))
		{
			int32 DeviceId = Self.DeviceMapping[Event->jbutton.which];
			Self.EventInterface->JoystickButton(DeviceId, Event->jbutton.button, Event->jbutton.state == SDL_PRESSED);

			UE_LOG(JoystickPluginLog, Log, TEXT("Event JoystickButton Device=%d Button=%d State=%d"), DeviceId, Event->jbutton.button, Event->jbutton.state);
		}
		break;
	case SDL_JOYAXISMOTION:
		if (Self.DeviceMapping.Contains(Event->jaxis.which))
		{
			int32 DeviceId = Self.DeviceMapping[Event->jaxis.which];
			Self.EventInterface->JoystickAxis(DeviceId, Event->jaxis.axis, Event->jaxis.value / (Event->jaxis.value < 0 ? 32768.0f : 32767.0f));
		}
		break;
	case SDL_JOYHATMOTION:
		if (Self.DeviceMapping.Contains(Event->jhat.which))
		{
			int32 DeviceId = Self.DeviceMapping[Event->jhat.which];
			Self.EventInterface->JoystickHat(DeviceId, Event->jhat.hat, SDL_hatValToDirection(Event->jhat.value));
		}
		break;
	case SDL_JOYBALLMOTION:
		if (Self.DeviceMapping.Contains(Event->jball.which))
		{
			int32 DeviceId = Self.DeviceMapping[Event->jball.which];
			Self.EventInterface->JoystickBall(DeviceId, Event->jball.ball, FVector2D(Event->jball.xrel, Event->jball.yrel));
		}
		break;
	}

	return 0;
}

