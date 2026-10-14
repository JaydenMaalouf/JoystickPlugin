// JoystickPlugin is licensed under the MIT License.
// Copyright Jayden Maalouf 2026. All Rights Reserved.

#pragma once

#include "Misc/Optional.h"
#include "Widgets/SLeafWidget.h"

// The axis' state at an instant, normalised so position spans -1..1, velocity is in half-ranges per second and acceleration in half-ranges per second squared.
struct FHapticConditionMotion
{
	float Position = 0.0f;
	float Velocity = 0.0f;
	float Acceleration = 0.0f;
};

// Given the motion as the device sees it: acceleration is a fraction of the graph's full scale, clamped to -1..1, as the device clamps its own.
DECLARE_DELEGATE_RetVal_OneParam(float, FOnEvaluateHapticConditionMotion, const FHapticConditionMotion& /*Motion*/);

// What the condition graph's horizontal axis measures.
enum class EHapticConditionGraphAxis : uint8
{
	// Force against position while the axis is swept lock to lock and back. Each type traces its own loop: a spring a line, a damper an ellipse and friction a rectangle.
	// The area inside is the energy taken out per sweep.
	Position,

	// Force against acceleration, for inertia. Under the sweep acceleration only mirrors position, which would make inertia look like it depends on where the axis is.
	Acceleration
};

// The Haptic Effect Editor's graph for condition effects, whose force follows the axis rather than time.
class SHapticConditionGraph : public SLeafWidget
{
public:
	SLATE_BEGIN_ARGS(SHapticConditionGraph)
			: _Axis(EHapticConditionGraphAxis::Position)
			  , _SweepRate(1.0f)
			  , _FullScaleAcceleration(10.0f)
		{
		}

		SLATE_EVENT(FOnEvaluateHapticConditionMotion, OnEvaluate)
		SLATE_ATTRIBUTE(EHapticConditionGraphAxis, Axis)

		// The sweep's angular rate in radians per second, which is also its peak velocity. Only used against position.
		SLATE_ATTRIBUTE(float, SweepRate)

		// The acceleration the device treats as full scale, in half-ranges per second squared, which is also the graph's edge. Only used against acceleration.
		SLATE_ATTRIBUTE(float, FullScaleAcceleration)

		// The device's axis as it moves right now. Unset hides the marker and clears its trail.
		SLATE_ATTRIBUTE(TOptional<FHapticConditionMotion>, LiveMotion)
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs);

	virtual int32 OnPaint(
		const FPaintArgs& Args,
		const FGeometry& AllottedGeometry,
		const FSlateRect& MyCullingRect,
		FSlateWindowElementList& OutDrawElements,
		int32 LayerId,
		const FWidgetStyle& InWidgetStyle,
		bool bParentEnabled
	) const override;

	virtual void Tick(const FGeometry& AllottedGeometry, double InCurrentTime, float InDeltaTime) override;
	virtual FVector2D ComputeDesiredSize(float) const override;

private:
	void PaintGrid(const FGeometry& AllottedGeometry, FSlateWindowElementList& OutDrawElements, int32 LayerId) const;
	void PaintLoop(const FGeometry& AllottedGeometry, FSlateWindowElementList& OutDrawElements, int32 LayerId) const;
	void PaintAccelerationResponse(const FGeometry& AllottedGeometry, FSlateWindowElementList& OutDrawElements, int32 LayerId) const;
	void PaintLiveMotion(const FGeometry& AllottedGeometry, FSlateWindowElementList& OutDrawElements, int32 LayerId) const;

	// Maps a horizontal value and force, both normalised to -1..1, to a point inside the plot's margins.
	static FVector2D ToLocal(const FVector2D& Size, float Input, float Force);

	bool IsPlottingAcceleration() const;

	// The value at the horizontal axis' right edge, which the plot divides by to normalise.
	float GetInputRange() const;

	// The acceleration the device treats as full scale, kept above zero.
	float GetFullScaleAcceleration() const;

	// Where a motion sits on the horizontal axis, normalised to -1..1.
	float GetNormalisedInput(const FHapticConditionMotion& Motion) const;

	// The sweep x = sin(Rate * t) at Position, on the stroke towards the positive side or back towards the negative side.
	static FHapticConditionMotion GetSweepMotion(float Position, float Rate, bool bMovingPositive);

	float Evaluate(const FHapticConditionMotion& Motion) const;

	FOnEvaluateHapticConditionMotion OnEvaluate;
	TAttribute<EHapticConditionGraphAxis> Axis;
	TAttribute<float> SweepRate;
	TAttribute<float> FullScaleAcceleration;
	TAttribute<TOptional<FHapticConditionMotion>> LiveMotion;

	// The live motion over the last moment, oldest first, with the time each was taken. The force is evaluated at paint time so edits apply to the trail too.
	TArray<TPair<double, FHapticConditionMotion>> LiveTrail;
};
