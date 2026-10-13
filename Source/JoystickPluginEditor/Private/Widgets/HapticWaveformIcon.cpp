// JoystickPlugin is licensed under the MIT License.
// Copyright Jayden Maalouf 2026. All Rights Reserved.

#include "Widgets/HapticWaveformIcon.h"

#include "Rendering/DrawElements.h"

void SHapticWaveformIcon::Construct(const FArguments& InArgs)
{
	WaveformType = InArgs._WaveformType;
	Color = InArgs._Color;
}

int32 SHapticWaveformIcon::OnPaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry, const FSlateRect& MyCullingRect, FSlateWindowElementList& OutDrawElements, const int32 LayerId, const FWidgetStyle& InWidgetStyle,
                                   bool bParentEnabled) const
{
	const FVector2D Size = AllottedGeometry.GetLocalSize();
	const FLinearColor DrawColor = Color.Get(FLinearColor::White);

	const TArray<FVector2D> Points = BuildWaveformPoints(Size);
	FSlateDrawElement::MakeLines(OutDrawElements, LayerId, AllottedGeometry.ToPaintGeometry(), Points, ESlateDrawEffect::None, DrawColor, true, 1.5f);

	return LayerId + 1;
}

FVector2D SHapticWaveformIcon::ComputeDesiredSize(float) const
{
	return FVector2D(28.0f, 16.0f);
}

TArray<FVector2D> SHapticWaveformIcon::BuildWaveformPoints(const FVector2D& Size) const
{
	// Mirrors the shape formulas in ForceFeedbackEffectEvaluation.cpp's EvaluatePeriodic, but fixed
	// to a canonical two-cycle/full-magnitude shape rather than the live effect data - this is a
	// decorative glyph for shape recognition, not a data-accurate preview.
	const EForceFeedbackPeriodicEffectType Type = WaveformType.Get(EForceFeedbackPeriodicEffectType::Sine);

	constexpr int32 NumSamples = 24;
	constexpr float NumCycles = 2.0f;
	const float MidY = Size.Y * 0.5f;
	const float Amplitude = Size.Y * 0.45f;

	TArray<FVector2D> Points;
	Points.Reserve(NumSamples);

	for (int32 Index = 0; Index < NumSamples; ++Index)
	{
		const float Alpha = static_cast<float>(Index) / (NumSamples - 1);
		const float CyclePosition = FMath::Frac(Alpha * NumCycles);

		float Value = 0.0f;
		switch (Type)
		{
			case EForceFeedbackPeriodicEffectType::Sine:
				Value = FMath::Sin(2.0f * PI * CyclePosition);
				break;
			case EForceFeedbackPeriodicEffectType::Triangle:
				Value = CyclePosition < 0.5f ? (-1.0f + 4.0f * CyclePosition) : (3.0f - 4.0f * CyclePosition);
				break;
			case EForceFeedbackPeriodicEffectType::SawtoothUp:
				Value = 2.0f * CyclePosition - 1.0f;
				break;
			case EForceFeedbackPeriodicEffectType::SawtoothDown:
				Value = 1.0f - 2.0f * CyclePosition;
				break;
			case EForceFeedbackPeriodicEffectType::Square:
				Value = CyclePosition < 0.5f ? 1.0f : -1.0f;
				break;
			default:
				Value = 0.0f;
				break;
		}

		Points.Add(FVector2D(Alpha * Size.X, MidY - Value * Amplitude));
	}

	return Points;
}
