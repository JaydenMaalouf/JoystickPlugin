// JoystickPlugin is licensed under the MIT License.
// Copyright Jayden Maalouf 2026. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

enum class EHapticPreviewEffectKind : uint8
{
	Periodic,
	Constant,
	Ramp,
	LeftRight
};

// The forms an authored effect can be saved as: a Blueprint subclass, or an entry in an effect group data asset.
enum class EHapticEffectAssetType : uint8
{
	BlueprintClass,
	EffectGroup
};
