// JoystickPlugin is licensed under the MIT License.
// Copyright Jayden Maalouf 2026. All Rights Reserved.

#include "Menus/HapticEffectEditor.h"

#include "DetailsViewArgs.h"
#include "Editor.h"
#include "IDetailsView.h"
#include "PropertyEditorDelegates.h"
#include "PropertyEditorModule.h"
#include "ScopedTransaction.h"
#include "Fonts/FontMeasure.h"
#include "ForceFeedback/ForceFeedbackEffectEvaluation.h"
#include "ForceFeedback/Effects/ForceFeedbackEffectBase.h"
#include "ForceFeedback/Effects/ForceFeedbackEffectConstant.h"
#include "ForceFeedback/Effects/ForceFeedbackEffectLeftRight.h"
#include "ForceFeedback/Effects/ForceFeedbackEffectPeriodic.h"
#include "ForceFeedback/Effects/ForceFeedbackEffectRamp.h"
#include "Framework/Application/SlateApplication.h"
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
#include "Widgets/Layout/SBox.h"
#include "Widgets/Layout/SSpacer.h"
#include "Widgets/Layout/SSplitter.h"
#include "Widgets/Text/STextBlock.h"

#define LOCTEXT_NAMESPACE "HapticEffectEditor"

// Wraps a toolbar control in a bordered "pill" container.
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
				.DurationMilliseconds_Lambda([this] { return GetPreviewTimelineMilliseconds(); })
				.PlayheadTimeMilliseconds_Lambda([this] { return PlayheadTimeMilliseconds; })
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
	// Lets a click on the panel take keyboard focus, so the spacebar shortcut works without clicking the graph first.
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

TSharedRef<SWidget> SHapticEffectEditor::BuildStatusChip()
{
	const FSlateFontInfo& Font = FAppStyle::Get().GetWidgetStyle<FTextBlockStyle>("NormalText").Font;
	const TSharedRef<FSlateFontMeasure> FontMeasure = FSlateApplication::Get().GetRenderer()->GetFontMeasureService();

	// Each readout sits in a box sized for its widest expected value, so the chip doesn't resize as the numbers change during playback.
	auto MakeReadout = [&Font, &FontMeasure](const TCHAR* WidestValue, const TAttribute<FText>& Text)
	{
		return SNew(SBox)
			.MinDesiredWidth(FontMeasure->Measure(WidestValue, Font).X)
			[
				SNew(STextBlock)
				.Text(Text)
				.TextStyle(FAppStyle::Get(), "NormalText")
			];
	};

	return SNew(SHorizontalBox)
		+ SHorizontalBox::Slot()
		.AutoWidth()
		.Padding(0, 0, 12, 0)
		[
			MakeReadout(TEXT("t = 000.00s"), TAttribute<FText>::CreateLambda([this]
			{
				return FText::FromString(FString::Printf(TEXT("t = %.2fs"), PlayheadTimeMilliseconds / 1000.0));
			}))
		]
		+ SHorizontalBox::Slot()
		.AutoWidth()
		.Padding(0, 0, 12, 0)
		[
			MakeReadout(TEXT("elapsed = 0000.00s"), TAttribute<FText>::CreateLambda([this]
			{
				return FText::FromString(FString::Printf(TEXT("elapsed = %.2fs"), ElapsedTimeMilliseconds / 1000.0));
			}))
		]
		+ SHorizontalBox::Slot()
		.AutoWidth()
		.Padding(0, 0, 12, 0)
		[
			MakeReadout(TEXT("F = -0.00"), TAttribute<FText>::CreateLambda([this]
			{
				return FText::FromString(FString::Printf(TEXT("F = %.2f"), EvaluatePreviewEffect(static_cast<float>(PlayheadTimeMilliseconds))));
			}))
		]
		+ SHorizontalBox::Slot()
		.AutoWidth()
		[
			MakeReadout(TEXT("i = 000/000"), TAttribute<FText>::CreateLambda([this]
			{
				// Looping has no last iteration to count towards, so it shows an infinity sign instead. So does an authored count of SDL_HAPTIC_INFINITY, which the device runs forever.
				const int64 Iterations = PreviewEffect.IsValid() ? PreviewEffect->GetRunIterations() : 0;
				const FString IterationCount = bLoop || Iterations == SDL_HAPTIC_INFINITY ? FString::Chr(static_cast<TCHAR>(0x221E)) : FString::Printf(TEXT("%lld"), Iterations);

				return FText::FromString(FString::Printf(TEXT("i = %lld/%s"), CurrentIteration, *IterationCount));
			}))
		];
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
	DeviceEffect->SetTickableInEditor(true);

	if (const TSharedPtr<FJoystickInstanceId> SelectedJoystick = GetSelectedJoystick())
	{
		DeviceEffect->SetInstanceId(*SelectedJoystick);
	}

	if (DetailsView.IsValid())
	{
		DetailsView->SetObject(PreviewEffect.Get());
	}

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
	// The preview plays the authored number of iterations unless looping.
	DeviceEffect->InfiniteIterations = bLoop;
	CurrentIteration = 1;

	return DeviceEffect->StartEffect();
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
	if (!bSuccess || !PreviewEffect.IsValid())
	{
		return;
	}

	// The changed property isn't known here, so resync Loop and restart a playing effect.
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

#undef LOCTEXT_NAMESPACE
