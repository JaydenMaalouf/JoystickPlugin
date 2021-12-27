#include "JoystickSubsystem.h"

#include "JoystickFunctionLibrary.h"
#include "JoystickInputDeviceAxisProperties.h"
#include "JoystickInputSettings.h"

DEFINE_LOG_CATEGORY(LogJoystickPlugin);

UJoystickSubsystem::UJoystickSubsystem()
{
	IgnoreGameControllers = true;
}

void UJoystickSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	
	UE_LOG(LogJoystickPlugin, Log, TEXT("DeviceSDL Starting"));

	if (SDL_WasInit(0) != 0)
	{
		UE_LOG(LogJoystickPlugin, Log, TEXT("SDL already loaded"));
		OwnsSDL = false;
	}
	else
	{
		UE_LOG(LogJoystickPlugin, Log, TEXT("DeviceSDL::InitSDL() SDL init 0"));
		SDL_Init(0);
		OwnsSDL = true;
	}

	int32 Result = SDL_InitSubSystem(SDL_INIT_HAPTIC);
	if (Result == 0)
	{
		UE_LOG(LogJoystickPlugin, Log, TEXT("DeviceSDL::InitSDL() SDL init subsystem haptic"));
	}

	Result = SDL_InitSubSystem(SDL_INIT_GAMECONTROLLER);
	if (Result == 0)
	{
		UE_LOG(LogJoystickPlugin, Log, TEXT("DeviceSDL::InitSDL() SDL init subsystem joystick"));
	}

	Result = SDL_InitSubSystem(SDL_INIT_JOYSTICK);
	if (Result == 0)
	{
		UE_LOG(LogJoystickPlugin, Log, TEXT("DeviceSDL::InitSDL() SDL init subsystem joystick"));
	}

	if (JoystickSubsystemReady.IsBound())
	{
		JoystickSubsystemReady.Broadcast();		
	}
}

void UJoystickSubsystem::Deinitialize()
{
	Super::Deinitialize();
	
	UE_LOG(LogJoystickPlugin, Log, TEXT("DeviceSDL Closing"));

	for (const auto & Device : Devices)
	{
		RemoveDevice(Device.Key);
	}

	SDL_DelEventWatch(HandleSDLEvent, this);

	if (OwnsSDL)
	{
		SDL_Quit();
	}
}

void UJoystickSubsystem::InitialiseInputDevice(const TSharedPtr<FJoystickInputDevice> NewInputDevice)
{
	if (NewInputDevice == nullptr)
	{
		return;
	}

	InputDevice = NewInputDevice;
	
	const int32 Result = SDL_WasInit(SDL_INIT_JOYSTICK);
	if (Result == 0)
	{
		return;
	}

	const int32 JoystickCount = GetJoystickCount();
	for (int32 i = 0; i < JoystickCount; i++)
	{
		AddDevice(i);
	}

	SDL_AddEventWatch(HandleSDLEvent, this);
}

int32 UJoystickSubsystem::GetJoystickCount() const
{
	return SDL_NumJoysticks();
}

void UJoystickSubsystem::SetIgnoreGameControllers(const bool IgnoreControllers)
{
	if (IgnoreControllers && !IgnoreGameControllers)
	{
		IgnoreGameControllers = true;
		for (const auto &Device : Devices)
		{
			if (DeviceMapping.Contains(Device.Value.InstanceId) && SDL_IsGameController(Device.Value.DeviceIndex))
			{
				RemoveDevice(Device.Key);
			}
		}
	}
	else if (!IgnoreControllers && IgnoreGameControllers)
	{
		IgnoreGameControllers = false;
		const int32 JoystickCount = GetJoystickCount();
		for (int32 i = 0; i < JoystickCount; i++)
		{
			if (SDL_IsGameController(i))
			{
				AddDevice(i);
			}
		}
	}
}

