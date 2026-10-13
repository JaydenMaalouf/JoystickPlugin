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

// The two forms the plugin can consume an authored effect in: a Blueprint subclass of the effect
// (for a component's EffectType, or to spawn directly), or an effect group data asset holding it as
// an entry.
enum class EHapticEffectAssetType : uint8
{
	BlueprintClass,
	EffectGroup
};
