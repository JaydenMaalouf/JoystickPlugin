// JoystickPlugin is licensed under the MIT License.
// Copyright Jayden Maalouf 2026. All Rights Reserved.

#pragma once

THIRD_PARTY_INCLUDES_START

#include "SDL_hidapi.h"

THIRD_PARTY_INCLUDES_END

#include "DeviceHID.generated.h"

USTRUCT(BlueprintType)
struct FDeviceHID
{
	GENERATED_BODY()

	FDeviceHID() : Device(nullptr)
	{
	}

	SDL_hid_device* Device;
};
