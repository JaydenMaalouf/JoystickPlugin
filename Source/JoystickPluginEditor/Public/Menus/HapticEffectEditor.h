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

// Editor tool for authoring and previewing force feedback effects against a connected device.
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

	// Whether the asset is an effect Blueprint or effect group this editor could open, judged without loading it.
	static bool CanEditAsset(const FAssetData& AssetData);

	// The indices of the group's entries that hold an effect this editor can open.
	static void GetEditableEntries(const UForceFeedbackEffectGroup& EffectGroup, TArray<int32>& OutEntryIndices);
	static FText GetEntryLabel(const UForceFeedbackEffectGroup& EffectGroup, int32 EntryIndex);

	// Opens an effect Blueprint, or one entry of an effect group, as the working copy. Returns false only if the asset has nothing this editor can open.
	bool EditAsset(UObject* Asset, int32 EntryIndex = INDEX_NONE);

	// Asks to save unsaved changes before the working copy is discarded. Returns false if the user cancelled.
	bool PromptToSaveChanges();

	// The opened asset's name (with an asterisk while unsaved), or the editor's own name for a new effect.
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
	void OnGraphScrubbed(float TimeMilliseconds);

	// Takes a fractional time, as the graph samples the curve more finely than a millisecond when zoomed in.
	float EvaluatePreviewEffect(float TimeMilliseconds) const;
	bool IsPreviewDurationInfinite() const;

	// The wait before the first iteration plays.
	int64 GetPreviewDelayMilliseconds() const;

	// The length of a single iteration, without the delay.
	int64 GetPreviewDurationMilliseconds() const;

	// The length the graph covers: the delay followed by a single iteration.
	int64 GetPreviewTimelineMilliseconds() const;

	TOptional<EForceFeedbackPeriodicEffectType> GetPeriodicWaveform() const;
	void SetPeriodicWaveform(EForceFeedbackPeriodicEffectType WaveformType);

	TSharedPtr<SHapticDevicePicker> DevicePicker;
	TSharedPtr<SHapticEffectGraph> EffectGraph;
	TSharedPtr<SHapticDevicePreviewPanel> DevicePreviewPanel;
	TSharedPtr<IDetailsView> DetailsView;

	EHapticPreviewEffectKind EffectKind = EHapticPreviewEffectKind::Periodic;

	// PreviewEffect is the authored effect. DeviceEffect is the copy uploaded to the device.
	TStrongObjectPtr<UForceFeedbackEffectBase> PreviewEffect;
	TStrongObjectPtr<UForceFeedbackEffectBase> DeviceEffect;

	// The asset (and group entry) Save writes back to. Unset while authoring a new effect.
	TWeakObjectPtr<UObject> SourceAsset;
	int32 SourceEntryIndex = INDEX_NONE;
	TArray<TSharedPtr<int32>> SourceEntryOptions;
	TSharedPtr<SComboBox<TSharedPtr<int32>>> SourceEntryComboBox;

	int64 PlayheadTimeMilliseconds = 0;

	// Time since playback started, across every iteration, where PlayheadTimeMilliseconds is only the position within the current pass.
	int64 ElapsedTimeMilliseconds = 0;

	// The pass the playhead is on, counted from one.
	int64 CurrentIteration = 1;
	bool bIsPlaying = false;
	bool bLoop = false;
};
