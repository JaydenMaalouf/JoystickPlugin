// JoystickPlugin is licensed under the MIT License.
// Copyright Jayden Maalouf 2026. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Runtime/Launch/Resources/Version.h"

// UInputPlatformSettings arrived in 5.1
#if ENGINE_MAJOR_VERSION == 5 && ENGINE_MINOR_VERSION >= 1

struct FJoystickInstanceId;

// Warns once per editor session when the device mapping policy will stop pairing newly connected
// devices to the primary user, after which a connected device reports nothing and looks broken.
class FJoystickDevicePairingNotifier
{
public:
	void Register();
	void Unregister();

private:
	void OnDeviceConnected(const FJoystickInstanceId& InstanceId);

	FDelegateHandle DeviceConnectedHandle;
	bool bNotified = false;
};

#endif
