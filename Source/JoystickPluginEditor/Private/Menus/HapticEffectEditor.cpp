// JoystickPlugin is licensed under the MIT License.
// Copyright Jayden Maalouf 2026. All Rights Reserved.

#include "Menus/HapticEffectEditor.h"

#include "DetailsViewArgs.h"
#include "Editor.h"
#include "IDetailsView.h"
#include "JoystickFunctionLibrary.h"
#include "JoystickSubsystem.h"
#include "PropertyEditorDelegates.h"
#include "PropertyEditorModule.h"
#include "ScopedTransaction.h"
#include "Data/JoystickInformation.h"
#include "ForceFeedback/ForceFeedbackEffectEvaluation.h"
#include "ForceFeedback/Effects/ForceFeedbackEffectBase.h"
#include "ForceFeedback/Effects/ForceFeedbackEffectCondition.h"
#include "ForceFeedback/Effects/ForceFeedbackEffectConstant.h"
#include "ForceFeedback/Effects/ForceFeedbackEffectLeftRight.h"
#include "ForceFeedback/Effects/ForceFeedbackEffectPeriodic.h"
#include "ForceFeedback/Effects/ForceFeedbackEffectRamp.h"
#include "Menus/HapticEffectEditorProperties.h"
#include "Misc/ITransaction.h"
#include "Misc/TransactionObjectEvent.h"
#include "Modules/ModuleManager.h"
#include "UObject/Package.h"
#include "Widgets/HapticConditionPanel.h"
#include "Widgets/HapticConditionTypeSelector.h"
#include "Widgets/HapticDevicePicker.h"
#include "Widgets/HapticDevicePreviewPanel.h"
#include "Widgets/HapticEffectGraph.h"
#include "Widgets/HapticEffectTypeSelector.h"
#include "Widgets/HapticStatusChip.h"
#include "Widgets/HapticToolbarPill.h"
#include "Widgets/HapticTransportBar.h"
#include "Widgets/HapticWaveformSelector.h"
#include "Widgets/SBoxPanel.h"
#include "Widgets/SOverlay.h"
#include "Widgets/Layout/SSpacer.h"
#include "Widgets/Layout/SSplitter.h"

#define LOCTEXT_NAMESPACE "HapticEffectEditor"

