// JoystickPlugin is licensed under the MIT License.
// Copyright Jayden Maalouf 2026. All Rights Reserved.

#pragma once

#include "Data/JoystickType.h"
#include "Widgets/SLeafWidget.h"

// 2D top-down representation of the selected device, posed from a live axis value so the Haptic
// Effect Editor shows what the hardware is physically doing while an effect plays. Only
// EJoystickType::Wheel has a drawing so far - other types paint nothing (see SupportsType), leaving
// the owner to show its own placeholder.
//
// AxisValue is the normalised -1..1 axis reading; for a wheel it is spread across
// MaxRotationDegrees each way from centre, so +1 is MaxRotationDegrees clockwise and -1 the same
// anticlockwise.
class SHapticDevicePreview : public SLeafWidget
{
public:
	SLATE_BEGIN_ARGS(SHapticDevicePreview)
			: _JoystickType(EJoystickType::Unknown)
			  , _AxisValue(0.0f)
			  , _MaxRotationDegrees(540.0f)
		{
		}

		SLATE_ATTRIBUTE(EJoystickType, JoystickType)
		SLATE_ATTRIBUTE(float, AxisValue)
		SLATE_ATTRIBUTE(float, MaxRotationDegrees)
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs);

	static bool SupportsType(EJoystickType Type);

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
	int32 PaintWheel(const FGeometry& AllottedGeometry, FSlateWindowElementList& OutDrawElements, int32 LayerId) const;

	TAttribute<EJoystickType> JoystickType;
	TAttribute<float> AxisValue;
	TAttribute<float> MaxRotationDegrees;
};
