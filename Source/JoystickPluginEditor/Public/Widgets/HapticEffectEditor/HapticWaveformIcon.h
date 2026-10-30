// JoystickPlugin is licensed under the MIT License.
// Copyright Jayden Maalouf 2026. All Rights Reserved.

#pragma once

#include "Widgets/SLeafWidget.h"
#include "ForceFeedback/Types/ForceFeedbackPeriodicEffectType.h"

// Small fixed-shape glyph showing two cycles of a periodic waveform type. An icon, not a data preview.
class SHapticWaveformIcon : public SLeafWidget
{
public:
	SLATE_BEGIN_ARGS(SHapticWaveformIcon)
			: _WaveformType(EForceFeedbackPeriodicEffectType::Sine)
			  , _Color(FLinearColor::White)
		{
		}

		SLATE_ATTRIBUTE(EForceFeedbackPeriodicEffectType, WaveformType)
		SLATE_ATTRIBUTE(FLinearColor, Color)
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
	TArray<FVector2D> BuildWaveformPoints(const FVector2D& Size) const;

	TAttribute<EForceFeedbackPeriodicEffectType> WaveformType;
	TAttribute<FLinearColor> Color;
};