void SHapticEffectEditor::Construct(const FArguments& InArgs)
{
	if (GEditor)
	{
		GEditor->RegisterForUndo(this);
	}

	FPropertyEditorModule& PropertyModule = FModuleManager::LoadModuleChecked<FPropertyEditorModule>("PropertyEditor");
	FDetailsViewArgs DetailsViewArgs;
	DetailsViewArgs.bAllowSearch = false;
	DetailsViewArgs.bShowOptions = false;
	DetailsViewArgs.bHideSelectionTip = true;
	DetailsViewArgs.bShowScrollBar = true;
	DetailsViewArgs.NameAreaSettings = FDetailsViewArgs::HideNameArea;
	DetailsView = PropertyModule.CreateDetailView(DetailsViewArgs);
	DetailsView->OnFinishedChangingProperties().AddLambda([this](const FPropertyChangedEvent& PropertyChangedEvent)
	{
		OnPropertiesChanged(PropertyChangedEvent);
	});
	DetailsView->SetIsPropertyVisibleDelegate(FIsPropertyVisible::CreateSP(this, &SHapticEffectEditor::IsPropertyVisible));

	// Made ahead of the rest of the layout: the preview effect is built against the selected device.
	DevicePicker = SNew(SHapticDevicePicker)
		.OnDeviceChanged(this, &SHapticEffectEditor::OnDeviceChanged);

	UpdateSelectedHapticInformation();
	RebuildPreviewEffect();

	ChildSlot
	[
		SNew(SVerticalBox)

		+ SVerticalBox::Slot()
		.AutoHeight()
		.Padding(12, 12, 12, 8)
		[
			SNew(SHorizontalBox)
			+ SHorizontalBox::Slot()
			.AutoWidth()
			.VAlign(VAlign_Center)
			.Padding(0, 0, 8, 0)
			[
				SNew(SHapticToolbarPill)
				[
					DevicePicker.ToSharedRef()
				]
			]
			+ SHorizontalBox::Slot()
			.AutoWidth()
			.VAlign(VAlign_Center)
			.Padding(0, 0, 8, 0)
			[
				SNew(SHapticToolbarPill)
				[
					SNew(SHapticTransportBar)
					.HasDevice_Lambda([this] { return GetSelectedJoystick().IsValid() && IsPreviewEffectSupported(); })
					.IsPlaying_Lambda([this] { return bIsPlaying; })
					.IsLooping_Lambda([this] { return bLoop; })
					.OnPlayClicked(this, &SHapticEffectEditor::OnPlayClicked)
					.OnStopClicked(this, &SHapticEffectEditor::OnStopClicked)
					.OnLoopClicked(this, &SHapticEffectEditor::OnLoopClicked)
				]
			]
			+ SHorizontalBox::Slot()
			.AutoWidth()
			.VAlign(VAlign_Center)
			.Padding(0, 0, 8, 0)
			[
				SNew(SHapticToolbarPill)
				[
					SNew(SHapticEffectTypeSelector)
					.EffectKind_Lambda([this] { return EffectKind; })
					.OnEffectKindSelected(this, &SHapticEffectEditor::SetEffectKind)
					.IsEffectKindSupported(this, &SHapticEffectEditor::IsEffectKindSupported)
				]
			]
			+ SHorizontalBox::Slot()
			.AutoWidth()
			.VAlign(VAlign_Center)
			.Padding(0, 0, 8, 0)
			[
				SNew(SHapticToolbarPill)
				.Visibility_Lambda([this] { return EffectKind == EHapticPreviewEffectKind::Periodic ? EVisibility::Visible : EVisibility::Collapsed; })
				[
					SNew(SHapticWaveformSelector)
					.SelectedWaveform_Lambda([this] { return GetPeriodicWaveform(); })
					.OnWaveformSelected(this, &SHapticEffectEditor::SetPeriodicWaveform)
					.IsWaveformSupported(this, &SHapticEffectEditor::IsWaveformSupported)
				]
			]
			+ SHorizontalBox::Slot()
			.AutoWidth()
			.VAlign(VAlign_Center)
			.Padding(0, 0, 8, 0)
			[
				SNew(SHapticToolbarPill)
				.Visibility_Lambda([this] { return EffectKind == EHapticPreviewEffectKind::Condition ? EVisibility::Visible : EVisibility::Collapsed; })
				[
					SNew(SHapticConditionTypeSelector)
					.SelectedConditionType_Lambda([this] { return GetConditionType(); })
					.OnConditionTypeSelected(this, &SHapticEffectEditor::SetConditionType)
					.IsConditionTypeSupported(this, &SHapticEffectEditor::IsConditionTypeSupported)
				]
			]
			+ SHorizontalBox::Slot()
			.FillWidth(1.0f)
			[
				SNew(SSpacer)
			]
			+ SHorizontalBox::Slot()
			.AutoWidth()
			.VAlign(VAlign_Center)
			.Padding(0, 0, 8, 0)
			[
				SNew(SHapticToolbarPill)
				[
					BuildAssetBar()
				]
			]
			+ SHorizontalBox::Slot()
			.AutoWidth()
			.VAlign(VAlign_Center)
			[
				SNew(SHapticToolbarPill)
				[
					SNew(SHapticStatusChip)
					.PlayheadTimeMilliseconds_Lambda([this] { return PlayheadTimeMilliseconds; })
					.ElapsedTimeMilliseconds_Lambda([this] { return ElapsedTimeMilliseconds; })
					.Force_Lambda([this] { return EvaluatePreviewEffect(static_cast<float>(PlayheadTimeMilliseconds)); })
					.Iteration_Lambda([this] { return CurrentIteration; })
					.Iterations_Lambda([this] { return GetPreviewIterations(); })
				]
			]
		]

		+ SVerticalBox::Slot()
		.FillHeight(1.0f)
		.Padding(12, 0, 12, 12)
		[
			SNew(SSplitter)
			.Orientation(Orient_Horizontal)
			+ SSplitter::Slot()
			.Value(0.7f)
			[
				SNew(SOverlay)
				+ SOverlay::Slot()
				[
					SAssignNew(EffectGraph, SHapticEffectGraph)
					.Visibility_Lambda([this] { return EffectKind == EHapticPreviewEffectKind::Condition ? EVisibility::Collapsed : EVisibility::Visible; })
					.OnEvaluate(FOnEvaluateHapticEffect::CreateSP(this, &SHapticEffectEditor::EvaluatePreviewEffect))
					.DurationMilliseconds_Lambda([this] { return GetPreviewTimelineMilliseconds(); })
					.PlayheadTimeMilliseconds_Lambda([this] { return PlayheadTimeMilliseconds; })
					.OnScrubbed(this, &SHapticEffectEditor::OnGraphScrubbed)
				]
				+ SOverlay::Slot()
				[
					// A condition's force follows the axis rather than time, so it is graphed against the axis instead of the timeline.
					SNew(SHapticConditionPanel)
					.Visibility_Lambda([this] { return EffectKind == EHapticPreviewEffectKind::Condition ? EVisibility::Visible : EVisibility::Collapsed; })
					.OnEvaluate(FOnEvaluateHapticConditionMotion::CreateSP(this, &SHapticEffectEditor::EvaluateConditionMotion))
					.ConditionType_Lambda([this] { return GetConditionType(); })
					.LiveMotion_Lambda([this] { return ConditionLiveMotion; })
				]
			]
			+ SSplitter::Slot()
			.Value(0.3f)
			[
				SNew(SVerticalBox)
				+ SVerticalBox::Slot()
				.FillHeight(1.0f)
				[
					DetailsView.ToSharedRef()
				]
				+ SVerticalBox::Slot()
				.AutoHeight()
				.Padding(0, 4, 0, 0)
				[
					SAssignNew(DevicePreviewPanel, SHapticDevicePreviewPanel)
					.Joystick_Lambda([this] { return GetSelectedJoystick(); })
					.IsPlaying_Lambda([this] { return bIsPlaying; })
				]
			]
		]
	];
}

