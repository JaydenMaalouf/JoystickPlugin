// JoystickPlugin is licensed under the MIT License.
// Copyright Jayden Maalouf 2026. All Rights Reserved.

#include "Menus/HapticEffectEditor.h"

#include "DetailsViewArgs.h"
#include "Editor.h"
#include "IDetailsView.h"
#include "PropertyEditorDelegates.h"
#include "PropertyEditorModule.h"
#include "ScopedTransaction.h"
#include "ForceFeedback/ForceFeedbackEffectEvaluation.h"
#include "ForceFeedback/Effects/ForceFeedbackEffectBase.h"
#include "ForceFeedback/Effects/ForceFeedbackEffectConstant.h"
#include "ForceFeedback/Effects/ForceFeedbackEffectLeftRight.h"
#include "ForceFeedback/Effects/ForceFeedbackEffectPeriodic.h"
#include "ForceFeedback/Effects/ForceFeedbackEffectRamp.h"
#include "Menus/HapticEffectEditorProperties.h"
#include "Misc/ITransaction.h"
#include "Misc/TransactionObjectEvent.h"
#include "Modules/ModuleManager.h"
#include "Styling/AppStyle.h"
#include "UObject/Package.h"
#include "Widgets/HapticDevicePicker.h"
#include "Widgets/HapticDevicePreviewPanel.h"
#include "Widgets/HapticEffectGraph.h"
#include "Widgets/HapticEffectTypeSelector.h"
#include "Widgets/HapticTransportBar.h"
#include "Widgets/HapticWaveformSelector.h"
#include "Widgets/SBoxPanel.h"
#include "Widgets/Layout/SBorder.h"
#include "Widgets/Layout/SSpacer.h"
#include "Widgets/Layout/SSplitter.h"
#include "Widgets/Text/STextBlock.h"

#define LOCTEXT_NAMESPACE "HapticEffectEditor"

