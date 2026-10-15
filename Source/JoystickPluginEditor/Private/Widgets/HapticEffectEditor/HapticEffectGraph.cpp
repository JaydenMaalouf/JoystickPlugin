// JoystickPlugin is licensed under the MIT License.
// Copyright Jayden Maalouf 2026. All Rights Reserved.

#include "Widgets/HapticEffectEditor/HapticEffectGraph.h"

#include "CurveEditor.h"
#include "CurveEditorCommands.h"
#include "ICurveEditorBounds.h"
#include "SCurveEditorPanel.h"
#include "SCurveEditorView.h"
#include "Framework/Commands/UICommandList.h"
#include "Runtime/Launch/Resources/Version.h"
#include "Styling/AppStyle.h"
#include "Widgets/HapticEffectEditor/HapticGraphPlayhead.h"
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
	InitialCurveEditorBounds->SetInputBounds(0.0, DefaultVisibleWindowMilliseconds / 1000.0);
	CurveEditor->SetBounds(MoveTemp(InitialCurveEditorBounds));

	TUniquePtr<FHapticCurveModel> HapticCurveModel = MakeUnique<FHapticCurveModel>();
	HapticCurveModel->BindEvaluate(InArgs._OnEvaluate);
	HapticCurveModel->SetDurationAttribute(InArgs._DurationMilliseconds);
	HapticCurveModelID = CurveEditor->AddCurve(MoveTemp(HapticCurveModel));
	CurveEditor->PinCurve(HapticCurveModelID);

	CurveEditorPanel = SNew(SCurveEditorPanel, CurveEditor.ToSharedRef());

	// The curve is read-only, so hide the key selection and filter commands the engine's context menu would otherwise show.
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

		// Bound on the curve editor's own list, but the panel's binding is looked up first.
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
					.PlayheadTimeMilliseconds(InArgs._PlayheadTimeMilliseconds)
					.ReferenceGeometry_Lambda([this] { return GetViewGeometry(); })
				]
			]
			+ SVerticalBox::Slot()
			.AutoHeight()
			.Padding(0, 2, 0, 0)
			[
				SNew(SHapticTimelineScrubber)
				.Bounds_Lambda([this] { return GetViewBounds(); })
				.PlayheadTimeMilliseconds(InArgs._PlayheadTimeMilliseconds)
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
	Bounds->SetInputBounds(0.0, DefaultVisibleWindowMilliseconds / 1000.0);
	CurveEditor->SetBounds(MoveTemp(Bounds));
}

void SHapticEffectGraph::PageTimeWindowTo(const double StartMilliseconds) const
{
	if (!CurveEditor.IsValid())
	{
		return;
	}

	// Moves the window without resizing it, so whatever zoom level the user picked survives.
	const TRange<double> ViewBounds = GetViewBounds();
	CurveEditor->GetBounds().SetInputBounds(StartMilliseconds / 1000.0, (StartMilliseconds + ViewBounds.Size<double>()) / 1000.0);
}

TRange<double> SHapticEffectGraph::GetViewBounds() const
{
	if (!CurveEditor.IsValid())
	{
		return TRange<double>(0.0, 1000.0);
	}

	// The Curve Editor keeps its input bounds in seconds.
	double MinBounds, MaxBounds;
	CurveEditor->GetBounds().GetInputBounds(MinBounds, MaxBounds);
	return TRange<double>(MinBounds * 1000.0, MaxBounds * 1000.0);
}

FGeometry SHapticEffectGraph::GetViewGeometry() const
{
	// Use the view's own cached geometry, as the panel's container geometry isn't flush with where the curve is drawn.
	if (CurveEditorPanel.IsValid())
	{
		FCurveModelID CurveModelID = HapticCurveModelID;
		if (const auto It = CurveEditorPanel->FindViews(CurveModelID))
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

	// Re-centre the output bounds on zero every tick, so the user can zoom vertically but never pan.
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
