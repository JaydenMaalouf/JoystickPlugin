// JoystickPlugin is licensed under the MIT License.
// Copyright Jayden Maalouf 2026. All Rights Reserved.

#include "Widgets/HapticConditionGraph.h"

#include "Framework/Application/SlateApplication.h"
#include "Rendering/DrawElements.h"
#include "Rendering/SlateRenderer.h"
#include "Styling/AppStyle.h"
#include "Styling/CoreStyle.h"

#define LOCTEXT_NAMESPACE "HapticEffectEditor"

void SHapticConditionGraph::Construct(const FArguments& InArgs)
{
	Style = InArgs._Style;
	OnEvaluate = InArgs._OnEvaluate;
	Axis = InArgs._Axis;
	SweepRate = InArgs._SweepRate;
	FullScaleAcceleration = InArgs._FullScaleAcceleration;
	LiveMotion = InArgs._LiveMotion;
}

int32 SHapticConditionGraph::OnPaint(
	const FPaintArgs& Args,
	const FGeometry& AllottedGeometry,
	const FSlateRect& MyCullingRect,
	FSlateWindowElementList& OutDrawElements,
	const int32 LayerId,
	const FWidgetStyle& InWidgetStyle,
	bool bParentEnabled
) const
{
	PaintGrid(AllottedGeometry, OutDrawElements, LayerId);
	if (IsPlottingAcceleration())
	{
		PaintAccelerationResponse(AllottedGeometry, OutDrawElements, LayerId + 2);
	}
	else
	{
		PaintLoop(AllottedGeometry, OutDrawElements, LayerId + 2);
	}

	PaintLiveMotion(AllottedGeometry, OutDrawElements, LayerId + 7);

	return LayerId + 8;
}

void SHapticConditionGraph::Tick(const FGeometry& AllottedGeometry, const double InCurrentTime, const float InDeltaTime)
{
	SLeafWidget::Tick(AllottedGeometry, InCurrentTime, InDeltaTime);

	const TOptional<FHapticConditionMotion> CurrentMotion = LiveMotion.Get();
	if (!CurrentMotion.IsSet())
	{
		LiveTrail.Reset();
		return;
	}

	LiveTrail.Emplace(InCurrentTime, CurrentMotion.GetValue());

	const int32 FirstFreshIndex = LiveTrail.IndexOfByPredicate([InCurrentTime](const TPair<double, FHapticConditionMotion>& Sample)
	{
		return InCurrentTime - Sample.Key <= TrailSeconds;
	});
	LiveTrail.RemoveAt(0, FirstFreshIndex == INDEX_NONE ? LiveTrail.Num() : FirstFreshIndex);
}

FVector2D SHapticConditionGraph::ComputeDesiredSize(float) const
{
	return FVector2D(600.0f, 220.0f);
}

FVector2D SHapticConditionGraph::ToLocal(const FVector2D& Size, const float Input, const float Force)
{
	const float PlotWidth = FMath::Max(Size.X - MarginX * 2.0f, 1.0f);
	const float PlotHeight = FMath::Max(Size.Y - MarginTop - MarginBottom, 1.0f);

	return FVector2D(
		MarginX + (Input + 1.0f) * 0.5f * PlotWidth,
		MarginTop + (1.0f - Force) * 0.5f * PlotHeight);
}

bool SHapticConditionGraph::IsPlottingAcceleration() const
{
	return Axis.Get() == EHapticConditionGraphAxis::Acceleration;
}

float SHapticConditionGraph::GetInputRange() const
{
	return IsPlottingAcceleration() ? GetFullScaleAcceleration() : 1.0f;
}

float SHapticConditionGraph::GetFullScaleAcceleration() const
{
	return FMath::Max(FullScaleAcceleration.Get(), KINDA_SMALL_NUMBER);
}

float SHapticConditionGraph::GetNormalisedInput(const FHapticConditionMotion& Motion) const
{
	const float Input = IsPlottingAcceleration() ? Motion.Acceleration : Motion.Position;
	return FMath::Clamp(Input / GetInputRange(), -1.0f, 1.0f);
}