FDeviceInfoSDL UJoystickSubsystem::AddDevice(const int32 DeviceIndex)
{
	FDeviceInfoSDL Device;
	if (SDL_IsGameController(DeviceIndex) && IgnoreGameControllers)
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
	UE_LOG(LogJoystickPlugin, Log, TEXT("--- %s"), *Device.DeviceName);
	UE_LOG(LogJoystickPlugin, Log, TEXT("--- Number of Axis %i"), SDL_JoystickNumAxes(Device.Joystick));
	UE_LOG(LogJoystickPlugin, Log, TEXT("--- Number of Balls %i"), SDL_JoystickNumBalls(Device.Joystick));
	UE_LOG(LogJoystickPlugin, Log, TEXT("--- Number of Buttons %i"), SDL_JoystickNumButtons(Device.Joystick));
	UE_LOG(LogJoystickPlugin, Log, TEXT("--- Number of Hats %i"), SDL_JoystickNumHats(Device.Joystick));

	if (SDL_JoystickIsHaptic(Device.Joystick))
	{
		Device.Haptic = SDL_HapticOpenFromJoystick(Device.Joystick);
		if (Device.Haptic != nullptr)
		{
			UE_LOG(LogJoystickPlugin, Log, TEXT("--- Haptic device detected"));

			UE_LOG(LogJoystickPlugin, Log, TEXT("Number of Haptic Axis: %i"), SDL_HapticNumAxes(Device.Haptic));
			UE_LOG(LogJoystickPlugin, Log, TEXT("Rumble Support: %i"), SDL_HapticRumbleSupported(Device.Haptic));

			UE_LOG(LogJoystickPlugin, Log, TEXT("SDL_HAPTIC_CONSTANT support: %i"), (SDL_HapticQuery(Device.Haptic) & SDL_HAPTIC_CONSTANT));
			UE_LOG(LogJoystickPlugin, Log, TEXT("SDL_HAPTIC_SINE support: %i"), (SDL_HapticQuery(Device.Haptic) & SDL_HAPTIC_SINE));
			UE_LOG(LogJoystickPlugin, Log, TEXT("SDL_HAPTIC_TRIANGLE support: %i"), (SDL_HapticQuery(Device.Haptic) & SDL_HAPTIC_TRIANGLE));
			UE_LOG(LogJoystickPlugin, Log, TEXT("SDL_HAPTIC_SAWTOOTHUP support: %i"), (SDL_HapticQuery(Device.Haptic) & SDL_HAPTIC_SAWTOOTHUP));
			UE_LOG(LogJoystickPlugin, Log, TEXT("SDL_HAPTIC_SAWTOOTHDOWN support: %i"), (SDL_HapticQuery(Device.Haptic) & SDL_HAPTIC_SAWTOOTHDOWN));
			UE_LOG(LogJoystickPlugin, Log, TEXT("SDL_HAPTIC_RAMP support: %i"), (SDL_HapticQuery(Device.Haptic) & SDL_HAPTIC_RAMP));
			UE_LOG(LogJoystickPlugin, Log, TEXT("SDL_HAPTIC_SPRING support: %i"), (SDL_HapticQuery(Device.Haptic) & SDL_HAPTIC_SPRING));
			UE_LOG(LogJoystickPlugin, Log, TEXT("SDL_HAPTIC_DAMPER support: %i"), (SDL_HapticQuery(Device.Haptic) &  SDL_HAPTIC_DAMPER));
			UE_LOG(LogJoystickPlugin, Log, TEXT("SDL_HAPTIC_INERTIA support: %i"), (SDL_HapticQuery(Device.Haptic) &  SDL_HAPTIC_INERTIA));
			UE_LOG(LogJoystickPlugin, Log, TEXT("SDL_HAPTIC_FRICTION support: %i"), (SDL_HapticQuery(Device.Haptic) &  SDL_HAPTIC_FRICTION));
			UE_LOG(LogJoystickPlugin, Log, TEXT("SDL_HAPTIC_CUSTOM support: %i"), (SDL_HapticQuery(Device.Haptic) &  SDL_HAPTIC_CUSTOM));
			UE_LOG(LogJoystickPlugin, Log, TEXT("SDL_HAPTIC_GAIN support: %i"), (SDL_HapticQuery(Device.Haptic) &  SDL_HAPTIC_GAIN));
			UE_LOG(LogJoystickPlugin, Log, TEXT("SDL_HAPTIC_AUTOCENTER support: %i"), (SDL_HapticQuery(Device.Haptic) & SDL_HAPTIC_AUTOCENTER));
			
			if (SDL_HapticRumbleInit(Device.Haptic) != -1)
			{
				UE_LOG(LogJoystickPlugin, Log, TEXT("--- init Rumble device SUCCESSFUL"));
			}
			else {
				UE_LOG(LogJoystickPlugin, Log, TEXT("ERROR HapticRumbleInit FAILED"));
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
			JoystickPluggedIn(Device);
			return Device;
		}
	}

	Device.DeviceId = Devices.Num();
	Devices.Add(Device.DeviceId, Device);

	DeviceMapping.Add(Device.InstanceId, Device.DeviceId);
	JoystickPluggedIn(Device);
	return Device;
}

void UJoystickSubsystem::RemoveDevice(const int32 DeviceId)
{
	JoystickUnplugged(DeviceId);

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

void UJoystickSubsystem::Update() const
{
	if (OwnsSDL)
	{
		SDL_Event Event;
		while (SDL_PollEvent(&Event))
		{
			// The event watcher handles it
		}
	}
}

int32 UJoystickSubsystem::HandleSDLEvent(void* Userdata, SDL_Event* Event)
{
	UJoystickSubsystem& Self = *static_cast<UJoystickSubsystem*>(Userdata);
	if (Self.InputDevice == nullptr)
	{
		return -1;
	}

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
			const int32 DeviceId = Self.DeviceMapping[Event->jbutton.which];
			Self.InputDevice->JoystickButton(DeviceId, Event->jbutton.button, Event->jbutton.state == SDL_PRESSED);

			UE_LOG(LogJoystickPlugin, Log, TEXT("Event JoystickButton Device=%d Button=%d State=%d"), DeviceId, Event->jbutton.button, Event->jbutton.state);
		}
		break;
	case SDL_JOYAXISMOTION:
		if (Self.DeviceMapping.Contains(Event->jaxis.which))
		{
			const int32 DeviceId = Self.DeviceMapping[Event->jaxis.which];
			Self.InputDevice->JoystickAxis(DeviceId, Event->jaxis.axis, Event->jaxis.value / (Event->jaxis.value < 0 ? 32768.0f : 32767.0f));
		}
		break;
	case SDL_JOYHATMOTION:
		if (Self.DeviceMapping.Contains(Event->jhat.which))
		{
			const int32 DeviceId = Self.DeviceMapping[Event->jhat.which];
			Self.InputDevice->JoystickHat(DeviceId, Event->jhat.hat, UJoystickFunctionLibrary::HatValueToDirection(Event->jhat.value));
		}
		break;
	case SDL_JOYBALLMOTION:
		if (Self.DeviceMapping.Contains(Event->jball.which))
		{
			const int32 DeviceId = Self.DeviceMapping[Event->jball.which];
			Self.InputDevice->JoystickBall(DeviceId, Event->jball.ball, FVector2D(Event->jball.xrel, Event->jball.yrel));
		}
		break;
	default:
		break;
	}

	return 0;
}

