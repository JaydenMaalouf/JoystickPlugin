// JoystickPlugin is licensed under the MIT License.
// Copyright Jayden Maalouf. All Rights Reserved.

#include "JoystickSubsystem.h"
#include "JoystickFunctionLibrary.h"
#include "JoystickInputDevice.h"
#include "JoystickInputSettings.h"
#include "JoystickLogManager.h"
#include "Runtime/Launch/Resources/Version.h"

THIRD_PARTY_INCLUDES_START

#include "SDL.h"
#include "SDL_haptic.h"
#include "SDL_joystick.h"

THIRD_PARTY_INCLUDES_END

UJoystickSubsystem::UJoystickSubsystem()
	: OwnsSDL(false)
	  , IsInitialised(false)
{
}

void UJoystickSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	UJoystickInputSettings* JoystickInputSettings = GetMutableDefault<UJoystickInputSettings>();
	if (IsValid(JoystickInputSettings))
	{
		JoystickInputSettings->ResetDevices();
	}

	FJoystickLogManager::Get()->LogDebug(TEXT("DeviceSDL Starting"));

	if (SDL_WasInit(SDL_INIT_JOYSTICK | SDL_INIT_GAMECONTROLLER | SDL_INIT_HAPTIC) != 0)
	{
		FJoystickLogManager::Get()->LogDebug(TEXT("SDL already loaded"));
		OwnsSDL = false;
	}
	else
	{
		FJoystickLogManager::Get()->LogDebug(TEXT("DeviceSDL::InitSDL() SDL init 0"));
		SDL_Init(SDL_INIT_JOYSTICK | SDL_INIT_GAMECONTROLLER  | SDL_INIT_HAPTIC);
		OwnsSDL = true;
	}

	if (JoystickSubsystemReady.IsBound())
	{
		JoystickSubsystemReady.Broadcast();
	}

	IsInitialised = true;
}

void UJoystickSubsystem::Deinitialize()
{
	Super::Deinitialize();

	FJoystickLogManager::Get()->LogDebug(TEXT("DeviceSDL Closing"));

	for (const auto& Device : Devices)
	{
		RemoveDevice(Device.Key);
	}

	SDL_DelEventWatch(HandleSDLEvent, this);

	if (OwnsSDL)
	{
		SDL_Quit();
	}

	IsInitialised = false;
}

void UJoystickSubsystem::InitialiseInputDevice(const TSharedPtr<FJoystickInputDevice> NewInputDevice)
{
	if (NewInputDevice == nullptr || !NewInputDevice.IsValid())
	{
		return;
	}

	InputDevicePtr = NewInputDevice;

	const int Result = SDL_WasInit(SDL_INIT_JOYSTICK);
	if (Result == 0)
	{
		return;
	}

	const int JoystickCount = GetJoystickCount();
	for (int i = 0; i < JoystickCount; i++)
	{
		AddDevice(i);
	}

	SDL_AddEventWatch(HandleSDLEvent, this);
}

int UJoystickSubsystem::GetJoystickCount() const
{
	return SDL_NumJoysticks();
}

int UJoystickSubsystem::GetRegisteredDeviceCount() const
{
	const FJoystickInputDevice* InputDevice = GetInputDevice();
	if (InputDevice == nullptr)
	{
		return -1;
	}

	return InputDevice->GetDeviceCount();
}

bool UJoystickSubsystem::GetJoystickData(const int DeviceId, FJoystickDeviceData& JoystickDeviceData) const
{
	FJoystickInputDevice* InputDevice = GetInputDevice();
	if (InputDevice == nullptr)
	{
		return false;
	}


	if (const FJoystickDeviceData* DeviceData = InputDevice->GetDeviceData(DeviceId))
	{
		JoystickDeviceData = *DeviceData;
		return true;
	}
	
	return false;
}

bool UJoystickSubsystem::GetJoystickInfo(const int DeviceId, FJoystickInfo& JoystickInfo) const
{
	FJoystickInputDevice* InputDevice = GetInputDevice();
	if (InputDevice == nullptr)
	{
		return false;
	}

	if (const FJoystickInfo* Info = InputDevice->GetDeviceInfo(DeviceId))
	{
		JoystickInfo = *Info;
		return true;
	}

	return false;
}