SHapticEffectEditor::~SHapticEffectEditor()
{
	if (GEditor)
	{
		GEditor->UnregisterForUndo(this);
	}

	DestroyPreviewEffect();
}

void SHapticEffectEditor::Tick(const FGeometry& AllottedGeometry, const double InCurrentTime, const float InDeltaTime)
{
	SCompoundWidget::Tick(AllottedGeometry, InCurrentTime, InDeltaTime);

	UpdateConditionLiveMotion();

	if (!bIsPlaying || !PreviewEffect.IsValid() || !DeviceEffect.IsValid())
	{
		return;
	}

	// Read from the device effect's own start time, so the playhead stays on the same clock as the device.
	const int64 ElapsedTime = static_cast<int64>(DeviceEffect->GetElapsedTimeMilliseconds());
	const int64 PreviousPlayheadTime = PlayheadTimeMilliseconds;

	// An infinite effect never ends, so the view pages along as the playhead crosses the right edge.
	if (IsPreviewDurationInfinite())
	{
		PlayheadTimeMilliseconds = ElapsedTime;
		ElapsedTimeMilliseconds = ElapsedTime;

		const TRange<double> ViewBounds = EffectGraph->GetViewBounds();
		if (PreviousPlayheadTime <= ViewBounds.GetUpperBoundValue() && PlayheadTimeMilliseconds > ViewBounds.GetUpperBoundValue())
		{
			EffectGraph->PageTimeWindowTo(ViewBounds.GetUpperBoundValue());
		}

		return;
	}

	const int64 Delay = GetPreviewDelayMilliseconds();
	const int64 Duration = GetPreviewDurationMilliseconds();

	// A zero-length effect has no pass to sweep, and would divide by zero below.
	if (Duration <= 0)
	{
		PlayheadTimeMilliseconds = 0;
		ElapsedTimeMilliseconds = ElapsedTime;
		bIsPlaying = bLoop;
		return;
	}

	// The graph shows the delay then a single pass, so the playhead crosses the delay once, sweeps the pass once per iteration and stops after the last one.
	const int64 ActiveTime = FMath::Max<int64>(ElapsedTime - Delay, 0);
	const int64 Iterations = DeviceEffect->GetRunIterations();
	const int64 ElapsedIteration = ActiveTime / Duration + 1;
	if (!bLoop && (!DeviceEffect->IsEffectRunning() || ElapsedIteration > Iterations))
	{
		// A stopped device effect reports no elapsed time, so the last ticked value stands unless every iteration ran.
		if (ElapsedIteration > Iterations)
		{
			ElapsedTimeMilliseconds = Delay + Duration * Iterations;
		}

		PlayheadTimeMilliseconds = Delay + Duration;
		bIsPlaying = false;
		return;
	}

	PlayheadTimeMilliseconds = FMath::Min(ElapsedTime, Delay) + ActiveTime % Duration;
	ElapsedTimeMilliseconds = ElapsedTime;
	CurrentIteration = ElapsedIteration;
}

bool SHapticEffectEditor::SupportsKeyboardFocus() const
{
	return true;
}

FReply SHapticEffectEditor::OnKeyDown(const FGeometry& MyGeometry, const FKeyEvent& InKeyEvent)
{
	// Ctrl+S saves the effect and is always swallowed, so it never falls through to saving the level.
	if (InKeyEvent.GetKey() == EKeys::S && InKeyEvent.IsControlDown() && !InKeyEvent.IsAltDown() && !InKeyEvent.IsShiftDown())
	{
		if (!InKeyEvent.IsRepeat())
		{
			OnSaveClicked();
		}

		return FReply::Handled();
	}

	const bool bHasModifier = InKeyEvent.IsControlDown() || InKeyEvent.IsAltDown() || InKeyEvent.IsShiftDown() || InKeyEvent.IsCommandDown();
	if (InKeyEvent.GetKey() == EKeys::SpaceBar && !bHasModifier)
	{
		// Holding the key down would otherwise restart/stop the device effect on every repeat.
		if (!InKeyEvent.IsRepeat())
		{
			if (bIsPlaying)
			{
				OnStopClicked();
			}
			else
			{
				OnPlayClicked();
			}
		}

		return FReply::Handled();
	}

	// Escape only stops playback, and is left to bubble while idle so it keeps its usual editor behaviour.
	if (InKeyEvent.GetKey() == EKeys::Escape && !bHasModifier && bIsPlaying)
	{
		OnStopClicked();
		return FReply::Handled();
	}

	return SCompoundWidget::OnKeyDown(MyGeometry, InKeyEvent);
}