FJoystickDeviceData UJoystickSubsystem::GetInitialDeviceState(const int32 DeviceId)
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

FString UJoystickSubsystem::DeviceGUIDtoString(const int32 DeviceIndex) const
{
	char Buffer[32];
	constexpr int8 SizeBuffer = sizeof(Buffer);

	const SDL_JoystickGUID GUID = SDL_JoystickGetDeviceGUID(DeviceIndex);
	SDL_JoystickGetGUIDString(GUID, Buffer, SizeBuffer);
	return ANSI_TO_TCHAR(Buffer);
}

FGuid UJoystickSubsystem::DeviceIndexToGUID(const int32 DeviceIndex) const
{
	FGuid Result;
	const SDL_JoystickGUID GUID = SDL_JoystickGetDeviceGUID(DeviceIndex);
	memcpy(&Result, &GUID, sizeof(FGuid));
	return Result;
}

void UJoystickSubsystem::ReinitialiseJoystickData(const int32 DeviceId)
{	
	if (!InputDevice.IsValid())
	{
		return;
	}

	const FJoystickDeviceData InitialState = GetInitialDeviceState(DeviceId);
	InputDevice->ReinitialiseJoystickData(DeviceId, InitialState);
}

FDeviceInfoSDL* UJoystickSubsystem::GetDeviceInfo(const int32 DeviceId)
{
	if (!InputDevice.IsValid())
	{
		return nullptr;
	}
	
	if (Devices.Num() == 0 || !Devices.Contains(DeviceId))
	{
		return nullptr;
	}

	return &Devices[DeviceId];
}

void UJoystickSubsystem::JoystickPluggedIn(const FDeviceInfoSDL &Device) const
{
	InputDevice->JoystickPluggedIn(Device);
	UJoystickDeviceManager* JoystickDeviceManager = UJoystickDeviceManager::GetJoystickDeviceManager();
	if (JoystickDeviceManager == nullptr)
	{
		return;
	}

	if (JoystickDeviceManager->JoystickPluggedIn.IsBound())
	{
		JoystickDeviceManager->JoystickPluggedIn.Broadcast(Device.DeviceId);		
	}
}

void UJoystickSubsystem::JoystickUnplugged(const int32 DeviceId) const
{
	InputDevice->JoystickUnplugged(DeviceId);
	UJoystickDeviceManager* JoystickDeviceManager = UJoystickDeviceManager::GetJoystickDeviceManager();
	if (JoystickDeviceManager == nullptr)
	{
		return;
	}

	if (JoystickDeviceManager->JoystickUnplugged.IsBound())
	{
		JoystickDeviceManager->JoystickUnplugged.Broadcast(DeviceId);		
	}
}
