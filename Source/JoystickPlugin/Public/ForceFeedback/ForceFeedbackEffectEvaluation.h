// JoystickPlugin is licensed under the MIT License.
// Copyright Jayden Maalouf 2026. All Rights Reserved.

#pragma once

#include "ForceFeedback/Data/ForceFeedbackEffectConditionData.h"
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

	static float EvaluateConditionResponse(const FForceFeedbackEffectConditionData& EffectData, float Input);
	static float EvaluateConditionMotion(const FForceFeedbackEffectConditionData& EffectData, float Position, float Velocity, float Acceleration);

	// Splits the time since an effect was started into its iteration (from 1) and the time into that iteration's pass, after the start delay.
	// Returns false once every iteration has played. The duration must be above zero.
	static bool GetIterationProgress(int64 ElapsedMilliseconds, int64 DelayMilliseconds, int64 DurationMilliseconds, int64 Iterations, int64& OutIteration, int64& OutPassMilliseconds);
};
