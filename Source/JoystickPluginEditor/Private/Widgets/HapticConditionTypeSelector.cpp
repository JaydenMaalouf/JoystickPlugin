// JoystickPlugin is licensed under the MIT License.
// Copyright Jayden Maalouf 2026. All Rights Reserved.

#include "Widgets/HapticConditionTypeSelector.h"

#include "Styling/AppStyle.h"
#include "Styling/CoreStyle.h"
#include "Widgets/HapticEditorStyle.h"
#include "Widgets/SBoxPanel.h"
#include "Widgets/Input/SButton.h"
#include "Widgets/Layout/SBorder.h"
#include "Widgets/Text/STextBlock.h"

#define LOCTEXT_NAMESPACE "HapticEffectEditor"

void SHapticConditionTypeSelector::Construct(const FArguments& InArgs)
{
	SelectedConditionType = InArgs._SelectedConditionType;
	OnConditionTypeSelected = InArgs._OnConditionTypeSelected;
	IsConditionTypeSupported = InArgs._IsConditionTypeSupported;

	ChildSlot
	[
		SNew(SHorizontalBox)
		+ SHorizontalBox::Slot()
		.AutoWidth()
		.Padding(0, 0, 2, 0)
		[
			MakeConditionTypeButton(EForceFeedbackConditionEffectType::Spring, LOCTEXT("Spring", "Spring"), LOCTEXT("SpringTooltip", "Pushes back in proportion to how far the axis is from the centre."))
		]
		+ SHorizontalBox::Slot()
		.AutoWidth()
		.Padding(0, 0, 2, 0)
		[
			MakeConditionTypeButton(EForceFeedbackConditionEffectType::Damper, LOCTEXT("Damper", "Damper"), LOCTEXT("DamperTooltip", "Resists in proportion to how fast the axis is moving."))
		]
		+ SHorizontalBox::Slot()
		.AutoWidth()
		.Padding(0, 0, 2, 0)
		[
			MakeConditionTypeButton(EForceFeedbackConditionEffectType::Inertia, LOCTEXT("Inertia", "Inertia"), LOCTEXT("InertiaTooltip", "Resists in proportion to how quickly the axis speeds up or slows down."))
		]
		+ SHorizontalBox::Slot()
		.AutoWidth()
		[
			MakeConditionTypeButton(EForceFeedbackConditionEffectType::Friction, LOCTEXT("Friction", "Friction"), LOCTEXT("FrictionTooltip", "Resists with a constant force whenever the axis is moving."))
		]
	];
}

TSharedRef<SWidget> SHapticConditionTypeSelector::MakeConditionTypeButton(const EForceFeedbackConditionEffectType ConditionType, const FText& Label, const FText& Tooltip)
{
	return SNew(SButton)
		.ButtonStyle(FAppStyle::Get(), "NoBorder")
		.ContentPadding(0.0f)
		.IsFocusable(false)
		.IsEnabled_Lambda([this, ConditionType] { return IsSupported(ConditionType); })
		.ToolTipText_Lambda([this, ConditionType, Tooltip]
		{
			return IsSupported(ConditionType) ? Tooltip : FText::Format(LOCTEXT("UnsupportedConditionTypeTooltip", "{0} (not supported by the selected device)"), Tooltip);
		})
		.OnClicked_Lambda([this, ConditionType]
		{
			OnConditionTypeSelected.ExecuteIfBound(ConditionType);
			return FReply::Handled();
		})
		[
			SNew(SBorder)
			.BorderImage(FCoreStyle::Get().GetBrush("WhiteBrush"))
			.Padding(FMargin(6.0f, 3.0f))
			.BorderBackgroundColor_Lambda([this, ConditionType]
			{
				return FHapticEffectEditorColors::GetToggleBackgroundColor(IsConditionTypeSelected(ConditionType));
			})
			[
				SNew(STextBlock)
				.Text(Label)
				.TextStyle(FAppStyle::Get(), "NormalText")
				.ColorAndOpacity_Lambda([this, ConditionType]
				{
					return IsSupported(ConditionType) ? FHapticEffectEditorColors::GetToggleForegroundColor(IsConditionTypeSelected(ConditionType)) : FHapticEffectEditorColors::GetUnsupportedForegroundColor();
				})
			]
		];
}

bool SHapticConditionTypeSelector::IsConditionTypeSelected(const EForceFeedbackConditionEffectType ConditionType) const
{
	const TOptional<EForceFeedbackConditionEffectType> Selected = SelectedConditionType.Get();
	return Selected.IsSet() && Selected.GetValue() == ConditionType;
}

bool SHapticConditionTypeSelector::IsSupported(const EForceFeedbackConditionEffectType ConditionType) const
{
	return !IsConditionTypeSupported.IsBound() || IsConditionTypeSupported.Execute(ConditionType);
}

#undef LOCTEXT_NAMESPACE
