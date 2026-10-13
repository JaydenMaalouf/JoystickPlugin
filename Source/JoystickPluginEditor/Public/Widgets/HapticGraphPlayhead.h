// JoystickPlugin is licensed under the MIT License.
// Copyright Jayden Maalouf 2026. All Rights Reserved.

#pragma once

#include "Math/Range.h"
#include "Widgets/SLeafWidget.h"

// Purely visual vertical playhead line drawn as a sibling overlay on top of the Haptic Effect
// Editor's SCurveEditorPanel, so the current preview/scrub position is visible directly on the
// waveform rather than only on the SHapticTimelineScrubber strip below it. Always
// HitTestInvisible - it must never intercept the curve editor's own pan/zoom/selection gestures
// underneath it.
//
// ReferenceGeometry must be the geometry of the region the line should span - in practice
// SCurveEditorPanel::GetViewContainerGeometry(), not this widget's own AllottedGeometry, since the
// panel reserves internal space (toolbar, tree column) that this overlay sits on top of as a sibling
// spanning the *whole* panel. The line is placed by converting through absolute screen space, so it
// stays aligned with the curve underneath regardless of that internal layout.
class SHapticGraphPlayhead : public SLeafWidget
{
public:
	SLATE_BEGIN_ARGS(SHapticGraphPlayhead)
			: _Bounds(TRange<double>(0.0, 1.0))
			  , _PlayheadTime(-1.0f)
		{
		}

		SLATE_ATTRIBUTE(TRange<double>, Bounds)
		SLATE_ATTRIBUTE(float, PlayheadTime)
		SLATE_ATTRIBUTE(FGeometry, ReferenceGeometry)
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs);

	virtual int32 OnPaint(
		const FPaintArgs& Args,
		const FGeometry& AllottedGeometry,
		const FSlateRect& MyCullingRect,
		FSlateWindowElementList& OutDrawElements,
		int32 LayerId,
		const FWidgetStyle& InWidgetStyle,
		bool bParentEnabled
	) const override;

	virtual FVector2D ComputeDesiredSize(float) const override;

private:
	TAttribute<TRange<double>> Bounds;
	TAttribute<float> PlayheadTime;
	TAttribute<FGeometry> ReferenceGeometry;
};
