// JoystickPlugin is licensed under the MIT License.
// Copyright Jayden Maalouf 2026. All Rights Reserved.

#pragma once

#include "JoystickConnectionType.h"
#include "JoystickLedInformation.h"
#include "JoystickPowerInformation.h"
#include "JoystickRumbleInformation.h"
#include "Data/JoystickInstanceId.h"
#include "Data/JoystickGamepadType.h"
#include "Data/JoystickType.h"
#include "Data/JoystickHapticInformation.h"
#include "Data/JoystickHapticRumbleInformation.h"
#include "Data/JoystickSensorInformation.h"

#include "JoystickInformation.generated.h"

USTRUCT(BlueprintType)
struct JOYSTICKPLUGIN_API FJoystickInformation
{
	GENERATED_BODY()

	FJoystickInformation();

	bool IsCollectionSiblingOf(const FJoystickInformation& Other) const;

	/**
	 * Whether this device is a gamepad the engine will handle itself, which is what IgnoreGamepads
	 * asks about.
	 *
	 * NOT IsGamepad, which only says SDL has a mapping for the device. SDL ships mappings for a great
	 * deal of hardware that is not a pad, and a mapping with no `type:` hint also makes
	 * SDL_GetJoystickTypeForID answer Gamepad - so both of those fields report a mapped flight stick
	 * or throttle as a pad, and ignoring it hands a device the engine has no joystick support for to
	 * the engine. It then works nowhere.
	 *
	 * Three questions, and a pad answers all three:
	 *  - it has a mapping at all;
	 *  - its joystick type is not something that is explicitly NOT a pad (a flight stick, a throttle,
	 *    a wheel, an arcade stick, a dance pad, a guitar, a drum kit);
	 *  - SDL recognises its vendor and product ids as a console pad family, rather than falling back
	 *    to Standard the way it does for every mapped device it does not know.
	 */
	bool IsEngineHandledGamepad() const;

	void UpdateCollectionFromPath();
	void UpdateDeviceHash();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Device Config")
	FJoystickInstanceId InstanceId;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Device Config")
	FString DeviceName;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Device Config")
	FString SafeDeviceName;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Device Config")
	EJoystickType Type;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Device Config")
	int32 ProductId;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Device Config")
	FString DeviceHash;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Device Config")
	FGuid ProductGuid;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Device Config")
	FString SerialNumber;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Device Config")
	FString Path;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Device Config")
	int32 FirmwareVersion;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Device Config")
	int32 ProductVersion;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Device Config")
	int32 VendorId;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Device Config")
	bool IsGamepad;

	/**
	 * The gamepad family SDL recognises this device as from its vendor and product ids, ignoring any
	 * mapping it carries (SDL_GetRealGamepadTypeForID).
	 *
	 * Distinct from Type and IsGamepad, which both read off the mapping: a device with a mapping and
	 * no type hint in it reports IsGamepad true AND Type Gamepad however little it resembles a pad.
	 * This is the field that still tells them apart - Standard for anything SDL does not recognise as
	 * a console pad.
	 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Device Config")
	EJoystickGamepadType GamepadType;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Device Config")
	EJoystickConnectionType Connection;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Device Config|Power")
	FJoystickPowerInformation Power;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Device Config|Led")
	FJoystickLedInformation Led;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Device Config|Rumble")
	FJoystickRumbleInformation Rumble;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Device Config|Haptic")
	FJoystickHapticInformation Haptic;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Device Config|Haptic Rumble")
	FJoystickHapticRumbleInformation HapticRumble;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Device Config|Sensors")
	FJoystickSensorInformation Gyro;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Device Config|Sensors")
	FJoystickSensorInformation Accelerometer;

	int CollectionIndex;
	FString CollectionGroup;

	FString GetDeviceDisplayName() const;
};
