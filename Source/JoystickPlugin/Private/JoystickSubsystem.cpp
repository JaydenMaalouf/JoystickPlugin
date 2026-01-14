// JoystickPlugin is licensed under the MIT License.
// Copyright Jayden Maalouf 2026. All Rights Reserved.

#include "JoystickSubsystem.h"

#include "Async/Async.h"
#include "Data/DeviceInfoSDL.h"
#include "Data/JoystickSensorType.h"
#include "Data/PlatformTypes.h"
#include "HAL/FileManager.h"
#include "JoystickFunctionLibrary.h"
#include "JoystickInputDevice.h"
#include "JoystickInputSettings.h"
#include "Managers/JoystickLogManager.h"
#include "JoystickPluginModule.h"
#include "Managers/JoystickProfileManager.h"
#include "Misc/ConfigCacheIni.h"
#include "Misc/CoreMiscDefines.h"
#include "Misc/Paths.h"
#include "Runtime/Launch/Resources/Version.h"

THIRD_PARTY_INCLUDES_START

#include "SDL_init.h"
#include "SDL_guid.h"
#include "SDL_sensor.h"
#include "SDL_haptic.h"
#include "SDL_joystick.h"
#include "SDL_gamepad.h"
#include "SDL_power.h"
#include "SDL_properties.h"

THIRD_PARTY_INCLUDES_END

UJoystickSubsystem::UJoystickSubsystem()
	: OwnsSDL(false)
	  , bIsInitialised(false)
{
}

constexpr unsigned SdlRequiredFlags = SDL_INIT_EVENTS | SDL_INIT_JOYSTICK | SDL_INIT_GAMEPAD | SDL_INIT_HAPTIC;

void UJoystickSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	UJoystickProfileManager* JoystickProfileManager = GetMutableDefault<UJoystickProfileManager>();
	if (IsValid(JoystickProfileManager))
	{
		JoystickProfileManager->LoadJoystickProfiles();
	}

	UJoystickInputSettings* JoystickInputSettings = GetMutableDefault<UJoystickInputSettings>();
	if (IsValid(JoystickInputSettings))
	{
		JoystickInputSettings->ResetDevices();
	}

	FJoystickLogManager::Get()->LogDebug(TEXT("UJoystickSubsystem initialising"));

	if (SDL_WasInit(SdlRequiredFlags))
	{
		FJoystickLogManager::Get()->LogDebug(TEXT("SDL already loaded"));
		OwnsSDL = false;
	}
	else
	{
		FJoystickLogManager::Get()->LogDebug(TEXT("SDL_WasInit returned 0. Initialising SDL"));
		if (SDL_Init(SdlRequiredFlags))
		{
			OwnsSDL = true;
		}
		else
		{
			FJoystickLogManager::Get()->LogSDLError(TEXT("SDL_Init failed to initialise SDL"));
			return;
		}
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

	FJoystickLogManager::Get()->LogDebug(TEXT("UJoystickSubsystem deinitialising"));

	for (TPair<FJoystickInstanceId, FDeviceInfoSDL>& Device : Devices)
	{
		RemoveDevice(Device.Key);
	}

	SDL_RemoveEventWatch(HandleSDLEvent, this);

	if (OwnsSDL)
	{
		SDL_Quit();
	}

	bIsInitialised = false;
}

bool UJoystickSubsystem::IsReady() const
{
	return bIsInitialised && InputDevicePtr.IsValid();
}