void UJoystickSubsystem::MapJoystickDeviceToPlayer(const int DeviceId, const int PlayerId)
{
	FJoystickInputDevice* InputDevice = GetInputDevice();
	if (InputDevice == nullptr)
	{
		return;
	}

	InputDevice->SetPlayerOwnership(DeviceId, PlayerId);
}

void UJoystickSubsystem::SetIgnoreGameControllers(const bool IgnoreControllers)
{
	UJoystickInputSettings* JoystickInputSettings = GetMutableDefault<UJoystickInputSettings>();
	if (!IsValid(JoystickInputSettings))
	{
		return;
	}
	
	const bool ChangedValue = JoystickInputSettings->SetIgnoreGameControllers(IgnoreControllers);
	if (ChangedValue && IgnoreControllers)
	{
		for (const auto& Device : Devices)
		{
			if (DeviceMapping.Contains(Device.Value.InstanceId) && SDL_IsGameController(Device.Value.DeviceIndex))
			{
				RemoveDevice(Device.Key);
			}
		}
	}
	else if (ChangedValue && !IgnoreControllers)
	{
		const int JoystickCount = GetJoystickCount();
		for (int i = 0; i < JoystickCount; i++)
		{
			if (SDL_IsGameController(i))
			{
				AddDevice(i);
			}
		}
	}
}

void UJoystickSubsystem::GetDeviceIds(TArray<int>& DeviceIds) const
{
	const FJoystickInputDevice* InputDevice = GetInputDevice();
	if (InputDevice == nullptr)
	{
		return;
	}

	InputDevice->GetDeviceIds(DeviceIds);
}

void UJoystickSubsystem::AddHapticDevice(FDeviceInfoSDL& Device) const
{
	Device.Haptic = SDL_HapticOpenFromJoystick(Device.Joystick);
	if (Device.Haptic != nullptr)
	{
		FJoystickLogManager::Get()->LogDebug(TEXT("Haptic Device detected"));
		FJoystickLogManager::Get()->LogDebug(TEXT("Number of Haptic Axis: %i"), SDL_HapticNumAxes(Device.Haptic));
		FJoystickLogManager::Get()->LogDebug(TEXT("SDL_HAPTIC_CONSTANT support: %s"), (SDL_HapticQuery(Device.Haptic) & SDL_HAPTIC_CONSTANT) == 1 ? TEXT("true") : TEXT("false"));
		FJoystickLogManager::Get()->LogDebug(TEXT("SDL_HAPTIC_SINE support: %s"), (SDL_HapticQuery(Device.Haptic) & SDL_HAPTIC_SINE) == 1 ? TEXT("true") : TEXT("false"));
		FJoystickLogManager::Get()->LogDebug(TEXT("SDL_HAPTIC_TRIANGLE support: %s"), (SDL_HapticQuery(Device.Haptic) & SDL_HAPTIC_TRIANGLE) == 1 ? TEXT("true") : TEXT("false"));
		FJoystickLogManager::Get()->LogDebug(TEXT("SDL_HAPTIC_SAWTOOTHUP support: %s"), (SDL_HapticQuery(Device.Haptic) & SDL_HAPTIC_SAWTOOTHUP) == 1 ? TEXT("true") : TEXT("false"));
		FJoystickLogManager::Get()->LogDebug(TEXT("SDL_HAPTIC_SAWTOOTHDOWN support: %s"), (SDL_HapticQuery(Device.Haptic) & SDL_HAPTIC_SAWTOOTHDOWN) == 1 ? TEXT("true") : TEXT("false"));
		FJoystickLogManager::Get()->LogDebug(TEXT("SDL_HAPTIC_RAMP support: %s"), (SDL_HapticQuery(Device.Haptic) & SDL_HAPTIC_RAMP) == 1 ? TEXT("true") : TEXT("false"));
		FJoystickLogManager::Get()->LogDebug(TEXT("SDL_HAPTIC_SPRING support: %s"), (SDL_HapticQuery(Device.Haptic) & SDL_HAPTIC_SPRING) == 1 ? TEXT("true") : TEXT("false"));
		FJoystickLogManager::Get()->LogDebug(TEXT("SDL_HAPTIC_DAMPER support: %s"), (SDL_HapticQuery(Device.Haptic) & SDL_HAPTIC_DAMPER) == 1 ? TEXT("true") : TEXT("false"));
		FJoystickLogManager::Get()->LogDebug(TEXT("SDL_HAPTIC_INERTIA support: %s"), (SDL_HapticQuery(Device.Haptic) & SDL_HAPTIC_INERTIA) == 1 ? TEXT("true") : TEXT("false"));
		FJoystickLogManager::Get()->LogDebug(TEXT("SDL_HAPTIC_FRICTION support: %s"), (SDL_HapticQuery(Device.Haptic) & SDL_HAPTIC_FRICTION) == 1 ? TEXT("true") : TEXT("false"));
		FJoystickLogManager::Get()->LogDebug(TEXT("SDL_HAPTIC_CUSTOM support: %s"), (SDL_HapticQuery(Device.Haptic) & SDL_HAPTIC_CUSTOM) == 1 ? TEXT("true") : TEXT("false"));
		FJoystickLogManager::Get()->LogDebug(TEXT("SDL_HAPTIC_GAIN support: %s"), (SDL_HapticQuery(Device.Haptic) & SDL_HAPTIC_GAIN) == 1 ? TEXT("true") : TEXT("false"));
		FJoystickLogManager::Get()->LogDebug(TEXT("SDL_HAPTIC_AUTOCENTER support: %s"), (SDL_HapticQuery(Device.Haptic) & SDL_HAPTIC_AUTOCENTER) == 1 ? TEXT("true") : TEXT("false"));
	}
}

