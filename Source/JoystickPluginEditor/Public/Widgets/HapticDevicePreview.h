// JoystickPlugin is licensed under the MIT License.
// Copyright Jayden Maalouf 2026. All Rights Reserved.

#pragma once

#include "Data/JoystickType.h"
#include "Styling/JoystickPluginEditorStyle.h"
#include "Styling/JoystickPluginEditorWidgetStyles.h"
#include "Widgets/SLeafWidget.h"

// 2D top-down drawing of the selected device, posed from a normalised -1..1 axis value. Only wheels are drawn so far (see SupportsType).
class SHapticDevicePreview : public SLeafWidget
{
public:
	SLATE_BEGIN_ARGS(SHapticDevicePreview)
			: _Style(&FJoystickPluginEditorStyle::Get().GetWidgetStyle<FHapticDevicePreviewStyle>("HapticDevicePreview"))
			  , _JoystickType(EJoystickType::Unknown)
			  , _AxisValue(0.0f)
			  , _MaxRotationDegrees(540.0f)
		{
		}

		SLATE_STYLE_ARGUMENT(FHapticDevicePreviewStyle, Style)
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

	const FHapticDevicePreviewStyle* Style = nullptr;
	TAttribute<EJoystickType> JoystickType;
	TAttribute<float> AxisValue;
	TAttribute<float> MaxRotationDegrees;
};
