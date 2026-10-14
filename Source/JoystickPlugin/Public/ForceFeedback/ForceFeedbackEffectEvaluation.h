// JoystickPlugin is licensed under the MIT License.
// Copyright Jayden Maalouf 2026. All Rights Reserved.

#pragma once

#include "ForceFeedback/Data/ForceFeedbackEffectConstantData.h"
#include "ForceFeedback/Data/ForceFeedbackEffectLeftRightData.h"
#include "ForceFeedback/Data/ForceFeedbackEffectPeriodicData.h"
#include "ForceFeedback/Data/ForceFeedbackEffectRampData.h"

// Pure functions describing an effect's force output at a point in time, in the same units UpdateEffectData() uploads to SDL.
struct JOYSTICKPLUGIN_API FForceFeedbackEffectEvaluation
{
	static float EvaluateEnvelope(const FForceFeedbackEffectEnvelopeData& EnvelopeData, const FForceFeedbackEffectDurationData& DurationData, float TimeSeconds);
	static float EvaluatePeriodic(const FForceFeedbackEffectPeriodicData& EffectData, float TimeSeconds);
	static float EvaluateConstant(const FForceFeedbackEffectConstantData& EffectData, float TimeSeconds);
	static float EvaluateRamp(const FForceFeedbackEffectRampData& EffectData, float TimeSeconds);
	static float EvaluateLeftRight(const FForceFeedbackEffectLeftRightData& EffectData, float TimeSeconds);
};