bool UJoystickSubsystem::AddDevice(const int DeviceIndex)
{
	const UJoystickInputSettings* JoystickInputSettings = GetMutableDefault<UJoystickInputSettings>();
	if (!IsValid(JoystickInputSettings))
	{
		return false;
	}

	const bool IsGamepad = SDL_IsGameController(DeviceIndex) == SDL_TRUE;
	if (IsGamepad && JoystickInputSettings->GetIgnoreGameControllers())
	{
		// Let UE handle it
		return false;
	}

	FDeviceInfoSDL Device;
	Device.DeviceIndex = DeviceIndex;
	Device.IsGamepad = IsGamepad;

	Device.Joystick = SDL_JoystickOpen(DeviceIndex);
	if (Device.Joystick == nullptr)
	{
		return false;
	}

	Device.InstanceId = SDL_JoystickInstanceID(Device.Joystick);
	GetDeviceIndexGuid(DeviceIndex, Device.ProductId);

	// DEBUG
	Device.DeviceName = FString(ANSI_TO_TCHAR(SDL_JoystickName(Device.Joystick)));
	FJoystickLogManager::Get()->LogDebug(TEXT("%s:"), *Device.DeviceName);
	FJoystickLogManager::Get()->LogDebug(TEXT("\tInstance ID: %d"), Device.InstanceId);
	FJoystickLogManager::Get()->LogDebug(TEXT("\tDevice Index: %d"), Device.DeviceIndex);
	FJoystickLogManager::Get()->LogDebug(TEXT("\tNumber of Axis %i"), SDL_JoystickNumAxes(Device.Joystick));
	FJoystickLogManager::Get()->LogDebug(TEXT("\tNumber of Balls %i"), SDL_JoystickNumBalls(Device.Joystick));
	FJoystickLogManager::Get()->LogDebug(TEXT("\tNumber of Buttons %i"), SDL_JoystickNumButtons(Device.Joystick));
	FJoystickLogManager::Get()->LogDebug(TEXT("\tNumber of Hats %i"), SDL_JoystickNumHats(Device.Joystick));

#if ENGINE_MAJOR_VERSION >= 5
	const bool HasRumble = SDL_JoystickHasRumble(Device.Joystick) == SDL_TRUE;
#else ENGINE_MAJOR_VERSION == 4 && ENGINE_MINOR_VERSION >= 27
	const bool HasRumble = SDL_JoystickRumble(Device.Joystick, static_cast<Uint16>(0), static_cast<Uint16>(0), static_cast<Uint32>(1)) == 0;
#else
	const bool HasRumble = false;
#endif
	FJoystickLogManager::Get()->LogDebug(TEXT("\tRumble Support: %s"), HasRumble ? TEXT("true") : TEXT("false"));
	Device.HasRumble = HasRumble;

	if (SDL_JoystickIsHaptic(Device.Joystick))
	{
		AddHapticDevice(Device);
	}

	for (auto& ExistingDevice : Devices)
	{
		if (ExistingDevice.Value.Joystick == nullptr && ExistingDevice.Value.DeviceName == Device.DeviceName)
		{
			Device.DeviceId = ExistingDevice.Key;
			Devices[Device.DeviceId] = Device;

			DeviceMapping.Add(Device.InstanceId, Device.DeviceId);
			JoystickPluggedIn(Device);
			return true;
		}
	}

	Device.DeviceId = Devices.Num();
	Devices.Add(Device.DeviceId, Device);

	DeviceMapping.Add(Device.InstanceId, Device.DeviceId);
	JoystickPluggedIn(Device);
	return true;
}

