// JoystickPlugin is licensed under the MIT License.
// Copyright Jayden Maalouf 2026. All Rights Reserved.

#pragma once

#include "Widgets/SLeafWidget.h"

DECLARE_DELEGATE_RetVal_OneParam(float, FOnEvaluateHapticEffect, float /*TimeSeconds*/);
DECLARE_DELEGATE_OneParam(FOnHapticGraphScrubbed, float /*TimeSeconds*/);

// Oscilloscope-style preview of an effect's output over time, sampled through the OnEvaluateEffect delegate.
class SHapticWaveformGraph : public SLeafWidget
{
public:
	SLATE_BEGIN_ARGS(SHapticWaveformGraph)
			: _DurationSeconds(1.0f)
			  , _PlayheadTime(-1.0f)
		{
		}

		SLATE_ATTRIBUTE(float, DurationSeconds)
		SLATE_ATTRIBUTE(float, PlayheadTime)
		SLATE_EVENT(FOnEvaluateHapticEffect, OnEvaluateEffect)
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

	void PaintGrid(const FGeometry& AllottedGeometry, FSlateWindowElementList& OutDrawElements, int32 LayerId, float Duration) const;
	void PaintCurve(const FGeometry& AllottedGeometry, FSlateWindowElementList& OutDrawElements, int32 LayerId, float Duration) const;
	void PaintPlayhead(const FGeometry& AllottedGeometry, FSlateWindowElementList& OutDrawElements, int32 LayerId, float Duration) const;

	TAttribute<float> DurationSeconds;
	TAttribute<float> PlayheadTime;
	FOnEvaluateHapticEffect OnEvaluateEffect;
	FOnHapticGraphScrubbed OnScrubbed;
	bool bIsDragging = false;
};
