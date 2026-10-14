// JoystickPlugin is licensed under the MIT License.
// Copyright Jayden Maalouf 2026. All Rights Reserved.

#include "Widgets/HapticTimelineScrubber.h"

#include "InputCoreTypes.h"
#include "Rendering/DrawElements.h"
#include "Styling/AppStyle.h"

void SHapticTimelineScrubber::Construct(const FArguments& InArgs)
{
	Style = InArgs._Style;
	Bounds = InArgs._Bounds;
	PlayheadTimeMilliseconds = InArgs._PlayheadTimeMilliseconds;
	OnScrubbed = InArgs._OnScrubbed;
}

int32 SHapticTimelineScrubber::OnPaint(
	const FPaintArgs& Args,
	const FGeometry& AllottedGeometry,
	const FSlateRect& MyCullingRect,
	FSlateWindowElementList& OutDrawElements,
	const int32 LayerId,
	const FWidgetStyle& InWidgetStyle,
	bool bParentEnabled
) const
{
	PaintBackground(AllottedGeometry, OutDrawElements, LayerId);
	PaintPlayhead(AllottedGeometry, OutDrawElements, LayerId + 1);

	return LayerId + 2;
}

FVector2D SHapticTimelineScrubber::ComputeDesiredSize(float) const
{
	return FVector2D(600.0f, 28.0f);
}

void SHapticTimelineScrubber::PaintBackground(const FGeometry& AllottedGeometry, FSlateWindowElementList& OutDrawElements, const int32 LayerId) const
{
	FSlateDrawElement::MakeBox(
		OutDrawElements,
		LayerId,
		AllottedGeometry.ToPaintGeometry(),
		FAppStyle::GetBrush("Graph.Panel.SolidBackground"),
		ESlateDrawEffect::None,
		FLinearColor::White
	);
}

void SHapticTimelineScrubber::PaintPlayhead(const FGeometry& AllottedGeometry, FSlateWindowElementList& OutDrawElements, const int32 LayerId) const
{
	const int64 TimeMilliseconds = PlayheadTimeMilliseconds.Get(-1);
	if (TimeMilliseconds < 0)
	{
		return;
	}

	const TRange<double> CurrentBounds = Bounds.Get(TRange<double>(0.0, 1000.0));
	const double RangeSize = CurrentBounds.Size<double>();
	if (RangeSize <= 0.0)
	{
		return;
	}

	const FVector2D Size = AllottedGeometry.GetLocalSize();
	const double Alpha = FMath::Clamp((static_cast<double>(TimeMilliseconds) - CurrentBounds.GetLowerBoundValue()) / RangeSize, 0.0, 1.0);
	const float X = static_cast<float>(Alpha) * Size.X;

	const TArray<FVector2D> PlayheadLinePoints = {FVector2D(X, 0.0f), FVector2D(X, Size.Y)};
	FSlateDrawElement::MakeLines(
		OutDrawElements, LayerId, AllottedGeometry.ToPaintGeometry(),
		PlayheadLinePoints,
		ESlateDrawEffect::None, Style->MarkerColor, true, 1.5f);
}

FReply SHapticTimelineScrubber::OnMouseButtonDown(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent)
{
	if (MouseEvent.GetEffectingButton() != EKeys::LeftMouseButton)
	{
		return FReply::Unhandled();
	}

	bIsDragging = true;
	ScrubToLocalPosition(MyGeometry, MouseEvent.GetScreenSpacePosition());

	return FReply::Handled().CaptureMouse(SharedThis(this));
}

FReply SHapticTimelineScrubber::OnMouseMove(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent)
{
	if (!bIsDragging)
	{
		return FReply::Unhandled();
	}

	ScrubToLocalPosition(MyGeometry, MouseEvent.GetScreenSpacePosition());

	return FReply::Handled();
}

FReply SHapticTimelineScrubber::OnMouseButtonUp(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent)
{
	if (!bIsDragging)
	{
		return FReply::Unhandled();
	}

	bIsDragging = false;

	return FReply::Handled().ReleaseMouseCapture();
}

void SHapticTimelineScrubber::ScrubToLocalPosition(const FGeometry& MyGeometry, const FVector2D& ScreenSpacePosition)
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

	const TRange<double> CurrentBounds = Bounds.Get(TRange<double>(0.0, 1000.0));
	const FVector2D LocalPosition = MyGeometry.AbsoluteToLocal(ScreenSpacePosition);
	const double Alpha = FMath::Clamp(LocalPosition.X / Width, 0.0, 1.0);

	OnScrubbed.Execute(static_cast<float>(CurrentBounds.GetLowerBoundValue() + Alpha * CurrentBounds.Size<double>()));
}
