// JoystickPlugin is licensed under the MIT License.
// Copyright Jayden Maalouf 2026. All Rights Reserved.

#include "Widgets/HapticWaveformGraph.h"

#include "InputCoreTypes.h"
#include "Rendering/DrawElements.h"
#include "Styling/AppStyle.h"
#include "Styling/CoreStyle.h"

// Matches the palette already established by AxisBar/ButtonBox/HatSwitch/BallSwitch: mid-gray
// (0.4,0.4,0.4) neutral/center indicators, bright green (0.2,0.9,0.3) for the active/highlight
// accent, and white primary text. The background reuses the engine's own graph canvas brush
// instead of a hand-picked flat tint.
struct FHapticWaveformGraphColors
{
	static constexpr FLinearColor MinorGridLine = FLinearColor(1.0f, 1.0f, 1.0f, 0.06f);
	static constexpr FLinearColor ZeroLine = FLinearColor(0.4f, 0.4f, 0.4f, 1.0f);
	static constexpr FLinearColor AxisLabel = FLinearColor(0.7f, 0.7f, 0.7f, 0.9f);
	static constexpr FLinearColor CurveGlow = FLinearColor(0.2f, 0.9f, 0.3f, 0.22f);
	static constexpr FLinearColor CurveBright = FLinearColor(0.2f, 0.9f, 0.3f, 1.0f);
	static constexpr FLinearColor Playhead = FLinearColor(1.0f, 1.0f, 1.0f, 0.95f);
};

void SHapticWaveformGraph::Construct(const FArguments& InArgs)
{
	DurationSeconds = InArgs._DurationSeconds;
	PlayheadTime = InArgs._PlayheadTime;
	OnEvaluateEffect = InArgs._OnEvaluateEffect;
	OnScrubbed = InArgs._OnScrubbed;
}

int32 SHapticWaveformGraph::OnPaint(
	const FPaintArgs& Args,
	const FGeometry& AllottedGeometry,
	const FSlateRect& MyCullingRect,
	FSlateWindowElementList& OutDrawElements,
	const int32 LayerId,
	const FWidgetStyle& InWidgetStyle,
	bool bParentEnabled
) const
{
	const float Duration = FMath::Max(DurationSeconds.Get(1.0f), KINDA_SMALL_NUMBER);

	PaintGrid(AllottedGeometry, OutDrawElements, LayerId, Duration);
	PaintCurve(AllottedGeometry, OutDrawElements, LayerId + 2, Duration);
	PaintPlayhead(AllottedGeometry, OutDrawElements, LayerId + 4, Duration);

	return LayerId + 5;
}

FVector2D SHapticWaveformGraph::ComputeDesiredSize(float) const
{
	return FVector2D(600.0f, 220.0f);
}

void SHapticWaveformGraph::PaintGrid(const FGeometry& AllottedGeometry, FSlateWindowElementList& OutDrawElements, const int32 LayerId, const float Duration) const
{
	const FVector2D Size = AllottedGeometry.GetLocalSize();
	const float MidY = Size.Y * 0.5f;
	const FSlateFontInfo LabelFont = FCoreStyle::GetDefaultFontStyle("Regular", 8);

	FSlateDrawElement::MakeBox(
		OutDrawElements,
		LayerId,
		AllottedGeometry.ToPaintGeometry(),
		FAppStyle::GetBrush("Graph.Panel.SolidBackground"),
		ESlateDrawEffect::None,
		FLinearColor::White
	);

	// Horizontal force gridlines at -1, -0.5, 0, 0.5, 1
	for (int32 Step = -2; Step <= 2; ++Step)
	{
		const float Value = Step * 0.5f;
		const float Y = MidY - Value * MidY * 0.9f;
		const bool bIsZeroLine = Step == 0;

		const TArray<FVector2D> HorizontalLinePoints = {FVector2D(0.0f, Y), FVector2D(Size.X, Y)};
		FSlateDrawElement::MakeLines(
			OutDrawElements, LayerId + 1, AllottedGeometry.ToPaintGeometry(),
			HorizontalLinePoints,
			ESlateDrawEffect::None, bIsZeroLine ? FHapticWaveformGraphColors::ZeroLine : FHapticWaveformGraphColors::MinorGridLine, true, bIsZeroLine ? 1.5f : 1.0f);

		FSlateDrawElement::MakeText(
			OutDrawElements, LayerId + 1, AllottedGeometry.ToOffsetPaintGeometry(FVector2D(4.0f, Y + 2.0f)),
			FString::Printf(TEXT("%.1f"), Value), LabelFont, ESlateDrawEffect::None, FHapticWaveformGraphColors::AxisLabel);
	}

	// Vertical time gridlines, quartered across the duration
	constexpr int32 NumDivisions = 4;
	for (int32 Step = 0; Step <= NumDivisions; ++Step)
	{
		const float Alpha = static_cast<float>(Step) / NumDivisions;
		const float X = Alpha * Size.X;

		const TArray<FVector2D> VerticalLinePoints = {FVector2D(X, 0.0f), FVector2D(X, Size.Y)};
		FSlateDrawElement::MakeLines(
			OutDrawElements, LayerId + 1, AllottedGeometry.ToPaintGeometry(),
			VerticalLinePoints,
			ESlateDrawEffect::None, FHapticWaveformGraphColors::MinorGridLine, true, 1.0f);

		const FString TimeLabel = FString::Printf(TEXT("%.2fs"), Alpha * Duration);
		const float LabelX = Step == NumDivisions ? X - 32.0f : X + 4.0f;
		FSlateDrawElement::MakeText(
			OutDrawElements, LayerId + 1, AllottedGeometry.ToOffsetPaintGeometry(FVector2D(LabelX, Size.Y - 14.0f)),
			TimeLabel, LabelFont, ESlateDrawEffect::None, FHapticWaveformGraphColors::AxisLabel);
	}
}

