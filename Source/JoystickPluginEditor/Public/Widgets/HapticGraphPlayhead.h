// JoystickPlugin is licensed under the MIT License.
// Copyright Jayden Maalouf 2026. All Rights Reserved.

#pragma once

#include "Math/Range.h"
#include "Widgets/SLeafWidget.h"

// Hit-test-invisible playhead line overlaid on the curve editor panel. ReferenceGeometry is the region the line should span.
// Bounds is the visible time range in milliseconds.
class SHapticGraphPlayhead : public SLeafWidget
{
public:
	SLATE_BEGIN_ARGS(SHapticGraphPlayhead)
			: _Bounds(TRange<double>(0.0, 1000.0))
			  , _PlayheadTimeMilliseconds(-1)
		{
		}

		SLATE_ATTRIBUTE(TRange<double>, Bounds)
		SLATE_ATTRIBUTE(int64, PlayheadTimeMilliseconds)
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
	TAttribute<int64> PlayheadTimeMilliseconds;
	TAttribute<FGeometry> ReferenceGeometry;
};
