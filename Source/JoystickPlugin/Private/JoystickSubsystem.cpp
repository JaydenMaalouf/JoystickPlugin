﻿// JoystickPlugin is licensed under the MIT License.
// Copyright Jayden Maalouf. All Rights Reserved.

#include "JoystickSubsystem.h"
#include "JoystickFunctionLibrary.h"
#include "JoystickInputDevice.h"
#include "JoystickInputSettings.h"
#include "JoystickLogManager.h"
#include "Data/JoystickInformation.h"
#include "Runtime/Launch/Resources/Version.h"

THIRD_PARTY_INCLUDES_START

#include "SDL.h"
#include "SDL_haptic.h"
#include "SDL_joystick.h"

THIRD_PARTY_INCLUDES_END

UJoystickSubsystem::UJoystickSubsystem()
	: OwnsSDL(false)
	  , IsInitialised(false)
	  , PersistentDeviceCount(0)
{
}

constexpr unsigned SdlRequiredFlags = SDL_INIT_JOYSTICK | SDL_INIT_GAMECONTROLLER | SDL_INIT_HAPTIC;

void UJoystickSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	UJoystickInputSettings* JoystickInputSettings = GetMutableDefault<UJoystickInputSettings>();
	if (IsValid(JoystickInputSettings))
	{
		JoystickInputSettings->ResetDevices();
	}

	FJoystickLogManager::Get()->LogDebug(TEXT("DeviceSDL Starting"));

	if (SDL_WasInit(SdlRequiredFlags) != 0)
	{
		FJoystickLogManager::Get()->LogDebug(TEXT("SDL already loaded"));
		OwnsSDL = false;
	}
	else
	{
		FJoystickLogManager::Get()->LogDebug(TEXT("DeviceSDL::InitSDL() SDL init 0"));
		SDL_Init(SdlRequiredFlags);
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

	for (int DeviceIndex = 0; DeviceIndex < GetRawJoystickCount(); DeviceIndex++)
	{
		RemoveDevice(DeviceIndex);
	}

	SDL_DelEventWatch(HandleSDLEvent, this);

	if (OwnsSDL)
	{
		SDL_Quit();
	}

	IsInitialised = false;
}

void UJoystickSubsystem::InitialiseExistingJoysticks()
{
	const int JoystickCount = GetRawJoystickCount();
	for (int i = 0; i < JoystickCount; i++)
	{
		AddDevice(i);
	}
}

void UJoystickSubsystem::InitialiseInputDevice(const TSharedPtr<FJoystickInputDevice> NewInputDevice)
{
	if (NewInputDevice == nullptr || !NewInputDevice.IsValid())
	{
		return;
	}

	InputDevicePtr = NewInputDevice;

	const int Result = SDL_WasInit(SdlRequiredFlags);
	if (Result == 0)
	{
		return;
	}

	InitialiseExistingJoysticks();

	SDL_AddEventWatch(HandleSDLEvent, this);
}

int UJoystickSubsystem::GetRawJoystickCount() const
{
	return SDL_NumJoysticks();
}

int UJoystickSubsystem::GetJoystickCount() const
{
	return Devices.Num();
}

int UJoystickSubsystem::GetConnectedJoystickCount() const
{
	int Count = 0;
	for (const TPair<FJoystickInstanceId, FDeviceInfoSDL>& DeviceInfo : Devices)
	{
		if (!DeviceInfo.Value.Connected)
		{
			continue;
		}

		Count++;
	}

	return Count;
}

bool UJoystickSubsystem::GetJoystickState(const FJoystickInstanceId& InstanceId, FJoystickDeviceState& JoystickDeviceState)
{
	FJoystickInputDevice* InputDevice = GetInputDevice();
	if (InputDevice == nullptr)
	{
		return false;
	}

	if (const FJoystickDeviceState* DeviceData = InputDevice->GetDeviceData(InstanceId))
	{
		JoystickDeviceState = *DeviceData;
		return true;
	}

	return false;
}

bool UJoystickSubsystem::GetJoystickInfo(const FJoystickInstanceId& InstanceId, FJoystickInformation& JoystickInfo)
{
	const FDeviceInfoSDL* DeviceInfo = GetDeviceInfo(InstanceId);
	if (DeviceInfo == nullptr)
	{
		return false;
	}

	JoystickInfo = static_cast<FJoystickInformation>(*DeviceInfo);

	return true;
}

EJoystickType UJoystickSubsystem::GetJoystickType(const FJoystickInstanceId& InstanceId)
{
	const FDeviceInfoSDL* DeviceInfo = GetDeviceInfo(InstanceId);
	if (DeviceInfo == nullptr)
	{
		return EJoystickType::Unknown;
	}

	return DeviceInfo->Type;
}

