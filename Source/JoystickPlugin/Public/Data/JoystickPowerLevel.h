#pragma once

#include "JoystickPowerLevel.generated.h"

UENUM(BlueprintType)
enum class EJoystickPowerLevel : uint8
{
	Unknown = 0,
	Empty,
	/* <= 5% */
	Low,
	/* <= 20% */
	Medium,
	/* <= 70% */
	Full,
	/* <= 100% */
	Wired,
	Max
};