bool UJoystickSubsystem::RemoveDevice(const int DeviceId)
{
	JoystickUnplugged(DeviceId);

	FDeviceInfoSDL* DeviceInfo = GetDeviceInfo(DeviceId);
	if (DeviceInfo == nullptr)
	{
		return nullptr;
	}

	if (DeviceInfo->Haptic != nullptr)
	{
		FJoystickLogManager::Get()->LogDebug(TEXT("Closing Haptic Device for %d"), DeviceId);
		SDL_HapticClose(DeviceInfo->Haptic);
		DeviceInfo->Haptic = nullptr;
	}
	if (DeviceInfo->Joystick != nullptr)
	{
		FJoystickLogManager::Get()->LogDebug(TEXT("Closing Joystick Device for %d"), DeviceId);
		SDL_JoystickClose(DeviceInfo->Joystick);
		DeviceInfo->Joystick = nullptr;
	}

	FJoystickLogManager::Get()->LogInformation(TEXT("Device Removed %d"), DeviceId);
	DeviceMapping.Remove(DeviceInfo->InstanceId);
	return true;
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

int UJoystickSubsystem::HandleSDLEvent(void* UserData, SDL_Event* Event)
{
	UJoystickSubsystem& JoystickSubsystem = *static_cast<UJoystickSubsystem*>(UserData);
	FJoystickInputDevice* InputDevice = JoystickSubsystem.GetInputDevice();
	if (InputDevice == nullptr)
	{
		return -1;
	}

	switch (Event->type)
	{
		case SDL_JOYDEVICEADDED:
			JoystickSubsystem.AddDevice(Event->cdevice.which);
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
				const int InstanceId = Event->cdevice.which;
				if (JoystickSubsystem.DeviceMapping.Contains(InstanceId))
				{
					const int DeviceId = JoystickSubsystem.DeviceMapping[InstanceId];
					JoystickSubsystem.RemoveDevice(DeviceId);
				}
				break;
			}
		case SDL_JOYBUTTONDOWN:
		case SDL_JOYBUTTONUP:
			if (JoystickSubsystem.DeviceMapping.Contains(Event->jbutton.which))
			{
				const int DeviceId = JoystickSubsystem.DeviceMapping[Event->jbutton.which];
				InputDevice->JoystickButton(DeviceId, Event->jbutton.button, Event->jbutton.state == SDL_PRESSED);

				FJoystickLogManager::Get()->LogDebug(TEXT("Event JoystickButton Device=%d Button=%d State=%d"), DeviceId, Event->jbutton.button, Event->jbutton.state);
			}
			break;
		case SDL_JOYAXISMOTION:
			if (JoystickSubsystem.DeviceMapping.Contains(Event->jaxis.which))
			{
				const int DeviceId = JoystickSubsystem.DeviceMapping[Event->jaxis.which];
				InputDevice->JoystickAxis(DeviceId, Event->jaxis.axis, Event->jaxis.value / (Event->jaxis.value < 0 ? 32768.0f : 32767.0f));
			}
			break;
		case SDL_JOYHATMOTION:
			if (JoystickSubsystem.DeviceMapping.Contains(Event->jhat.which))
			{
				const int DeviceId = JoystickSubsystem.DeviceMapping[Event->jhat.which];
				InputDevice->JoystickHat(DeviceId, Event->jhat.hat, UJoystickFunctionLibrary::HatValueToDirection(Event->jhat.value));
			}
			break;
		case SDL_JOYBALLMOTION:
			if (JoystickSubsystem.DeviceMapping.Contains(Event->jball.which))
			{
				const int DeviceId = JoystickSubsystem.DeviceMapping[Event->jball.which];
				InputDevice->JoystickBall(DeviceId, Event->jball.ball, FVector2D(Event->jball.xrel, Event->jball.yrel));
			}
			break;
		default:
			break;
	}

	return 0;
}

