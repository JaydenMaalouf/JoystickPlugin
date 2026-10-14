// JoystickPlugin is licensed under the MIT License.
// Copyright Jayden Maalouf 2026. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Menus/HapticEffectEditorTypes.h"

class FProperty;
class UClass;
class UForceFeedbackEffectBase;
struct FAssetData;

// What the Haptic Effect Editor treats as the authored part of an effect, and which effect classes it can open.
struct FHapticEffectEditorProperties
{
	// Runtime plumbing and read-only state the panel manages itself, so it is neither shown nor exported.
	static bool IsManagedByEditor(const FProperty& Property);

	// Copies only the authored properties between two effects. Both must be instances of EffectClass.
	static void CopyAuthoredProperties(const UForceFeedbackEffectBase* From, UForceFeedbackEffectBase* To, const UClass* EffectClass);

	static bool AuthoredPropertiesIdentical(const UForceFeedbackEffectBase* A, const UForceFeedbackEffectBase* B, const UClass* EffectClass);

	// The preview kind an effect class is edited as. Condition and custom effects have no kind and can't be opened.
	static bool FindEffectKind(const UClass* EffectClass, EHapticPreviewEffectKind& OutKind);

	static UClass* GetEffectKindClass(EHapticPreviewEffectKind Kind);

	// Whether a Blueprint derives from an effect class the editor can open, read from asset registry tags without loading it.
	static bool IsSupportedEffectBlueprint(const FAssetData& AssetData);
};
