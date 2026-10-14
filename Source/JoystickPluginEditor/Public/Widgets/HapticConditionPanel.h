// JoystickPlugin is licensed under the MIT License.
// Copyright Jayden Maalouf 2026. All Rights Reserved.

#pragma once

#include "ForceFeedback/Types/ForceFeedbackConditionEffectType.h"
#include "Misc/Optional.h"
#include "Widgets/HapticConditionGraph.h"
#include "Widgets/SCompoundWidget.h"

// The Haptic Effect Editor's view of a condition effect: its graph against the axis, with the sweep rate (or inertia's full scale) the graph is drawn at.
class SHapticConditionPanel : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SHapticConditionPanel)
		{
		}

		SLATE_EVENT(FOnEvaluateHapticConditionMotion, OnEvaluate)

		// Unset is treated as a spring.
		SLATE_ATTRIBUTE(TOptional<EForceFeedbackConditionEffectType>, ConditionType)

		// The device's axis as it moves right now. Unset hides the graph's live marker.
		SLATE_ATTRIBUTE(TOptional<FHapticConditionMotion>, LiveMotion)
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs);

private:
	EForceFeedbackConditionEffectType GetConditionType() const;

	// Inertia responds to acceleration rather than position, so the graph plots it against that instead.
	bool IsInertiaCondition() const;
	FText GetSweepCaption() const;

	// The graph's sweep rate, in radians per second.
	static constexpr float MinSweepRate = 0.1f;
	static constexpr float MaxSweepRate = 4.0f;

	// The acceleration the device treats as full scale for inertia, in half-ranges per second squared. A quick flick of a wheel by hand reaches well past 10.
	static constexpr float MinFullScaleAcceleration = 0.5f;
	static constexpr float MaxFullScaleAcceleration = 100.0f;

	TAttribute<TOptional<EForceFeedbackConditionEffectType>> ConditionType;

	// How fast the graph sweeps the axis, in radians per second. See SHapticConditionGraph.
	float SweepRate = 1.0f;

	// The acceleration the device treats as full scale for inertia. See SHapticConditionGraph.
	float FullScaleAcceleration = 10.0f;
};
