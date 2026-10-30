// JoystickPlugin is licensed under the MIT License.
// Copyright Jayden Maalouf 2026. All Rights Reserved.

#pragma once

#include "Menus/HapticEffectEditorTypes.h"
#include "Styling/JoystickPluginEditorStyle.h"
#include "Styling/JoystickPluginEditorWidgetStyles.h"
#include "Widgets/SCompoundWidget.h"

DECLARE_DELEGATE_OneParam(FOnHapticEffectKindSelected, EHapticPreviewEffectKind /*Kind*/);
DECLARE_DELEGATE_RetVal_OneParam(bool, FIsHapticEffectKindSupported, EHapticPreviewEffectKind /*Kind*/);

// The Haptic Effect Editor's row of Periodic/Constant/Ramp/Condition/Left-Right segments. Picking one only reports it to the owner.
// Kinds the selected device can't play are greyed out and can't be picked.
class SHapticEffectTypeSelector : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SHapticEffectTypeSelector)
			: _Style(&FJoystickPluginEditorStyle::Get().GetWidgetStyle<FHapticToggleStyle>("HapticToggle"))
			  , _EffectKind(EHapticPreviewEffectKind::Periodic)
		{
		}

		SLATE_STYLE_ARGUMENT(FHapticToggleStyle, Style)
		SLATE_ATTRIBUTE(EHapticPreviewEffectKind, EffectKind)
		SLATE_EVENT(FOnHapticEffectKindSelected, OnEffectKindSelected)

		// Unbound treats every kind as supported.
		SLATE_EVENT(FIsHapticEffectKindSupported, IsEffectKindSupported)
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs);

private:
	TSharedRef<SWidget> MakeTypeButton(const FText& Label, EHapticPreviewEffectKind Kind);
	bool IsKindSupported(EHapticPreviewEffectKind Kind) const;

	const FHapticToggleStyle* Style = nullptr;
	TAttribute<EHapticPreviewEffectKind> EffectKind;
	FOnHapticEffectKindSelected OnEffectKindSelected;
	FIsHapticEffectKindSupported IsEffectKindSupported;
};