FHapticConditionMotion SHapticConditionGraph::GetSweepMotion(const float Position, const float Rate, const bool bMovingPositive)
{
	// For x = sin(Rate * t): v = Rate * cos(Rate * t), which is +/- Rate * sqrt(1 - x^2) depending on the stroke, and a = -Rate^2 * x on both.
	const float Speed = Rate * FMath::Sqrt(FMath::Max(1.0f - Position * Position, 0.0f));

	FHapticConditionMotion Motion;
	Motion.Position = Position;
	Motion.Velocity = bMovingPositive ? Speed : -Speed;
	Motion.Acceleration = -Rate * Rate * Position;
	return Motion;
}

float SHapticConditionGraph::Evaluate(const FHapticConditionMotion& Motion) const
{
	if (!OnEvaluate.IsBound())
	{
		return 0.0f;
	}

	// A condition's centre, dead zone and coefficients are all in the device's -1..1 terms, so the acceleration is measured against its full scale first.
	FHapticConditionMotion DeviceMotion = Motion;
	DeviceMotion.Acceleration = FMath::Clamp(Motion.Acceleration / GetFullScaleAcceleration(), -1.0f, 1.0f);
	return FMath::Clamp(OnEvaluate.Execute(DeviceMotion), -1.0f, 1.0f);
}

void SHapticConditionGraph::PaintGrid(const FGeometry& AllottedGeometry, FSlateWindowElementList& OutDrawElements, const int32 LayerId) const
{
	const FVector2D Size = AllottedGeometry.GetLocalSize();
	const FSlateFontInfo LabelFont = FCoreStyle::GetDefaultFontStyle("Regular", 8);
	const float InputRange = GetInputRange();

	FSlateDrawElement::MakeBox(
		OutDrawElements,
		LayerId,
		AllottedGeometry.ToPaintGeometry(),
		FAppStyle::GetBrush("Graph.Panel.SolidBackground"),
		ESlateDrawEffect::None,
		FLinearColor::White
	);

	// Gridlines at -1, -0.5, 0, 0.5 and 1 on both axes, with the zero lines picked out.
	for (int32 Step = -2; Step <= 2; ++Step)
	{
		const float Value = Step * 0.5f;
		const bool bIsZeroLine = Step == 0;
		const FLinearColor LineColor = bIsZeroLine ? Style->ZeroLineColor : Style->GridLineColor;
		const float LineThickness = bIsZeroLine ? 1.5f : 1.0f;

		const FVector2D ForceLineStart = ToLocal(Size, -1.0f, Value);
		const FVector2D ForceLineEnd = ToLocal(Size, 1.0f, Value);
		FSlateDrawElement::MakeLines(
			OutDrawElements, LayerId + 1, AllottedGeometry.ToPaintGeometry(),
			TArray<FVector2D>{ForceLineStart, ForceLineEnd},
			ESlateDrawEffect::None, LineColor, true, LineThickness);

		FSlateDrawElement::MakeText(
			OutDrawElements, LayerId + 1, AllottedGeometry.ToOffsetPaintGeometry(FVector2D(4.0f, ForceLineStart.Y - 6.0f)),
			FString::Printf(TEXT("%+.1f"), Value), LabelFont, ESlateDrawEffect::None, Style->AxisLabelColor);

		const FVector2D InputLineTop = ToLocal(Size, Value, 1.0f);
		const FVector2D InputLineBottom = ToLocal(Size, Value, -1.0f);
		FSlateDrawElement::MakeLines(
			OutDrawElements, LayerId + 1, AllottedGeometry.ToPaintGeometry(),
			TArray<FVector2D>{InputLineTop, InputLineBottom},
			ESlateDrawEffect::None, LineColor, true, LineThickness);

		// A small full scale needs the extra digit to tell its gridlines apart.
		const float InputValue = Value * InputRange;
		FSlateDrawElement::MakeText(
			OutDrawElements, LayerId + 1, AllottedGeometry.ToOffsetPaintGeometry(FVector2D(InputLineBottom.X - 10.0f, InputLineBottom.Y + 4.0f)),
			InputRange < 1.0f ? FString::Printf(TEXT("%+.2f"), InputValue) : FString::Printf(TEXT("%+.1f"), InputValue), LabelFont, ESlateDrawEffect::None, Style->AxisLabelColor);
	}

	FSlateDrawElement::MakeText(
		OutDrawElements, LayerId + 1, AllottedGeometry.ToOffsetPaintGeometry(FVector2D(MarginX, 4.0f)),
		LOCTEXT("ConditionGraphForceLabel", "Force (+ pushes towards the positive side)").ToString(), LabelFont, ESlateDrawEffect::None, Style->AxisLabelColor);

	const FVector2D InputLabelAnchor = ToLocal(Size, 0.0f, -1.0f);
	const FText InputLabel = IsPlottingAcceleration() ? LOCTEXT("ConditionGraphAccelerationLabel", "Axis acceleration") : LOCTEXT("ConditionGraphPositionLabel", "Axis position");
	FSlateDrawElement::MakeText(
		OutDrawElements, LayerId + 1, AllottedGeometry.ToOffsetPaintGeometry(FVector2D(InputLabelAnchor.X - 36.0f, InputLabelAnchor.Y + 18.0f)),
		InputLabel.ToString(), LabelFont, ESlateDrawEffect::None, Style->AxisLabelColor);
}

