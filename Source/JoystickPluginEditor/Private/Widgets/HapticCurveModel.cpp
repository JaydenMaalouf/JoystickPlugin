// JoystickPlugin is licensed under the MIT License.
// Copyright Jayden Maalouf 2026. All Rights Reserved.

#include "Widgets/HapticCurveModel.h"

#include "CurveEditorScreenSpace.h"
#include "CurveDataAbstraction.h"
#include "CurveDrawInfo.h"

struct FHapticCurveModelSampling
{
	// Hard ceiling on the pixel columns sampled per draw, so the cost is bounded by the panel rather than the effect.
	static constexpr int32 MaxColumns = 4096;

	// Sub-sample positions within a pixel column, deliberately uneven so they can't all land on the same phase of the waveform.
	static constexpr double ColumnSampleOffsets[] = {0.0, 0.2023, 0.2795, 0.4818, 0.5590, 0.6363, 0.8385, 0.9158};
};

FHapticCurveModel::FHapticCurveModel()
{
	// Matches the bright green the old SHapticWaveformGraph used for its curve.
	SetColor(FLinearColor(0.2f, 0.9f, 0.3f, 1.0f));
}

const void* FHapticCurveModel::GetCurve() const
{
	return nullptr;
}

void FHapticCurveModel::Modify()
{
}

void FHapticCurveModel::DrawCurve(const FCurveEditor& CurveEditor, const FCurveEditorScreenSpace& ScreenSpace, TArray<TTuple<double, double>>& InterpolatingPoints) const
{
	if (!OnEvaluate.IsBound())
	{
		return;
	}

	const double StartTimeSeconds = ScreenSpace.GetInputMin();
	const double EndTimeSeconds = ScreenSpace.GetInputMax();
	if (EndTimeSeconds <= StartTimeSeconds)
	{
		return;
	}

	// Oscilloscope-style min/max decimation: each pixel column is sampled several times and contributes its lowest and highest value.
	const int32 NumColumns = FMath::Clamp(FMath::CeilToInt32(ScreenSpace.GetPhysicalWidth()), 1, FHapticCurveModelSampling::MaxColumns);
	const double ColumnSeconds = (EndTimeSeconds - StartTimeSeconds) / NumColumns;

	InterpolatingPoints.Reserve(InterpolatingPoints.Num() + NumColumns * 2 + 1);

	for (int32 ColumnIndex = 0; ColumnIndex < NumColumns; ++ColumnIndex)
	{
		const double ColumnStartSeconds = StartTimeSeconds + ColumnIndex * ColumnSeconds;

		TTuple<double, double> Lowest(0.0, TNumericLimits<double>::Max());
		TTuple<double, double> Highest(0.0, TNumericLimits<double>::Lowest());

		for (const double SampleOffset : FHapticCurveModelSampling::ColumnSampleOffsets)
		{
			const double SampleSeconds = ColumnStartSeconds + SampleOffset * ColumnSeconds;
			const double Value = FMath::Clamp(OnEvaluate.Execute(static_cast<float>(SampleSeconds * 1000.0)), -1.0f, 1.0f);

			if (Value < Lowest.Get<1>())
			{
				Lowest = MakeTuple(SampleSeconds, Value);
			}

			if (Value > Highest.Get<1>())
			{
				Highest = MakeTuple(SampleSeconds, Value);
			}
		}

		if (Lowest.Get<0>() == Highest.Get<0>())
		{
			InterpolatingPoints.Add(Lowest);
		}
		else if (Lowest.Get<0>() < Highest.Get<0>())
		{
			InterpolatingPoints.Add(Lowest);
			InterpolatingPoints.Add(Highest);
		}
		else
		{
			InterpolatingPoints.Add(Highest);
			InterpolatingPoints.Add(Lowest);
		}
	}

	InterpolatingPoints.Add(MakeTuple(EndTimeSeconds, static_cast<double>(FMath::Clamp(OnEvaluate.Execute(static_cast<float>(EndTimeSeconds * 1000.0)), -1.0f, 1.0f))));
}

#if ENGINE_MAJOR_VERSION == 5 && ENGINE_MINOR_VERSION >= 5
void FHapticCurveModel::GetKeys(double MinTime, double MaxTime, double MinValue, double MaxValue, TArray<FKeyHandle>& OutKeyHandles) const
#else
void FHapticCurveModel::GetKeys(const FCurveEditor& CurveEditor, double MinTime, double MaxTime, double MinValue, double MaxValue, TArray<FKeyHandle>& OutKeyHandles) const
#endif
{
	OutKeyHandles.Reset();
}

void FHapticCurveModel::AddKeys(TArrayView<const FKeyPosition> InPositions, TArrayView<const FKeyAttributes> InAttributes, TArrayView<TOptional<FKeyHandle>>* OutKeyHandles)
{
}

#if ENGINE_MAJOR_VERSION == 5 && ENGINE_MINOR_VERSION >= 6
void FHapticCurveModel::RemoveKeys(TArrayView<const FKeyHandle> InKeys, double InCurrentTime)
#else
void FHapticCurveModel::RemoveKeys(TArrayView<const FKeyHandle> InKeys)
#endif
{
}

void FHapticCurveModel::GetKeyPositions(TArrayView<const FKeyHandle> InKeys, TArrayView<FKeyPosition> OutKeyPositions) const
{
}

void FHapticCurveModel::SetKeyPositions(TArrayView<const FKeyHandle> InKeys, TArrayView<const FKeyPosition> InKeyPositions, EPropertyChangeType::Type ChangeType)
{
}

void FHapticCurveModel::GetKeyDrawInfo(ECurvePointType PointType, const FKeyHandle InKeyHandle, FKeyDrawInfo& OutDrawInfo) const
{
}

void FHapticCurveModel::GetTimeRange(double& MinTime, double& MaxTime) const
{
	MinTime = 0.0;
	MaxTime = DurationMilliseconds.Get(1000) / 1000.0;
}

void FHapticCurveModel::GetValueRange(double& MinValue, double& MaxValue) const
{
	MinValue = -1.0;
	MaxValue = 1.0;
}

int32 FHapticCurveModel::GetNumKeys() const
{
	return 0;
}

void FHapticCurveModel::GetNeighboringKeys(const FKeyHandle InKeyHandle, TOptional<FKeyHandle>& OutPreviousKeyHandle, TOptional<FKeyHandle>& OutNextKeyHandle) const
{
	OutPreviousKeyHandle.Reset();
	OutNextKeyHandle.Reset();
}

bool FHapticCurveModel::Evaluate(double InTime, double& OutValue) const
{
	if (!OnEvaluate.IsBound())
	{
		OutValue = 0.0;
		return false;
	}

	OutValue = FMath::Clamp(OnEvaluate.Execute(static_cast<float>(InTime * 1000.0)), -1.0f, 1.0f);
	return true;
}