// Limits undo handling to transactions that touch the authored effect.
bool SHapticEffectEditor::MatchesContext(const FTransactionContext& InContext, const TArray<TPair<UObject*, FTransactionObjectEvent>>& TransactionObjectContexts) const
{
	return TransactionObjectContexts.ContainsByPredicate([this](const TPair<UObject*, FTransactionObjectEvent>& TransactionObjectContext)
	{
		return TransactionObjectContext.Key == PreviewEffect.Get();
	});
}

void SHapticEffectEditor::PostUndo(const bool bSuccess)
{
	PostEdit(bSuccess);
}

void SHapticEffectEditor::PostRedo(const bool bSuccess)
{
	PostEdit(bSuccess);
}

void SHapticEffectEditor::PostEdit(const bool bSuccess)
{
	if (!bSuccess || !PreviewEffect.IsValid())
	{
		return;
	}

	// The changed property isn't known here, so resync Loop and support, and restart a playing effect.
	bLoop = PreviewEffect->InfiniteIterations;
	UpdatePreviewEffectSupported();

	if (bIsPlaying)
	{
		OnPlayClicked();
	}
}

TSharedPtr<FJoystickInstanceId> SHapticEffectEditor::GetSelectedJoystick() const
{
	return DevicePicker.IsValid() ? DevicePicker->GetSelectedJoystick() : nullptr;
}

void SHapticEffectEditor::OnDeviceChanged()
{
	UpdateSelectedHapticInformation();

	// Carries the edits over to an effect built against the newly selected device.
	RebuildPreviewEffect(PreviewEffect.Get());
}

void SHapticEffectEditor::UpdateSelectedHapticInformation()
{
	SelectedHapticInformation.Reset();
	UnsupportedEffectKinds.Reset();
	UnsupportedWaveforms.Reset();
	UnsupportedConditionTypes.Reset();

	const TSharedPtr<FJoystickInstanceId> SelectedJoystick = GetSelectedJoystick();
	UJoystickSubsystem* JoystickSubsystem = GEngine ? GEngine->GetEngineSubsystem<UJoystickSubsystem>() : nullptr;
	if (!SelectedJoystick.IsValid() || !JoystickSubsystem)
	{
		return;
	}

	FJoystickInformation JoystickInfo;
	if (!JoystickSubsystem->GetJoystickInfo(*SelectedJoystick, JoystickInfo))
	{
		return;
	}

	SelectedHapticInformation = JoystickInfo.Haptic;

	for (const EHapticPreviewEffectKind Kind : TEnumRange<EHapticPreviewEffectKind>())
	{
		if (!FHapticEffectEditorProperties::IsEffectKindSupported(Kind, JoystickInfo.Haptic))
		{
			UnsupportedEffectKinds.Add(Kind);
		}
	}

	for (const EForceFeedbackPeriodicEffectType WaveformType : TEnumRange<EForceFeedbackPeriodicEffectType>())
	{
		if (!FHapticEffectEditorProperties::IsWaveformSupported(WaveformType, JoystickInfo.Haptic))
		{
			UnsupportedWaveforms.Add(WaveformType);
		}
	}

	for (const EForceFeedbackConditionEffectType ConditionType : TEnumRange<EForceFeedbackConditionEffectType>())
	{
		if (!FHapticEffectEditorProperties::IsConditionTypeSupported(ConditionType, JoystickInfo.Haptic))
		{
			UnsupportedConditionTypes.Add(ConditionType);
		}
	}
}

bool SHapticEffectEditor::IsEffectKindSupported(const EHapticPreviewEffectKind Kind) const
{
	return !UnsupportedEffectKinds.Contains(Kind);
}

bool SHapticEffectEditor::IsWaveformSupported(const EForceFeedbackPeriodicEffectType WaveformType) const
{
	return !UnsupportedWaveforms.Contains(WaveformType);
}

bool SHapticEffectEditor::IsConditionTypeSupported(const EForceFeedbackConditionEffectType ConditionType) const
{
	return !UnsupportedConditionTypes.Contains(ConditionType);
}

bool SHapticEffectEditor::IsPreviewEffectSupported() const
{
	return bPreviewEffectSupported;
}

void SHapticEffectEditor::UpdatePreviewEffectSupported()
{
	// Checked against the authored effect rather than its kind, so an unsupported waveform or condition type also counts.
	bPreviewEffectSupported = !SelectedHapticInformation.IsSet() || !PreviewEffect.IsValid() || PreviewEffect->IsSupportedByDevice(SelectedHapticInformation.GetValue());
}

void SHapticEffectEditor::SetEffectKind(const EHapticPreviewEffectKind NewKind)
{
	if (EffectKind == NewKind)
	{
		return;
	}

	// An opened asset holds one kind of effect, so picking another kind starts a new effect instead.
	if (!PromptToSaveChanges())
	{
		return;
	}

	CloseSource();

	EffectKind = NewKind;
	RebuildPreviewEffect();
}

