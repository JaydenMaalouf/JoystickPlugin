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

// The Haptic Effect Editor's graph: a read-only Curve Editor panel with a playhead line over it and a scrub strip below.
// Every time this widget takes or returns is in milliseconds. Only the Curve Editor's own axis is in seconds.
class SHapticEffectGraph : public SCompoundWidget
{
public:
	// The view always opens on this many milliseconds, so effects render at a consistent scale regardless of Duration.
	static constexpr int64 DefaultVisibleWindowMilliseconds = 2000;

	SLATE_BEGIN_ARGS(SHapticEffectGraph)
			: _DurationMilliseconds(1000)
			  , _PlayheadTimeMilliseconds(-1)
		{
		}

		SLATE_EVENT(FOnEvaluateHapticEffect, OnEvaluate)
		SLATE_ATTRIBUTE(int64, DurationMilliseconds)
		SLATE_ATTRIBUTE(int64, PlayheadTimeMilliseconds)
		SLATE_EVENT(FOnHapticGraphScrubbed, OnScrubbed)
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs);

	virtual void Tick(const FGeometry& AllottedGeometry, double InCurrentTime, float InDeltaTime) override;

	void ResetTimeWindow() const;
	void PageTimeWindowTo(double StartMilliseconds) const;
	TRange<double> GetViewBounds() const;

private:
	void RecenterCurveViewYAxis() const;
	FGeometry GetViewGeometry() const;

	TSharedPtr<FCurveEditor> CurveEditor;
	TSharedPtr<SCurveEditorPanel> CurveEditorPanel;
	FCurveModelID HapticCurveModelID = FCurveModelID::Unique();
};
