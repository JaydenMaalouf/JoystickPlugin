// JoystickPlugin is licensed under the MIT License.
// Copyright Jayden Maalouf 2026. All Rights Reserved.

#pragma once

#include "Math/Range.h"
#include "Widgets/SLeafWidget.h"

DECLARE_DELEGATE_OneParam(FOnHapticGraphScrubbed, float /*TimeSeconds*/);

// Slim scrub strip rendered below the Haptic Effect Editor's curve view: draws the playhead and lets
// the user click/drag to seek PlayheadTime. Curve and grid rendering now live in the Curve Editor
// (see HapticCurveModel.h) - this widget only needs to know the *currently visible* time range (the
// Bounds attribute, read from the curve editor's live view) so its mouse-to-time mapping stays
// correct after the user pans or zooms the curve view, rather than always assuming the full effect
// duration is on screen.
class SHapticTimelineScrubber : public SLeafWidget
{
public:
	SLATE_BEGIN_ARGS(SHapticTimelineScrubber)
			: _Bounds(TRange<double>(0.0, 1.0))
			  , _PlayheadTime(-1.0f)
		{
		}

		SLATE_ATTRIBUTE(TRange<double>, Bounds)
		SLATE_ATTRIBUTE(float, PlayheadTime)
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
	TAttribute<float> PlayheadTime;
	FOnHapticGraphScrubbed OnScrubbed;
	bool bIsDragging = false;
};