bool UJoystickSubsystem::IsInitialised() const
{
	return bIsInitialised;
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

bool UJoystickSubsystem::GetJoystickState(const FJoystickInstanceId& InstanceId, FJoystickDeviceState& JoystickDeviceState) const
{
	FJoystickInputDevice* InputDevice = GetInputDevice();
	if (InputDevice == nullptr)
	{
		return false;
	}

	auto [DeviceState, Result] = InputDevice->GetDeviceState(InstanceId);
	if (DeviceState)
	{
		JoystickDeviceState = *DeviceState;
		return true;
	}

	return false;
}

bool UJoystickSubsystem::GetJoystickInfo(const FJoystickInstanceId& InstanceId, FJoystickInformation& JoystickInfo)
{
	auto [DeviceInfo, Result] = GetDeviceInfo(InstanceId);
	if (Result.bSuccess == false || DeviceInfo == nullptr)
	{
		FJoystickLogManager::Get()->LogError(Result);
		return false;
	}

	JoystickInfo = static_cast<FJoystickInformation>(*DeviceInfo);

	return true;
}

EJoystickType UJoystickSubsystem::GetJoystickType(const FJoystickInstanceId& InstanceId)
{
	auto [DeviceInfo, Result] = GetDeviceInfo(InstanceId);
	if (Result.bSuccess == false || DeviceInfo == nullptr)
	{
		FJoystickLogManager::Get()->LogError(Result);
		return EJoystickType::Unknown;
	}

	return DeviceInfo->Type;
}

bool UJoystickSubsystem::GetJoystickPowerInformation(const FJoystickInstanceId& InstanceId, FJoystickPowerInformation& PowerInformation)
{
	auto [DeviceInfo, Result] = GetDeviceInfo(InstanceId);
	if (Result.bSuccess == false || DeviceInfo == nullptr || DeviceInfo->SDLJoystick == nullptr)
	{
		FJoystickLogManager::Get()->LogError(Result);
		return false;
	}

	PowerInformation = DeviceInfo->Power;
	return true;
}

void UJoystickSubsystem::MapJoystickDeviceToPlayer(const FJoystickInstanceId& InstanceId, const int PlayerId)
{
	FJoystickLogManager::Get()->LogDebug(TEXT("Remapping Joystick %d to Player Id %d"), InstanceId.Value, PlayerId);
	auto [DeviceInfo, Result] = GetDeviceInfo(InstanceId);
	if (Result.bSuccess == false || DeviceInfo == nullptr)
	{
		FJoystickLogManager::Get()->LogError(Result);
		return;
	}

	DeviceInfo->SetPlatformUserId(FPlatformUserId::CreateFromInternalId(PlayerId));

#if ENGINE_MAJOR_VERSION == 5 && ENGINE_MINOR_VERSION >= 1
	IPlatformInputDeviceMapper& DeviceMapper = IPlatformInputDeviceMapper::Get();
	DeviceMapper.Internal_MapInputDeviceToUser(DeviceInfo->GetInputDeviceId(), DeviceInfo->GetPlatformUserId(), EInputDeviceConnectionState::Unknown);
#endif
}

void UJoystickSubsystem::SetIgnoreGamepads(const bool IgnoreControllers)
{
	UJoystickInputSettings* JoystickInputSettings = GetMutableDefault<UJoystickInputSettings>();
	if (!IsValid(JoystickInputSettings))
	{
		return;
	}

	const int JoystickCount = Devices.Num();
	const bool ChangedValue = JoystickInputSettings->SetIgnoreGamepads(IgnoreControllers);
	if (ChangedValue && IgnoreControllers)
	{
		for (TPair<FJoystickInstanceId, FDeviceInfoSDL>& Device : Devices)
		{
			if (SDL_IsGamepad(Device.Key) == false)
			{
				continue;
			}

			RemoveDevice(Device.Key);
		}
	}
	else if (ChangedValue && !IgnoreControllers)
	{
		for (TPair<FJoystickInstanceId, FDeviceInfoSDL>& Device : Devices)
		{
			if (SDL_IsGamepad(Device.Key) == false)
			{
				continue;
			}

			AddDevice(Device.Key);
		}
	}
}

bool UJoystickSubsystem::SetJoystickSensorEnabled(const FJoystickInstanceId& InstanceId, const EJoystickSensorType SensorType, const bool Enabled)
{
	auto [DeviceInfo, Result] = GetDeviceInfo(InstanceId);
	if (Result.bSuccess == false || DeviceInfo == nullptr)
	{
		FJoystickLogManager::Get()->LogError(Result);
		return false;
	}

	switch (SensorType)
	{
	case EJoystickSensorType::Gyro:
		{
			if (SDL_SetGamepadSensorEnabled(DeviceInfo->SDLGamepad, SDL_SENSOR_GYRO, Enabled))
			{
				DeviceInfo->Gyro.Enabled = Enabled;
				return true;
			}
			break;
		}
	case EJoystickSensorType::Accelerometer:
		{
			if (SDL_SetGamepadSensorEnabled(DeviceInfo->SDLGamepad, SDL_SENSOR_ACCEL, Enabled))
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
	auto [DeviceInfo, Result] = GetDeviceInfo(InstanceId);
	if (Result.bSuccess == false || DeviceInfo == nullptr || DeviceInfo->SDLJoystick == nullptr)
	{
		FJoystickLogManager::Get()->LogError(Result);
		return false;
	}

	return SDL_SetJoystickLED(DeviceInfo->SDLJoystick, Color.R, Color.G, Color.B) == 0;
}

void UJoystickSubsystem::GetInstanceIds(TArray<FJoystickInstanceId>& InstanceIds, const bool IncludeDisconnected) const
{
	if (IncludeDisconnected)
	{
		Devices.GenerateKeyArray(InstanceIds);
		return;
	}

	const TMap<FJoystickInstanceId, FDeviceInfoSDL> FilteredDevices = Devices.FilterByPredicate([](const TPair<FJoystickInstanceId, FDeviceInfoSDL>& Item)
	{
		return Item.Value.Connected == true;
	});

	FilteredDevices.GenerateKeyArray(InstanceIds);
}

bool UJoystickSubsystem::HasRumbleDevice() const
{
	for (const TTuple<FJoystickInstanceId, FDeviceInfoSDL>& Device : Devices)
	{
		if (Device.Value.Rumble.StandardRumble || Device.Value.Rumble.TriggerRumble)
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

	LoadGamepadMappings();

	InitialiseExistingJoysticks();

	SDL_AddEventWatch(HandleSDLEvent, this);

	if (JoystickSubsystemReady.IsBound())
	{
		JoystickSubsystemReady.Broadcast();
	}
}

void UJoystickSubsystem::Update() const
{
	if (OwnsSDL)
	{
		SDL_Event Event = {};
		while (SDL_PollEvent(&Event))
		{
			// The event watcher handles it
		}
	}
}

TTuple<FDeviceInfoSDL*, FInternalResultMessage> UJoystickSubsystem::GetDeviceInfo(const FJoystickInstanceId& InstanceId)
{
	if (GetJoystickCount() == 0)
	{
		return {nullptr, FInternalResultMessage(false, TEXT("Device list is empty."))};
	}

	FDeviceInfoSDL* Device = Devices.Find(InstanceId);
	if (Device == nullptr)
	{
		return {nullptr, FInternalResultMessage(false, TEXT("Device not found."))};
	}

	return {Device, FInternalResultMessage(true)};
}

FJoystickDeviceState UJoystickSubsystem::CreateInitialDeviceState(const FJoystickInstanceId& InstanceId)
{
	auto [DeviceInfo, Result] = GetDeviceInfo(InstanceId);
	if (Result.bSuccess == false || DeviceInfo == nullptr || DeviceInfo->SDLJoystick == nullptr)
	{
		FJoystickLogManager::Get()->LogError(Result);
		return FJoystickDeviceState();
	}

	FJoystickDeviceState DeviceState = FJoystickDeviceState();
	const int AxesCount = SDL_GetNumJoystickAxes(DeviceInfo->SDLJoystick);
	const int ButtonCount = SDL_GetNumJoystickButtons(DeviceInfo->SDLJoystick);
	const int HatsCount = SDL_GetNumJoystickHats(DeviceInfo->SDLJoystick);
	const int BallsCount = SDL_GetNumJoystickBalls(DeviceInfo->SDLJoystick);

	DeviceState.Axes.SetNumZeroed(AxesCount);
	for (int AxisIndex = 0; AxisIndex < AxesCount; AxisIndex++)
	{
		int16 CurrentValue = 0;
		if (SDL_GetJoystickAxisInitialState(DeviceInfo->SDLJoystick, AxisIndex, &CurrentValue) == false)
		{
			CurrentValue = SDL_GetJoystickAxis(DeviceInfo->SDLJoystick, AxisIndex);
		}

		DeviceState.Axes[AxisIndex].Update(UJoystickFunctionLibrary::NormalizeAxisRaw(CurrentValue));
	}

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

const TMap<FJoystickInstanceId, FDeviceInfoSDL>& UJoystickSubsystem::GetDevices()
{
	return Devices;
}

bool UJoystickSubsystem::HandleSDLEvent(void* UserData, SDL_Event* Event)
{
	UJoystickSubsystem& JoystickSubsystem = *static_cast<UJoystickSubsystem*>(UserData);
	FJoystickInputDevice* InputDevice = JoystickSubsystem.GetInputDevice();
	if (InputDevice == nullptr)
	{
		return false;
	}

	switch (Event->type)
	{
	case SDL_EVENT_JOYSTICK_ADDED:
		{
			JoystickSubsystem.AddDevice(Event->jdevice.which);
			break;
		}
	case SDL_EVENT_JOYSTICK_REMOVED:
		{
			JoystickSubsystem.RemoveDevice(Event->jdevice.which);
			break;
		}
	case SDL_EVENT_JOYSTICK_BUTTON_DOWN:
	case SDL_EVENT_JOYSTICK_BUTTON_UP:
		{
			InputDevice->JoystickButton(Event->jbutton.which, Event->jbutton.button, Event->jbutton.type == SDL_EVENT_JOYSTICK_BUTTON_DOWN);
			break;
		}
	case SDL_EVENT_JOYSTICK_AXIS_MOTION:
		{
			InputDevice->JoystickAxis(Event->jaxis.which, Event->jaxis.axis, UJoystickFunctionLibrary::NormalizeAxisRaw(Event->jaxis.value));
			break;
		}
	case SDL_EVENT_JOYSTICK_HAT_MOTION:
		{
			InputDevice->JoystickHat(Event->jhat.which, Event->jhat.hat, UJoystickFunctionLibrary::HatValueToDirection(Event->jhat.value));
			break;
		}
	case SDL_EVENT_JOYSTICK_BALL_MOTION:
		{
			InputDevice->JoystickBall(Event->jball.which, Event->jball.ball, FVector2D(Event->jball.xrel, Event->jball.yrel));
			break;
		}
	case SDL_EVENT_JOYSTICK_BATTERY_UPDATED:
		{
			JoystickSubsystem.JoystickPower(Event->jdevice.which);
			break;
		}
	case SDL_EVENT_GAMEPAD_SENSOR_UPDATE:
		{
			switch (static_cast<SDL_SensorType>(Event->gsensor.type))
			{
			case SDL_SENSOR_GYRO:
				{
					InputDevice->JoystickGyro(Event->gsensor.which, FVector(Event->gsensor.data[0], Event->gsensor.data[1], Event->gsensor.data[2]));
					break;
				}
			case SDL_SENSOR_ACCEL:
				{
					InputDevice->JoystickAccelerometer(Event->gsensor.which, FVector(Event->gsensor.data[0], Event->gsensor.data[1], Event->gsensor.data[2]));
					break;
				}
			default:
				break;
			}
			break;
		}
	default:
		break;
	}

	return true;
}

bool UJoystickSubsystem::AddDevice(const SDL_JoystickID JoystickId)
{
	const UJoystickInputSettings* JoystickInputSettings = GetDefault<UJoystickInputSettings>();
	if (!IsValid(JoystickInputSettings))
	{
		return false;
	}

	const bool IsGamepad = SDL_IsGamepad(JoystickId);
	if (IsGamepad && JoystickInputSettings->GetIgnoreGamepads())
	{
		// Let UE handle it
		return false;
	}

	FDeviceInfoSDL Device;
	Device.IsGamepad = IsGamepad;
	Device.InstanceId = JoystickId;

	Device.SDLJoystick = SDL_OpenJoystick(JoystickId);
	if (Device.SDLJoystick == nullptr)
	{
		FJoystickLogManager::Get()->LogSDLError(FString::Printf(TEXT("SDL_JoystickOpen failed for %d"), Device.InstanceId.Value));
		return false;
	}

	Device.Connected = true;
	Device.VendorId = SDL_GetJoystickVendor(Device.SDLJoystick);
	Device.ProductId = SDL_GetJoystickProduct(Device.SDLJoystick);
	Device.ProductVersion = SDL_GetJoystickProductVersion(Device.SDLJoystick);

	const SDL_GUID SDLGuid = SDL_GetJoystickGUID(Device.SDLJoystick);
	ConvertSDLGuid(SDLGuid, Device.ProductGuid);

	if (IsGamepad)
	{
		Device.SDLGamepad = SDL_OpenGamepad(JoystickId);
		if (Device.SDLGamepad)
		{
			AddSensorDevice(Device);
		}
		else
		{
			FJoystickLogManager::Get()->LogSDLError(FString::Printf(TEXT("SDL_GamepadOpen failed for %d"), Device.InstanceId.Value));
		}
	}

	const SDL_JoystickType Type = SDL_GetJoystickType(Device.SDLJoystick);
	Device.Type = static_cast<EJoystickType>(Type);

	int PowerLevel = -1;
	const SDL_PowerState PowerState = SDL_GetJoystickPowerInfo(Device.SDLJoystick, &PowerLevel);
	Device.Power.State = static_cast<EJoystickPowerState>(PowerState + 1);
	Device.Power.Level = PowerLevel;

	Device.DeviceName = UJoystickFunctionLibrary::SafelyStringify(SDL_GetJoystickName(Device.SDLJoystick));
	Device.SafeDeviceName = UJoystickFunctionLibrary::SanitiseDeviceName(Device.DeviceName);

	Device.SerialNumber = UJoystickFunctionLibrary::SafelyStringify(SDL_GetJoystickSerial(Device.SDLJoystick));

	const unsigned int QueryResult = SDL_GetJoystickProperties(Device.SDLJoystick);
	Device.Led.Mono = SDL_GetBooleanProperty(QueryResult,SDL_PROP_JOYSTICK_CAP_MONO_LED_BOOLEAN, false);
	Device.Led.RGB = SDL_GetBooleanProperty(QueryResult,SDL_PROP_JOYSTICK_CAP_RGB_LED_BOOLEAN, false);
	Device.Led.Player = SDL_GetBooleanProperty(QueryResult,SDL_PROP_JOYSTICK_CAP_PLAYER_LED_BOOLEAN, false);
	Device.Rumble.StandardRumble = SDL_GetBooleanProperty(QueryResult,SDL_PROP_JOYSTICK_CAP_RUMBLE_BOOLEAN, false);
	Device.Rumble.TriggerRumble = SDL_GetBooleanProperty(QueryResult,SDL_PROP_JOYSTICK_CAP_TRIGGER_RUMBLE_BOOLEAN, false);

	Device.FirmwareVersion = SDL_GetJoystickFirmwareVersion(Device.SDLJoystick);
	Device.Path = UJoystickFunctionLibrary::SafelyStringify(SDL_GetJoystickPath(Device.SDLJoystick));

	Device.DeviceHash = GenerateDeviceHash(Device);

	if (SDL_IsJoystickHaptic(Device.SDLJoystick))
	{
		AddHapticDevice(Device);
	}

	FJoystickLogManager::Get()->LogDebug(TEXT("%s:"), *Device.DeviceName);
	FJoystickLogManager::Get()->LogDebug(TEXT("\tInstance Id: %d"), Device.InstanceId.Value);
	FJoystickLogManager::Get()->LogDebug(TEXT("\tProduct Id: %d"), Device.ProductId);
	FJoystickLogManager::Get()->LogDebug(TEXT("\tProduct Guid: %s"), *Device.ProductGuid.ToString());
	FJoystickLogManager::Get()->LogDebug(TEXT("\tProduct Version: %d"), Device.ProductVersion);
	FJoystickLogManager::Get()->LogDebug(TEXT("\tVendor Id: %d"), Device.VendorId);
	FJoystickLogManager::Get()->LogDebug(TEXT("\tSerial Number: %s"), *Device.SerialNumber);
	FJoystickLogManager::Get()->LogDebug(TEXT("\tIs Game Controller: %s"), Device.IsGamepad ? TEXT("true") : TEXT("false"));
	FJoystickLogManager::Get()->LogDebug(TEXT("\tHaptic Support: %s"), Device.Haptic.Supported ? TEXT("true") : TEXT("false"));
	FJoystickLogManager::Get()->LogDebug(TEXT("\tHaptic Rumble Support: %s"), Device.HapticRumble.Supported ? TEXT("true") : TEXT("false"));
	FJoystickLogManager::Get()->LogDebug(TEXT("\tLED Support:"));
	FJoystickLogManager::Get()->LogDebug(TEXT("\t\tMono: %s"), Device.Led.Mono ? TEXT("true") : TEXT("false"));
	FJoystickLogManager::Get()->LogDebug(TEXT("\t\tRGB: %s"), Device.Led.RGB ? TEXT("true") : TEXT("false"));
	FJoystickLogManager::Get()->LogDebug(TEXT("\t\tPlayer: %s"), Device.Led.Player ? TEXT("true") : TEXT("false"));
	FJoystickLogManager::Get()->LogDebug(TEXT("\tRumble Support:"));
	FJoystickLogManager::Get()->LogDebug(TEXT("\t\tStandard Rumble: %s"), Device.Rumble.StandardRumble ? TEXT("true") : TEXT("false"));
	FJoystickLogManager::Get()->LogDebug(TEXT("\t\tTrigger Rumble: %s"), Device.Rumble.TriggerRumble ? TEXT("true") : TEXT("false"));
	FJoystickLogManager::Get()->LogDebug(TEXT("\tType: %d"), Device.Type);
	FJoystickLogManager::Get()->LogDebug(TEXT("\tPower Level: %d"), Device.Power.Level);
	FJoystickLogManager::Get()->LogDebug(TEXT("\tNumber of Axes: %d"), SDL_GetNumJoystickAxes(Device.SDLJoystick));
	FJoystickLogManager::Get()->LogDebug(TEXT("\tNumber of Balls: %d"), SDL_GetNumJoystickBalls(Device.SDLJoystick));
	FJoystickLogManager::Get()->LogDebug(TEXT("\tNumber of Buttons: %d"), SDL_GetNumJoystickButtons(Device.SDLJoystick));
	FJoystickLogManager::Get()->LogDebug(TEXT("\tNumber of Hats: %d"), SDL_GetNumJoystickHats(Device.SDLJoystick));
	FJoystickLogManager::Get()->LogDebug(TEXT("\tGyro Support: %s"), Device.Gyro.Supported ? TEXT("true") : TEXT("false"));
	FJoystickLogManager::Get()->LogDebug(TEXT("\tAccelerometer Support: %s"), Device.Accelerometer.Supported ? TEXT("true") : TEXT("false"));

	FJoystickInstanceId PreviousJoystickInstanceId;
	FInputDeviceId ExistingInputDeviceId;
	FPlatformUserId ExistingPlatformUserId;
	if (FindExistingDevice(Device, PreviousJoystickInstanceId, ExistingInputDeviceId, ExistingPlatformUserId))
	{
		Device.SetInputDeviceId(ExistingInputDeviceId);
		Device.SetPlatformUserId(ExistingPlatformUserId);

		// Remove existing devices from devices list so we don't collect old data
		Devices.Remove(PreviousJoystickInstanceId);

		FJoystickLogManager::Get()->LogDebug(TEXT("Previously disconnected device has reconnected: %s (Device Id: %d)"), *Device.DeviceName, Device.GetInputDeviceId().GetId());
	}
	else
	{
#if ENGINE_MAJOR_VERSION == 5 && ENGINE_MINOR_VERSION >= 1
		IPlatformInputDeviceMapper& DeviceMapper = IPlatformInputDeviceMapper::Get();
		Device.SetInputDeviceId(DeviceMapper.AllocateNewInputDeviceId());
		Device.SetPlatformUserId(DeviceMapper.GetPrimaryPlatformUser());
#endif
		FJoystickLogManager::Get()->LogDebug(TEXT("New device connected: %s (Device Id: %d)"), *Device.DeviceName, Device.GetInputDeviceId().GetId());
	}

	Devices.Emplace(Device.InstanceId, Device);
	JoystickPluggedIn(Device);

	return true;
}

void UJoystickSubsystem::AddHapticDevice(FDeviceInfoSDL& Device) const
{
	Device.SDLHaptic = SDL_OpenHapticFromJoystick(Device.SDLJoystick);
	if (Device.SDLHaptic == nullptr)
	{
		return;
	}

	Device.Haptic.Supported = true;
	FJoystickLogManager::Get()->LogDebug(TEXT("Haptic device detected"));
	Device.Haptic.AxesCount = SDL_GetNumHapticAxes(Device.SDLHaptic);
	FJoystickLogManager::Get()->LogDebug(TEXT("\tNumber of haptic axes: %d"), Device.Haptic.AxesCount);

	Device.HapticRumble.Supported = SDL_HapticRumbleSupported(Device.SDLHaptic) == true;
	if (Device.HapticRumble.Supported)
	{
		Device.HapticRumble.Enabled = SDL_InitHapticRumble(Device.SDLHaptic) == 0;
	}

	const unsigned int QueryResult = SDL_GetHapticFeatures(Device.SDLHaptic);
	Device.Haptic.Constant = (QueryResult & SDL_HAPTIC_CONSTANT) != 0;
	Device.Haptic.Sine = (QueryResult & SDL_HAPTIC_SINE) != 0;
	Device.Haptic.LeftRight = (QueryResult & SDL_HAPTIC_LEFTRIGHT) != 0;
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
	Device.Haptic.Status = (QueryResult & SDL_HAPTIC_STATUS) != 0;
	Device.Haptic.Pause = (QueryResult & SDL_HAPTIC_PAUSE) != 0;

	FJoystickLogManager::Get()->LogDebug(TEXT("\tSDL_HAPTIC_CONSTANT support: %s"), Device.Haptic.Constant ? TEXT("true") : TEXT("false"));
	FJoystickLogManager::Get()->LogDebug(TEXT("\tSDL_HAPTIC_SINE support: %s"), Device.Haptic.Sine ? TEXT("true") : TEXT("false"));
	FJoystickLogManager::Get()->LogDebug(TEXT("\tSDL_HAPTIC_LEFTRIGHT support: %s"), Device.Haptic.LeftRight ? TEXT("true") : TEXT("false"));
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
	FJoystickLogManager::Get()->LogDebug(TEXT("\tSDL_HAPTIC_STATUS support: %s"), Device.Haptic.Status ? TEXT("true") : TEXT("false"));
	FJoystickLogManager::Get()->LogDebug(TEXT("\tSDL_HAPTIC_PAUSE support: %s"), Device.Haptic.Pause ? TEXT("true") : TEXT("false"));
}

void UJoystickSubsystem::AddSensorDevice(FDeviceInfoSDL& Device) const
{
	Device.Gyro.Supported = SDL_GamepadHasSensor(Device.SDLGamepad, SDL_SENSOR_GYRO) == true;
	Device.Accelerometer.Supported = SDL_GamepadHasSensor(Device.SDLGamepad, SDL_SENSOR_ACCEL) == true;

	if (Device.Gyro.Supported == false && Device.Accelerometer.Supported == false)
	{
		return;
	}

	if (Device.Gyro.Supported)
	{
		Device.Gyro.Enabled = SDL_SetGamepadSensorEnabled(Device.SDLGamepad, SDL_SENSOR_GYRO, true) == 0;
	}
	if (Device.Accelerometer.Supported)
	{
		Device.Accelerometer.Enabled = SDL_SetGamepadSensorEnabled(Device.SDLGamepad, SDL_SENSOR_ACCEL, true) == 0;
	}
}

bool UJoystickSubsystem::RemoveDevice(const FJoystickInstanceId& InstanceId)
{
	auto [DeviceInfo, Result] = GetDeviceInfo(InstanceId);
	if (DeviceInfo == nullptr)
	{
		//Unable to find device information, assuming we don't own this SDL device
		return false;
	}

	JoystickUnplugged(InstanceId, DeviceInfo->GetInputDeviceId());

	if (DeviceInfo->SDLHaptic != nullptr)
	{
		FJoystickLogManager::Get()->LogDebug(TEXT("Closing haptic for device %d"), InstanceId.Value);
		SDL_CloseHaptic(DeviceInfo->SDLHaptic);
		DeviceInfo->SDLHaptic = nullptr;
	}
	if (DeviceInfo->SDLJoystick != nullptr)
	{
		FJoystickLogManager::Get()->LogDebug(TEXT("Closing joystick for device %d"), InstanceId.Value);
		SDL_CloseJoystick(DeviceInfo->SDLJoystick);
		DeviceInfo->SDLJoystick = nullptr;
	}
	if (DeviceInfo->SDLGamepad != nullptr)
	{
		FJoystickLogManager::Get()->LogDebug(TEXT("Closing game controller for device %d"), InstanceId.Value);
		SDL_CloseGamepad(DeviceInfo->SDLGamepad);
		DeviceInfo->SDLGamepad = nullptr;
	}

	DeviceInfo->Connected = false;
	FJoystickLogManager::Get()->LogInformation(TEXT("Device removed: %d"), InstanceId.Value);

	return true;
}

bool UJoystickSubsystem::FindExistingDevice(const FDeviceInfoSDL& Device, FJoystickInstanceId& PreviousJoystickInstanceId, FInputDeviceId& ExistingInputDeviceId, FPlatformUserId& ExistingPlatformUserId)
{
	for (const TPair<FJoystickInstanceId, FDeviceInfoSDL>& DeviceInfo : Devices)
	{
		if (DeviceInfo.Value.Connected)
		{
			continue;
		}

		if ((Device.Path.IsEmpty() && Device.SerialNumber == DeviceInfo.Value.SerialNumber && Device.ProductGuid == DeviceInfo.Value.ProductGuid) ||
			(Device.Path == DeviceInfo.Value.Path && Device.DeviceHash == DeviceInfo.Value.DeviceHash))
		{
			PreviousJoystickInstanceId = DeviceInfo.Value.InstanceId;
			ExistingInputDeviceId = DeviceInfo.Value.GetInputDeviceId();
			ExistingPlatformUserId = DeviceInfo.Value.GetPlatformUserId();
			return true;
		}
	}

	return false;
}

void UJoystickSubsystem::InitialiseExistingJoysticks()
{
	int JoystickCount = 0;
	const SDL_JoystickID* JoystickIds = SDL_GetJoysticks(&JoystickCount);
	for (int i = 0; i < JoystickCount; i++)
	{
		const SDL_JoystickID& JoystickId = JoystickIds[i];
		AddDevice(JoystickId);
	}
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

	const FInternalJoystickEvent DelegateCopy = Internal_JoystickPluggedInDelegate;
	const int32 InstanceId = Device.InstanceId;
	AsyncTask(ENamedThreads::GameThread, [DelegateCopy, InstanceId]
	{
		if (DelegateCopy.IsBound())
		{
			DelegateCopy.Broadcast(InstanceId);
		}
	});
}

void UJoystickSubsystem::JoystickUnplugged(const FJoystickInstanceId& InstanceId, const FInputDeviceId& InputDeviceId) const
{
	const FJoystickInputDevice* InputDevice = GetInputDevice();
	if (InputDevice == nullptr)
	{
		return;
	}

	InputDevice->JoystickUnplugged(InstanceId, InputDeviceId);
	if (JoystickUnpluggedDelegate.IsBound())
	{
		JoystickUnpluggedDelegate.Broadcast(InstanceId);
	}

	const FInternalJoystickEvent DelegateCopy = Internal_JoystickUnpluggedDelegate;
	AsyncTask(ENamedThreads::GameThread, [DelegateCopy, InstanceId]
	{
		if (DelegateCopy.IsBound())
		{
			DelegateCopy.Broadcast(InstanceId);
		}
	});
}

void UJoystickSubsystem::JoystickPower(const FJoystickInstanceId& InstanceId)
{
	auto [DeviceInfo, Result] = GetDeviceInfo(InstanceId);
	if (Result.bSuccess == false || DeviceInfo == nullptr || DeviceInfo->SDLJoystick == nullptr)
	{
		return;
	}

	int PowerLevel = -1;
	const SDL_PowerState PowerState = SDL_GetJoystickPowerInfo(DeviceInfo->SDLJoystick, &PowerLevel);
	DeviceInfo->Power.State = static_cast<EJoystickPowerState>(PowerState + 1);
	DeviceInfo->Power.Level = PowerLevel;
}

void UJoystickSubsystem::LoadGamepadMappings() const
{
	const FString GamepadDbPath = FPaths::Combine(*FJoystickPluginModule::PluginThirdPartyDirectory, GamepadMappingFile);
	if (FPaths::FileExists(GamepadDbPath))
	{
		const int MappingsAdded = SDL_AddGamepadMappingsFromFile(TCHAR_TO_UTF8(*GamepadDbPath));
		FJoystickLogManager::Get()->LogDebug(TEXT("SDL_GamepadAddMappingsFromFile added mappings: %d"), MappingsAdded);
	}
	else
	{
		FJoystickLogManager::Get()->LogDebug(TEXT("File %s was not found, skipping loading external mappings"), *GamepadMappingFile);
	}
}

void UJoystickSubsystem::ConvertSDLGuid(const SDL_GUID& SdlGuid, FGuid& OutGuid) const
{
	FMemory::Memcpy(&OutGuid.A, &SdlGuid.data[0], 4);
	FMemory::Memcpy(&OutGuid.B, &SdlGuid.data[4], 4);
	FMemory::Memcpy(&OutGuid.C, &SdlGuid.data[8], 4);
	FMemory::Memcpy(&OutGuid.D, &SdlGuid.data[12], 4);
}

FString UJoystickSubsystem::GenerateDeviceHash(const FDeviceInfoSDL& Device) const
{
	FString FullSignature = FString::Printf(TEXT("%s::%04X:%04X"),
	                                        *Device.DeviceName.TrimStartAndEnd(),
	                                        Device.VendorId,
	                                        Device.ProductId
	);

	if (!Device.SerialNumber.IsEmpty())
	{
		FullSignature += FString::Printf(TEXT("::%s"), *Device.SerialNumber.TrimStartAndEnd());
	}

	const uint32 HashValue = GetTypeHash(FullSignature);
	return FString::Printf(TEXT("%08X"), HashValue);
}

FString UJoystickSubsystem::GamepadMappingFile("gamecontrollerdb.txt");