FJoystickDeviceData UJoystickSubsystem::CreateInitialDeviceState(const int DeviceId)
{
	const FDeviceInfoSDL* DeviceInfo = GetDeviceInfo(DeviceId);
	if (DeviceInfo == nullptr || DeviceInfo->Joystick == nullptr)
	{
		return FJoystickDeviceData();
	}

	FJoystickDeviceData DeviceState = FJoystickDeviceData();
	const int AxesCount = SDL_JoystickNumAxes(DeviceInfo->Joystick);
	const int ButtonCount = SDL_JoystickNumButtons(DeviceInfo->Joystick);
	const int HatsCount = SDL_JoystickNumHats(DeviceInfo->Joystick);
	const int BallsCount = SDL_JoystickNumBalls(DeviceInfo->Joystick);

	DeviceState.Axes.SetNumZeroed(AxesCount);
	DeviceState.Buttons.SetNumZeroed(ButtonCount);
	DeviceState.Hats.SetNumZeroed(HatsCount);
	DeviceState.Balls.SetNumZeroed(BallsCount);
	return DeviceState;
}

FJoystickInputDevice* UJoystickSubsystem::GetInputDevice() const
{
	if (!InputDevicePtr.IsValid())
	{
		return nullptr;
	}

	return InputDevicePtr.Get();
}

bool UJoystickSubsystem::IsReady() const
{
	return IsInitialised;
}

FString UJoystickSubsystem::GetDeviceIndexGuidString(const int DeviceIndex) const
{
	char Buffer[32];
	constexpr int8 SizeBuffer = sizeof(Buffer);

	const SDL_JoystickGUID GUID = SDL_JoystickGetDeviceGUID(DeviceIndex);
	SDL_JoystickGetGUIDString(GUID, Buffer, SizeBuffer);
	return ANSI_TO_TCHAR(Buffer);
}

void UJoystickSubsystem::GetDeviceIndexGuid(const int DeviceIndex, FGuid& Guid) const
{
	const SDL_JoystickGUID SDLGuid = SDL_JoystickGetDeviceGUID(DeviceIndex);
	memcpy(&Guid, &SDLGuid, sizeof(FGuid));
}

FDeviceInfoSDL* UJoystickSubsystem::GetDeviceInfo(const int DeviceId)
{
	const FJoystickInputDevice* InputDevice = GetInputDevice();
	if (InputDevice == nullptr)
	{
		return nullptr;
	}

	if (Devices.Num() == 0 || !Devices.Contains(DeviceId))
	{
		return nullptr;
	}

	return Devices.Find(DeviceId);
}

void UJoystickSubsystem::JoystickPluggedIn(const FDeviceInfoSDL& Device) const
{
	FJoystickInputDevice* InputDevice = GetInputDevice();
	if (InputDevice == nullptr)
	{
		return;
	}

	InputDevice->JoystickPluggedIn(Device);
	if (JoystickPluggedInDelegate.IsBound())
	{
		JoystickPluggedInDelegate.Broadcast(Device.DeviceId);
	}
}

void UJoystickSubsystem::JoystickUnplugged(const int DeviceId) const
{
	FJoystickInputDevice* InputDevice = GetInputDevice();
	if (InputDevice == nullptr)
	{
		return;
	}

	InputDevice->JoystickUnplugged(DeviceId);
	if (JoystickUnpluggedDelegate.IsBound())
	{
		JoystickUnpluggedDelegate.Broadcast(DeviceId);
	}
}
