// JoystickPlugin is licensed under the MIT License.
// Copyright Jayden Maalouf 2026. All Rights Reserved.

#pragma once

#include "Menus/HapticEffectEditorTypes.h"
#include "Widgets/SCompoundWidget.h"

DECLARE_DELEGATE_OneParam(FOnHapticEffectKindSelected, EHapticPreviewEffectKind /*Kind*/);

// The Haptic Effect Editor's row of Periodic/Constant/Ramp/Left-Right segments. EffectKind is which
// one is lit - picking one only reports it, leaving the owner to decide whether the kind changes.
class SHapticEffectTypeSelector : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SHapticEffectTypeSelector)
			: _EffectKind(EHapticPreviewEffectKind::Periodic)
		{
		}

		SLATE_ATTRIBUTE(EHapticPreviewEffectKind, EffectKind)
		SLATE_EVENT(FOnHapticEffectKindSelected, OnEffectKindSelected)
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs);

private:
	TSharedRef<SWidget> MakeTypeButton(const FText& Label, EHapticPreviewEffectKind Kind);

	TAttribute<EHapticPreviewEffectKind> EffectKind;
	FOnHapticEffectKindSelected OnEffectKindSelected;
};
