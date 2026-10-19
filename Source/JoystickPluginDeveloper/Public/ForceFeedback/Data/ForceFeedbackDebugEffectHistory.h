// JoystickPlugin is licensed under the MIT License.
// Copyright Jayden Maalouf 2026. All Rights Reserved.

#pragma once

#include "Containers/Array.h"

struct FForceFeedbackDebugEffectHistory
{
	TArray<float> Samples;
	int32 Head = 0;
	uint64 LastFrame = 0;
};