void SHapticEffectEditor::RebuildPreviewEffect(const UForceFeedbackEffectBase* InitialValues)
{
	// InitialValues may be the outgoing preview effect itself, so keep it alive until its values are copied out.
	const TStrongObjectPtr<UForceFeedbackEffectBase> PreviousEffect = PreviewEffect;
	DestroyPreviewEffect();

	// Always one of the plugin's own effect classes, as a subclass's tick logic expects a component in a game world.
	UClass* EffectClass = FHapticEffectEditorProperties::GetEffectKindClass(EffectKind);

	// Transactional, so the details panel's edits are recorded for undo/redo like any other object's.
	PreviewEffect = TStrongObjectPtr<UForceFeedbackEffectBase>(NewObject<UForceFeedbackEffectBase>(GetTransientPackage(), EffectClass, NAME_None, RF_Transient | RF_Transactional));
	PreviewEffect->InfiniteIterations = bLoop;

	if (InitialValues && InitialValues->IsA(EffectClass))
	{
		FHapticEffectEditorProperties::CopyAuthoredProperties(InitialValues, PreviewEffect.Get(), EffectClass);
		bLoop = PreviewEffect->InfiniteIterations;
	}

	DeviceEffect = TStrongObjectPtr<UForceFeedbackEffectBase>(NewObject<UForceFeedbackEffectBase>(GetTransientPackage(), EffectClass, NAME_None, RF_Transient));
	DeviceEffect->Configuration.AutoStartOnInitialisation = false;
	DeviceEffect->Configuration.AutoUpdatePostTick = false;
	DeviceEffect->SetTickableInEditor(true);

	if (const TSharedPtr<FJoystickInstanceId> SelectedJoystick = GetSelectedJoystick())
	{
		DeviceEffect->SetInstanceId(*SelectedJoystick);
	}

	if (DetailsView.IsValid())
	{
		DetailsView->SetObject(PreviewEffect.Get());
	}

	UpdatePreviewEffectSupported();

	// Reset the view to the default window rather than keeping the previous effect's pan/zoom.
	if (EffectGraph.IsValid())
	{
		EffectGraph->ResetTimeWindow();
	}
}

void SHapticEffectEditor::DestroyPreviewEffect()
{
	if (DeviceEffect)
	{
		DeviceEffect->StopEffect();
		DeviceEffect->DestroyEffect();
	}

	DeviceEffect.Reset();
	PreviewEffect.Reset();
	bIsPlaying = false;
	PlayheadTimeMilliseconds = 0;
	ElapsedTimeMilliseconds = 0;
	CurrentIteration = 1;
}

void SHapticEffectEditor::OnPropertiesChanged(const FPropertyChangedEvent& PropertyChangedEvent)
{
	if (!PreviewEffect.IsValid())
	{
		return;
	}

	// SDL can't change an uploaded effect's type in place, so an EffectType edit reinitialises the device effect.
	static const FName EffectTypePropertyName(GET_MEMBER_NAME_CHECKED(FForceFeedbackEffectPeriodicData, EffectType));
	if (PropertyChangedEvent.GetPropertyName() == EffectTypePropertyName)
	{
		RecreateDeviceEffect();
		return;
	}

	// InfiniteIterations mirrors the Loop button and only reaches the device on start, so a playing effect is restarted.
	static const FName InfiniteIterationsPropertyName(GET_MEMBER_NAME_CHECKED(UForceFeedbackEffectBase, InfiniteIterations));
	if (PropertyChangedEvent.GetPropertyName() == InfiniteIterationsPropertyName)
	{
		bLoop = PreviewEffect->InfiniteIterations;
		if (bIsPlaying)
		{
			OnPlayClicked();
		}

		return;
	}

	if (DeviceEffect.IsValid() && DeviceEffect->IsInitialised)
	{
		SyncDeviceEffect();
		DeviceEffect->UpdateEffect();
	}
}

void SHapticEffectEditor::RecreateDeviceEffect()
{
	UpdatePreviewEffectSupported();

	if (!DeviceEffect.IsValid() || !DeviceEffect->IsInitialised)
	{
		return;
	}

	const bool bWasPlaying = bIsPlaying;

	DeviceEffect->StopEffect();
	DeviceEffect->DestroyEffect();

	if (!IsPreviewEffectSupported())
	{
		bIsPlaying = false;
		return;
	}

	SyncDeviceEffect();
	if (!DeviceEffect->InitialiseEffect())
	{
		bIsPlaying = false;
		return;
	}

	if (bWasPlaying)
	{
		bIsPlaying = StartPreviewEffect();
	}
}

bool SHapticEffectEditor::IsPropertyVisible(const FPropertyAndParent& PropertyAndParent) const
{
	if (FHapticEffectEditorProperties::IsManagedByEditor(PropertyAndParent.Property))
	{
		return false;
	}

	for (const FProperty* ParentProperty : PropertyAndParent.ParentProperties)
	{
		if (ParentProperty && FHapticEffectEditorProperties::IsManagedByEditor(*ParentProperty))
		{
			return false;
		}
	}

	return true;
}