void SHapticWaveformGraph::PaintCurve(const FGeometry& AllottedGeometry, FSlateWindowElementList& OutDrawElements, const int32 LayerId, const float Duration) const
{
	if (!OnEvaluateEffect.IsBound())
	{
		return;
	}

	const FVector2D Size = AllottedGeometry.GetLocalSize();
	const float MidY = Size.Y * 0.5f;

	const int32 NumSamples = FMath::Clamp(static_cast<int32>(Size.X), 2, 512);
	TArray<FVector2D> CurvePoints;
	CurvePoints.Reserve(NumSamples);

	for (int32 Index = 0; Index < NumSamples; ++Index)
	{
		const float Alpha = static_cast<float>(Index) / (NumSamples - 1);
		const float TimeSeconds = Alpha * Duration;
		const float Value = FMath::Clamp(OnEvaluateEffect.Execute(TimeSeconds), -1.0f, 1.0f);

		CurvePoints.Add(FVector2D(Alpha * Size.X, MidY - Value * MidY * 0.9f));
	}

	// Two-pass glow: a soft wide underlay followed by a bright core line.
	FSlateDrawElement::MakeLines(
		OutDrawElements, LayerId, AllottedGeometry.ToPaintGeometry(),
		CurvePoints, ESlateDrawEffect::None, FHapticWaveformGraphColors::CurveGlow, true, 6.0f);

	FSlateDrawElement::MakeLines(
		OutDrawElements, LayerId + 1, AllottedGeometry.ToPaintGeometry(),
		CurvePoints, ESlateDrawEffect::None, FHapticWaveformGraphColors::CurveBright, true, 2.0f);
}

void SHapticWaveformGraph::PaintPlayhead(const FGeometry& AllottedGeometry, FSlateWindowElementList& OutDrawElements, const int32 LayerId, const float Duration) const
{
	const float TimeSeconds = PlayheadTime.Get(-1.0f);
	if (TimeSeconds < 0.0f)
	{
		return;
	}

	const FVector2D Size = AllottedGeometry.GetLocalSize();
	const float Alpha = FMath::Clamp(TimeSeconds / Duration, 0.0f, 1.0f);
	const float X = Alpha * Size.X;

	const TArray<FVector2D> PlayheadLinePoints = {FVector2D(X, 0.0f), FVector2D(X, Size.Y)};
	FSlateDrawElement::MakeLines(
		OutDrawElements, LayerId, AllottedGeometry.ToPaintGeometry(),
		PlayheadLinePoints,
		ESlateDrawEffect::None, FHapticWaveformGraphColors::Playhead, true, 1.5f);

	const float ForceValue = OnEvaluateEffect.IsBound() ? FMath::Clamp(OnEvaluateEffect.Execute(TimeSeconds), -1.0f, 1.0f) : 0.0f;
	const FString Readout = FString::Printf(TEXT("t=%.2fs  F=%.2f"), TimeSeconds, ForceValue);
	const FSlateFontInfo ReadoutFont = FCoreStyle::GetDefaultFontStyle("Bold", 9);
	const float ReadoutX = FMath::Clamp(X + 6.0f, 0.0f, FMath::Max(Size.X - 90.0f, 0.0f));

	FSlateDrawElement::MakeText(
		OutDrawElements, LayerId + 1, AllottedGeometry.ToOffsetPaintGeometry(FVector2D(ReadoutX, 6.0f)),
		Readout, ReadoutFont, ESlateDrawEffect::None, FHapticWaveformGraphColors::Playhead);
}

FReply SHapticWaveformGraph::OnMouseButtonDown(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent)
{
	if (MouseEvent.GetEffectingButton() != EKeys::LeftMouseButton)
	{
		return FReply::Unhandled();
	}

	bIsDragging = true;
	ScrubToLocalPosition(MyGeometry, MouseEvent.GetScreenSpacePosition());

	return FReply::Handled().CaptureMouse(SharedThis(this));
}

FReply SHapticWaveformGraph::OnMouseMove(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent)
{
	if (!bIsDragging)
	{
		return FReply::Unhandled();
	}

	ScrubToLocalPosition(MyGeometry, MouseEvent.GetScreenSpacePosition());

	return FReply::Handled();
}

FReply SHapticWaveformGraph::OnMouseButtonUp(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent)
{
	if (!bIsDragging)
	{
		return FReply::Unhandled();
	}

	bIsDragging = false;

	return FReply::Handled().ReleaseMouseCapture();
}

void SHapticWaveformGraph::ScrubToLocalPosition(const FGeometry& MyGeometry, const FVector2D& ScreenSpacePosition)
{
	if (!OnScrubbed.IsBound())
	{
		return;
	}

	const float Width = MyGeometry.GetLocalSize().X;
	if (Width <= 0.0f)
	{
		return;
	}

	const FVector2D LocalPosition = MyGeometry.AbsoluteToLocal(ScreenSpacePosition);
	const float Alpha = FMath::Clamp(LocalPosition.X / Width, 0.0f, 1.0f);
	OnScrubbed.Execute(Alpha * DurationSeconds.Get(1.0f));
}
