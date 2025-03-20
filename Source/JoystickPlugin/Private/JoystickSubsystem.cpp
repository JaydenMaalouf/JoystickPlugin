// JoystickPlugin is licensed under the MIT License.
// Copyright Jayden Maalouf. All Rights Reserved.

#include "JoystickSubsystem.h"
#include "JoystickFunctionLibrary.h"
#include "JoystickInputDevice.h"
#include "JoystickInputSettings.h"
#include "JoystickLogManager.h"
#include "Data/JoystickInstanceId.h"
#include "Data/JoystickInformation.h"
#include "Data/JoystickSensorType.h"
#include "Runtime/Launch/Resources/Version.h"

THIRD_PARTY_INCLUDES_START

#include "SDL.h"
#include "SDL_sensor.h"
#include "SDL_haptic.h"
#include "SDL_joystick.h"
#include "SDL_gamecontroller.h"

THIRD_PARTY_INCLUDES_END

UJoystickSubsystem::UJoystickSubsystem()
	: OwnsSDL(false)
	  , bIsInitialised(false)
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

	bIsInitialised = true;

	if (JoystickSubsystemInitialised.IsBound())
	{
		JoystickSubsystemInitialised.Broadcast();
	}
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

	bIsInitialised = false;
}

void UJoystickSubsystem::InitialiseExistingJoysticks()
{
	const int JoystickCount = GetRawJoystickCount();
	for (int i = 0; i < JoystickCount; i++)
	{
		AddDevice(i);
	}
}

