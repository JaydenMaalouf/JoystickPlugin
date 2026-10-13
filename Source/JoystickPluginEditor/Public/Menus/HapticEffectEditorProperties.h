// JoystickPlugin is licensed under the MIT License.
// Copyright Jayden Maalouf 2026. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Menus/HapticEffectEditorTypes.h"

class FProperty;
class UClass;
class UForceFeedbackEffectBase;
struct FAssetData;

// What the Haptic Effect Editor treats as the authored part of an effect, and which effect classes
// it can open at all.
struct FHapticEffectEditorProperties
{
	// The runtime plumbing the panel already manages via the device picker and transport controls
	// (InstanceId, the whole Configuration section, and the Tick section), plus read-only (Visible*)
	// properties, which are runtime state - EffectId/IsInitialised, the EffectName/EffectTag/GroupTag
	// stamped on by the spawning effect group, and anything similar a subclass adds. None of it is
	// the user's to author on the preview effect, so it is neither shown nor exported.
	static bool IsManagedByEditor(const FProperty& Property);

	// Copies only the authored properties between two effects, rather than duplicating one wholesale:
	// the preview effect is transient and carries state the panel put there to drive the device (its
	// InstanceId, AutoStartOnInitialisation, and whether it is initialised right now), none of which
	// belongs in an asset. Both effects must be instances of EffectClass.
	static void CopyAuthoredProperties(const UForceFeedbackEffectBase* From, UForceFeedbackEffectBase* To, const UClass* EffectClass);

	static bool AuthoredPropertiesIdentical(const UForceFeedbackEffectBase* A, const UForceFeedbackEffectBase* B, const UClass* EffectClass);

	// The preview kind an effect class is edited as - its own, or the one it derives from. Condition
	// and custom effects (and anything built on them) have no kind and can't be opened.
	static bool FindEffectKind(const UClass* EffectClass, EHapticPreviewEffectKind& OutKind);

	static UClass* GetEffectKindClass(EHapticPreviewEffectKind Kind);

	// Whether a Blueprint asset derives from an effect class the editor can open, read from its
	// asset registry tags so the picker doesn't have to load every Blueprint in the project to tell.
	static bool IsSupportedEffectBlueprint(const FAssetData& AssetData);
};
