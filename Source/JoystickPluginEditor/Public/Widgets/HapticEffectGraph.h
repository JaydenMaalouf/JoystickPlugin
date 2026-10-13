// JoystickPlugin is licensed under the MIT License.
// Copyright Jayden Maalouf 2026. All Rights Reserved.

#pragma once

#include "CurveEditorTypes.h"
#include "Math/Range.h"
#include "Widgets/HapticCurveModel.h"
#include "Widgets/HapticTimelineScrubber.h"
#include "Widgets/SCompoundWidget.h"

class FCurveEditor;
class SCurveEditorPanel;

// The Haptic Effect Editor's oscilloscope-style graph: a read-only Curve Editor panel drawing the
// effect's shape (see HapticCurveModel.h), with a playhead line over it and a scrub strip below.
// The curve is evaluated through OnEvaluate rather than held here, so it always shows whatever the
// owner's effect currently is.
class SHapticEffectGraph : public SCompoundWidget
{
public:
	// The curve view always opens on this many seconds of the effect regardless of its Duration, so
	// effects render at a consistent time-per-pixel scale instead of being auto-fit/squashed to the
	// panel width - the user can pan/zoom from there to see the rest of a longer effect.
	static constexpr double DefaultVisibleWindowSeconds = 2.0;

	SLATE_BEGIN_ARGS(SHapticEffectGraph)
			: _DurationSeconds(1.0f)
			  , _PlayheadTime(-1.0f)
		{
		}

		SLATE_EVENT(FOnEvaluateHapticEffect, OnEvaluate)
		SLATE_ATTRIBUTE(float, DurationSeconds)
		SLATE_ATTRIBUTE(float, PlayheadTime)
		SLATE_EVENT(FOnHapticGraphScrubbed, OnScrubbed)
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs);

	virtual void Tick(const FGeometry& AllottedGeometry, double InCurrentTime, float InDeltaTime) override;

	void ResetTimeWindow() const;
	void PageTimeWindowTo(double StartSeconds) const;
	TRange<double> GetViewBounds() const;

private:
	void RecenterCurveViewYAxis() const;
	FGeometry GetViewGeometry() const;

	TSharedPtr<FCurveEditor> CurveEditor;
	TSharedPtr<SCurveEditorPanel> CurveEditorPanel;
	FCurveModelID HapticCurveModelID = FCurveModelID::Unique();
};
