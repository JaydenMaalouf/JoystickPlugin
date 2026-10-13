// JoystickPlugin is licensed under the MIT License.
// Copyright Jayden Maalouf 2026. All Rights Reserved.

#include "Widgets/HapticCurveModel.h"

#include "CurveEditorScreenSpace.h"
#include "CurveDataAbstraction.h"
#include "CurveDrawInfo.h"

struct FHapticCurveModelSampling
{
	// Hard ceiling on the pixel columns sampled per draw, so the work stays bounded by the panel
	// rather than by the effect (an infinite effect, or a view zoomed out over hours, costs the same
	// as a one second one).
	static constexpr int32 MaxColumns = 4096;

	// Where inside a pixel column each sub-sample is taken, as a fraction of the column's width.
	// Deliberately uneven (stratified by the golden ratio): evenly spaced samples all land on the
	// same phase whenever their spacing is a multiple of half the waveform's period, which flattens
	// the curve to a straight line at that particular zoom level.
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

	// Oscilloscope-style min/max decimation: every pixel column across the visible range is sampled
	// several times and contributes its lowest and highest value, in the order they occur. Zoomed in,
	// that traces the waveform to a fraction of a pixel; zoomed out past the point where a cycle fits
	// in a column, it fills the band between the true peaks rather than aliasing.
	// This replaces a port of FRichCurveEditorModel's bisecting refine, which only works when seeded
	// with a curve's keys - seeded with just the two view edges, its three probes per segment could
	// all land on zero crossings of a periodic effect and declare the whole window a straight line.
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
			const double Value = FMath::Clamp(OnEvaluate.Execute(static_cast<float>(SampleSeconds)), -1.0f, 1.0f);

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

	InterpolatingPoints.Add(MakeTuple(EndTimeSeconds, static_cast<double>(FMath::Clamp(OnEvaluate.Execute(static_cast<float>(EndTimeSeconds)), -1.0f, 1.0f))));
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
	MaxTime = DurationSeconds.Get(1.0f);
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

	OutValue = FMath::Clamp(OnEvaluate.Execute(static_cast<float>(InTime)), -1.0f, 1.0f);
	return true;
}