void SHapticConditionGraph::PaintLoop(const FGeometry& AllottedGeometry, FSlateWindowElementList& OutDrawElements, const int32 LayerId) const
{
	if (!OnEvaluate.IsBound())
	{
		return;
	}

	const FVector2D Size = AllottedGeometry.GetLocalSize();
	const float Rate = FMath::Max(SweepRate.Get(), 0.0f);
	const int32 NumSamples = FMath::Clamp(static_cast<int32>((Size.X - MarginX * 2.0f) * 0.5f), 2, MaxSamples);

	// Both strokes are sampled at the same positions so the fill between them is a simple strip.
	TArray<FVector2D> PositiveStroke;
	TArray<FVector2D> NegativeStroke;
	PositiveStroke.Reserve(NumSamples);
	NegativeStroke.Reserve(NumSamples);

	float MaxLoopWidth = 0.0f;
	for (int32 Index = 0; Index < NumSamples; ++Index)
	{
		const float Position = -FMath::Cos(PI * Index / (NumSamples - 1));
		const float PositiveForce = Evaluate(GetSweepMotion(Position, Rate, true));
		const float NegativeForce = Evaluate(GetSweepMotion(Position, Rate, false));

		PositiveStroke.Add(ToLocal(Size, Position, PositiveForce));
		NegativeStroke.Add(ToLocal(Size, Position, NegativeForce));
		MaxLoopWidth = FMath::Max(MaxLoopWidth, FMath::Abs(PositiveForce - NegativeForce));
	}

	const bool bHasArea = MaxLoopWidth > MinLoopWidth;
	if (bHasArea)
	{
		const FSlateRenderTransform& GraphRenderTransform = AllottedGeometry.GetAccumulatedRenderTransform();
		const FColor FillColor = Style->LoopFillColor.ToFColor(true);

		TArray<FSlateVertex> FillVertices;
		TArray<SlateIndex> FillIndices;
		FillVertices.Reserve(NumSamples * 2);
		FillIndices.Reserve((NumSamples - 1) * 6);
		for (int32 Index = 0; Index < NumSamples; ++Index)
		{
			FillVertices.Add(FSlateVertex::Make<ESlateVertexRounding::Disabled>(GraphRenderTransform, FVector2f(PositiveStroke[Index]), FVector2f::ZeroVector, FillColor));
			FillVertices.Add(FSlateVertex::Make<ESlateVertexRounding::Disabled>(GraphRenderTransform, FVector2f(NegativeStroke[Index]), FVector2f::ZeroVector, FillColor));

			if (Index > 0)
			{
				const SlateIndex Base = static_cast<SlateIndex>((Index - 1) * 2);
				FillIndices.Append({Base, (Base + 1), (Base + 2)});
				FillIndices.Append({(Base + 1), (Base + 3), (Base + 2)});
			}
		}

		const FSlateResourceHandle ResourceHandle = FSlateApplication::Get().GetRenderer()->GetResourceHandle(*FAppStyle::GetBrush("WhiteBrush"));
		FSlateDrawElement::MakeCustomVerts(OutDrawElements, LayerId, ResourceHandle, FillVertices, FillIndices, nullptr, 0, 0);
	}

	// The negative stroke goes underneath, so a spring's single line keeps the positive stroke's green.
	// Two-pass glow per stroke: a soft wide underlay followed by a bright core line.
	FSlateDrawElement::MakeLines(
		OutDrawElements, LayerId + 1, AllottedGeometry.ToPaintGeometry(),
		NegativeStroke, ESlateDrawEffect::None, Style->SecondaryCurveGlowColor, true, 6.0f);

	FSlateDrawElement::MakeLines(
		OutDrawElements, LayerId + 2, AllottedGeometry.ToPaintGeometry(),
		NegativeStroke, ESlateDrawEffect::None, Style->SecondaryCurveColor, true, 2.0f);

	FSlateDrawElement::MakeLines(
		OutDrawElements, LayerId + 3, AllottedGeometry.ToPaintGeometry(),
		PositiveStroke, ESlateDrawEffect::None, Style->CurveGlowColor, true, 6.0f);

	FSlateDrawElement::MakeLines(
		OutDrawElements, LayerId + 4, AllottedGeometry.ToPaintGeometry(),
		PositiveStroke, ESlateDrawEffect::None, Style->CurveColor, true, 2.0f);

	if (bHasArea)
	{
		const FSlateFontInfo LegendFont = FCoreStyle::GetDefaultFontStyle("Regular", 8);
		const float LegendX = Size.X - MarginX - 190.0f;

		FSlateDrawElement::MakeText(
			OutDrawElements, LayerId + 4, AllottedGeometry.ToOffsetPaintGeometry(FVector2D(LegendX, 4.0f)),
			LOCTEXT("ConditionGraphPositiveStroke", "→ moving positive").ToString(), LegendFont, ESlateDrawEffect::None, Style->CurveColor);

		FSlateDrawElement::MakeText(
			OutDrawElements, LayerId + 4, AllottedGeometry.ToOffsetPaintGeometry(FVector2D(LegendX + 100.0f, 4.0f)),
			LOCTEXT("ConditionGraphNegativeStroke", "← moving negative").ToString(), LegendFont, ESlateDrawEffect::None, Style->SecondaryCurveColor);
	}
}

