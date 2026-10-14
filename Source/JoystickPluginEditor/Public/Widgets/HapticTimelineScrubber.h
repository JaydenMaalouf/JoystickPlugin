// JoystickPlugin is licensed under the MIT License.
// Copyright Jayden Maalouf 2026. All Rights Reserved.

#pragma once

#include "Math/Range.h"
#include "Widgets/SLeafWidget.h"

DECLARE_DELEGATE_OneParam(FOnHapticGraphScrubbed, float /*TimeMilliseconds*/);

// Slim scrub strip below the curve view: draws the playhead and seeks it within the currently visible time range (Bounds, in milliseconds).
class SHapticTimelineScrubber : public SLeafWidget
{
public:
	SLATE_BEGIN_ARGS(SHapticTimelineScrubber)
			: _Bounds(TRange<double>(0.0, 1000.0))
			  , _PlayheadTimeMilliseconds(-1)
		{
		}

		SLATE_ATTRIBUTE(TRange<double>, Bounds)
		SLATE_ATTRIBUTE(int64, PlayheadTimeMilliseconds)
		SLATE_EVENT(FOnHapticGraphScrubbed, OnScrubbed)
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

	virtual FReply OnMouseButtonDown(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent) override;
	virtual FReply OnMouseMove(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent) override;
	virtual FReply OnMouseButtonUp(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent) override;

private:
	void ScrubToLocalPosition(const FGeometry& MyGeometry, const FVector2D& ScreenSpacePosition);

	void PaintBackground(const FGeometry& AllottedGeometry, FSlateWindowElementList& OutDrawElements, int32 LayerId) const;
	void PaintPlayhead(const FGeometry& AllottedGeometry, FSlateWindowElementList& OutDrawElements, int32 LayerId) const;

	TAttribute<TRange<double>> Bounds;
	TAttribute<int64> PlayheadTimeMilliseconds;
	FOnHapticGraphScrubbed OnScrubbed;
	bool bIsDragging = false;
};
