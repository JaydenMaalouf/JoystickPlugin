// JoystickPlugin is licensed under the MIT License.
// Copyright Jayden Maalouf 2026. All Rights Reserved.

#pragma once

#include "ForceFeedback/Types/ForceFeedbackPeriodicEffectType.h"
#include "Misc/Optional.h"
#include "Styling/JoystickPluginEditorStyle.h"
#include "Styling/JoystickPluginEditorWidgetStyles.h"
#include "Widgets/SCompoundWidget.h"

DECLARE_DELEGATE_OneParam(FOnHapticWaveformSelected, EForceFeedbackPeriodicEffectType /*WaveformType*/);
DECLARE_DELEGATE_RetVal_OneParam(bool, FIsHapticWaveformSupported, EForceFeedbackPeriodicEffectType /*WaveformType*/);

// The Haptic Effect Editor's row of waveform glyphs. SelectedWaveform is unset when the effect isn't periodic.
// Waveforms the selected device can't play are greyed out and can't be picked.
class SHapticWaveformSelector : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SHapticWaveformSelector)
			: _Style(&FJoystickPluginEditorStyle::Get().GetWidgetStyle<FHapticToggleStyle>("HapticToggle"))
		{
		}

		SLATE_STYLE_ARGUMENT(FHapticToggleStyle, Style)
		SLATE_ATTRIBUTE(TOptional<EForceFeedbackPeriodicEffectType>, SelectedWaveform)
		SLATE_EVENT(FOnHapticWaveformSelected, OnWaveformSelected)

		// Unbound treats every waveform as supported.
		SLATE_EVENT(FIsHapticWaveformSupported, IsWaveformSupported)
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs);

private:
	TSharedRef<SWidget> MakeWaveformButton(EForceFeedbackPeriodicEffectType WaveformType, const FText& Tooltip);
	bool IsWaveformSelected(EForceFeedbackPeriodicEffectType WaveformType) const;
	bool IsSupported(EForceFeedbackPeriodicEffectType WaveformType) const;

	const FHapticToggleStyle* Style = nullptr;
	TAttribute<TOptional<EForceFeedbackPeriodicEffectType>> SelectedWaveform;
	FOnHapticWaveformSelected OnWaveformSelected;
	FIsHapticWaveformSupported IsWaveformSupported;
};
