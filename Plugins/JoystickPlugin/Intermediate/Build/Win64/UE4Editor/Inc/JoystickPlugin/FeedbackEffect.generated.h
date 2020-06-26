// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/ObjectMacros.h"
#include "UObject/ScriptMacros.h"

PRAGMA_DISABLE_DEPRECATION_WARNINGS
#ifdef JOYSTICKPLUGIN_FeedbackEffect_generated_h
#error "FeedbackEffect.generated.h already included, missing '#pragma once' in FeedbackEffect.h"
#endif
#define JOYSTICKPLUGIN_FeedbackEffect_generated_h

#define HostProject_Plugins_JoystickPlugin_Source_JoystickPlugin_Public_FeedbackEffect_h_24_GENERATED_BODY \
	friend struct Z_Construct_UScriptStruct_FFeedbackData_Statics; \
	JOYSTICKPLUGIN_API static class UScriptStruct* StaticStruct();


template<> JOYSTICKPLUGIN_API UScriptStruct* StaticStruct<struct FFeedbackData>();

#undef CURRENT_FILE_ID
#define CURRENT_FILE_ID HostProject_Plugins_JoystickPlugin_Source_JoystickPlugin_Public_FeedbackEffect_h


#define FOREACH_ENUM_EFEEDBACKTYPE(op) \
	op(EFeedbackType::CONSTANT) \
	op(EFeedbackType::SPRING) \
	op(EFeedbackType::DAMPER) \
	op(EFeedbackType::INERTIA) \
	op(EFeedbackType::FRICTION) \
	op(EFeedbackType::SINE) \
	op(EFeedbackType::TRIANGLE) \
	op(EFeedbackType::SAWTOOTHUP) \
	op(EFeedbackType::SAWTOOTHDOWN) \
	op(EFeedbackType::RAMP) 

enum class EFeedbackType : uint8;
template<> JOYSTICKPLUGIN_API UEnum* StaticEnum<EFeedbackType>();

PRAGMA_ENABLE_DEPRECATION_WARNINGS
