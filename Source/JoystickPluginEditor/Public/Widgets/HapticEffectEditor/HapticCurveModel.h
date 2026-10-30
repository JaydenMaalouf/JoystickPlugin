// JoystickPlugin is licensed under the MIT License.
// Copyright Jayden Maalouf 2026. All Rights Reserved.

#pragma once

#include "CurveModel.h"
#include "Delegates/Delegate.h"
#include "Misc/Attribute.h"
#include "Runtime/Launch/Resources/Version.h"

DECLARE_DELEGATE_RetVal_OneParam(float, FOnEvaluateHapticEffect, float /*TimeMilliseconds*/);

// Read-only curve model that draws an effect's evaluation function (see ForceFeedbackEffectEvaluation.h) in UE's Curve Editor.
// The Curve Editor's time axis is in seconds, so the model converts to and from the editor's milliseconds.
class FHapticCurveModel : public FCurveModel
{
public:
	FHapticCurveModel();

	void BindEvaluate(FOnEvaluateHapticEffect InOnEvaluate) { OnEvaluate = MoveTemp(InOnEvaluate); }
	void SetDurationAttribute(TAttribute<int64> InDurationMilliseconds) { DurationMilliseconds = MoveTemp(InDurationMilliseconds); }

	virtual const void* GetCurve() const override;
	virtual void Modify() override;

	virtual void DrawCurve(const FCurveEditor& CurveEditor, const FCurveEditorScreenSpace& ScreenSpace, TArray<TTuple<double, double>>& InterpolatingPoints) const override;

#if ENGINE_MAJOR_VERSION == 5 && ENGINE_MINOR_VERSION >= 5
	virtual void GetKeys(double MinTime, double MaxTime, double MinValue, double MaxValue, TArray<FKeyHandle>& OutKeyHandles) const override;
#else
	virtual void GetKeys(const FCurveEditor& CurveEditor, double MinTime, double MaxTime, double MinValue, double MaxValue, TArray<FKeyHandle>& OutKeyHandles) const override;
#endif
	virtual void AddKeys(TArrayView<const FKeyPosition> InPositions, TArrayView<const FKeyAttributes> InAttributes, TArrayView<TOptional<FKeyHandle>>* OutKeyHandles = nullptr) override;
#if ENGINE_MAJOR_VERSION == 5 && ENGINE_MINOR_VERSION >= 6
	virtual void RemoveKeys(TArrayView<const FKeyHandle> InKeys, double InCurrentTime) override;
#else
	virtual void RemoveKeys(TArrayView<const FKeyHandle> InKeys) override;
#endif
	virtual void GetKeyPositions(TArrayView<const FKeyHandle> InKeys, TArrayView<FKeyPosition> OutKeyPositions) const override;
	virtual void SetKeyPositions(TArrayView<const FKeyHandle> InKeys, TArrayView<const FKeyPosition> InKeyPositions, EPropertyChangeType::Type ChangeType = EPropertyChangeType::Unspecified) override;
	virtual void GetKeyDrawInfo(ECurvePointType PointType, const FKeyHandle InKeyHandle, FKeyDrawInfo& OutDrawInfo) const override;

	virtual void GetTimeRange(double& MinTime, double& MaxTime) const override;
	virtual void GetValueRange(double& MinValue, double& MaxValue) const override;
	virtual int32 GetNumKeys() const override;
	virtual void GetNeighboringKeys(const FKeyHandle InKeyHandle, TOptional<FKeyHandle>& OutPreviousKeyHandle, TOptional<FKeyHandle>& OutNextKeyHandle) const override;

	virtual bool Evaluate(double InTime, double& OutValue) const override;

	virtual bool IsReadOnly() const override { return true; }

	// FCurveModel dims read-only curves by default, but this is a live preview and should stay at full colour.
	virtual FLinearColor GetColor() const override { return Color; }

private:
	// Hard ceiling on the pixel columns sampled per draw, so the cost is bounded by the panel rather than the effect.
	static constexpr int32 MaxColumns = 4096;

	// Sub-sample positions within a pixel column, deliberately uneven so they can't all land on the same phase of the waveform.
	static constexpr double ColumnSampleOffsets[] = {0.0, 0.2023, 0.2795, 0.4818, 0.5590, 0.6363, 0.8385, 0.9158};

	FOnEvaluateHapticEffect OnEvaluate;
	TAttribute<int64> DurationMilliseconds;
};