EJoystickPowerLevel UJoystickSubsystem::GetJoystickPowerLevel(const FJoystickInstanceId& InstanceId)
{
	SDL_Joystick* Joystick = SDL_JoystickFromInstanceID(InstanceId);
	if (Joystick == nullptr)
	{
		return EJoystickPowerLevel::Unknown;
	}

	FDeviceInfoSDL* DeviceInfo = GetDeviceInfo(InstanceId);
	if (DeviceInfo == nullptr)
	{
		return EJoystickPowerLevel::Unknown;
	}

	const SDL_JoystickPowerLevel PowerLevel = SDL_JoystickCurrentPowerLevel(Joystick);
	DeviceInfo->PowerLevel = static_cast<EJoystickPowerLevel>(PowerLevel + 1);

	return DeviceInfo->PowerLevel;
}

void UJoystickSubsystem::MapJoystickDeviceToPlayer(const FJoystickInstanceId& InstanceId, const int PlayerId)
{
	FDeviceInfoSDL* DeviceInfo = GetDeviceInfo(InstanceId);
	if (DeviceInfo == nullptr)
	{
		return;
	}

	DeviceInfo->PlayerId = PlayerId;
}

void UJoystickSubsystem::SetIgnoreGameControllers(const bool IgnoreControllers)
{
	UJoystickInputSettings* JoystickInputSettings = GetMutableDefault<UJoystickInputSettings>();
	if (!IsValid(JoystickInputSettings))
	{
		return;
	}

	const int JoystickCount = GetRawJoystickCount();
	const bool ChangedValue = JoystickInputSettings->SetIgnoreGameControllers(IgnoreControllers);
	if (ChangedValue && IgnoreControllers)
	{
		for (int DeviceIndex = 0; DeviceIndex < JoystickCount; DeviceIndex++)
		{
			if (SDL_IsGameController(DeviceIndex) == SDL_FALSE)
			{
				continue;
			}

			RemoveDeviceByIndex(DeviceIndex);
		}
	}
	else if (ChangedValue && !IgnoreControllers)
	{
		for (int DeviceIndex = 0; DeviceIndex < JoystickCount; DeviceIndex++)
		{
			if (SDL_IsGameController(DeviceIndex) == SDL_FALSE)
			{
				continue;
			}

			AddDevice(DeviceIndex);
		}
	}
}

bool UJoystickSubsystem::SetJoystickLedColor(const FJoystickInstanceId& InstanceId, const FColor Color)
{
#if ENGINE_MAJOR_VERSION == 5
	const FDeviceInfoSDL* DeviceInfo = GetDeviceInfo(InstanceId);
	if (DeviceInfo == nullptr || DeviceInfo->Joystick == nullptr)
	{
		return false;
	}

	return SDL_JoystickSetLED(DeviceInfo->Joystick, Color.R, Color.G, Color.B) == 0;
#else
	return false;
#endif
}

void UJoystickSubsystem::GetInstanceIds(TArray<FJoystickInstanceId>& InstanceIds) const
{
	Devices.GenerateKeyArray(InstanceIds);
}

bool UJoystickSubsystem::HasRumbleDevice() const
{
	for (const TTuple<FJoystickInstanceId, FDeviceInfoSDL>& Device : Devices)
	{
		if (Device.Value.IsGamepad || Device.Value.RumbleSupport)
		{
			return true;
		}
	}

	return false;
}

bool UJoystickSubsystem::IsConnected(const FJoystickInstanceId& InstanceId) const
{
	if (!Devices.Contains(InstanceId))
	{
		return false;
	}

	return Devices[InstanceId].Connected;
}

