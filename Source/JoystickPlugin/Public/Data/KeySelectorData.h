// JoystickPlugin is licensed under the MIT License.
// Copyright Jayden Maalouf. All Rights Reserved.

#pragma once

#include "KeySelectorData.generated.h"

USTRUCT()
struct JOYSTICKPLUGIN_API FKeySelectorData
{
	GENERATED_BODY()

	FKeySelectorData()
		: MinStartTime(FTimespan::Zero())
		  , MaxStartTime(FTimespan::Zero())
	{
	}

	FTimespan MinStartTime;
	FTimespan MaxStartTime;
};