void UJoystickSubsystem::InitialiseInputDevice(const TSharedPtr<FJoystickInputDevice>& NewInputDevice)
{
	if (NewInputDevice == nullptr || !NewInputDevice.IsValid())
	{
		return;
	}

	InputDevicePtr = NewInputDevice;

	if (SDL_WasInit(SdlRequiredFlags) == 0)
	{
		return;
	}

	InitialiseExistingJoysticks();

	SDL_AddEventWatch(HandleSDLEvent, this);

	if (JoystickSubsystemReady.IsBound())
	{
		JoystickSubsystemReady.Broadcast();
	}
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

bool UJoystickSubsystem::SetJoystickSensorEnabled(const FJoystickInstanceId& InstanceId, const EJoystickSensorType SensorType, const bool Enabled)
{
	FDeviceInfoSDL* DeviceInfo = GetDeviceInfo(InstanceId);
	if (DeviceInfo == nullptr || DeviceInfo->SDLGameController == nullptr)
	{
		return false;
	}

	switch (SensorType)
	{
		case EJoystickSensorType::Gyro:
			{
				if (SDL_GameControllerSetSensorEnabled(DeviceInfo->SDLGameController, SDL_SENSOR_GYRO, Enabled ? SDL_TRUE : SDL_FALSE) == 0)
				{
					DeviceInfo->Gyro.Enabled = Enabled;
					return true;
				}
				break;
			}
		case EJoystickSensorType::Accelerometer:
			{
				if (SDL_GameControllerSetSensorEnabled(DeviceInfo->SDLGameController, SDL_SENSOR_ACCEL, Enabled ? SDL_TRUE : SDL_FALSE) == 0)
				{
					DeviceInfo->Accelerometer.Enabled = Enabled;
					return true;
				}
				break;
			}
	}

	return false;
}

bool UJoystickSubsystem::SetJoystickLedColor(const FJoystickInstanceId& InstanceId, const FColor Color)
{
#if ENGINE_MAJOR_VERSION == 5
	const FDeviceInfoSDL* DeviceInfo = GetDeviceInfo(InstanceId);
	if (DeviceInfo == nullptr || DeviceInfo->SDLJoystick == nullptr)
	{
		return false;
	}

	return SDL_JoystickSetLED(DeviceInfo->SDLJoystick, Color.R, Color.G, Color.B) == 0;
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
		if (Device.Value.IsGamepad || Device.Value.Rumble.Enabled)
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
	Device.SDLHaptic = SDL_HapticOpenFromJoystick(Device.SDLJoystick);
	if (Device.SDLHaptic == nullptr)
	{
		return;
	}

	Device.Haptic.Supported = true;
	FJoystickLogManager::Get()->LogDebug(TEXT("Haptic Device detected"));
	Device.Haptic.AxesCount = SDL_HapticNumAxes(Device.SDLHaptic);
	FJoystickLogManager::Get()->LogDebug(TEXT("\tNumber of Haptic Axis: %d"), Device.Haptic.AxesCount);

#if ENGINE_MAJOR_VERSION == 5
	Device.Rumble.Supported = SDL_HapticRumbleSupported(Device.SDLHaptic) == SDL_TRUE;
	if (Device.Rumble.Supported)
	{
		Device.Rumble.Enabled = SDL_HapticRumbleInit(Device.SDLHaptic) == 0;
	}
#endif

	const unsigned int QueryResult = SDL_HapticQuery(Device.SDLHaptic);
	Device.Haptic.Constant = (QueryResult & SDL_HAPTIC_CONSTANT) != 0;
	Device.Haptic.Sine = (QueryResult & SDL_HAPTIC_SINE) != 0;
	Device.Haptic.Triangle = (QueryResult & SDL_HAPTIC_TRIANGLE) != 0;
	Device.Haptic.SawToothUp = (QueryResult & SDL_HAPTIC_SAWTOOTHUP) != 0;
	Device.Haptic.SawToothDown = (QueryResult & SDL_HAPTIC_SAWTOOTHDOWN) != 0;
	Device.Haptic.Ramp = (QueryResult & SDL_HAPTIC_RAMP) != 0;
	Device.Haptic.Spring = (QueryResult & SDL_HAPTIC_SPRING) != 0;
	Device.Haptic.Damper = (QueryResult & SDL_HAPTIC_DAMPER) != 0;
	Device.Haptic.Inertia = (QueryResult & SDL_HAPTIC_INERTIA) != 0;
	Device.Haptic.Friction = (QueryResult & SDL_HAPTIC_FRICTION) != 0;
	Device.Haptic.Custom = (QueryResult & SDL_HAPTIC_CUSTOM) != 0;
	Device.Haptic.Gain = (QueryResult & SDL_HAPTIC_GAIN) != 0;
	Device.Haptic.AutoCenter = (QueryResult & SDL_HAPTIC_AUTOCENTER) != 0;

	FJoystickLogManager::Get()->LogDebug(TEXT("\tSDL_HAPTIC_CONSTANT support: %s"), Device.Haptic.Constant ? TEXT("true") : TEXT("false"));
	FJoystickLogManager::Get()->LogDebug(TEXT("\tSDL_HAPTIC_SINE support: %s"), Device.Haptic.Sine ? TEXT("true") : TEXT("false"));
	FJoystickLogManager::Get()->LogDebug(TEXT("\tSDL_HAPTIC_TRIANGLE support: %s"), Device.Haptic.Triangle ? TEXT("true") : TEXT("false"));
	FJoystickLogManager::Get()->LogDebug(TEXT("\tSDL_HAPTIC_SAWTOOTHUP support: %s"), Device.Haptic.SawToothUp ? TEXT("true") : TEXT("false"));
	FJoystickLogManager::Get()->LogDebug(TEXT("\tSDL_HAPTIC_SAWTOOTHDOWN support: %s"), Device.Haptic.SawToothDown ? TEXT("true") : TEXT("false"));
	FJoystickLogManager::Get()->LogDebug(TEXT("\tSDL_HAPTIC_RAMP support: %s"), Device.Haptic.Ramp ? TEXT("true") : TEXT("false"));
	FJoystickLogManager::Get()->LogDebug(TEXT("\tSDL_HAPTIC_SPRING support: %s"), Device.Haptic.Spring ? TEXT("true") : TEXT("false"));
	FJoystickLogManager::Get()->LogDebug(TEXT("\tSDL_HAPTIC_DAMPER support: %s"), Device.Haptic.Damper ? TEXT("true") : TEXT("false"));
	FJoystickLogManager::Get()->LogDebug(TEXT("\tSDL_HAPTIC_INERTIA support: %s"), Device.Haptic.Inertia ? TEXT("true") : TEXT("false"));
	FJoystickLogManager::Get()->LogDebug(TEXT("\tSDL_HAPTIC_FRICTION support: %s"), Device.Haptic.Friction ? TEXT("true") : TEXT("false"));
	FJoystickLogManager::Get()->LogDebug(TEXT("\tSDL_HAPTIC_CUSTOM support: %s"), Device.Haptic.Custom ? TEXT("true") : TEXT("false"));
	FJoystickLogManager::Get()->LogDebug(TEXT("\tSDL_HAPTIC_GAIN support: %s"), Device.Haptic.Gain ? TEXT("true") : TEXT("false"));
	FJoystickLogManager::Get()->LogDebug(TEXT("\tSDL_HAPTIC_AUTOCENTER support: %s"), Device.Haptic.AutoCenter ? TEXT("true") : TEXT("false"));
}

void UJoystickSubsystem::AddSensorDevice(FDeviceInfoSDL& Device) const
{
	Device.Gyro.Supported = SDL_GameControllerHasSensor(Device.SDLGameController, SDL_SENSOR_GYRO) == SDL_TRUE;
	Device.Accelerometer.Supported = SDL_GameControllerHasSensor(Device.SDLGameController, SDL_SENSOR_ACCEL) == SDL_TRUE;

	if (Device.Gyro.Supported == false && Device.Accelerometer.Supported == false)
	{
		return;
	}

	if (Device.Gyro.Supported)
	{
		Device.Gyro.Enabled = SDL_GameControllerSetSensorEnabled(Device.SDLGameController, SDL_SENSOR_GYRO, SDL_TRUE) == 0;
	}
	if (Device.Accelerometer.Supported)
	{
		Device.Accelerometer.Enabled = SDL_GameControllerSetSensorEnabled(Device.SDLGameController, SDL_SENSOR_ACCEL, SDL_TRUE) == 0;
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

	Device.SDLJoystick = SDL_JoystickOpen(DeviceIndex);
	if (Device.SDLJoystick == nullptr)
	{
		const FString ErrorMessage = FString(SDL_GetError());
		FJoystickLogManager::Get()->LogError(TEXT("Joystick %d Open Error: %s"), &Device.InstanceId, *ErrorMessage);
		return false;
	}

	if (IsGamepad)
	{
		Device.SDLGameController = SDL_GameControllerOpen(DeviceIndex);
		if (Device.SDLGameController == nullptr)
		{
			const FString ErrorMessage = FString(SDL_GetError());
			FJoystickLogManager::Get()->LogError(TEXT("Game Controller %d Open Error: %s"), &Device.InstanceId, *ErrorMessage);
		}
		else
		{
			AddSensorDevice(Device);
		}
	}

	const SDL_JoystickType Type = SDL_JoystickGetType(Device.SDLJoystick);
	Device.Type = static_cast<EJoystickType>(Type);
	const SDL_JoystickPowerLevel PowerLevel = SDL_JoystickCurrentPowerLevel(Device.SDLJoystick);
	Device.PowerLevel = static_cast<EJoystickPowerLevel>(PowerLevel + 1);

	Device.DeviceName = FString(ANSI_TO_TCHAR(SDL_JoystickName(Device.SDLJoystick)));

	Device.SafeDeviceName = Device.DeviceName.Replace(TEXT("."), TEXT("")).Replace(TEXT(","), TEXT(""));
	Device.ProductName = Device.SafeDeviceName.Replace(TEXT(" "), TEXT(""));

#if ENGINE_MAJOR_VERSION == 5
	Device.SerialNumber = FString(ANSI_TO_TCHAR(SDL_JoystickGetSerial(Device.SDLJoystick)));
	Device.LedSupport = SDL_JoystickHasLED(Device.SDLJoystick) == SDL_TRUE;
#endif

	if (SDL_JoystickIsHaptic(Device.SDLJoystick))
	{
		AddHapticDevice(Device);
	}

	FJoystickLogManager::Get()->LogDebug(TEXT("%s:"), *Device.DeviceName);
	FJoystickLogManager::Get()->LogDebug(TEXT("\tInstance Id: %d"), &Device.InstanceId);
	FJoystickLogManager::Get()->LogDebug(TEXT("\tSDL Device Index: %d"), DeviceIndex);
	FJoystickLogManager::Get()->LogDebug(TEXT("\tProduct: %d"), Device.ProductId);
	FJoystickLogManager::Get()->LogDebug(TEXT("\tProduct Id: %s"), *Device.ProductGuid.ToString());
	FJoystickLogManager::Get()->LogDebug(TEXT("\tProduct Version: %d"), Device.ProductVersion);
	FJoystickLogManager::Get()->LogDebug(TEXT("\tVendor Id: %d"), Device.VendorId);
	FJoystickLogManager::Get()->LogDebug(TEXT("\tSerial Number: %s"), *Device.SerialNumber);
	FJoystickLogManager::Get()->LogDebug(TEXT("\tIs Gamepad: %s"), Device.IsGamepad ? TEXT("true") : TEXT("false"));
	FJoystickLogManager::Get()->LogDebug(TEXT("\tHaptic Support: %s"), Device.Haptic.Supported ? TEXT("true") : TEXT("false"));
	FJoystickLogManager::Get()->LogDebug(TEXT("\tRumble Support: %s"), Device.Rumble.Supported ? TEXT("true") : TEXT("false"));
	FJoystickLogManager::Get()->LogDebug(TEXT("\tLED Support: %s"), Device.LedSupport ? TEXT("true") : TEXT("false"));
	FJoystickLogManager::Get()->LogDebug(TEXT("\tType: %d"), Device.Type);
	FJoystickLogManager::Get()->LogDebug(TEXT("\tPower Level: %d"), static_cast<int>(Device.PowerLevel));
	FJoystickLogManager::Get()->LogDebug(TEXT("\tNumber of Axis %d"), SDL_JoystickNumAxes(Device.SDLJoystick));
	FJoystickLogManager::Get()->LogDebug(TEXT("\tNumber of Balls %d"), SDL_JoystickNumBalls(Device.SDLJoystick));
	FJoystickLogManager::Get()->LogDebug(TEXT("\tNumber of Buttons %d"), SDL_JoystickNumButtons(Device.SDLJoystick));
	FJoystickLogManager::Get()->LogDebug(TEXT("\tNumber of Hats %d"), SDL_JoystickNumHats(Device.SDLJoystick));
	FJoystickLogManager::Get()->LogDebug(TEXT("\tGyro Support: %s"), Device.Gyro.Supported ? TEXT("true") : TEXT("false"));
	FJoystickLogManager::Get()->LogDebug(TEXT("\tAccelerometer Support: %s"), Device.Accelerometer.Supported ? TEXT("true") : TEXT("false"));

	int ExistingDeviceIndex;
	if (FindExistingDeviceIndex(Device, ExistingDeviceIndex))
	{
		Device.InternalDeviceIndex = ExistingDeviceIndex;
		FJoystickLogManager::Get()->LogDebug(TEXT("Previously disconnected device has reconnected: %s (%d)"), *Device.DeviceName, Device.InternalDeviceIndex);
	}
	else
	{
		Device.InternalDeviceIndex = PersistentDeviceCount;
		PersistentDeviceCount++;
		FJoystickLogManager::Get()->LogDebug(TEXT("New device connected: %s (%d)"), *Device.DeviceName, Device.InternalDeviceIndex);
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

	if (DeviceInfo->SDLHaptic != nullptr)
	{
		FJoystickLogManager::Get()->LogDebug(TEXT("Closing Haptic for Device %d"), &InstanceId);
		SDL_HapticClose(DeviceInfo->SDLHaptic);
		DeviceInfo->SDLHaptic = nullptr;
	}
	if (DeviceInfo->SDLJoystick != nullptr)
	{
		FJoystickLogManager::Get()->LogDebug(TEXT("Closing Joystick for Device %d"), &InstanceId);
		SDL_JoystickClose(DeviceInfo->SDLJoystick);
		DeviceInfo->SDLJoystick = nullptr;
	}
	if (DeviceInfo->SDLGameController != nullptr)
	{
		FJoystickLogManager::Get()->LogDebug(TEXT("Closing Game Controller for Device %d"), &InstanceId);
		SDL_GameControllerClose(DeviceInfo->SDLGameController);
		DeviceInfo->SDLGameController = nullptr;
	}

	DeviceInfo->Connected = false;
	FJoystickLogManager::Get()->LogInformation(TEXT("Device Removed %d"), &InstanceId);

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
		case SDL_CONTROLLERSENSORUPDATE:
			{
				switch (Event->csensor.sensor)
				{
					case SDL_SENSOR_GYRO:
						{
							InputDevice->JoystickGyro(Event->csensor.which, Event->csensor.timestamp, FVector(Event->csensor.data[0], Event->csensor.data[1], Event->csensor.data[2]));
							break;
						}
					case SDL_SENSOR_ACCEL:
						{
							InputDevice->JoystickAccelerometer(Event->csensor.which, Event->csensor.timestamp, FVector(Event->csensor.data[0], Event->csensor.data[1], Event->csensor.data[2]));
							break;
						}
					default: break;
				}
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
	if (DeviceInfo == nullptr || DeviceInfo->SDLJoystick == nullptr)
	{
		return FJoystickDeviceState();
	}

	FJoystickDeviceState DeviceState = FJoystickDeviceState();
	const int AxesCount = SDL_JoystickNumAxes(DeviceInfo->SDLJoystick);
	const int ButtonCount = SDL_JoystickNumButtons(DeviceInfo->SDLJoystick);
	const int HatsCount = SDL_JoystickNumHats(DeviceInfo->SDLJoystick);
	const int BallsCount = SDL_JoystickNumBalls(DeviceInfo->SDLJoystick);

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
	return bIsInitialised && InputDevicePtr.IsValid();
}

bool UJoystickSubsystem::IsInitialised() const
{
	return bIsInitialised;
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