void UJoystickSubsystem::AddHapticDevice(FDeviceInfoSDL& Device) const
{
	Device.Haptic = SDL_HapticOpenFromJoystick(Device.Joystick);
	if (Device.Haptic != nullptr)
	{
		Device.HapticSupport = true;
		FJoystickLogManager::Get()->LogDebug(TEXT("Haptic Device detected"));
		FJoystickLogManager::Get()->LogDebug(TEXT("\tNumber of Haptic Axis: %i"), SDL_HapticNumAxes(Device.Haptic));
		FJoystickLogManager::Get()->LogDebug(TEXT("\tSDL_HAPTIC_CONSTANT support: %s"), (SDL_HapticQuery(Device.Haptic) & SDL_HAPTIC_CONSTANT) == 1 ? TEXT("true") : TEXT("false"));
		FJoystickLogManager::Get()->LogDebug(TEXT("\tSDL_HAPTIC_SINE support: %s"), (SDL_HapticQuery(Device.Haptic) & SDL_HAPTIC_SINE) == 1 ? TEXT("true") : TEXT("false"));
		FJoystickLogManager::Get()->LogDebug(TEXT("\tSDL_HAPTIC_TRIANGLE support: %s"), (SDL_HapticQuery(Device.Haptic) & SDL_HAPTIC_TRIANGLE) == 1 ? TEXT("true") : TEXT("false"));
		FJoystickLogManager::Get()->LogDebug(TEXT("\tSDL_HAPTIC_SAWTOOTHUP support: %s"), (SDL_HapticQuery(Device.Haptic) & SDL_HAPTIC_SAWTOOTHUP) == 1 ? TEXT("true") : TEXT("false"));
		FJoystickLogManager::Get()->LogDebug(TEXT("\tSDL_HAPTIC_SAWTOOTHDOWN support: %s"), (SDL_HapticQuery(Device.Haptic) & SDL_HAPTIC_SAWTOOTHDOWN) == 1 ? TEXT("true") : TEXT("false"));
		FJoystickLogManager::Get()->LogDebug(TEXT("\tSDL_HAPTIC_RAMP support: %s"), (SDL_HapticQuery(Device.Haptic) & SDL_HAPTIC_RAMP) == 1 ? TEXT("true") : TEXT("false"));
		FJoystickLogManager::Get()->LogDebug(TEXT("\tSDL_HAPTIC_SPRING support: %s"), (SDL_HapticQuery(Device.Haptic) & SDL_HAPTIC_SPRING) == 1 ? TEXT("true") : TEXT("false"));
		FJoystickLogManager::Get()->LogDebug(TEXT("\tSDL_HAPTIC_DAMPER support: %s"), (SDL_HapticQuery(Device.Haptic) & SDL_HAPTIC_DAMPER) == 1 ? TEXT("true") : TEXT("false"));
		FJoystickLogManager::Get()->LogDebug(TEXT("\tSDL_HAPTIC_INERTIA support: %s"), (SDL_HapticQuery(Device.Haptic) & SDL_HAPTIC_INERTIA) == 1 ? TEXT("true") : TEXT("false"));
		FJoystickLogManager::Get()->LogDebug(TEXT("\tSDL_HAPTIC_FRICTION support: %s"), (SDL_HapticQuery(Device.Haptic) & SDL_HAPTIC_FRICTION) == 1 ? TEXT("true") : TEXT("false"));
		FJoystickLogManager::Get()->LogDebug(TEXT("\tSDL_HAPTIC_CUSTOM support: %s"), (SDL_HapticQuery(Device.Haptic) & SDL_HAPTIC_CUSTOM) == 1 ? TEXT("true") : TEXT("false"));
		FJoystickLogManager::Get()->LogDebug(TEXT("\tSDL_HAPTIC_GAIN support: %s"), (SDL_HapticQuery(Device.Haptic) & SDL_HAPTIC_GAIN) == 1 ? TEXT("true") : TEXT("false"));
		FJoystickLogManager::Get()->LogDebug(TEXT("\tSDL_HAPTIC_AUTOCENTER support: %s"), (SDL_HapticQuery(Device.Haptic) & SDL_HAPTIC_AUTOCENTER) == 1 ? TEXT("true") : TEXT("false"));
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
	Device.IsGamepad = IsGamepad;
	Device.Connected = true;
	Device.InstanceId = SDL_JoystickGetDeviceInstanceID(DeviceIndex);
	Device.VendorId = SDL_JoystickGetDeviceVendor(DeviceIndex);
	Device.ProductId = SDL_JoystickGetDeviceProduct(DeviceIndex);
	Device.ProductVersion = SDL_JoystickGetDeviceProductVersion(DeviceIndex);

	const SDL_JoystickGUID SDLGuid = SDL_JoystickGetDeviceGUID(DeviceIndex);
	memcpy(&Device.ProductGuid, &SDLGuid, sizeof(FGuid));

	Device.Joystick = SDL_JoystickOpen(DeviceIndex);
	if (Device.Joystick == nullptr)
	{
		const FString ErrorMessage = FString(SDL_GetError());
		FJoystickLogManager::Get()->LogError(TEXT("Joystick Open Error: %s"), *ErrorMessage);
		return false;
	}

	const SDL_JoystickType Type = SDL_JoystickGetType(Device.Joystick);
	Device.Type = static_cast<EJoystickType>(Type);
	const SDL_JoystickPowerLevel PowerLevel = SDL_JoystickCurrentPowerLevel(Device.Joystick);
	Device.PowerLevel = static_cast<EJoystickPowerLevel>(PowerLevel + 1);

	// DEBUG
	Device.DeviceName = FString(ANSI_TO_TCHAR(SDL_JoystickName(Device.Joystick)));

#if ENGINE_MAJOR_VERSION == 5
	Device.SerialNumber = FString(ANSI_TO_TCHAR(SDL_JoystickGetSerial(Device.Joystick)));
#endif

	Device.SafeDeviceName = Device.DeviceName.Replace(TEXT("."), TEXT("")).Replace(TEXT(","), TEXT(""));
	Device.ProductName = Device.SafeDeviceName.Replace(TEXT(" "), TEXT(""));

#if ENGINE_MAJOR_VERSION == 5
	const bool HasRumble = SDL_JoystickHasRumble(Device.Joystick) == SDL_TRUE;
#else
	const bool HasRumble = false;
#endif
	Device.RumbleSupport = HasRumble;

#if ENGINE_MAJOR_VERSION == 5
	const bool HasLed = SDL_JoystickHasLED(Device.Joystick) == SDL_TRUE;
#else
	const bool HasLed = false;
#endif
	Device.LedSupport = HasLed;

	if (SDL_JoystickIsHaptic(Device.Joystick))
	{
		AddHapticDevice(Device);
	}

	FJoystickLogManager::Get()->LogDebug(TEXT("%s:"), *Device.DeviceName);
	FJoystickLogManager::Get()->LogDebug(TEXT("\tSDL Device Index: %d"), DeviceIndex);
	FJoystickLogManager::Get()->LogDebug(TEXT("\tProduct: %d"), Device.ProductId);
	FJoystickLogManager::Get()->LogDebug(TEXT("\tProduct Id: %s"), *Device.ProductGuid.ToString());
	FJoystickLogManager::Get()->LogDebug(TEXT("\tProduct Version: %d"), Device.ProductVersion);
	FJoystickLogManager::Get()->LogDebug(TEXT("\tVendor Id: %d"), Device.VendorId);
	FJoystickLogManager::Get()->LogDebug(TEXT("\tIs Gamepad: %s"), Device.IsGamepad ? TEXT("true") : TEXT("false"));
	FJoystickLogManager::Get()->LogDebug(TEXT("\tRumble Support: %s"), HasRumble ? TEXT("true") : TEXT("false"));
	FJoystickLogManager::Get()->LogDebug(TEXT("\tHaptic Support: %s"), Device.HapticSupport ? TEXT("true") : TEXT("false"));
	FJoystickLogManager::Get()->LogDebug(TEXT("\tType: %d"), Device.Type);
	FJoystickLogManager::Get()->LogDebug(TEXT("\tPower Level: %d"), Device.PowerLevel);
	FJoystickLogManager::Get()->LogDebug(TEXT("\tNumber of Axis %i"), SDL_JoystickNumAxes(Device.Joystick));
	FJoystickLogManager::Get()->LogDebug(TEXT("\tNumber of Balls %i"), SDL_JoystickNumBalls(Device.Joystick));
	FJoystickLogManager::Get()->LogDebug(TEXT("\tNumber of Buttons %i"), SDL_JoystickNumButtons(Device.Joystick));
	FJoystickLogManager::Get()->LogDebug(TEXT("\tNumber of Hats %i"), SDL_JoystickNumHats(Device.Joystick));

	int ExistingDeviceIndex;
	if (FindExistingDeviceIndex(Device, ExistingDeviceIndex))
	{
		Device.InternalDeviceIndex = ExistingDeviceIndex;
		FJoystickLogManager::Get()->LogDebug(TEXT("Previously disconnected device has reconnected: %s (%i)"), *Device.DeviceName, Device.InternalDeviceIndex);
	}
	else
	{
		Device.InternalDeviceIndex = PersistentDeviceCount;
		PersistentDeviceCount++;
		FJoystickLogManager::Get()->LogDebug(TEXT("New device connected: %s (%i)"), *Device.DeviceName, Device.InternalDeviceIndex);
	}

	Devices.Emplace(Device.InstanceId, Device);
	JoystickPluggedIn(Device);

	return true;
}

bool UJoystickSubsystem::FindExistingDeviceIndex(const FDeviceInfoSDL& Device, int& ExistingDeviceIndex)
{
	for (const TPair<FJoystickInstanceId, FDeviceInfoSDL>& DeviceInfo : Devices)
	{
		if (DeviceInfo.Value.Connected)
		{
			continue;
		}

		if (Device.DeviceName == DeviceInfo.Value.DeviceName
			&& Device.Type == DeviceInfo.Value.Type
			&& Device.ProductId == DeviceInfo.Value.ProductId
			&& Device.ProductGuid == DeviceInfo.Value.ProductGuid
			&& Device.ProductVersion == DeviceInfo.Value.ProductVersion
			&& Device.VendorId == DeviceInfo.Value.VendorId)
		{
			ExistingDeviceIndex = DeviceInfo.Value.InternalDeviceIndex;
			return true;
		}
	}

	return false;
}

bool UJoystickSubsystem::RemoveDeviceByIndex(const int DeviceIndex)
{
	const FJoystickInstanceId& InstanceId = SDL_JoystickGetDeviceInstanceID(DeviceIndex);
	if (InstanceId == -1)
	{
		return false;
	}

	return RemoveDevice(InstanceId);
}

bool UJoystickSubsystem::RemoveDevice(const FJoystickInstanceId& InstanceId)
{
	JoystickUnplugged(InstanceId);

	FDeviceInfoSDL* DeviceInfo = GetDeviceInfo(InstanceId);
	if (DeviceInfo == nullptr)
	{
		return false;
	}

	if (DeviceInfo->Haptic != nullptr)
	{
		FJoystickLogManager::Get()->LogDebug(TEXT("Closing Haptic Device for %d"), InstanceId);
		SDL_HapticClose(DeviceInfo->Haptic);
		DeviceInfo->Haptic = nullptr;
	}
	if (DeviceInfo->Joystick != nullptr)
	{
		FJoystickLogManager::Get()->LogDebug(TEXT("Closing Joystick Device for %d"), InstanceId);
		SDL_JoystickClose(DeviceInfo->Joystick);
		DeviceInfo->Joystick = nullptr;
	}

	DeviceInfo->Connected = false;
	FJoystickLogManager::Get()->LogInformation(TEXT("Device Removed %d"), InstanceId);

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
			{
				JoystickSubsystem.AddDevice(Event->jdevice.which);
				break;
			}
		case SDL_JOYDEVICEREMOVED:
			{
				JoystickSubsystem.RemoveDevice(Event->jdevice.which);
				break;
			}
		case SDL_JOYBUTTONDOWN:
		case SDL_JOYBUTTONUP:
			{
				InputDevice->JoystickButton(Event->jbutton.which, Event->jbutton.button, Event->jbutton.state == SDL_PRESSED);
				break;
			}
		case SDL_JOYAXISMOTION:
			{
				InputDevice->JoystickAxis(Event->jaxis.which, Event->jaxis.axis, Event->jaxis.value / (Event->jaxis.value < 0 ? 32768.0f : 32767.0f));
				break;
			}
		case SDL_JOYHATMOTION:
			{
				InputDevice->JoystickHat(Event->jhat.which, Event->jhat.hat, UJoystickFunctionLibrary::HatValueToDirection(Event->jhat.value));
				break;
			}
		case SDL_JOYBALLMOTION:
			{
				InputDevice->JoystickBall(Event->jball.which, Event->jball.ball, FVector2D(Event->jball.xrel, Event->jball.yrel));
				break;
			}
		default:
			break;
	}

	return 0;
}

