// JoystickPlugin is licensed under the MIT License.
// Copyright Jayden Maalouf 2026. All Rights Reserved.

#pragma once

#include "KeySelectorTypes.generated.h"

UENUM(BlueprintType, meta = (Bitflags, UseEnumValuesAsMaskValuesInEditor = "true"))
enum class EKeySelectorTypes : uint8
{
	None = 0 UMETA(Hidden),
	Button = 1 << 0,
	Axis = 1 << 1,
	Modifier = 1 << 2
};

ENUM_CLASS_FLAGS(EKeySelectorTypes)
