// JoystickPlugin is licensed under the MIT License.
// Copyright Jayden Maalouf 2026. All Rights Reserved.

#pragma once

#include "EditorUndoClient.h"
#include "Widgets/SCompoundWidget.h"
#include "Widgets/Input/SComboBox.h"
#include "Data/JoystickHapticInformation.h"
#include "Data/Input/AxisData.h"
#include "Data/JoystickInstanceId.h"
#include "ForceFeedback/Types/ForceFeedbackConditionEffectType.h"
#include "ForceFeedback/Types/ForceFeedbackPeriodicEffectType.h"
#include "Menus/HapticEffectEditorTypes.h"
#include "Misc/Optional.h"
#include "UObject/StrongObjectPtr.h"
#include "Widgets/HapticConditionGraph.h"

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

	TSharedPtr<FJoystickInstanceId> GetSelectedJoystick() const;
	void OnDeviceChanged();
	void UpdateSelectedHapticInformation();

	// Each is true while no device is selected, as there is nothing to judge support against.
	bool IsEffectKindSupported(EHapticPreviewEffectKind Kind) const;
	bool IsWaveformSupported(EForceFeedbackPeriodicEffectType WaveformType) const;
	bool IsConditionTypeSupported(EForceFeedbackConditionEffectType ConditionType) const;
	bool IsPreviewEffectSupported() const;

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

	// The condition's force for the axis in the given motion.
	float EvaluateConditionMotion(const FHapticConditionMotion& Motion) const;

	// Follows the device's axis, fitting its recent positions for a velocity and acceleration. Cleared while there is no device or no condition effect.
	void UpdateConditionLiveMotion();

	// The previewed axis as the device reports it now, with its raw position in -1..1 and when SDL read it, or unset while no device is selected.
	TOptional<FAxisData> GetPreviewAxis() const;

	bool IsPreviewDurationInfinite() const;

	// The authored iteration count, or unset while there is no last iteration to count towards.
	TOptional<int64> GetPreviewIterations() const;

	// The wait before the first iteration plays.
	int64 GetPreviewDelayMilliseconds() const;

	// The length of a single iteration, without the delay.
	int64 GetPreviewDurationMilliseconds() const;

	// The length the graph covers: the delay followed by a single iteration.
	int64 GetPreviewTimelineMilliseconds() const;

	TOptional<EForceFeedbackPeriodicEffectType> GetPeriodicWaveform() const;
	void SetPeriodicWaveform(EForceFeedbackPeriodicEffectType WaveformType);

	TOptional<EForceFeedbackConditionEffectType> GetConditionType() const;
	void SetConditionType(EForceFeedbackConditionEffectType ConditionType);

	TSharedPtr<SHapticDevicePicker> DevicePicker;
	TSharedPtr<SHapticEffectGraph> EffectGraph;
	TSharedPtr<SHapticDevicePreviewPanel> DevicePreviewPanel;
	TSharedPtr<IDetailsView> DetailsView;

	// The selected device's supported effect types, read once per device change rather than on every paint. Unset while no device is selected.
	TOptional<FJoystickHapticInformation> SelectedHapticInformation;

	EHapticPreviewEffectKind EffectKind = EHapticPreviewEffectKind::Periodic;

	// How much of the axis' recent movement the live velocity and acceleration are fitted to. Longer is steadier but lags further behind the wheel.
	// Acceleration is the most sensitive: a hand's speed naturally wobbles by 10-15% a few times a second, which a 0.1s window reads as +/-3 even through a
	// steady turn. This window smooths that out, at the cost of a quick flick (speeding up then slowing down within ~0.2-0.3s) partly cancelling itself out.
	static constexpr double ConditionMotionFitWindowSeconds = 0.15;

	// A still axis still jitters by its quantisation, and the fit's rounding never cancels to exactly zero. Friction's force flips on the velocity's sign alone,
	// so a window whose positions all sit within this span reads as still rather than as a tiny motion either way. ~0.1% of the half range.
	static constexpr float ConditionStillPositionSpan = 0.002f;

	TOptional<FHapticConditionMotion> ConditionLiveMotion;

	// The axis' recent positions with the time each was read, in seconds on SDL's clock.
	TArray<TPair<double, float>> ConditionPositionHistory;

	// The timestamp of the last axis read added to the history, to tell a new read from one already fitted.
	int64 ConditionLastReadTimestamp = 0;

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