FReply SHapticEffectEditor::OnPlayClicked()
{
	if (!PreviewEffect.IsValid() || !DeviceEffect.IsValid() || !GetSelectedJoystick().IsValid())
	{
		return FReply::Handled();
	}

	// Always upload a fresh effect, as SDL's in-place updates aren't reliable for every change.
	if (DeviceEffect->IsInitialised)
	{
		DeviceEffect->StopEffect();
		DeviceEffect->DestroyEffect();
	}

	SyncDeviceEffect();
	if (!DeviceEffect->InitialiseEffect())
	{
		bIsPlaying = false;
		return FReply::Handled();
	}

	PlayheadTimeMilliseconds = 0;
	ElapsedTimeMilliseconds = 0;
	bIsPlaying = StartPreviewEffect();

	// Infinite playback may have paged the view past zero, leaving the restarted playhead off-screen.
	if (bIsPlaying && IsPreviewDurationInfinite() && !EffectGraph->GetViewBounds().Contains(0.0))
	{
		EffectGraph->PageTimeWindowTo(0.0);
	}

	DevicePreviewPanel->ResetAxisDetection();

	return FReply::Handled();
}

FReply SHapticEffectEditor::OnStopClicked()
{
	if (DeviceEffect.IsValid())
	{
		DeviceEffect->StopEffect();
		DeviceEffect->DestroyEffect();
	}

	bIsPlaying = false;

	return FReply::Handled();
}

FReply SHapticEffectEditor::OnLoopClicked()
{
	bLoop = !bLoop;

	// Mirrored straight away rather than left for Play, so the details panel shows the same state.
	if (PreviewEffect.IsValid())
	{
		const FScopedTransaction Transaction(LOCTEXT("ToggleLoopTransaction", "Toggle Haptic Effect Loop"));
		PreviewEffect->Modify();
		PreviewEffect->InfiniteIterations = bLoop;
	}

	return FReply::Handled();
}

void SHapticEffectEditor::SyncDeviceEffect()
{
	if (PreviewEffect.IsValid() && DeviceEffect.IsValid())
	{
		FHapticEffectEditorProperties::CopyAuthoredProperties(PreviewEffect.Get(), DeviceEffect.Get(), PreviewEffect->GetClass());
	}
}

bool SHapticEffectEditor::StartPreviewEffect()
{
	// The preview plays the authored number of iterations unless looping.
	DeviceEffect->InfiniteIterations = bLoop;
	CurrentIteration = 1;

	return DeviceEffect->StartEffect();
}

void SHapticEffectEditor::OnGraphScrubbed(const float TimeMilliseconds)
{
	// Scrubbing only seeks the graph (SDL can't sample an effect at an arbitrary time) and ends playback, so stop the device effect too.
	if (bIsPlaying)
	{
		OnStopClicked();
	}

	// An infinite effect has no end to clamp to, so any point at or after its start is valid.
	const int64 ScrubbedTime = FMath::RoundToInt64(TimeMilliseconds);
	PlayheadTimeMilliseconds = IsPreviewDurationInfinite() ? FMath::Max<int64>(ScrubbedTime, 0) : FMath::Clamp<int64>(ScrubbedTime, 0, GetPreviewTimelineMilliseconds());
	ElapsedTimeMilliseconds = PlayheadTimeMilliseconds;
	CurrentIteration = 1;
}

float SHapticEffectEditor::EvaluatePreviewEffect(const float TimeMilliseconds) const
{
	if (!PreviewEffect.IsValid())
	{
		return 0.0f;
	}

	// The effect data is authored in seconds, so the evaluation functions take seconds too.
	// They are timed from the effect's first sample, which the device only plays once the delay has passed, and are zero before it.
	const float TimeSeconds = (TimeMilliseconds - static_cast<float>(GetPreviewDelayMilliseconds())) / 1000.0f;

	switch (EffectKind)
	{
		case EHapticPreviewEffectKind::Periodic:
			if (const UForceFeedbackEffectPeriodic* Periodic = Cast<UForceFeedbackEffectPeriodic>(PreviewEffect.Get()))
			{
				return FForceFeedbackEffectEvaluation::EvaluatePeriodic(Periodic->EffectData, TimeSeconds);
			}
			break;
		case EHapticPreviewEffectKind::Constant:
			if (const UForceFeedbackEffectConstant* Constant = Cast<UForceFeedbackEffectConstant>(PreviewEffect.Get()))
			{
				return FForceFeedbackEffectEvaluation::EvaluateConstant(Constant->EffectData, TimeSeconds);
			}
			break;
		case EHapticPreviewEffectKind::Ramp:
			if (const UForceFeedbackEffectRamp* Ramp = Cast<UForceFeedbackEffectRamp>(PreviewEffect.Get()))
			{
				return FForceFeedbackEffectEvaluation::EvaluateRamp(Ramp->EffectData, TimeSeconds);
			}
			break;
		case EHapticPreviewEffectKind::Condition:
			{
				// Time doesn't drive a condition, so this is the force for the device's current motion, the same at every time.
				return ConditionLiveMotion.IsSet() ? EvaluateConditionMotion(ConditionLiveMotion.GetValue()) : 0.0f;
			}
		case EHapticPreviewEffectKind::LeftRight:
			if (const UForceFeedbackEffectLeftRight* LeftRight = Cast<UForceFeedbackEffectLeftRight>(PreviewEffect.Get()))
			{
				return FForceFeedbackEffectEvaluation::EvaluateLeftRight(LeftRight->EffectData, TimeSeconds);
			}
			break;
	}

	return 0.0f;
}

