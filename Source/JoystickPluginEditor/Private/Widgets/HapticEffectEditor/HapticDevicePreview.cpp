// JoystickPlugin is licensed under the MIT License.
// Copyright Jayden Maalouf 2026. All Rights Reserved.

#include "Widgets/HapticEffectEditor/HapticDevicePreview.h"

#include "Rendering/DrawElements.h"

void SHapticDevicePreview::Construct(const FArguments& InArgs)
{
	Style = InArgs._Style;
	JoystickType = InArgs._JoystickType;
	AxisValue = InArgs._AxisValue;
	MaxRotationDegrees = InArgs._MaxRotationDegrees;
}

bool SHapticDevicePreview::SupportsType(const EJoystickType Type)
{
	return Type == EJoystickType::Wheel;
}

int32 SHapticDevicePreview::OnPaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry, const FSlateRect& MyCullingRect, FSlateWindowElementList& OutDrawElements, const int32 LayerId, const FWidgetStyle& InWidgetStyle,
                                    bool bParentEnabled) const
{
	switch (JoystickType.Get(EJoystickType::Unknown))
	{
		case EJoystickType::Wheel:
			return PaintWheel(AllottedGeometry, OutDrawElements, LayerId);
		default:
			return LayerId;
	}
}

FVector2D SHapticDevicePreview::ComputeDesiredSize(float) const
{
	return FVector2D(160.0f, 160.0f);
}

