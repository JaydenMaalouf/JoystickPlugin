// JoystickPlugin is licensed under the MIT License.
// Copyright Jayden Maalouf 2026. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Data/JoystickInstanceId.h"
#include "UObject/ObjectKey.h"

struct FForceFeedbackEffectDebugInfo
{
	using FValueList = TArray<TPair<FName, float>, TInlineAllocator<8>>;
	using FTextValueList = TArray<TPair<FName, FName>, TInlineAllocator<2>>;

	FObjectKey Effect;
	FString Label;
	FName Kind;
	FJoystickInstanceId InstanceId;
	bool IsRunning = false;
	bool IsCondition = false;
	float Level = 0.0f;
	float EvaluatedLevel = 0.0f;
	float FrequencyHz = 0.0f;
	float WritesPerSecond = 0.0f;
	FValueList Values;
	FTextValueList TextValues;
};

#if UE_BUILD_SHIPPING
#define JOYSTICK_FFB_DEBUG_VALUE(Name, Value)
#define JOYSTICK_FFB_DEBUG_TEXT(Name, Value)
#else
#define JOYSTICK_FFB_DEBUG_VALUE(Name, Value) do { static const FName DebugValueName(Name); AddDebugValue(DebugValueName, Value); } while (0)
#define JOYSTICK_FFB_DEBUG_TEXT(Name, Value) do { static const FName DebugValueName(Name); AddDebugText(DebugValueName, Value); } while (0)
#endif