float SHapticEffectEditor::EvaluateConditionMotion(const FHapticConditionMotion& Motion) const
{
	if (const UForceFeedbackEffectCondition* Condition = Cast<UForceFeedbackEffectCondition>(PreviewEffect.Get()))
	{
		return FForceFeedbackEffectEvaluation::EvaluateConditionMotion(Condition->EffectData, Motion.Position, Motion.Velocity, Motion.Acceleration);
	}

	return 0.0f;
}

void SHapticEffectEditor::UpdateConditionLiveMotion()
{
	// Read from the device here rather than taken from the preview panel, which ticks after this widget and would still hold last frame's position.
	const TOptional<FAxisData> Axis = EffectKind == EHapticPreviewEffectKind::Condition ? GetPreviewAxis() : TOptional<FAxisData>();
	if (!Axis.IsSet())
	{
		ConditionPositionHistory.Reset();
		ConditionLastReadTimestamp = 0;
		ConditionLiveMotion.Reset();
		return;
	}

	// Each position is fitted at the time SDL read it rather than when this widget ticks. The rest of the editor's frame runs between the two, and its few
	// milliseconds of jitter put every sample off the motion's curve by enough to flip the fitted acceleration's sign many times a second.
	// SDL only reports an axis when it changes, so no new read means the axis has held still since the last one, and its position is sampled again now.
	// Without that a stopped axis would keep the motion it last had until its final reads left the window.
	// Raw Value rather than GetValue(), the same as the device preview, to follow where the hardware physically is.
	constexpr double NanosecondsToSeconds = 1.0e-9;
	const double CurrentTime = UJoystickFunctionLibrary::GetTicksNS() * NanosecondsToSeconds;
	const bool bIsNewRead = Axis->Timestamp != ConditionLastReadTimestamp;
	ConditionLastReadTimestamp = Axis->Timestamp;
	ConditionPositionHistory.Emplace(bIsNewRead ? Axis->Timestamp * NanosecondsToSeconds : CurrentTime, Axis->Value);
	ConditionPositionHistory.RemoveAll([CurrentTime](const TPair<double, float>& Sample)
	{
		return CurrentTime - Sample.Key > ConditionMotionFitWindowSeconds;
	});

	// Differencing frame to frame divides the axis' quantisation and its uneven updates by a frame's time twice over, which swamps the acceleration with noise.
	// A least-squares quadratic over the last moment, x(t) = c0 + c1 t + c2 t^2 with t = 0 now, gives the velocity (c1) and acceleration (2 c2) of the motion's overall shape instead.
	double S0 = 0.0, S1 = 0.0, S2 = 0.0, S3 = 0.0, S4 = 0.0;
	double Y0 = 0.0, Y1 = 0.0, Y2 = 0.0;
	float MinPosition = Axis->Value;
	float MaxPosition = Axis->Value;
	for (const TPair<double, float>& Sample : ConditionPositionHistory)
	{
		MinPosition = FMath::Min(MinPosition, Sample.Value);
		MaxPosition = FMath::Max(MaxPosition, Sample.Value);

		const double T = Sample.Key - CurrentTime;
		const double T2 = T * T;
		const double X = Sample.Value;
		S0 += 1.0;
		S1 += T;
		S2 += T2;
		S3 += T2 * T;
		S4 += T2 * T2;
		Y0 += X;
		Y1 += T * X;
		Y2 += T2 * X;
	}

	FHapticConditionMotion Motion;
	Motion.Position = Axis->Value;

	// Solved by Cramer's rule. Too few samples, too short a span (an editor that has only just started ticking), or positions within the axis' noise leave the axis treated as still.
	const double Determinant = S0 * (S2 * S4 - S3 * S3) - S1 * (S1 * S4 - S3 * S2) + S2 * (S1 * S3 - S2 * S2);
	const bool bIsStill = MaxPosition - MinPosition <= ConditionStillPositionSpan;
	if (!bIsStill && ConditionPositionHistory.Num() >= 3 && FMath::Abs(Determinant) > UE_DOUBLE_SMALL_NUMBER)
	{
		const double C1 = (S0 * (Y1 * S4 - S3 * Y2) - Y0 * (S1 * S4 - S3 * S2) + S2 * (S1 * Y2 - Y1 * S2)) / Determinant;
		const double C2 = (S0 * (S2 * Y2 - Y1 * S3) - S1 * (S1 * Y2 - Y1 * S2) + Y0 * (S1 * S3 - S2 * S2)) / Determinant;
		Motion.Velocity = static_cast<float>(C1);
		Motion.Acceleration = static_cast<float>(2.0 * C2);
	}

	ConditionLiveMotion = Motion;
}

