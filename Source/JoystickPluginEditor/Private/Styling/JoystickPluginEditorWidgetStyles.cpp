// JoystickPlugin is licensed under the MIT License.
// Copyright Jayden Maalouf 2026. All Rights Reserved.

#include "Styling/JoystickPluginEditorWidgetStyles.h"

// The colours themselves are set by FJoystickPluginEditorStyle. These defaults are only seen if a widget is given a style it doesn't have.

const FName FHapticGraphStyle::TypeName(TEXT("FHapticGraphStyle"));

FHapticGraphStyle::FHapticGraphStyle()
	: GridLineColor(FLinearColor::White)
	  , ZeroLineColor(FLinearColor::White)
	  , AxisLabelColor(FLinearColor::White)
	  , CurveColor(FLinearColor::White)
	  , CurveGlowColor(FLinearColor::White)
	  , SecondaryCurveColor(FLinearColor::White)
	  , SecondaryCurveGlowColor(FLinearColor::White)
	  , LoopFillColor(FLinearColor::White)
	  , MarkerColor(FLinearColor::White)
{
}

const FHapticGraphStyle& FHapticGraphStyle::GetDefault()
{
	static const FHapticGraphStyle Default;
	return Default;
}

const FName FHapticToggleStyle::TypeName(TEXT("FHapticToggleStyle"));

FHapticToggleStyle::FHapticToggleStyle()
	: SelectedBackgroundColor(FLinearColor::White)
	  , UnselectedBackgroundColor(FLinearColor::Transparent)
	  , SelectedForegroundColor(FLinearColor::White)
	  , UnselectedForegroundColor(FLinearColor::White)
	  , UnselectedIconColor(FLinearColor::White)
	  , UnsupportedForegroundColor(FLinearColor::White)
{
}

const FHapticToggleStyle& FHapticToggleStyle::GetDefault()
{
	static const FHapticToggleStyle Default;
	return Default;
}

FLinearColor FHapticToggleStyle::GetBackgroundColor(const bool bIsSelected) const
{
	return bIsSelected ? SelectedBackgroundColor : UnselectedBackgroundColor;
}

FLinearColor FHapticToggleStyle::GetForegroundColor(const bool bIsSelected) const
{
	return bIsSelected ? SelectedForegroundColor : UnselectedForegroundColor;
}

const FName FHapticTransportBarStyle::TypeName(TEXT("FHapticTransportBarStyle"));

FHapticTransportBarStyle::FHapticTransportBarStyle()
	: PlayColor(FLinearColor::White)
	  , StopColor(FLinearColor::White)
	  , LoopOnColor(FLinearColor::White)
	  , LoopOffColor(FLinearColor::White)
{
}

const FHapticTransportBarStyle& FHapticTransportBarStyle::GetDefault()
{
	static const FHapticTransportBarStyle Default;
	return Default;
}

const FName FHapticDevicePreviewStyle::TypeName(TEXT("FHapticDevicePreviewStyle"));

FHapticDevicePreviewStyle::FHapticDevicePreviewStyle()
	: BodyColor(FLinearColor::White)
	  , ReferenceColor(FLinearColor::White)
	  , MarkerColor(FLinearColor::White)
{
}

const FHapticDevicePreviewStyle& FHapticDevicePreviewStyle::GetDefault()
{
	static const FHapticDevicePreviewStyle Default;
	return Default;
}

const FName FAxisBarStyle::TypeName(TEXT("FAxisBarStyle"));

FAxisBarStyle::FAxisBarStyle()
	: BorderColor(FLinearColor::White)
	  , TrackColor(FLinearColor::White)
	  , CentreLineColor(FLinearColor::White)
	  , PositiveFillColor(FLinearColor::White)
	  , PositiveFillPeakColor(FLinearColor::White)
	  , NegativeFillColor(FLinearColor::White)
	  , NegativeFillPeakColor(FLinearColor::White)
	  , LabelColor(FLinearColor::White)
{
}

const FAxisBarStyle& FAxisBarStyle::GetDefault()
{
	static const FAxisBarStyle Default;
	return Default;
}

const FName FButtonBoxStyle::TypeName(TEXT("FButtonBoxStyle"));

FButtonBoxStyle::FButtonBoxStyle()
	: PressedColor(FLinearColor::White)
	  , ReleasedColor(FLinearColor::White)
	  , PressedLabelColor(FLinearColor::White)
	  , ReleasedLabelColor(FLinearColor::White)
{
}

const FButtonBoxStyle& FButtonBoxStyle::GetDefault()
{
	static const FButtonBoxStyle Default;
	return Default;
}

const FName FHatSwitchStyle::TypeName(TEXT("FHatSwitchStyle"));

FHatSwitchStyle::FHatSwitchStyle()
	: BackgroundColor(FLinearColor::White)
	  , TrackColor(FLinearColor::White)
	  , CentreDotColor(FLinearColor::White)
	  , ActiveBallColor(FLinearColor::White)
	  , CentredBallColor(FLinearColor::White)
	  , LabelColor(FLinearColor::White)
{
}

const FHatSwitchStyle& FHatSwitchStyle::GetDefault()
{
	static const FHatSwitchStyle Default;
	return Default;
}

const FName FBallSwitchStyle::TypeName(TEXT("FBallSwitchStyle"));

FBallSwitchStyle::FBallSwitchStyle()
	: BackgroundColor(FLinearColor::White)
	  , CentreDotColor(FLinearColor::White)
	  , ActiveBallColor(FLinearColor::White)
	  , CentredBallColor(FLinearColor::White)
	  , LabelColor(FLinearColor::White)
{
}

const FBallSwitchStyle& FBallSwitchStyle::GetDefault()
{
	static const FBallSwitchStyle Default;
	return Default;
}
