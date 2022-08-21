#pragma once

#include "KeySelectorData.generated.h"

USTRUCT()
struct JOYSTICKPLUGIN_API FKeySelectorData
{
	GENERATED_BODY()

	FKeySelectorData()
		: MinStartTime(FTimespan::MinValue())
		  , MaxStartTime(FTimespan::MaxValue())
	{
	}

	FTimespan MinStartTime;
	FTimespan MaxStartTime;
};