int32 SHapticDevicePreview::PaintWheel(const FGeometry& AllottedGeometry, FSlateWindowElementList& OutDrawElements, const int32 LayerId) const
{
	// Leaves a margin outside the rim for the fixed 12 o'clock reference notch.
	constexpr float OuterMargin = 10.0f;
	constexpr int32 NumCircleSegments = 64;

	const FVector2D Size = AllottedGeometry.GetLocalSize();
	const FVector2D Center = Size * 0.5f;
	const float OuterRadius = FMath::Min(Size.X, Size.Y) * 0.5f - OuterMargin;
	if (OuterRadius <= 0.0f)
	{
		return LayerId;
	}

	const float RimThickness = FMath::Max(4.0f, OuterRadius * 0.14f);
	const float RimInnerRadius = OuterRadius - RimThickness;
	const float SpokeThickness = RimThickness * 0.8f;
	const float HubOuterRadius = OuterRadius * 0.24f + SpokeThickness * 0.5f;
	const float HubInnerRadius = HubOuterRadius - SpokeThickness;

	// Slate's Y axis points down, so a positive angle already reads as clockwise with no sign flip.
	const float Value = FMath::Clamp(AxisValue.Get(0.0f), -1.0f, 1.0f);
	const float Rotation = FMath::DegreesToRadians(Value * MaxRotationDegrees.Get(540.0f));

	const FColor BodyColor = Style->BodyColor.ToFColor(true);
	const FColor ReferenceColor = Style->ReferenceColor.ToFColor(true);
	const FColor MarkerColor = Style->MarkerColor.ToFColor(true);

	// Built as filled geometry with one-pixel feathered edges, as thick MakeLines strokes spike at the join and custom verts aren't antialiased.
	const float Feather = 1.0f / FMath::Max(AllottedGeometry.Scale, KINDA_SMALL_NUMBER);
	const float HalfFeather = Feather * 0.5f;

	const FSlateRenderTransform& AccumulatedRenderTransform = AllottedGeometry.GetAccumulatedRenderTransform();
	TArray<FSlateVertex> Vertices;
	TArray<SlateIndex> Indices;

	auto PointAt = [&Center](const float Angle, const float Distance)
	{
		return Center + FVector2D(FMath::Cos(Angle), FMath::Sin(Angle)) * Distance;
	};

	auto AddVertex = [&](const FVector2D& Point, const FColor& Color)
	{
		return static_cast<SlateIndex>(Vertices.Add(FSlateVertex::Make<ESlateVertexRounding::Disabled>(AccumulatedRenderTransform, FVector2f(Point), FVector2f(0.5f, 0.5f), Color)));
	};

	auto AddQuad = [&](const SlateIndex A, const SlateIndex B, const SlateIndex C, const SlateIndex D)
	{
		Indices.Append({A, B, C, A, C, D});
	};

	auto AddRing = [&](const float InnerRadius, const float InOuterRadius, const FColor& Color)
	{
		// Four concentric loops: the middle band is the solid ring, the bands either side are the feathered edges.
		constexpr int32 NumLoops = 4;
		const float LoopRadii[NumLoops] = {InnerRadius - HalfFeather, InnerRadius + HalfFeather, InOuterRadius - HalfFeather, InOuterRadius + HalfFeather};
		const FColor LoopColors[NumLoops] = {Color.WithAlpha(0), Color, Color, Color.WithAlpha(0)};

		const SlateIndex FirstIndex = static_cast<SlateIndex>(Vertices.Num());
		for (int32 Index = 0; Index < NumCircleSegments; ++Index)
		{
			const float Angle = 2.0f * PI * Index / NumCircleSegments;
			for (int32 Loop = 0; Loop < NumLoops; ++Loop)
			{
				AddVertex(PointAt(Angle, LoopRadii[Loop]), LoopColors[Loop]);
			}
		}

		for (int32 Index = 0; Index < NumCircleSegments; ++Index)
		{
			const SlateIndex Current = FirstIndex + Index * NumLoops;
			const SlateIndex Next = FirstIndex + ((Index + 1) % NumCircleSegments) * NumLoops;
			for (int32 Loop = 0; Loop < NumLoops - 1; ++Loop)
			{
				AddQuad(Current + Loop, Current + Loop + 1, Next + Loop + 1, Next + Loop);
			}
		}
	};

	// A bar of the given thickness running outwards from the centre along Angle.
	auto AddRadialBar = [&](const float Angle, const float InnerDistance, const float OuterDistance, const float Thickness, const FColor& Color)
	{
		const FVector2D Along(FMath::Cos(Angle), FMath::Sin(Angle));
		const FVector2D Across(-Along.Y, Along.X);

		// A solid rectangle inset by half the feather, with a transparent outset copy to fade the edges.
		auto AddCorners = [&](const float Inset, const FColor& CornerColor)
		{
			const FVector2D Inner = Center + Along * (InnerDistance + Inset);
			const FVector2D Outer = Center + Along * (OuterDistance - Inset);
			const FVector2D HalfWidth = Across * FMath::Max(Thickness * 0.5f - Inset, 0.0f);

			const SlateIndex FirstIndex = AddVertex(Inner - HalfWidth, CornerColor);
			AddVertex(Inner + HalfWidth, CornerColor);
			AddVertex(Outer + HalfWidth, CornerColor);
			AddVertex(Outer - HalfWidth, CornerColor);
			return FirstIndex;
		};

		constexpr int32 NumCorners = 4;
		const SlateIndex Solid = AddCorners(HalfFeather, Color);
		const SlateIndex Faded = AddCorners(-HalfFeather, Color.WithAlpha(0));

		AddQuad(Solid, Solid + 1, Solid + 2, Solid + 3);
		for (int32 Corner = 0; Corner < NumCorners; ++Corner)
		{
			const int32 NextCorner = (Corner + 1) % NumCorners;
			AddQuad(Solid + Corner, Solid + NextCorner, Faded + NextCorner, Faded + Corner);
		}
	};

	// Fixed notch marking straight-ahead, for the wheel's marker to be read against.
	AddRadialBar(-HALF_PI, OuterRadius + 3.0f, OuterRadius + OuterMargin - 1.0f, 2.0f, ReferenceColor);

	AddRing(RimInnerRadius, OuterRadius, BodyColor);
	AddRing(HubInnerRadius, HubOuterRadius, BodyColor);

	// Three spokes (left, right, down), slightly overlapping the hub and rim so no gap shows.
	for (const float SpokeAngle : {0.0f, PI, HALF_PI})
	{
		AddRadialBar(SpokeAngle + Rotation, HubOuterRadius - 1.0f, RimInnerRadius + 1.0f, SpokeThickness, BodyColor);
	}

	// Top-centre stripe showing the wheel's rotation, added last so it draws over the rim.
	AddRadialBar(-HALF_PI + Rotation, RimInnerRadius, OuterRadius, SpokeThickness, MarkerColor);

	const FSlateResourceHandle Handle; // No texture
	FSlateDrawElement::MakeCustomVerts(OutDrawElements, LayerId, Handle, Vertices, Indices, nullptr, 0, 0);

	return LayerId + 1;
}