// Groups a toolbar control into the mockup's "pill container" look: a bordered box distinct from
// its neighbours, replacing what used to be a row of identically-styled flat buttons.
static TSharedRef<SWidget> WrapInPill(const TSharedRef<SWidget>& Content)
{
	return SNew(SBorder)
		.BorderImage(FAppStyle::GetBrush("ToolPanel.GroupBorder"))
		.Padding(FMargin(8.0f, 6.0f))
		[
			Content
		];
}

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
				WrapInPill(DevicePicker.ToSharedRef())
			]
			+ SHorizontalBox::Slot()
			.AutoWidth()
			.VAlign(VAlign_Center)
			.Padding(0, 0, 8, 0)
			[
				WrapInPill(
					SNew(SHapticTransportBar)
					.HasDevice_Lambda([this] { return GetSelectedJoystick().IsValid(); })
					.IsPlaying_Lambda([this] { return bIsPlaying; })
					.IsLooping_Lambda([this] { return bLoop; })
					.OnPlayClicked(this, &SHapticEffectEditor::OnPlayClicked)
					.OnStopClicked(this, &SHapticEffectEditor::OnStopClicked)
					.OnLoopClicked(this, &SHapticEffectEditor::OnLoopClicked))
			]
			+ SHorizontalBox::Slot()
			.AutoWidth()
			.VAlign(VAlign_Center)
			.Padding(0, 0, 8, 0)
			[
				WrapInPill(
					SNew(SHapticEffectTypeSelector)
					.EffectKind_Lambda([this] { return EffectKind; })
					.OnEffectKindSelected(this, &SHapticEffectEditor::SetEffectKind))
			]
			+ SHorizontalBox::Slot()
			.AutoWidth()
			.VAlign(VAlign_Center)
			.Padding(0, 0, 8, 0)
			[
				SNew(SBorder)
				.BorderImage(FAppStyle::GetBrush("ToolPanel.GroupBorder"))
				.Padding(FMargin(8.0f, 6.0f))
				.Visibility_Lambda([this]
				{
					return EffectKind == EHapticPreviewEffectKind::Periodic ? EVisibility::Visible : EVisibility::Collapsed;
				})
				[
					SNew(SHapticWaveformSelector)
					.SelectedWaveform_Lambda([this] { return GetPeriodicWaveform(); })
					.OnWaveformSelected(this, &SHapticEffectEditor::SetPeriodicWaveform)
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
				WrapInPill(BuildAssetBar())
			]
			+ SHorizontalBox::Slot()
			.AutoWidth()
			.VAlign(VAlign_Center)
			[
				WrapInPill(BuildStatusChip())
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
				SAssignNew(EffectGraph, SHapticEffectGraph)
				.OnEvaluate(FOnEvaluateHapticEffect::CreateSP(this, &SHapticEffectEditor::EvaluatePreviewEffect))
				.DurationSeconds_Lambda([this] { return GetPreviewDurationSeconds(); })
				.PlayheadTime_Lambda([this] { return PlayheadTime; })
				.OnScrubbed(this, &SHapticEffectEditor::OnGraphScrubbed)
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

	if (!bIsPlaying || !PreviewEffect.IsValid())
	{
		return;
	}

	const float PreviousPlayheadTime = PlayheadTime;
	PlayheadTime += InDeltaTime;

	// An infinite effect has no end to stop or wrap at - it runs on the device until Stop is pressed,
	// so the playhead just keeps counting and the view pages along with it. Paging only as the
	// playhead crosses the right edge leaves alone a view the user has deliberately panned elsewhere.
	if (IsPreviewDurationInfinite())
	{
		const TRange<double> ViewBounds = EffectGraph->GetViewBounds();
		if (PreviousPlayheadTime <= ViewBounds.GetUpperBoundValue() && PlayheadTime > ViewBounds.GetUpperBoundValue())
		{
			EffectGraph->PageTimeWindowTo(ViewBounds.GetUpperBoundValue());
		}

		return;
	}

	const float Duration = GetPreviewDurationSeconds();
	if (PlayheadTime >= Duration)
	{
		if (bLoop)
		{
			PlayheadTime = FMath::Fmod(PlayheadTime, Duration);
		}
		else
		{
			PlayheadTime = Duration;
			bIsPlaying = false;
		}
	}
}

bool SHapticEffectEditor::SupportsKeyboardFocus() const
{
	// Lets a click on the panel's background or one of its (non-focusable) toolbar buttons land
	// keyboard focus here, so the spacebar shortcut works without first clicking into the graph.
	return true;
}

FReply SHapticEffectEditor::OnKeyDown(const FGeometry& MyGeometry, const FKeyEvent& InKeyEvent)
{
	// Key events bubble up from whichever child has focus, so this only sees a spacebar press the
	// focused widget didn't want - a text field being typed into keeps its spaces.
	// Ctrl+S saves the effect, as it does in the engine's own asset editors: back to the opened asset,
	// or as a new one if there isn't one. It is always swallowed, changed or not, so it never falls
	// through to saving the level instead.
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

	return SCompoundWidget::OnKeyDown(MyGeometry, InKeyEvent);
}

TSharedRef<SWidget> SHapticEffectEditor::BuildStatusChip()
{
	return SNew(STextBlock)
		.Text_Lambda([this]
		{
			const float ForceValue = EvaluatePreviewEffect(PlayheadTime);
			return FText::FromString(FString::Printf(TEXT("t = %.2fs   F = %.2f"), PlayheadTime, ForceValue));
		})
		.TextStyle(FAppStyle::Get(), "NormalText");
}

TSharedPtr<FJoystickInstanceId> SHapticEffectEditor::GetSelectedJoystick() const
{
	return DevicePicker.IsValid() ? DevicePicker->GetSelectedJoystick() : nullptr;
}

void SHapticEffectEditor::OnDeviceChanged()
{
	// Carries the edits over to an effect built against the newly selected device.
	RebuildPreviewEffect(PreviewEffect.Get());
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
	// InitialValues may be the outgoing preview effect itself (a device change carrying the edits
	// over), so it is kept alive until its values have been copied out.
	const TStrongObjectPtr<UForceFeedbackEffectBase> PreviousEffect = PreviewEffect;
	DestroyPreviewEffect();

	// Always one of the plugin's own effect classes, even when InitialValues is a subclass of one: a
	// subclass's tick logic expects to be running on a component in a game world, not in this panel.
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
	DeviceEffect->SetTickableInEditor(true);

	if (const TSharedPtr<FJoystickInstanceId> SelectedJoystick = GetSelectedJoystick())
	{
		DeviceEffect->SetInstanceId(*SelectedJoystick);
	}

	if (DetailsView.IsValid())
	{
		DetailsView->SetObject(PreviewEffect.Get());
	}

	// New effect/kind/device: start the view back at a consistent [0, DefaultVisibleWindowSeconds]
	// window rather than leaving it wherever the previous effect's pan/zoom happened to land.
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
	PlayheadTime = 0.0f;
}

void SHapticEffectEditor::OnPropertiesChanged(const FPropertyChangedEvent& PropertyChangedEvent)
{
	if (!PreviewEffect.IsValid())
	{
		return;
	}

	// SDL can't change an already-uploaded effect's fundamental type in place (Sine and Triangle
	// are distinct SDL_HAPTIC_* constants even though they share the same struct shape), so an
	// EffectType edit has to tear down and reinitialise the device effect rather than update it.
	static const FName EffectTypePropertyName(GET_MEMBER_NAME_CHECKED(FForceFeedbackEffectPeriodicData, EffectType));
	if (PropertyChangedEvent.GetPropertyName() == EffectTypePropertyName)
	{
		RecreateDeviceEffect();
		return;
	}

	// InfiniteIterations and the transport bar's Loop button are the same setting, so an edit here
	// has to carry over to the button. The iteration count only reaches the device when an effect is
	// started, so one that is already playing is restarted to pick it up.
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
	if (!DeviceEffect.IsValid() || !DeviceEffect->IsInitialised)
	{
		return;
	}

	const bool bWasPlaying = bIsPlaying;

	DeviceEffect->StopEffect();
	DeviceEffect->DestroyEffect();

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

void SHapticEffectEditor::SyncDeviceEffect()
{
	if (PreviewEffect.IsValid() && DeviceEffect.IsValid())
	{
		FHapticEffectEditorProperties::CopyAuthoredProperties(PreviewEffect.Get(), DeviceEffect.Get(), PreviewEffect->GetClass());
	}
}

bool SHapticEffectEditor::StartPreviewEffect()
{
	// The preview plays a single pass unless looping, whatever iteration count has been authored.
	// StartEffect also overwrites Iterations itself when InfiniteIterations is set - both land on the
	// device effect only, so neither shows up in the details panel or a saved or exported asset.
	DeviceEffect->InfiniteIterations = bLoop;
	if (!bLoop)
	{
		DeviceEffect->Iterations = 1;
	}

	return DeviceEffect->StartEffect();
}

// The authored effect can't be told apart from the rest of an undone transaction's contents by the
// base class, and without this any undo in the editor - a moved actor, say - would count as a change
// to the effect.
bool SHapticEffectEditor::MatchesContext(const FTransactionContext& InContext, const TArray<TPair<UObject*, FTransactionObjectEvent>>& TransactionObjectContexts) const
{
	return TransactionObjectContexts.ContainsByPredicate([this](const TPair<UObject*, FTransactionObjectEvent>& TransactionObjectContext)
	{
		return TransactionObjectContext.Key == PreviewEffect.Get();
	});
}

void SHapticEffectEditor::PostUndo(const bool bSuccess)
{
	if (!bSuccess || !PreviewEffect.IsValid())
	{
		return;
	}

	// Which property the transaction touched isn't known here, so this covers everything a details
	// panel edit can: the Loop button's mirror of InfiniteIterations, and a playing effect is started
	// over, which uploads whatever the values now are.
	bLoop = PreviewEffect->InfiniteIterations;

	if (bIsPlaying)
	{
		OnPlayClicked();
	}
}

void SHapticEffectEditor::PostRedo(const bool bSuccess)
{
	PostUndo(bSuccess);
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

	// Always upload a fresh effect rather than reusing an existing SDL effect handle via
	// UpdateEffect(): SDL only documents "dynamic" in-place updates as reliable for non-direction
	// changes, so a full recreate is the surest way to guarantee the current EffectData - envelope
	// included - actually reaches the device on Play.
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

	PlayheadTime = 0.0f;
	bIsPlaying = StartPreviewEffect();

	// A previous infinite playback may have paged the view well past zero, which would leave the
	// restarted playhead pinned off-screen until it caught up.
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

void SHapticEffectEditor::OnGraphScrubbed(const float TimeSeconds)
{
	// Scrubbing is a graph-only preview seek; it does not push an instantaneous force to the
	// device since SDL_Haptic has no API for sampling an uploaded effect at an arbitrary time.
	// Scrubbing ends playback, so the device effect has to be stopped along with it: a finite effect
	// would run out by itself, but an infinite one would carry on with the Stop button disabled.
	if (bIsPlaying)
	{
		OnStopClicked();
	}

	// An infinite effect has no end to clamp to, so any point at or after its start is valid.
	PlayheadTime = IsPreviewDurationInfinite() ? FMath::Max(TimeSeconds, 0.0f) : FMath::Clamp(TimeSeconds, 0.0f, GetPreviewDurationSeconds());
}

float SHapticEffectEditor::EvaluatePreviewEffect(const float TimeSeconds) const
{
	if (!PreviewEffect.IsValid())
	{
		return 0.0f;
	}

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
		case EHapticPreviewEffectKind::LeftRight:
			if (const UForceFeedbackEffectLeftRight* LeftRight = Cast<UForceFeedbackEffectLeftRight>(PreviewEffect.Get()))
			{
				return FForceFeedbackEffectEvaluation::EvaluateLeftRight(LeftRight->EffectData, TimeSeconds);
			}
			break;
	}

	return 0.0f;
}

bool SHapticEffectEditor::IsPreviewDurationInfinite() const
{
	return PreviewEffect.IsValid() && PreviewEffect->GetEffectDuration() == SDL_HAPTIC_INFINITY;
}

// For an infinite effect this is only a nominal length for the curve model's time range (what
// zoom-to-fit frames) - playback and scrubbing check IsPreviewDurationInfinite() instead of treating
// it as an end point.
float SHapticEffectEditor::GetPreviewDurationSeconds() const
{
	if (!PreviewEffect.IsValid())
	{
		return 1.0f;
	}

	if (IsPreviewDurationInfinite())
	{
		return SHapticEffectGraph::DefaultVisibleWindowSeconds;
	}

	return FMath::Max(PreviewEffect->GetEffectDuration() / 1000.0f, 0.05f);
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

#undef LOCTEXT_NAMESPACE
