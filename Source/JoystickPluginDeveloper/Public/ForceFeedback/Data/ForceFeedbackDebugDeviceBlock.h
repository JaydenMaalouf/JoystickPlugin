// JoystickPlugin is licensed under the MIT License.
// Copyright Jayden Maalouf 2026. All Rights Reserved.

#pragma once

#include "ForceFeedback/Debug/ForceFeedbackEffectDebugInfo.h"

struct FForceFeedbackDebugDeviceBlock
{
	TArray<FForceFeedbackEffectDebugInfo> Effects;
	float WritesPerSecond = 0.0f;
	float EstimatedTotal = 0.0f;
};
