// JoystickPlugin is licensed under the MIT License.
// Copyright Jayden Maalouf 2026. All Rights Reserved.

#include "Widgets/HapticEffectGraph.h"

#include "CurveEditor.h"
#include "CurveEditorCommands.h"
#include "ICurveEditorBounds.h"
#include "SCurveEditorPanel.h"
#include "SCurveEditorView.h"
#include "Framework/Commands/UICommandList.h"
#include "Runtime/Launch/Resources/Version.h"
#include "Styling/AppStyle.h"
#include "Widgets/HapticGraphPlayhead.h"
#include "Widgets/SBoxPanel.h"
#include "Widgets/SOverlay.h"
#include "Widgets/Layout/SBorder.h"

#if ENGINE_MAJOR_VERSION == 5 && ENGINE_MINOR_VERSION >= 6
#include "Filters/PromotedFilterContainer.h"
#endif

void SHapticEffectGraph::Construct(const FArguments& InArgs)
{
	CurveEditor = MakeShared<FCurveEditor>();

	const FCurveEditorInitParams CurveEditorInitParams;
	CurveEditor->InitCurveEditor(CurveEditorInitParams);

	TUniquePtr<FStaticCurveEditorBounds> InitialCurveEditorBounds = MakeUnique<FStaticCurveEditorBounds>();
	InitialCurveEditorBounds->SetInputBounds(0.0, DefaultVisibleWindowSeconds);
	CurveEditor->SetBounds(MoveTemp(InitialCurveEditorBounds));

	TUniquePtr<FHapticCurveModel> HapticCurveModel = MakeUnique<FHapticCurveModel>();
	HapticCurveModel->BindEvaluate(InArgs._OnEvaluate);
	HapticCurveModel->SetDurationAttribute(InArgs._DurationSeconds);
	HapticCurveModelID = CurveEditor->AddCurve(MoveTemp(HapticCurveModel));
	CurveEditor->PinCurve(HapticCurveModelID);

	CurveEditorPanel = SNew(SCurveEditorPanel, CurveEditor.ToSharedRef());

	// The haptic curve is read-only and evaluated procedurally, so key selection and curve filters
	// have nothing to act on. The context menu is built by the engine with no hook to drop entries,
	// so rebind those commands on the panel's command list to actions that report themselves as not
	// visible instead.
	if (const TSharedPtr<FUICommandList> PanelCommands = CurveEditorPanel->GetCommands())
	{
		const auto HideCommand = [&PanelCommands](const TSharedPtr<const FUICommandInfo>& Command)
		{
			PanelCommands->UnmapAction(Command);
			PanelCommands->MapAction(Command, FUIAction(
				                         FExecuteAction(),
				                         FCanExecuteAction::CreateLambda([] { return false; }),
				                         FIsActionChecked(),
				                         FIsActionButtonVisible::CreateLambda([] { return false; })));
		};

		HideCommand(FCurveEditorCommands::Get().OpenUserImplementableFilterWindow);

		// Bound on the curve editor's own list rather than the panel's, but the panel's binding is
		// looked up first so this still takes precedence. The curve has no keys to select anyway.
		HideCommand(FCurveEditorCommands::Get().SelectAllKeys);

#if ENGINE_MAJOR_VERSION == 5 && ENGINE_MINOR_VERSION >= 6
		if (const TSharedPtr<UE::CurveEditor::FPromotedFilterContainer> PromotedFilters = CurveEditor->GetToolbarPromotedFilters())
		{
			PromotedFilters->ForEachFilter([&HideCommand](UCurveEditorFilterBase&, const TSharedRef<FUICommandInfo>& Command)
			{
				HideCommand(Command);
			});
		}
#endif
	}

	ChildSlot
	[
		SNew(SBorder)
		.BorderImage(FAppStyle::GetBrush("ToolPanel.GroupBorder"))
		.Padding(4.0f)
		[
			SNew(SVerticalBox)
			+ SVerticalBox::Slot()
			.FillHeight(1.0f)
			[
				SNew(SOverlay)
				+ SOverlay::Slot()
				[
					CurveEditorPanel.ToSharedRef()
				]
				+ SOverlay::Slot()
				[
					SNew(SHapticGraphPlayhead)
					.Visibility(EVisibility::HitTestInvisible)
					.Bounds_Lambda([this] { return GetViewBounds(); })
					.PlayheadTime(InArgs._PlayheadTime)
					.ReferenceGeometry_Lambda([this] { return GetViewGeometry(); })
				]
			]
			+ SVerticalBox::Slot()
			.AutoHeight()
			.Padding(0, 2, 0, 0)
			[
				SNew(SHapticTimelineScrubber)
				.Bounds_Lambda([this] { return GetViewBounds(); })
				.PlayheadTime(InArgs._PlayheadTime)
				.OnScrubbed(InArgs._OnScrubbed)
			]
		]
	];
}

