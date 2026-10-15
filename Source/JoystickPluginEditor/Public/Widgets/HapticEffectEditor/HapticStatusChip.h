// JoystickPlugin is licensed under the MIT License.
// Copyright Jayden Maalouf 2026. All Rights Reserved.

#pragma once

#include "Misc/Optional.h"
#include "Widgets/SCompoundWidget.h"

// The Haptic Effect Editor's playback readouts: the playhead, the time since playback started, the force at the playhead and the iteration. Holds no playback state of its own.
class SHapticStatusChip : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SHapticStatusChip)
			: _PlayheadTimeMilliseconds(0)
			  , _ElapsedTimeMilliseconds(0)
			  , _Force(0.0f)
			  , _Iteration(1)
		{
		}

		// The position within the current pass.
		SLATE_ATTRIBUTE(int64, PlayheadTimeMilliseconds)

		// Time since playback started, across every iteration.
		SLATE_ATTRIBUTE(int64, ElapsedTimeMilliseconds)
		SLATE_ATTRIBUTE(float, Force)

		// The pass the playhead is on, counted from one.
		SLATE_ATTRIBUTE(int64, Iteration)

		// Unset while there is no last iteration to count towards, which shows an infinity sign instead.
		SLATE_ATTRIBUTE(TOptional<int64>, Iterations)
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs);

private:
	TAttribute<int64> PlayheadTimeMilliseconds;
	TAttribute<int64> ElapsedTimeMilliseconds;
	TAttribute<float> Force;
	TAttribute<int64> Iteration;
	TAttribute<TOptional<int64>> Iterations;
};
