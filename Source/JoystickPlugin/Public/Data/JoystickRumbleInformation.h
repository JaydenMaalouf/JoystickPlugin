// JoystickPlugin is licensed under the MIT License.
// Copyright Jayden Maalouf 2026. All Rights Reserved.

#pragma once

#include "JoystickRumbleInformation.generated.h"

USTRUCT(BlueprintType)
struct FJoystickRumbleInformation
{
	GENERATED_BODY()

	FJoystickRumbleInformation()
		: StandardRumble(false)
		  , TriggerRumble(false)
	{
		Supported = StandardRumble || TriggerRumble;
	}

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Rumble Config")
	bool Supported;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Rumble Config")
	bool StandardRumble;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Rumble Config")
	bool TriggerRumble;
};