void SHapticEffectGraph::Tick(const FGeometry& AllottedGeometry, const double InCurrentTime, const float InDeltaTime)
{
	SCompoundWidget::Tick(AllottedGeometry, InCurrentTime, InDeltaTime);

	RecenterCurveViewYAxis();
}

void SHapticEffectGraph::ResetTimeWindow() const
{
	if (!CurveEditor.IsValid())
	{
		return;
	}

	TUniquePtr<FStaticCurveEditorBounds> Bounds = MakeUnique<FStaticCurveEditorBounds>();
	Bounds->SetInputBounds(0.0, DefaultVisibleWindowSeconds);
	CurveEditor->SetBounds(MoveTemp(Bounds));
}

void SHapticEffectGraph::PageTimeWindowTo(const double StartSeconds) const
{
	if (!CurveEditor.IsValid())
	{
		return;
	}

	// Moves the window without resizing it, so whatever zoom level the user picked survives.
	const TRange<double> ViewBounds = GetViewBounds();
	CurveEditor->GetBounds().SetInputBounds(StartSeconds, StartSeconds + ViewBounds.Size<double>());
}

TRange<double> SHapticEffectGraph::GetViewBounds() const
{
	if (!CurveEditor.IsValid())
	{
		return TRange<double>(0.0, 1.0);
	}

	double MinBounds, MaxBounds;
	CurveEditor->GetBounds().GetInputBounds(MinBounds, MaxBounds);
	return TRange<double>(MinBounds, MaxBounds);
}

FGeometry SHapticEffectGraph::GetViewGeometry() const
{
	// SCurveEditorPanel::GetViewContainerGeometry() is the outer scroll/container area, which isn't
	// necessarily flush with the individual view's own geometry (e.g. internal padding), and the
	// curve itself is drawn relative to the view's *own* GetCachedGeometry() (see
	// SCurveEditorView::GetViewSpace()) - using the container here was producing a small but visible
	// horizontal offset between the graph overlay's playhead and the actual curve.
	if (CurveEditorPanel.IsValid())
	{
		FCurveModelID CurveModelID = HapticCurveModelID;
		auto It = CurveEditorPanel->FindViews(CurveModelID);
		if (It)
		{
			return It.Value()->GetCachedGeometry();
		}
	}

	return FGeometry();
}

void SHapticEffectGraph::RecenterCurveViewYAxis() const
{
	if (!CurveEditorPanel.IsValid())
	{
		return;
	}

	// Force values are always normalized around zero, so the zero line should never drift off-center
	// vertically - the user can zoom in/out (changing how much of the range is visible) but shouldn't
	// be able to pan vertically. The engine's built-in zoom/pan gestures don't distinguish "zoom" from
	// "pan" cleanly enough to block only the latter, so instead we let them run and, every tick,
	// collapse whatever output bounds they produced back to a symmetric range of the same span - the
	// zoom level survives, any off-center drift doesn't. Since this runs before this frame's Paint,
	// the correction is never visible as a jump.
	FCurveModelID CurveModelID = HapticCurveModelID;
	for (auto It = CurveEditorPanel->FindViews(CurveModelID); It; ++It)
	{
		SCurveEditorView& View = It.Value().Get();
		const double Extent = (View.GetOutputMax() - View.GetOutputMin()) / 2.0;
		if (!FMath::IsNearlyEqual(View.GetOutputMin(), -Extent, 1e-6) || !FMath::IsNearlyEqual(View.GetOutputMax(), Extent, 1e-6))
		{
			View.SetOutputBounds(-Extent, Extent);
		}
	}
}