FJoystickDeviceState UJoystickSubsystem::CreateInitialDeviceState(const FJoystickInstanceId& InstanceId)
{
	const FDeviceInfoSDL* DeviceInfo = GetDeviceInfo(InstanceId);
	if (DeviceInfo == nullptr || DeviceInfo->Joystick == nullptr)
	{
		return FJoystickDeviceState();
	}

	FJoystickDeviceState DeviceState = FJoystickDeviceState();
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

FDeviceInfoSDL* UJoystickSubsystem::GetDeviceInfo(const FJoystickInstanceId& InstanceId)
{
	if (Devices.Num() == 0)
	{
		return nullptr;
	}

	return Devices.Find(InstanceId);
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
		JoystickPluggedInDelegate.Broadcast(Device.InstanceId);
	}
}

void UJoystickSubsystem::JoystickUnplugged(const FJoystickInstanceId& InstanceId) const
{
	const FJoystickInputDevice* InputDevice = GetInputDevice();
	if (InputDevice == nullptr)
	{
		return;
	}

	InputDevice->JoystickUnplugged(InstanceId);
	if (JoystickUnpluggedDelegate.IsBound())
	{
		JoystickUnpluggedDelegate.Broadcast(InstanceId);
	}
}

const TMap<FJoystickInstanceId, FDeviceInfoSDL>& UJoystickSubsystem::GetDevices()
{
	return Devices;
}
