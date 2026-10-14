// JoystickPlugin is licensed under the MIT License.
// Copyright Jayden Maalouf 2026. All Rights Reserved.

#include "Widgets/HapticConditionPanel.h"

#include "Styling/AppStyle.h"
#include "Widgets/SBoxPanel.h"
#include "Widgets/Input/SSpinBox.h"
#include "Widgets/Layout/SBorder.h"
#include "Widgets/Layout/SBox.h"
#include "Widgets/Text/STextBlock.h"

#define LOCTEXT_NAMESPACE "HapticEffectEditor"

void SHapticConditionPanel::Construct(const FArguments& InArgs)
{
	ConditionType = InArgs._ConditionType;

	ChildSlot
	[
		SNew(SBorder)
		.BorderImage(FAppStyle::GetBrush("ToolPanel.GroupBorder"))
		.Padding(4.0f)
		[
			SNew(SVerticalBox)
			+ SVerticalBox::Slot()
			.AutoHeight()
			.Padding(4, 2, 4, 6)
			[
				// A spring only responds to position, so its line is the same at any sweep rate. Inertia is graphed against acceleration, so it sets that range instead.
				SNew(SHorizontalBox)
				.Visibility_Lambda([this] { return GetConditionType() == EForceFeedbackConditionEffectType::Spring ? EVisibility::Collapsed : EVisibility::Visible; })
				+ SHorizontalBox::Slot()
				.AutoWidth()
				.VAlign(VAlign_Center)
				.Padding(0, 0, 8, 0)
				[
					SNew(STextBlock)
					.Text_Lambda([this] { return IsInertiaCondition() ? LOCTEXT("FullScaleAccelerationLabel", "FULL SCALE") : LOCTEXT("SweepRateLabel", "PREVIEW SPEED"); })
					.TextStyle(FAppStyle::Get(), "SmallText")
					.ColorAndOpacity(FLinearColor(0.6f, 0.6f, 0.6f, 1.0f))
				]
				+ SHorizontalBox::Slot()
				.AutoWidth()
				.VAlign(VAlign_Center)
				.Padding(0, 0, 12, 0)
				[
					SNew(SBox)
					.WidthOverride(80.0f)
					[
						SNew(SSpinBox<float>)
						.MinValue_Lambda([this] { return TOptional<float>(IsInertiaCondition() ? MinFullScaleAcceleration : MinSweepRate); })
						.MaxValue_Lambda([this] { return TOptional<float>(IsInertiaCondition() ? MaxFullScaleAcceleration : MaxSweepRate); })
						.Delta_Lambda([this] { return IsInertiaCondition() ? 0.5f : 0.05f; })
						.MaxFractionalDigits(2)
						.ToolTipText_Lambda([this]
						{
							return IsInertiaCondition()
								       ? LOCTEXT("FullScaleAccelerationTooltip",
								                 "How sharp a change in speed the device treats as the strongest inertia. Devices don't report this, so it's a best guess.\nRaise it if the live marker keeps hitting the graph's edge when you move the axis, lower it if the marker barely moves. This only changes the graph, not the effect.")
								       : LOCTEXT("SweepRateTooltip",
								                 "How fast the graph's preview turns the axis from one end to the other and back. Higher is faster.\nThis only changes the graph, not the effect.");
						})
						.Value_Lambda([this] { return IsInertiaCondition() ? FullScaleAcceleration : SweepRate; })
						.OnValueChanged_Lambda([this](const float NewValue)
						{
							if (IsInertiaCondition())
							{
								FullScaleAcceleration = NewValue;
							}
							else
							{
								SweepRate = NewValue;
							}
						})
					]
				]
				+ SHorizontalBox::Slot()
				.FillWidth(1.0f)
				.VAlign(VAlign_Center)
				[
					SNew(STextBlock)
					.Text_Lambda([this] { return GetSweepCaption(); })
					.TextStyle(FAppStyle::Get(), "SmallText")
					.ColorAndOpacity(FLinearColor(0.6f, 0.6f, 0.6f, 1.0f))
					.AutoWrapText(true)
				]
			]
			+ SVerticalBox::Slot()
			.FillHeight(1.0f)
			[
				SNew(SHapticConditionGraph)
				.OnEvaluate(InArgs._OnEvaluate)
				.Axis_Lambda([this] { return IsInertiaCondition() ? EHapticConditionGraphAxis::Acceleration : EHapticConditionGraphAxis::Position; })
				.SweepRate_Lambda([this] { return SweepRate; })
				.FullScaleAcceleration_Lambda([this] { return FullScaleAcceleration; })
				.LiveMotion(InArgs._LiveMotion)
			]
		]
	];
}

EForceFeedbackConditionEffectType SHapticConditionPanel::GetConditionType() const
{
	return ConditionType.Get().Get(EForceFeedbackConditionEffectType::Spring);
}

bool SHapticConditionPanel::IsInertiaCondition() const
{
	return GetConditionType() == EForceFeedbackConditionEffectType::Inertia;
}

FText SHapticConditionPanel::GetSweepCaption() const
{
	if (IsInertiaCondition())
	{
		return LOCTEXT("InertiaCaption",
		               "Inertia only pushes back while the axis is speeding up or slowing down, so turning at a steady speed feels like nothing. The graph's edges are the acceleration set in Full Scale.");
	}

	FNumberFormattingOptions SecondsFormat;
	SecondsFormat.SetMaximumFractionalDigits(1);

	const FText SpeedEffect = GetConditionType() == EForceFeedbackConditionEffectType::Damper
		                          ? LOCTEXT("DamperSweepEffect", "A damper resists harder the faster the axis moves, so a higher speed makes the loop taller.")
		                          : LOCTEXT("FrictionSweepEffect", "Friction resists the same at any speed, so changing this won't change its shape.");

	// The round trip is one period of the sweep's sine.
	return FText::Format(
		LOCTEXT("SweepCaption", "The graph shows the force while the axis is turned from one end to the other and back, taking {0}s for the round trip. {1}"),
		FText::AsNumber(2.0f * PI / FMath::Max(SweepRate, KINDA_SMALL_NUMBER), &SecondsFormat),
		SpeedEffect);
}

#undef LOCTEXT_NAMESPACE
