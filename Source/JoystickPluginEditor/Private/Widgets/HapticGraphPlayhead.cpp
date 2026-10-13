// JoystickPlugin is licensed under the MIT License.
// Copyright Jayden Maalouf 2026. All Rights Reserved.

#include "Widgets/HapticGraphPlayhead.h"

#include "Rendering/DrawElements.h"

void SHapticGraphPlayhead::Construct(const FArguments& InArgs)
{
	Bounds = InArgs._Bounds;
	PlayheadTime = InArgs._PlayheadTime;
	ReferenceGeometry = InArgs._ReferenceGeometry;
}

int32 SHapticGraphPlayhead::OnPaint(
	const FPaintArgs& Args,
	const FGeometry& AllottedGeometry,
	const FSlateRect& MyCullingRect,
	FSlateWindowElementList& OutDrawElements,
	const int32 LayerId,
	const FWidgetStyle& InWidgetStyle,
	bool bParentEnabled
) const
{
	const float TimeSeconds = PlayheadTime.Get(-1.0f);
	if (TimeSeconds < 0.0f)
	{
		return LayerId;
	}

	const TRange<double> CurrentBounds = Bounds.Get(TRange<double>(0.0, 1.0));
	const double RangeSize = CurrentBounds.Size<double>();
	if (RangeSize <= 0.0)
	{
		return LayerId;
	}

	const FGeometry ViewGeometry = ReferenceGeometry.Get(AllottedGeometry);
	const double Alpha = FMath::Clamp((static_cast<double>(TimeSeconds) - CurrentBounds.GetLowerBoundValue()) / RangeSize, 0.0, 1.0);

	const FVector2D ViewLocalSize = ViewGeometry.GetLocalSize();
	const float LineX = static_cast<float>(Alpha) * ViewLocalSize.X;

	// Convert through absolute screen space rather than assuming ViewGeometry shares an origin/scale
	// with AllottedGeometry - the curve view sits inside the panel's own toolbar/tree layout, which
	// this overlay (a sibling spanning the whole panel) knows nothing about.
	const FVector2D TopAbsolute = ViewGeometry.LocalToAbsolute(FVector2D(LineX, 0.0f));
	const FVector2D BottomAbsolute = ViewGeometry.LocalToAbsolute(FVector2D(LineX, ViewLocalSize.Y));

	// ViewGeometry is the view's cached (tick-space) geometry, which is in desktop space, while
	// AllottedGeometry is in window space - bring ours into desktop space too before converting back,
	// otherwise the line is offset by the window's position on the desktop.
	FGeometry DesktopGeometry = AllottedGeometry;
	DesktopGeometry.AppendTransform(FSlateLayoutTransform(Args.GetWindowToDesktopTransform()));

	const TArray<FVector2D> PlayheadLinePoints = {
		DesktopGeometry.AbsoluteToLocal(TopAbsolute),
		DesktopGeometry.AbsoluteToLocal(BottomAbsolute)
	};

	FSlateDrawElement::MakeLines(
		OutDrawElements, LayerId, AllottedGeometry.ToPaintGeometry(),
		PlayheadLinePoints,
		ESlateDrawEffect::None, FLinearColor(1.0f, 1.0f, 1.0f, 0.95f), true, 1.5f);

	return LayerId + 1;
}

FVector2D SHapticGraphPlayhead::ComputeDesiredSize(float) const
{
	return FVector2D(1.0f, 1.0f);
}
