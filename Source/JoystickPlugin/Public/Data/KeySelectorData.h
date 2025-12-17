// JoystickPlugin is licensed under the MIT License.
// Copyright Jayden Maalouf. All Rights Reserved.

#pragma once

#include "KeySelectorData.generated.h"

enum class EAxisSelectState : uint8
{
	Idle,
	FromMin,
	FromMax,
};

USTRUCT()
struct JOYSTICKPLUGIN_API FKeySelectorData
{
	GENERATED_BODY()

	EAxisSelectState State = EAxisSelectState::Idle;
	double StartTimeSeconds = 0.0;

	// Edge detection (so “held at min/max” doesn't keep re-triggering)
	bool bPrevAtMin = false;
	bool bPrevAtMax = false;

	// Track selection session boundaries
	bool bWasSelecting = false;

	void Reset()
	{
		State = EAxisSelectState::Idle;
		StartTimeSeconds = 0.0;
		bPrevAtMin = false;
		bPrevAtMax = false;
	}
};
