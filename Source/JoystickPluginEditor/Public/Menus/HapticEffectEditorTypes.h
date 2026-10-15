// JoystickPlugin is licensed under the MIT License.
// Copyright Jayden Maalouf 2026. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Misc/EnumRange.h"

enum class EHapticPreviewEffectKind : uint8
{
	Periodic,
	Constant,
	Ramp,
	Condition,
	LeftRight
};

ENUM_RANGE_BY_FIRST_AND_LAST(EHapticPreviewEffectKind, EHapticPreviewEffectKind::Periodic, EHapticPreviewEffectKind::LeftRight);

// The forms an authored effect can be saved as: a Blueprint subclass, or an entry in an effect group data asset.
enum class EHapticEffectAssetType : uint8
{
	BlueprintClass,
	EffectGroup
};
