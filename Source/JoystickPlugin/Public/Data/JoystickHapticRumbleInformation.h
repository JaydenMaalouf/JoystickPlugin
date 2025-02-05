// JoystickPlugin is licensed under the MIT License.
// Copyright Jayden Maalouf. All Rights Reserved.

#pragma once

#include "JoystickHapticRumbleInformation.generated.h"

USTRUCT(BlueprintType)
struct JOYSTICKPLUGIN_API FJoystickHapticRumbleInformation
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
