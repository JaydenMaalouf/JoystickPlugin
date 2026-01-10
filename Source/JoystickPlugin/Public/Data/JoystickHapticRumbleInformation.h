// JoystickPlugin is licensed under the MIT License.
// Copyright Jayden Maalouf 2026. All Rights Reserved.

#pragma once

#include "JoystickHapticRumbleInformation.generated.h"

USTRUCT(BlueprintType)
struct FJoystickHapticRumbleInformation
{
	GENERATED_BODY()

	FJoystickHapticRumbleInformation()
		: Enabled(false)
		  , Supported(false)
	{
	}

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Rumble Config")
	bool Enabled;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Rumble Config")
	bool Supported;
};
