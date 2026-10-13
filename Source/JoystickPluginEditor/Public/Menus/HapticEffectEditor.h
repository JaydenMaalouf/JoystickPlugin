// JoystickPlugin is licensed under the MIT License.
// Copyright Jayden Maalouf 2026. All Rights Reserved.

#pragma once

#include "EditorUndoClient.h"
#include "Widgets/SCompoundWidget.h"
#include "Widgets/Input/SComboBox.h"
#include "Data/JoystickInstanceId.h"
#include "ForceFeedback/Types/ForceFeedbackPeriodicEffectType.h"
#include "Menus/HapticEffectEditorTypes.h"
#include "Misc/Optional.h"
#include "UObject/StrongObjectPtr.h"

class IDetailsView;
class SHapticDevicePicker;
class SHapticDevicePreviewPanel;
class SHapticEffectGraph;
class UForceFeedbackEffectBase;
class UForceFeedbackEffectGroup;
struct FAssetData;
struct FPropertyAndParent;
struct FPropertyChangedEvent;

// Editor tool for authoring/previewing JoystickPlugin force feedback effects: a details view bound
// to a live effect UObject, an oscilloscope-style graph of its shape, and playback controls that
// run a copy of that UObject against a connected device, through the same effect class the game
// uses, so the preview never drifts from what the effect actually does at runtime.
//
// The toolbar and side panel are built from their own widgets (see the Haptic* headers under
// Widgets/), and the implementation is split in two: HapticEffectEditor.cpp holds the layout,
// preview effect and playback, HapticEffectEditorAssets.cpp everything to do with opening and
// saving assets.
class JOYSTICKPLUGINEDITOR_API SHapticEffectEditor final : public SCompoundWidget, public FEditorUndoClient
{
public:
	SLATE_BEGIN_ARGS(SHapticEffectEditor)
		{
		}

	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs);
	virtual ~SHapticEffectEditor() override;

	virtual void Tick(const FGeometry& AllottedGeometry, double InCurrentTime, float InDeltaTime) override;
	virtual bool SupportsKeyboardFocus() const override;
	virtual FReply OnKeyDown(const FGeometry& MyGeometry, const FKeyEvent& InKeyEvent) override;

	// Begin FEditorUndoClient Interface.
	virtual bool MatchesContext(const FTransactionContext& InContext, const TArray<TPair<UObject*, FTransactionObjectEvent>>& TransactionObjectContexts) const override;
	virtual void PostUndo(bool bSuccess) override;
	virtual void PostRedo(bool bSuccess) override;
	// End FEditorUndoClient Interface.

	// Whether the asset is an effect Blueprint or effect group this editor could open, judged from
	// its asset registry data so a content browser menu can ask without loading it. A group still
	// has to be loaded to find out whether any of its entries can be edited (see GetEditableEntries).
	static bool CanEditAsset(const FAssetData& AssetData);

	// The indices of the group's entries that hold an effect this editor can open.
	static void GetEditableEntries(const UForceFeedbackEffectGroup& EffectGroup, TArray<int32>& OutEntryIndices);
	static FText GetEntryLabel(const UForceFeedbackEffectGroup& EffectGroup, int32 EntryIndex);

	// Loads an effect Blueprint, or one entry of an effect group, as the working copy being edited.
	// With no EntryIndex a group opens on its first editable entry. Returns false, leaving the editor
	// as it was, if the asset has nothing this editor can open. Cancelling the prompt to save the
	// current working copy also leaves the editor as it was, but isn't a failure and returns true.
	bool EditAsset(UObject* Asset, int32 EntryIndex = INDEX_NONE);

	// Called before anything that would throw the working copy away (closing the tab, opening another
	// asset or entry, starting a new effect). If it has unsaved changes the user is asked whether to
	// save them, and can cancel - false means cancelled, so whatever was about to happen shouldn't.
	bool PromptToSaveChanges();

	// The name of the opened asset, with a trailing asterisk while it has unsaved changes, or the
	// editor's own name while authoring a new effect.
	FText GetTabLabel() const;

private:
	TSharedRef<SWidget> BuildAssetBar();
	TSharedRef<SWidget> BuildOpenButton();
	TSharedRef<SWidget> BuildSourceEntryPicker();
	TSharedRef<SWidget> BuildSaveButton();
	TSharedRef<SWidget> BuildStatusChip();

	TSharedPtr<FJoystickInstanceId> GetSelectedJoystick() const;
	void OnDeviceChanged();

	void SetEffectKind(EHapticPreviewEffectKind NewKind);
	void RebuildPreviewEffect(const UForceFeedbackEffectBase* InitialValues = nullptr);
	void DestroyPreviewEffect();
	void OnPropertiesChanged(const FPropertyChangedEvent& PropertyChangedEvent);
	void RecreateDeviceEffect();
	bool IsPropertyVisible(const FPropertyAndParent& PropertyAndParent) const;

	FReply OnPlayClicked();
	FReply OnStopClicked();
	FReply OnLoopClicked();
	FReply OnSaveClicked();
	void PromptSaveAsNewAsset();
	void SaveAsNewAsset(EHapticEffectAssetType AssetType);

	void NewEffect();
	void OnSourceAssetPicked(const FAssetData& AssetData);
	void OpenSource(UObject* Asset, int32 EntryIndex);
	void CloseSource();
	UForceFeedbackEffectBase* ResolveSourceEffect() const;
	bool CanSaveToSource() const;
	FText GetSourceLabel() const;
	FText GetSourceEntryLabel(int32 EntryIndex) const;
	void SyncDeviceEffect();
	bool StartPreviewEffect();
	void OnGraphScrubbed(float TimeSeconds);

	float EvaluatePreviewEffect(float TimeSeconds) const;
	bool IsPreviewDurationInfinite() const;
	float GetPreviewDurationSeconds() const;
	TOptional<EForceFeedbackPeriodicEffectType> GetPeriodicWaveform() const;
	void SetPeriodicWaveform(EForceFeedbackPeriodicEffectType WaveformType);

	TSharedPtr<SHapticDevicePicker> DevicePicker;
	TSharedPtr<SHapticEffectGraph> EffectGraph;
	TSharedPtr<SHapticDevicePreviewPanel> DevicePreviewPanel;
	TSharedPtr<IDetailsView> DetailsView;

	EHapticPreviewEffectKind EffectKind = EHapticPreviewEffectKind::Periodic;

	// PreviewEffect is what is being authored: the details view edits it, the graph is drawn from it,
	// and it is what gets saved or exported. It never touches a device - DeviceEffect, a copy brought
	// up to date whenever it is uploaded, does. Keeping them apart means undoing an edit can only ever
	// rewind authored values, never the device effect's handle or whether it is initialised.
	TStrongObjectPtr<UForceFeedbackEffectBase> PreviewEffect;
	TStrongObjectPtr<UForceFeedbackEffectBase> DeviceEffect;

	// The existing asset the preview effect was loaded from and Save writes back to: an effect
	// Blueprint, or an effect group along with which of its entries is being edited. Unset while
	// authoring a new effect. The preview stays a separate transient working copy either way, so
	// nothing reaches the asset until Save - CanSaveToSource is whether it has anything to write.
	TWeakObjectPtr<UObject> SourceAsset;
	int32 SourceEntryIndex = INDEX_NONE;
	TArray<TSharedPtr<int32>> SourceEntryOptions;
	TSharedPtr<SComboBox<TSharedPtr<int32>>> SourceEntryComboBox;

	float PlayheadTime = 0.0f;
	bool bIsPlaying = false;
	bool bLoop = false;
};
