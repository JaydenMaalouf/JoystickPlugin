// JoystickPlugin is licensed under the MIT License.
// Copyright Jayden Maalouf 2026. All Rights Reserved.

#pragma once

#include "Math/Color.h"
#include "Styling/SlateWidgetStyle.h"

// The time and condition graphs, the playhead overlays and the Curve Editor's curve.
struct FHapticGraphStyle : FSlateWidgetStyle
{
	FHapticGraphStyle();

	static const FName TypeName;
	virtual const FName GetTypeName() const override { return TypeName; }
	static const FHapticGraphStyle& GetDefault();

	FLinearColor GridLineColor;
	FLinearColor ZeroLineColor;
	FLinearColor AxisLabelColor;

	// Drawn twice: a soft wide glow underneath a bright core line.
	FLinearColor CurveColor;
	FLinearColor CurveGlowColor;

	// The condition graph's stroke back towards the negative side, and the area between it and the curve.
	FLinearColor SecondaryCurveColor;
	FLinearColor SecondaryCurveGlowColor;
	FLinearColor LoopFillColor;

	// The playhead, and the condition graph's live input marker.
	FLinearColor MarkerColor;
};

// The Haptic Effect Editor's toggle buttons: effect type, waveform and condition type.
struct FHapticToggleStyle : FSlateWidgetStyle
{
	FHapticToggleStyle();

	static const FName TypeName;
	virtual const FName GetTypeName() const override { return TypeName; }
	static const FHapticToggleStyle& GetDefault();

	FLinearColor GetBackgroundColor(bool bIsSelected) const;
	FLinearColor GetForegroundColor(bool bIsSelected) const;

	FLinearColor SelectedBackgroundColor;
	FLinearColor UnselectedBackgroundColor;
	FLinearColor SelectedForegroundColor;
	FLinearColor UnselectedForegroundColor;

	// Icons sit a little dimmer than text when unselected.
	FLinearColor UnselectedIconColor;

	// For a toggle the selected device can't play, whether or not it is the selected one.
	FLinearColor UnsupportedForegroundColor;
};

struct FHapticTransportBarStyle : FSlateWidgetStyle
{
	FHapticTransportBarStyle();

	static const FName TypeName;
	virtual const FName GetTypeName() const override { return TypeName; }
	static const FHapticTransportBarStyle& GetDefault();

	FLinearColor PlayColor;
	FLinearColor StopColor;
	FLinearColor LoopOnColor;
	FLinearColor LoopOffColor;
};

struct FHapticDevicePreviewStyle : FSlateWidgetStyle
{
	FHapticDevicePreviewStyle();

	static const FName TypeName;
	virtual const FName GetTypeName() const override { return TypeName; }
	static const FHapticDevicePreviewStyle& GetDefault();

	FLinearColor BodyColor;

	// The fixed straight-ahead notch.
	FLinearColor ReferenceColor;

	// The stripe showing the device's rotation.
	FLinearColor MarkerColor;
};

struct FAxisBarStyle : FSlateWidgetStyle
{
	FAxisBarStyle();

	static const FName TypeName;
	virtual const FName GetTypeName() const override { return TypeName; }
	static const FAxisBarStyle& GetDefault();

	FLinearColor BorderColor;
	FLinearColor TrackColor;
	FLinearColor CentreLineColor;

	// Each fill brightens from its base colour towards its peak colour as the axis moves to its end.
	FLinearColor PositiveFillColor;
	FLinearColor PositiveFillPeakColor;
	FLinearColor NegativeFillColor;
	FLinearColor NegativeFillPeakColor;

	FLinearColor LabelColor;
};

struct FButtonBoxStyle : FSlateWidgetStyle
{
	FButtonBoxStyle();

	static const FName TypeName;
	virtual const FName GetTypeName() const override { return TypeName; }
	static const FButtonBoxStyle& GetDefault();

	FLinearColor PressedColor;
	FLinearColor ReleasedColor;
	FLinearColor PressedLabelColor;
	FLinearColor ReleasedLabelColor;
};

struct FHatSwitchStyle : FSlateWidgetStyle
{
	FHatSwitchStyle();

	static const FName TypeName;
	virtual const FName GetTypeName() const override { return TypeName; }
	static const FHatSwitchStyle& GetDefault();

	FLinearColor BackgroundColor;
	FLinearColor TrackColor;
	FLinearColor CentreDotColor;
	FLinearColor ActiveBallColor;
	FLinearColor CentredBallColor;
	FLinearColor LabelColor;
};

struct FBallSwitchStyle : FSlateWidgetStyle
{
	FBallSwitchStyle();

	static const FName TypeName;
	virtual const FName GetTypeName() const override { return TypeName; }
	static const FBallSwitchStyle& GetDefault();

	FLinearColor BackgroundColor;
	FLinearColor CentreDotColor;
	FLinearColor ActiveBallColor;
	FLinearColor CentredBallColor;
	FLinearColor LabelColor;
};