void SHapticConditionGraph::PaintAccelerationResponse(const FGeometry& AllottedGeometry, FSlateWindowElementList& OutDrawElements, const int32 LayerId) const
{
	if (!OnEvaluate.IsBound())
	{
		return;
	}

	const FVector2D Size = AllottedGeometry.GetLocalSize();
	const float InputRange = GetInputRange();
	const int32 NumSamples = FMath::Clamp(static_cast<int32>(Size.X - MarginX * 2.0f), 2, MaxSamples);

	// Inertia only looks at acceleration, so position and velocity are left at zero.
	TArray<FVector2D> CurvePoints;
	CurvePoints.Reserve(NumSamples);
	for (int32 Index = 0; Index < NumSamples; ++Index)
	{
		const float Input = -1.0f + 2.0f * Index / (NumSamples - 1);

		FHapticConditionMotion Motion;
		Motion.Acceleration = Input * InputRange;
		CurvePoints.Add(ToLocal(Size, Input, Evaluate(Motion)));
	}

	// Two-pass glow: a soft wide underlay followed by a bright core line.
	FSlateDrawElement::MakeLines(
		OutDrawElements, LayerId, AllottedGeometry.ToPaintGeometry(),
		CurvePoints, ESlateDrawEffect::None, Style->CurveGlowColor, true, 6.0f);

	FSlateDrawElement::MakeLines(
		OutDrawElements, LayerId + 1, AllottedGeometry.ToPaintGeometry(),
		CurvePoints, ESlateDrawEffect::None, Style->CurveColor, true, 2.0f);
}