TOptional<FAxisData> SHapticEffectEditor::GetPreviewAxis() const
{
	if (!IsValid(GEngine))
	{
		return TOptional<FAxisData>();
	}

	UJoystickSubsystem* JoystickSubsystem = GEngine->GetEngineSubsystem<UJoystickSubsystem>();
	const TSharedPtr<FJoystickInstanceId> SelectedJoystick = GetSelectedJoystick();
	if (!IsValid(JoystickSubsystem) || !SelectedJoystick.IsValid() || !DevicePreviewPanel.IsValid())
	{
		return TOptional<FAxisData>();
	}

	const int32 AxisIndex = DevicePreviewPanel->GetPreviewAxisIndex();
	FJoystickDeviceState JoystickState;
	if (!JoystickSubsystem->GetJoystickState(*SelectedJoystick, JoystickState) || !JoystickState.Axes.IsValidIndex(AxisIndex))
	{
		return TOptional<FAxisData>();
	}

	return JoystickState.Axes[AxisIndex];
}

bool SHapticEffectEditor::IsPreviewDurationInfinite() const
{
	return PreviewEffect.IsValid() && PreviewEffect->GetEffectDuration() == SDL_HAPTIC_INFINITY;
}

TOptional<int64> SHapticEffectEditor::GetPreviewIterations() const
{
	// Looping has no last iteration to count towards. Nor does an authored count of SDL_HAPTIC_INFINITY, which the device runs forever.
	if (bLoop)
	{
		return TOptional<int64>();
	}

	if (!PreviewEffect.IsValid())
	{
		return 0;
	}

	const int64 Iterations = PreviewEffect->GetRunIterations();
	if (Iterations == SDL_HAPTIC_INFINITY)
	{
		return TOptional<int64>();
	}

	return Iterations;
}

int64 SHapticEffectEditor::GetPreviewDelayMilliseconds() const
{
	return PreviewEffect.IsValid() ? PreviewEffect->GetEffectDelay() : 0;
}

// For an infinite effect this is only a nominal length for the curve model's time range, not an end point.
int64 SHapticEffectEditor::GetPreviewDurationMilliseconds() const
{
	if (!PreviewEffect.IsValid())
	{
		return 1000;
	}

	if (IsPreviewDurationInfinite())
	{
		return SHapticEffectGraph::DefaultVisibleWindowMilliseconds;
	}

	return PreviewEffect->GetEffectDuration();
}

int64 SHapticEffectEditor::GetPreviewTimelineMilliseconds() const
{
	// An infinite effect's nominal length already stands in for the whole timeline.
	if (IsPreviewDurationInfinite())
	{
		return GetPreviewDurationMilliseconds();
	}

	return GetPreviewDelayMilliseconds() + GetPreviewDurationMilliseconds();
}

TOptional<EForceFeedbackPeriodicEffectType> SHapticEffectEditor::GetPeriodicWaveform() const
{
	if (const UForceFeedbackEffectPeriodic* Periodic = Cast<UForceFeedbackEffectPeriodic>(PreviewEffect.Get()))
	{
		return Periodic->EffectData.EffectType;
	}

	return TOptional<EForceFeedbackPeriodicEffectType>();
}

void SHapticEffectEditor::SetPeriodicWaveform(const EForceFeedbackPeriodicEffectType WaveformType)
{
	if (UForceFeedbackEffectPeriodic* Periodic = Cast<UForceFeedbackEffectPeriodic>(PreviewEffect.Get()))
	{
		const FScopedTransaction Transaction(LOCTEXT("SetWaveformTransaction", "Set Haptic Effect Waveform"));
		Periodic->Modify();
		Periodic->EffectData.EffectType = WaveformType;
		RecreateDeviceEffect();
	}
}

TOptional<EForceFeedbackConditionEffectType> SHapticEffectEditor::GetConditionType() const
{
	if (const UForceFeedbackEffectCondition* Condition = Cast<UForceFeedbackEffectCondition>(PreviewEffect.Get()))
	{
		return Condition->EffectData.EffectType;
	}

	return TOptional<EForceFeedbackConditionEffectType>();
}

void SHapticEffectEditor::SetConditionType(const EForceFeedbackConditionEffectType ConditionType)
{
	if (UForceFeedbackEffectCondition* Condition = Cast<UForceFeedbackEffectCondition>(PreviewEffect.Get()))
	{
		// Like a waveform change, SDL can't change an uploaded effect's type in place, so the device effect is rebuilt.
		const FScopedTransaction Transaction(LOCTEXT("SetConditionTypeTransaction", "Set Haptic Effect Condition Type"));
		Condition->Modify();
		Condition->EffectData.EffectType = ConditionType;
		RecreateDeviceEffect();
	}
}

#undef LOCTEXT_NAMESPACE
