// JoystickPlugin is licensed under the MIT License.
// Copyright Jayden Maalouf 2026. All Rights Reserved.

#pragma once

#include "Menus/HapticEffectEditorTypes.h"
#include "Misc/Optional.h"

// Modal prompt for which kind of asset a new effect should be saved as, laid out like the engine's class picker.
class FHapticEffectAssetTypeDialog
{
public:
	// Blocks until the dialog is closed. Unset if it was cancelled.
	static TOptional<EHapticEffectAssetType> Open();
};