void SHapticConditionGraph::PaintLiveMotion(const FGeometry& AllottedGeometry, FSlateWindowElementList& OutDrawElements, const int32 LayerId) const
{
	const TOptional<FHapticConditionMotion> CurrentMotion = LiveMotion.Get();
	if (!CurrentMotion.IsSet())
	{
		return;
	}

	const FVector2D Size = AllottedGeometry.GetLocalSize();
	const FHapticConditionMotion& Motion = CurrentMotion.GetValue();
	const float Input = GetNormalisedInput(Motion);
	const float Force = Evaluate(Motion);

	const FVector2D LineTop = ToLocal(Size, Input, 1.0f);
	const FVector2D LineBottom = ToLocal(Size, Input, -1.0f);
	FSlateDrawElement::MakeLines(
		OutDrawElements, LayerId, AllottedGeometry.ToPaintGeometry(),
		TArray<FVector2D>{LineTop, LineBottom},
		ESlateDrawEffect::None, Style->MarkerColor.CopyWithNewOpacity(0.35f), true, 1.0f);

	// The trail fades in from its oldest sample, showing which way round the loop the axis is travelling.
	if (LiveTrail.Num() >= 2)
	{
		TArray<FVector2D> TrailPoints;
		TArray<FLinearColor> TrailColors;
		TrailPoints.Reserve(LiveTrail.Num());
		TrailColors.Reserve(LiveTrail.Num());
		for (int32 Index = 0; Index < LiveTrail.Num(); ++Index)
		{
			const FHapticConditionMotion& Sample = LiveTrail[Index].Value;
			TrailPoints.Add(ToLocal(Size, GetNormalisedInput(Sample), Evaluate(Sample)));
			TrailColors.Add(Style->MarkerColor.CopyWithNewOpacity(0.8f * Index / (LiveTrail.Num() - 1)));
		}

		FSlateDrawElement::MakeLines(
			OutDrawElements, LayerId, AllottedGeometry.ToPaintGeometry(),
			TrailPoints, TrailColors, ESlateDrawEffect::None, FLinearColor::White, true, 2.0f);
	}

	// A small square where the axis currently sits. Against acceleration it stays at zero through a steady turn, wherever the axis is.
	constexpr float MarkerSize = 7.0f;
	const FVector2D MarkerCentre = ToLocal(Size, Input, Force);
	FSlateDrawElement::MakeBox(
		OutDrawElements, LayerId + 1,
		AllottedGeometry.ToPaintGeometry(FVector2f(MarkerSize, MarkerSize), FSlateLayoutTransform(FVector2f(MarkerCentre - FVector2D(MarkerSize * 0.5f)))),
		FCoreStyle::Get().GetBrush("WhiteBrush"), ESlateDrawEffect::None, Style->MarkerColor);

	const FString Readout = FString::Printf(TEXT("x=%+.2f  v=%+.2f  a=%+.2f  F=%+.2f"), Motion.Position, Motion.Velocity, Motion.Acceleration, Force);
	const FSlateFontInfo ReadoutFont = FCoreStyle::GetDefaultFontStyle("Bold", 9);
	const float ReadoutX = FMath::Clamp(LineTop.X + 6.0f, 0.0f, FMath::Max(Size.X - 230.0f, 0.0f));
	FSlateDrawElement::MakeText(
		OutDrawElements, LayerId + 1, AllottedGeometry.ToOffsetPaintGeometry(FVector2D(ReadoutX, LineTop.Y + 2.0f)),
		Readout, ReadoutFont, ESlateDrawEffect::None, Style->MarkerColor);
}

#undef LOCTEXT_NAMESPACE
