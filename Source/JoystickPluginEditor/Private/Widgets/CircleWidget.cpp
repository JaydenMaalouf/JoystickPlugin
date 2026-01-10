// JoystickPlugin is licensed under the MIT License.
// Copyright Jayden Maalouf 2026. All Rights Reserved.

#include "Widgets/CircleWidget.h"

#include "Rendering/DrawElements.h"

void SCircleWidget::Construct(const FArguments& InArgs)
{
	Radius = InArgs._Radius;
	Color = InArgs._Color;
	Filled = InArgs._Filled;
	LineThickness = InArgs._LineThickness;
}

void SCircleWidget::SetColor(const TAttribute<FLinearColor>& InColor)
{
	Color = InColor;
}

int32 SCircleWidget::OnPaint(
	const FPaintArgs& Args,
	const FGeometry& AllottedGeometry,
	const FSlateRect& MyCullingRect,
	FSlateWindowElementList& OutDrawElements,
	const int32 LayerId,
	const FWidgetStyle& InWidgetStyle,
	bool bParentEnabled
) const
{
	const FVector2D Center = AllottedGeometry.GetLocalSize() / 2.0f;
	DrawCircle(OutDrawElements, AllottedGeometry, LayerId, Center, Radius.Get(), 64, Color.Get(), Filled.Get(false), LineThickness.Get(1.5f));

	return LayerId;
}

FVector2D SCircleWidget::ComputeDesiredSize(float) const
{
	return FVector2D(Radius.Get() * 2.0f, Radius.Get() * 2.0f);
}

void SCircleWidget::DrawCircle(FSlateWindowElementList& OutDrawElements, const FGeometry& InGeometry, const int32 InLayerId, const FVector2D InCenter, const float InRadius, const int32 InNumSegments, const FLinearColor InColor,
                               const bool InFilled, const float InLineThickness) const
{
	const FSlateRenderTransform& AccumulatedRenderTransform = InGeometry.GetAccumulatedRenderTransform();
	const FVector2D LocalCenter = InCenter;

	if (InFilled)
	{
		TArray<SlateIndex> Indices;
		TArray<FSlateVertex> Vertices;

		// Center vertex
		const int32 CenterIndex = Vertices.Num();
		Vertices.Add(FSlateVertex::Make<ESlateVertexRounding::Disabled>(AccumulatedRenderTransform, FVector2f(LocalCenter), FVector2f(0.5f, 0.5f), InColor.ToFColor(true)));

		for (int32 i = 0; i <= InNumSegments; ++i)
		{
			const float Angle = 2 * PI * i / InNumSegments;
			const FVector2D Point = LocalCenter + FVector2D(FMath::Cos(Angle), FMath::Sin(Angle)) * InRadius;
			Vertices.Add(FSlateVertex::Make<ESlateVertexRounding::Disabled>(AccumulatedRenderTransform, FVector2f(Point), FVector2f(0.5f, 0.5f), InColor.ToFColor(true)));

			if (i > 0)
			{
				Indices.Add(CenterIndex);
				Indices.Add(CenterIndex + i);
				Indices.Add(CenterIndex + i + 1);
			}
		}

		const FSlateResourceHandle Handle; // No texture
		FSlateDrawElement::MakeCustomVerts(
			OutDrawElements,
			InLayerId,
			Handle,
			Vertices,
			Indices,
			nullptr, // No clipping
			0, 0
		);
	}
	else
	{
		// Outline mode
		TArray<FVector2D> Points;
		for (int32 i = 0; i <= InNumSegments; ++i)
		{
			const float Angle = 2 * PI * i / InNumSegments;
			FVector2D Point = InCenter + FVector2D(FMath::Cos(Angle), FMath::Sin(Angle)) * InRadius;
			Points.Add(Point);
		}

		FSlateDrawElement::MakeLines(
			OutDrawElements,
			InLayerId,
			InGeometry.ToPaintGeometry(),
			Points,
			ESlateDrawEffect::None,
			InColor,
			true,
			InLineThickness
		);
	}
}
