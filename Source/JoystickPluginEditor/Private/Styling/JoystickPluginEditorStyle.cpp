// JoystickPlugin is licensed under the MIT License.
// Copyright Jayden Maalouf 2026. All Rights Reserved.

#include "Styling/JoystickPluginEditorStyle.h"

#include "Brushes/SlateColorBrush.h"
#include "Styling/AppStyle.h"
#include "Styling/JoystickPluginEditorWidgetStyles.h"
#include "Styling/SlateStyleRegistry.h"
#include "Styling/SlateTypes.h"

TUniquePtr<FJoystickPluginEditorStyle> FJoystickPluginEditorStyle::Instance;

void FJoystickPluginEditorStyle::Register()
{
	if (Instance.IsValid())
	{
		return;
	}

	Instance = TUniquePtr<FJoystickPluginEditorStyle>(new FJoystickPluginEditorStyle());
	FSlateStyleRegistry::RegisterSlateStyle(*Instance);
}

void FJoystickPluginEditorStyle::Unregister()
{
	if (!Instance.IsValid())
	{
		return;
	}

	FSlateStyleRegistry::UnRegisterSlateStyle(*Instance);
	Instance.Reset();
}

const ISlateStyle& FJoystickPluginEditorStyle::Get()
{
	check(Instance.IsValid());
	return *Instance;
}

FJoystickPluginEditorStyle::FJoystickPluginEditorStyle()
	: FSlateStyleSet(TEXT("JoystickPluginEditorStyle"))
{
	SetHapticEffectEditorStyles();
	SetInputViewerStyles();
	SetDetailsStyles();
}

void FJoystickPluginEditorStyle::SetHapticEffectEditorStyles()
{
	FHapticGraphStyle GraphStyle;
	GraphStyle.GridLineColor = FLinearColor::White.CopyWithNewOpacity(0.06f);
	GraphStyle.ZeroLineColor = Grey40;
	GraphStyle.AxisLabelColor = Grey70.CopyWithNewOpacity(0.9f);
	GraphStyle.CurveColor = Green;
	GraphStyle.CurveGlowColor = Green.CopyWithNewOpacity(0.22f);
	GraphStyle.SecondaryCurveColor = Blue;
	GraphStyle.SecondaryCurveGlowColor = Blue.CopyWithNewOpacity(0.22f);
	GraphStyle.LoopFillColor = Green.CopyWithNewOpacity(0.12f);
	GraphStyle.MarkerColor = FLinearColor::White.CopyWithNewOpacity(0.95f);
	Set("HapticGraph", GraphStyle);

	FHapticToggleStyle ToggleStyle;
	ToggleStyle.SelectedBackgroundColor = BrightGreen;
	ToggleStyle.UnselectedBackgroundColor = FLinearColor::Transparent;
	ToggleStyle.SelectedForegroundColor = FLinearColor::White;
	ToggleStyle.UnselectedForegroundColor = Grey70;
	ToggleStyle.UnselectedIconColor = Grey60;
	ToggleStyle.UnsupportedForegroundColor = Grey25;
	Set("HapticToggle", ToggleStyle);

	FHapticTransportBarStyle TransportBarStyle;
	TransportBarStyle.PlayColor = BrightGreen;
	TransportBarStyle.StopColor = Red;
	TransportBarStyle.LoopOnColor = BrightGreen;
	TransportBarStyle.LoopOffColor = Grey60;
	Set("HapticTransportBar", TransportBarStyle);

	FHapticDevicePreviewStyle DevicePreviewStyle;
	DevicePreviewStyle.BodyColor = Grey70;
	DevicePreviewStyle.ReferenceColor = Grey40;
	DevicePreviewStyle.MarkerColor = BrightGreen;
	Set("HapticDevicePreview", DevicePreviewStyle);

	// Small labels naming a control, such as "DEVICE" beside the device picker.
	FTextBlockStyle CaptionText = FAppStyle::Get().GetWidgetStyle<FTextBlockStyle>("SmallText");
	CaptionText.SetColorAndOpacity(Grey60);
	Set("CaptionText", CaptionText);
}

void FJoystickPluginEditorStyle::SetInputViewerStyles()
{
	FAxisBarStyle AxisBarStyle;
	AxisBarStyle.BorderColor = Grey10;
	AxisBarStyle.TrackColor = Grey15;
	AxisBarStyle.CentreLineColor = Grey40;
	AxisBarStyle.PositiveFillColor = DimGreen;
	AxisBarStyle.PositiveFillPeakColor = BrightGreen;
	AxisBarStyle.NegativeFillColor = DimRed;
	AxisBarStyle.NegativeFillPeakColor = BrightRed;
	AxisBarStyle.LabelColor = FLinearColor::White;
	Set("AxisBar", AxisBarStyle);

	FButtonBoxStyle ButtonBoxStyle;
	ButtonBoxStyle.PressedColor = Green;
	ButtonBoxStyle.ReleasedColor = Grey25;
	ButtonBoxStyle.PressedLabelColor = FLinearColor::White;
	ButtonBoxStyle.ReleasedLabelColor = Grey70;
	Set("ButtonBox", ButtonBoxStyle);

	FHatSwitchStyle HatSwitchStyle;
	HatSwitchStyle.BackgroundColor = Grey15;
	HatSwitchStyle.TrackColor = Grey25;
	HatSwitchStyle.CentreDotColor = Grey50;
	HatSwitchStyle.ActiveBallColor = Green;
	HatSwitchStyle.CentredBallColor = Grey40;
	HatSwitchStyle.LabelColor = FLinearColor::White;
	Set("HatSwitch", HatSwitchStyle);

	FBallSwitchStyle BallSwitchStyle;
	BallSwitchStyle.BackgroundColor = Grey15;
	BallSwitchStyle.CentreDotColor = Grey50;
	BallSwitchStyle.ActiveBallColor = Green;
	BallSwitchStyle.CentredBallColor = Grey40;
	BallSwitchStyle.LabelColor = FLinearColor::White;
	Set("BallSwitch", BallSwitchStyle);

	// The engine's progress bar without its fill animation, so it shows the axis value as it is.
	FProgressBarStyle AxisProgressBar = FAppStyle::Get().GetWidgetStyle<FProgressBarStyle>("ProgressBar");
	AxisProgressBar.SetEnableFillAnimation(false);
	AxisProgressBar.FillImage.TintColor = BrightGreen;
	Set("AxisConfiguration.ProgressBar", AxisProgressBar);

	// Dims the viewer while a configuration dialog is open over it.
	Set("InputViewer.InputBlocker", new FSlateColorBrush(FLinearColor::Black.CopyWithNewOpacity(0.3f)));
}

void FJoystickPluginEditorStyle::SetDetailsStyles()
{
	// The connected device's name beside a property's own name.
	FTextBlockStyle HintText = FAppStyle::Get().GetWidgetStyle<FTextBlockStyle>("SmallText");
	HintText.SetColorAndOpacity(Grey45);
	Set("HintText", HintText);
}
