// JoystickPlugin is licensed under the MIT License.
// Copyright Jayden Maalouf 2026. All Rights Reserved.

#pragma once

#include "ForceFeedback/Types/ForceFeedbackConditionEffectType.h"
#include "Misc/Optional.h"
#include "Styling/JoystickPluginEditorStyle.h"
#include "Styling/JoystickPluginEditorWidgetStyles.h"
#include "Widgets/SCompoundWidget.h"

DECLARE_DELEGATE_OneParam(FOnHapticConditionTypeSelected, EForceFeedbackConditionEffectType /*ConditionType*/);
DECLARE_DELEGATE_RetVal_OneParam(bool, FIsHapticConditionTypeSupported, EForceFeedbackConditionEffectType /*ConditionType*/);

// The Haptic Effect Editor's row of Spring/Damper/Inertia/Friction segments. SelectedConditionType is unset when the effect isn't a condition.
// Condition types the selected device can't play are greyed out and can't be picked.
class SHapticConditionTypeSelector : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SHapticConditionTypeSelector)
			: _Style(&FJoystickPluginEditorStyle::Get().GetWidgetStyle<FHapticToggleStyle>("HapticToggle"))
		{
		}

		SLATE_STYLE_ARGUMENT(FHapticToggleStyle, Style)
		SLATE_ATTRIBUTE(TOptional<EForceFeedbackConditionEffectType>, SelectedConditionType)
		SLATE_EVENT(FOnHapticConditionTypeSelected, OnConditionTypeSelected)

		// Unbound treats every condition type as supported.
		SLATE_EVENT(FIsHapticConditionTypeSupported, IsConditionTypeSupported)
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs);

private:
	TSharedRef<SWidget> MakeConditionTypeButton(EForceFeedbackConditionEffectType ConditionType, const FText& Label, const FText& Tooltip);
	bool IsConditionTypeSelected(EForceFeedbackConditionEffectType ConditionType) const;
	bool IsSupported(EForceFeedbackConditionEffectType ConditionType) const;

	const FHapticToggleStyle* Style = nullptr;
	TAttribute<TOptional<EForceFeedbackConditionEffectType>> SelectedConditionType;
	FOnHapticConditionTypeSelected OnConditionTypeSelected;
	FIsHapticConditionTypeSupported IsConditionTypeSupported;
};
