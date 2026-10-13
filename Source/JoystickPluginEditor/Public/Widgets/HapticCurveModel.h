// JoystickPlugin is licensed under the MIT License.
// Copyright Jayden Maalouf 2026. All Rights Reserved.

#pragma once

#include "CurveModel.h"
#include "Delegates/Delegate.h"
#include "Misc/Attribute.h"
#include "Runtime/Launch/Resources/Version.h"

DECLARE_DELEGATE_RetVal_OneParam(float, FOnEvaluateHapticEffect, float /*TimeSeconds*/);

// Adapts a JoystickPlugin force feedback effect's pure evaluation function (see
// ForceFeedbackEffectEvaluation.h) to UE's Curve Editor instead of baking it to an FRichCurve.
// DrawCurve samples per pixel column of whatever time range is actually visible/zoomed and keeps
// each column's min/max, rather than a fixed sample count spread across the whole effect duration -
// the latter is what caused the old hand-rolled SHapticWaveformGraph to alias into a beat pattern on
// long, high-frequency periodic effects. The cost is set by the panel's width, never by the effect's
// duration or iteration count.
// Read-only: nothing here is authored by dragging points, that's still the details panel's job for
// the built-in Sine/Triangle/Square/etc waveforms. A future custom/point-drawn effect kind would add
// a second, editable FCurveModel and host it through the same panel.
class FHapticCurveModel : public FCurveModel
{
public:
	FHapticCurveModel();

	void BindEvaluate(FOnEvaluateHapticEffect InOnEvaluate) { OnEvaluate = MoveTemp(InOnEvaluate); }
	void SetDurationAttribute(TAttribute<float> InDurationSeconds) { DurationSeconds = MoveTemp(InDurationSeconds); }

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

	// FCurveModel::GetColor() desaturates Color by default when IsReadOnly() is true (dimming
	// "can't drag keys" curves), but that's not the intent here - this is a live oscilloscope-style
	// preview, not a disabled/inactive control, so it should stay at full vibrancy.
	virtual FLinearColor GetColor() const override { return Color; }

private:
	FOnEvaluateHapticEffect OnEvaluate;
	TAttribute<float> DurationSeconds;
};
