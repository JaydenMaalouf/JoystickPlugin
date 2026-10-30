// JoystickPlugin is licensed under the MIT License.
// Copyright Jayden Maalouf 2026. All Rights Reserved.

#pragma once

#include "Widgets/SLeafWidget.h"

class SCircleWidget : public SLeafWidget
{
public:
	SLATE_BEGIN_ARGS(SCircleWidget) :
			_Radius(10.0f),
			_Color(FLinearColor::Black),
			_Filled(false),
			_LineThickness(1.0f)
		{
		}

		SLATE_ARGUMENT(float, Radius)
		SLATE_ARGUMENT(FLinearColor, Color)
		SLATE_ARGUMENT(bool, Filled)
		SLATE_ARGUMENT(float, LineThickness)
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs);

	void SetColor(const TAttribute<FLinearColor>& InColor);

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
	void DrawCircle(FSlateWindowElementList& OutDrawElements, const FGeometry& InGeometry, const int32 InLayerId, const FVector2D InCenter, const float InRadius, const int32 InNumSegments, const FLinearColor InColor, const bool InFilled,
	                const float InLineThickness) const;

	TAttribute<float> Radius;
	TAttribute<FLinearColor> Color;
	TAttribute<bool> Filled;
	TAttribute<float> LineThickness;
};
