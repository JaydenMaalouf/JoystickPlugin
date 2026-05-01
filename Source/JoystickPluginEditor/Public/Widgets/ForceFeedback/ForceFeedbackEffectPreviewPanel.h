// JoystickPlugin is licensed under the MIT License.
// Copyright Jayden Maalouf 2026. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"

class SForceFeedbackEffectDataEditor;
class UForceFeedbackEffectBase;
class FBlueprintEditor;

/**
 * Dockable editor panel that previews the currently-selected Force Feedback effect.
 *
 * Intended to be read-only (preview-only) so users edit values via the Details panel.
 */
class SForceFeedbackEffectPreviewPanel : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SForceFeedbackEffectPreviewPanel)
		{
		}

		SLATE_ARGUMENT(TWeakPtr<FBlueprintEditor>, BlueprintEditor)
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs);

private:
	void BindDelegates();
	void UnbindDelegates();
	void RefreshBlueprintCDO();
	void OnAnyBlueprintCompiled();
	void RebuildPropertyRows() const;
	void SetEffect(UForceFeedbackEffectBase* NewEffect);

	const FSlateBrush* GetButtonIcon() const;

	TWeakObjectPtr<UForceFeedbackEffectBase> EffectPtr;
	TSharedPtr<SForceFeedbackEffectDataEditor> EffectDataEditor;
	TSharedPtr<class SVerticalBox> PropertyRowsBox;

	TWeakPtr<FBlueprintEditor> BlueprintEditorWeak;

	FDelegateHandle BlueprintCompiledHandle;
};
