// JoystickPlugin is licensed under the MIT License.
// Copyright Jayden Maalouf 2026. All Rights Reserved.

#pragma once

#include "ForceFeedback/Types/ForceFeedbackPeriodicEffectType.h"
#include "Misc/Optional.h"
#include "Widgets/SCompoundWidget.h"

DECLARE_DELEGATE_OneParam(FOnHapticWaveformSelected, EForceFeedbackPeriodicEffectType /*WaveformType*/);

// The Haptic Effect Editor's row of waveform glyphs for a periodic effect. SelectedWaveform is which
// one is lit, unset when the effect being edited isn't periodic.
class SHapticWaveformSelector : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SHapticWaveformSelector)
		{
		}

		SLATE_ATTRIBUTE(TOptional<EForceFeedbackPeriodicEffectType>, SelectedWaveform)
		SLATE_EVENT(FOnHapticWaveformSelected, OnWaveformSelected)
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs);

private:
	TSharedRef<SWidget> MakeWaveformButton(EForceFeedbackPeriodicEffectType WaveformType, const FText& Tooltip);
	bool IsWaveformSelected(EForceFeedbackPeriodicEffectType WaveformType) const;

	TAttribute<TOptional<EForceFeedbackPeriodicEffectType>> SelectedWaveform;
	FOnHapticWaveformSelected OnWaveformSelected;
};
