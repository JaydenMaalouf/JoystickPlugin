// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "JoystickPlugin/Public/FeedbackFunctions.h"
#ifdef _MSC_VER
#pragma warning (push)
#pragma warning (disable : 4883)
#endif
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodeFeedbackFunctions() {}
// Cross Module References
	JOYSTICKPLUGIN_API UClass* Z_Construct_UClass_UFeedbackFunctions_NoRegister();
	JOYSTICKPLUGIN_API UClass* Z_Construct_UClass_UFeedbackFunctions();
	COREUOBJECT_API UClass* Z_Construct_UClass_UObject();
	UPackage* Z_Construct_UPackage__Script_JoystickPlugin();
	JOYSTICKPLUGIN_API UScriptStruct* Z_Construct_UScriptStruct_FFeedbackData();
// End Cross Module References
	DEFINE_FUNCTION(UFeedbackFunctions::execDestroyEffect)
	{
		P_GET_PROPERTY(FIntProperty,Z_Param_DeviceId);
		P_GET_PROPERTY(FIntProperty,Z_Param_EffectId);
		P_FINISH;
		P_NATIVE_BEGIN;
		UFeedbackFunctions::DestroyEffect(Z_Param_DeviceId,Z_Param_EffectId);
		P_NATIVE_END;
	}
	DEFINE_FUNCTION(UFeedbackFunctions::execStopAllEffects)
	{
		P_GET_PROPERTY(FIntProperty,Z_Param_DeviceId);
		P_FINISH;
		P_NATIVE_BEGIN;
		UFeedbackFunctions::StopAllEffects(Z_Param_DeviceId);
		P_NATIVE_END;
	}
	DEFINE_FUNCTION(UFeedbackFunctions::execStopEffect)
	{
		P_GET_PROPERTY(FIntProperty,Z_Param_DeviceId);
		P_GET_PROPERTY(FIntProperty,Z_Param_EffectId);
		P_FINISH;
		P_NATIVE_BEGIN;
		UFeedbackFunctions::StopEffect(Z_Param_DeviceId,Z_Param_EffectId);
		P_NATIVE_END;
	}
	DEFINE_FUNCTION(UFeedbackFunctions::execUnpauseDevice)
	{
		P_GET_PROPERTY(FIntProperty,Z_Param_DeviceId);
		P_FINISH;
		P_NATIVE_BEGIN;
		UFeedbackFunctions::UnpauseDevice(Z_Param_DeviceId);
		P_NATIVE_END;
	}
	DEFINE_FUNCTION(UFeedbackFunctions::execPauseDevice)
	{
		P_GET_PROPERTY(FIntProperty,Z_Param_DeviceId);
		P_FINISH;
		P_NATIVE_BEGIN;
		UFeedbackFunctions::PauseDevice(Z_Param_DeviceId);
		P_NATIVE_END;
	}
	DEFINE_FUNCTION(UFeedbackFunctions::execUpdateEffect)
	{
		P_GET_PROPERTY(FIntProperty,Z_Param_DeviceId);
		P_GET_PROPERTY(FIntProperty,Z_Param_EffectId);
		P_GET_STRUCT(FFeedbackData,Z_Param_EffectData);
		P_FINISH;
		P_NATIVE_BEGIN;
		UFeedbackFunctions::UpdateEffect(Z_Param_DeviceId,Z_Param_EffectId,Z_Param_EffectData);
		P_NATIVE_END;
	}
	DEFINE_FUNCTION(UFeedbackFunctions::execCreateEffect)
	{
		P_GET_PROPERTY(FIntProperty,Z_Param_DeviceId);
		P_GET_STRUCT(FFeedbackData,Z_Param_EffectData);
		P_GET_PROPERTY(FIntProperty,Z_Param_NumIterations);
		P_GET_UBOOL(Z_Param_Infinity);
		P_FINISH;
		P_NATIVE_BEGIN;
		*(int32*)Z_Param__Result=UFeedbackFunctions::CreateEffect(Z_Param_DeviceId,Z_Param_EffectData,Z_Param_NumIterations,Z_Param_Infinity);
		P_NATIVE_END;
	}
	DEFINE_FUNCTION(UFeedbackFunctions::execSetGain)
	{
		P_GET_PROPERTY(FIntProperty,Z_Param_DeviceId);
		P_GET_PROPERTY(FIntProperty,Z_Param_Gain);
		P_FINISH;
		P_NATIVE_BEGIN;
		*(bool*)Z_Param__Result=UFeedbackFunctions::SetGain(Z_Param_DeviceId,Z_Param_Gain);
		P_NATIVE_END;
	}
	DEFINE_FUNCTION(UFeedbackFunctions::execSetAutocenter)
	{
		P_GET_PROPERTY(FIntProperty,Z_Param_DeviceId);
		P_GET_PROPERTY(FIntProperty,Z_Param_Center);
		P_FINISH;
		P_NATIVE_BEGIN;
		*(bool*)Z_Param__Result=UFeedbackFunctions::SetAutocenter(Z_Param_DeviceId,Z_Param_Center);
		P_NATIVE_END;
	}
	DEFINE_FUNCTION(UFeedbackFunctions::execGetEffectStatus)
	{
		P_GET_PROPERTY(FIntProperty,Z_Param_DeviceId);
		P_GET_PROPERTY(FIntProperty,Z_Param_EffectId);
		P_FINISH;
		P_NATIVE_BEGIN;
		*(int32*)Z_Param__Result=UFeedbackFunctions::GetEffectStatus(Z_Param_DeviceId,Z_Param_EffectId);
		P_NATIVE_END;
	}
	DEFINE_FUNCTION(UFeedbackFunctions::execGetNumEffectsPlaying)
	{
		P_GET_PROPERTY(FIntProperty,Z_Param_DeviceId);
		P_FINISH;
		P_NATIVE_BEGIN;
		*(int32*)Z_Param__Result=UFeedbackFunctions::GetNumEffectsPlaying(Z_Param_DeviceId);
		P_NATIVE_END;
	}
	DEFINE_FUNCTION(UFeedbackFunctions::execGetNumEffects)
	{
		P_GET_PROPERTY(FIntProperty,Z_Param_DeviceId);
		P_FINISH;
		P_NATIVE_BEGIN;
		*(int32*)Z_Param__Result=UFeedbackFunctions::GetNumEffects(Z_Param_DeviceId);
		P_NATIVE_END;
	}
	void UFeedbackFunctions::StaticRegisterNativesUFeedbackFunctions()
	{
		UClass* Class = UFeedbackFunctions::StaticClass();
		static const FNameNativePtrPair Funcs[] = {
			{ "CreateEffect", &UFeedbackFunctions::execCreateEffect },
			{ "DestroyEffect", &UFeedbackFunctions::execDestroyEffect },
			{ "GetEffectStatus", &UFeedbackFunctions::execGetEffectStatus },
			{ "GetNumEffects", &UFeedbackFunctions::execGetNumEffects },
			{ "GetNumEffectsPlaying", &UFeedbackFunctions::execGetNumEffectsPlaying },
			{ "PauseDevice", &UFeedbackFunctions::execPauseDevice },
			{ "SetAutocenter", &UFeedbackFunctions::execSetAutocenter },
			{ "SetGain", &UFeedbackFunctions::execSetGain },
			{ "StopAllEffects", &UFeedbackFunctions::execStopAllEffects },
			{ "StopEffect", &UFeedbackFunctions::execStopEffect },
			{ "UnpauseDevice", &UFeedbackFunctions::execUnpauseDevice },
			{ "UpdateEffect", &UFeedbackFunctions::execUpdateEffect },
		};
		FNativeFunctionRegistrar::RegisterFunctions(Class, Funcs, UE_ARRAY_COUNT(Funcs));
	}
	struct Z_Construct_UFunction_UFeedbackFunctions_CreateEffect_Statics
	{
		struct FeedbackFunctions_eventCreateEffect_Parms
		{
			int32 DeviceId;
			FFeedbackData EffectData;
			int32 NumIterations;
			bool Infinity;
			int32 ReturnValue;
		};
		static const UE4CodeGen_Private::FIntPropertyParams NewProp_ReturnValue;
		static void NewProp_Infinity_SetBit(void* Obj);
		static const UE4CodeGen_Private::FBoolPropertyParams NewProp_Infinity;
		static const UE4CodeGen_Private::FUnsizedIntPropertyParams NewProp_NumIterations;
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam NewProp_EffectData_MetaData[];
#endif
		static const UE4CodeGen_Private::FStructPropertyParams NewProp_EffectData;
		static const UE4CodeGen_Private::FIntPropertyParams NewProp_DeviceId;
		static const UE4CodeGen_Private::FPropertyParamsBase* const PropPointers[];
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam Function_MetaDataParams[];
#endif
		static const UE4CodeGen_Private::FFunctionParams FuncParams;
	};
	const UE4CodeGen_Private::FIntPropertyParams Z_Construct_UFunction_UFeedbackFunctions_CreateEffect_Statics::NewProp_ReturnValue = { "ReturnValue", nullptr, (EPropertyFlags)0x0010000000000580, UE4CodeGen_Private::EPropertyGenFlags::Int, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(FeedbackFunctions_eventCreateEffect_Parms, ReturnValue), METADATA_PARAMS(nullptr, 0) };
	void Z_Construct_UFunction_UFeedbackFunctions_CreateEffect_Statics::NewProp_Infinity_SetBit(void* Obj)
	{
		((FeedbackFunctions_eventCreateEffect_Parms*)Obj)->Infinity = 1;
	}
	const UE4CodeGen_Private::FBoolPropertyParams Z_Construct_UFunction_UFeedbackFunctions_CreateEffect_Statics::NewProp_Infinity = { "Infinity", nullptr, (EPropertyFlags)0x0010000000000080, UE4CodeGen_Private::EPropertyGenFlags::Bool | UE4CodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, 1, sizeof(bool), sizeof(FeedbackFunctions_eventCreateEffect_Parms), &Z_Construct_UFunction_UFeedbackFunctions_CreateEffect_Statics::NewProp_Infinity_SetBit, METADATA_PARAMS(nullptr, 0) };
	const UE4CodeGen_Private::FUnsizedIntPropertyParams Z_Construct_UFunction_UFeedbackFunctions_CreateEffect_Statics::NewProp_NumIterations = { "NumIterations", nullptr, (EPropertyFlags)0x0010000000000080, UE4CodeGen_Private::EPropertyGenFlags::Int, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(FeedbackFunctions_eventCreateEffect_Parms, NumIterations), METADATA_PARAMS(nullptr, 0) };
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UFunction_UFeedbackFunctions_CreateEffect_Statics::NewProp_EffectData_MetaData[] = {
		{ "NativeConst", "" },
	};
#endif
	const UE4CodeGen_Private::FStructPropertyParams Z_Construct_UFunction_UFeedbackFunctions_CreateEffect_Statics::NewProp_EffectData = { "EffectData", nullptr, (EPropertyFlags)0x0010000000000082, UE4CodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(FeedbackFunctions_eventCreateEffect_Parms, EffectData), Z_Construct_UScriptStruct_FFeedbackData, METADATA_PARAMS(Z_Construct_UFunction_UFeedbackFunctions_CreateEffect_Statics::NewProp_EffectData_MetaData, UE_ARRAY_COUNT(Z_Construct_UFunction_UFeedbackFunctions_CreateEffect_Statics::NewProp_EffectData_MetaData)) };
	const UE4CodeGen_Private::FIntPropertyParams Z_Construct_UFunction_UFeedbackFunctions_CreateEffect_Statics::NewProp_DeviceId = { "DeviceId", nullptr, (EPropertyFlags)0x0010000000000080, UE4CodeGen_Private::EPropertyGenFlags::Int, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(FeedbackFunctions_eventCreateEffect_Parms, DeviceId), METADATA_PARAMS(nullptr, 0) };
	const UE4CodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UFeedbackFunctions_CreateEffect_Statics::PropPointers[] = {
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UFeedbackFunctions_CreateEffect_Statics::NewProp_ReturnValue,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UFeedbackFunctions_CreateEffect_Statics::NewProp_Infinity,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UFeedbackFunctions_CreateEffect_Statics::NewProp_NumIterations,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UFeedbackFunctions_CreateEffect_Statics::NewProp_EffectData,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UFeedbackFunctions_CreateEffect_Statics::NewProp_DeviceId,
	};
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UFunction_UFeedbackFunctions_CreateEffect_Statics::Function_MetaDataParams[] = {
		{ "Category", "Force Feedback" },
		{ "CPP_Default_Infinity", "false" },
		{ "CPP_Default_NumIterations", "1" },
		{ "ModuleRelativePath", "Public/FeedbackFunctions.h" },
	};
#endif
	const UE4CodeGen_Private::FFunctionParams Z_Construct_UFunction_UFeedbackFunctions_CreateEffect_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_UFeedbackFunctions, nullptr, "CreateEffect", nullptr, nullptr, sizeof(FeedbackFunctions_eventCreateEffect_Parms), Z_Construct_UFunction_UFeedbackFunctions_CreateEffect_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_UFeedbackFunctions_CreateEffect_Statics::PropPointers), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x04022401, 0, 0, METADATA_PARAMS(Z_Construct_UFunction_UFeedbackFunctions_CreateEffect_Statics::Function_MetaDataParams, UE_ARRAY_COUNT(Z_Construct_UFunction_UFeedbackFunctions_CreateEffect_Statics::Function_MetaDataParams)) };
	UFunction* Z_Construct_UFunction_UFeedbackFunctions_CreateEffect()
	{
		static UFunction* ReturnFunction = nullptr;
		if (!ReturnFunction)
		{
			UE4CodeGen_Private::ConstructUFunction(ReturnFunction, Z_Construct_UFunction_UFeedbackFunctions_CreateEffect_Statics::FuncParams);
		}
		return ReturnFunction;
	}
	struct Z_Construct_UFunction_UFeedbackFunctions_DestroyEffect_Statics
	{
		struct FeedbackFunctions_eventDestroyEffect_Parms
		{
			int32 DeviceId;
			int32 EffectId;
		};
		static const UE4CodeGen_Private::FIntPropertyParams NewProp_EffectId;
		static const UE4CodeGen_Private::FIntPropertyParams NewProp_DeviceId;
		static const UE4CodeGen_Private::FPropertyParamsBase* const PropPointers[];
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam Function_MetaDataParams[];
#endif
		static const UE4CodeGen_Private::FFunctionParams FuncParams;
	};
	const UE4CodeGen_Private::FIntPropertyParams Z_Construct_UFunction_UFeedbackFunctions_DestroyEffect_Statics::NewProp_EffectId = { "EffectId", nullptr, (EPropertyFlags)0x0010000000000080, UE4CodeGen_Private::EPropertyGenFlags::Int, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(FeedbackFunctions_eventDestroyEffect_Parms, EffectId), METADATA_PARAMS(nullptr, 0) };
	const UE4CodeGen_Private::FIntPropertyParams Z_Construct_UFunction_UFeedbackFunctions_DestroyEffect_Statics::NewProp_DeviceId = { "DeviceId", nullptr, (EPropertyFlags)0x0010000000000080, UE4CodeGen_Private::EPropertyGenFlags::Int, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(FeedbackFunctions_eventDestroyEffect_Parms, DeviceId), METADATA_PARAMS(nullptr, 0) };
	const UE4CodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UFeedbackFunctions_DestroyEffect_Statics::PropPointers[] = {
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UFeedbackFunctions_DestroyEffect_Statics::NewProp_EffectId,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UFeedbackFunctions_DestroyEffect_Statics::NewProp_DeviceId,
	};
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UFunction_UFeedbackFunctions_DestroyEffect_Statics::Function_MetaDataParams[] = {
		{ "Category", "Force Feedback" },
		{ "ModuleRelativePath", "Public/FeedbackFunctions.h" },
	};
#endif
	const UE4CodeGen_Private::FFunctionParams Z_Construct_UFunction_UFeedbackFunctions_DestroyEffect_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_UFeedbackFunctions, nullptr, "DestroyEffect", nullptr, nullptr, sizeof(FeedbackFunctions_eventDestroyEffect_Parms), Z_Construct_UFunction_UFeedbackFunctions_DestroyEffect_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_UFeedbackFunctions_DestroyEffect_Statics::PropPointers), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x04022401, 0, 0, METADATA_PARAMS(Z_Construct_UFunction_UFeedbackFunctions_DestroyEffect_Statics::Function_MetaDataParams, UE_ARRAY_COUNT(Z_Construct_UFunction_UFeedbackFunctions_DestroyEffect_Statics::Function_MetaDataParams)) };
	UFunction* Z_Construct_UFunction_UFeedbackFunctions_DestroyEffect()
	{
		static UFunction* ReturnFunction = nullptr;
		if (!ReturnFunction)
		{
			UE4CodeGen_Private::ConstructUFunction(ReturnFunction, Z_Construct_UFunction_UFeedbackFunctions_DestroyEffect_Statics::FuncParams);
		}
		return ReturnFunction;
	}
	struct Z_Construct_UFunction_UFeedbackFunctions_GetEffectStatus_Statics
	{
		struct FeedbackFunctions_eventGetEffectStatus_Parms
		{
			int32 DeviceId;
			int32 EffectId;
			int32 ReturnValue;
		};
		static const UE4CodeGen_Private::FUnsizedIntPropertyParams NewProp_ReturnValue;
		static const UE4CodeGen_Private::FIntPropertyParams NewProp_EffectId;
		static const UE4CodeGen_Private::FIntPropertyParams NewProp_DeviceId;
		static const UE4CodeGen_Private::FPropertyParamsBase* const PropPointers[];
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam Function_MetaDataParams[];
#endif
		static const UE4CodeGen_Private::FFunctionParams FuncParams;
	};
	const UE4CodeGen_Private::FUnsizedIntPropertyParams Z_Construct_UFunction_UFeedbackFunctions_GetEffectStatus_Statics::NewProp_ReturnValue = { "ReturnValue", nullptr, (EPropertyFlags)0x0010000000000580, UE4CodeGen_Private::EPropertyGenFlags::Int, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(FeedbackFunctions_eventGetEffectStatus_Parms, ReturnValue), METADATA_PARAMS(nullptr, 0) };
	const UE4CodeGen_Private::FIntPropertyParams Z_Construct_UFunction_UFeedbackFunctions_GetEffectStatus_Statics::NewProp_EffectId = { "EffectId", nullptr, (EPropertyFlags)0x0010000000000080, UE4CodeGen_Private::EPropertyGenFlags::Int, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(FeedbackFunctions_eventGetEffectStatus_Parms, EffectId), METADATA_PARAMS(nullptr, 0) };
	const UE4CodeGen_Private::FIntPropertyParams Z_Construct_UFunction_UFeedbackFunctions_GetEffectStatus_Statics::NewProp_DeviceId = { "DeviceId", nullptr, (EPropertyFlags)0x0010000000000080, UE4CodeGen_Private::EPropertyGenFlags::Int, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(FeedbackFunctions_eventGetEffectStatus_Parms, DeviceId), METADATA_PARAMS(nullptr, 0) };
	const UE4CodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UFeedbackFunctions_GetEffectStatus_Statics::PropPointers[] = {
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UFeedbackFunctions_GetEffectStatus_Statics::NewProp_ReturnValue,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UFeedbackFunctions_GetEffectStatus_Statics::NewProp_EffectId,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UFeedbackFunctions_GetEffectStatus_Statics::NewProp_DeviceId,
	};
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UFunction_UFeedbackFunctions_GetEffectStatus_Statics::Function_MetaDataParams[] = {
		{ "Category", "Force Feedback" },
		{ "ModuleRelativePath", "Public/FeedbackFunctions.h" },
	};
#endif
	const UE4CodeGen_Private::FFunctionParams Z_Construct_UFunction_UFeedbackFunctions_GetEffectStatus_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_UFeedbackFunctions, nullptr, "GetEffectStatus", nullptr, nullptr, sizeof(FeedbackFunctions_eventGetEffectStatus_Parms), Z_Construct_UFunction_UFeedbackFunctions_GetEffectStatus_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_UFeedbackFunctions_GetEffectStatus_Statics::PropPointers), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x14022401, 0, 0, METADATA_PARAMS(Z_Construct_UFunction_UFeedbackFunctions_GetEffectStatus_Statics::Function_MetaDataParams, UE_ARRAY_COUNT(Z_Construct_UFunction_UFeedbackFunctions_GetEffectStatus_Statics::Function_MetaDataParams)) };
	UFunction* Z_Construct_UFunction_UFeedbackFunctions_GetEffectStatus()
	{
		static UFunction* ReturnFunction = nullptr;
		if (!ReturnFunction)
		{
			UE4CodeGen_Private::ConstructUFunction(ReturnFunction, Z_Construct_UFunction_UFeedbackFunctions_GetEffectStatus_Statics::FuncParams);
		}
		return ReturnFunction;
	}
	struct Z_Construct_UFunction_UFeedbackFunctions_GetNumEffects_Statics
	{
		struct FeedbackFunctions_eventGetNumEffects_Parms
		{
			int32 DeviceId;
			int32 ReturnValue;
		};
		static const UE4CodeGen_Private::FUnsizedIntPropertyParams NewProp_ReturnValue;
		static const UE4CodeGen_Private::FIntPropertyParams NewProp_DeviceId;
		static const UE4CodeGen_Private::FPropertyParamsBase* const PropPointers[];
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam Function_MetaDataParams[];
#endif
		static const UE4CodeGen_Private::FFunctionParams FuncParams;
	};
	const UE4CodeGen_Private::FUnsizedIntPropertyParams Z_Construct_UFunction_UFeedbackFunctions_GetNumEffects_Statics::NewProp_ReturnValue = { "ReturnValue", nullptr, (EPropertyFlags)0x0010000000000580, UE4CodeGen_Private::EPropertyGenFlags::Int, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(FeedbackFunctions_eventGetNumEffects_Parms, ReturnValue), METADATA_PARAMS(nullptr, 0) };
	const UE4CodeGen_Private::FIntPropertyParams Z_Construct_UFunction_UFeedbackFunctions_GetNumEffects_Statics::NewProp_DeviceId = { "DeviceId", nullptr, (EPropertyFlags)0x0010000000000080, UE4CodeGen_Private::EPropertyGenFlags::Int, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(FeedbackFunctions_eventGetNumEffects_Parms, DeviceId), METADATA_PARAMS(nullptr, 0) };
	const UE4CodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UFeedbackFunctions_GetNumEffects_Statics::PropPointers[] = {
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UFeedbackFunctions_GetNumEffects_Statics::NewProp_ReturnValue,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UFeedbackFunctions_GetNumEffects_Statics::NewProp_DeviceId,
	};
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UFunction_UFeedbackFunctions_GetNumEffects_Statics::Function_MetaDataParams[] = {
		{ "Category", "Force Feedback" },
		{ "ModuleRelativePath", "Public/FeedbackFunctions.h" },
	};
#endif
	const UE4CodeGen_Private::FFunctionParams Z_Construct_UFunction_UFeedbackFunctions_GetNumEffects_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_UFeedbackFunctions, nullptr, "GetNumEffects", nullptr, nullptr, sizeof(FeedbackFunctions_eventGetNumEffects_Parms), Z_Construct_UFunction_UFeedbackFunctions_GetNumEffects_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_UFeedbackFunctions_GetNumEffects_Statics::PropPointers), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x14022401, 0, 0, METADATA_PARAMS(Z_Construct_UFunction_UFeedbackFunctions_GetNumEffects_Statics::Function_MetaDataParams, UE_ARRAY_COUNT(Z_Construct_UFunction_UFeedbackFunctions_GetNumEffects_Statics::Function_MetaDataParams)) };
	UFunction* Z_Construct_UFunction_UFeedbackFunctions_GetNumEffects()
	{
		static UFunction* ReturnFunction = nullptr;
		if (!ReturnFunction)
		{
			UE4CodeGen_Private::ConstructUFunction(ReturnFunction, Z_Construct_UFunction_UFeedbackFunctions_GetNumEffects_Statics::FuncParams);
		}
		return ReturnFunction;
	}
	struct Z_Construct_UFunction_UFeedbackFunctions_GetNumEffectsPlaying_Statics
	{
		struct FeedbackFunctions_eventGetNumEffectsPlaying_Parms
		{
			int32 DeviceId;
			int32 ReturnValue;
		};
		static const UE4CodeGen_Private::FUnsizedIntPropertyParams NewProp_ReturnValue;
		static const UE4CodeGen_Private::FIntPropertyParams NewProp_DeviceId;
		static const UE4CodeGen_Private::FPropertyParamsBase* const PropPointers[];
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam Function_MetaDataParams[];
#endif
		static const UE4CodeGen_Private::FFunctionParams FuncParams;
	};
	const UE4CodeGen_Private::FUnsizedIntPropertyParams Z_Construct_UFunction_UFeedbackFunctions_GetNumEffectsPlaying_Statics::NewProp_ReturnValue = { "ReturnValue", nullptr, (EPropertyFlags)0x0010000000000580, UE4CodeGen_Private::EPropertyGenFlags::Int, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(FeedbackFunctions_eventGetNumEffectsPlaying_Parms, ReturnValue), METADATA_PARAMS(nullptr, 0) };
	const UE4CodeGen_Private::FIntPropertyParams Z_Construct_UFunction_UFeedbackFunctions_GetNumEffectsPlaying_Statics::NewProp_DeviceId = { "DeviceId", nullptr, (EPropertyFlags)0x0010000000000080, UE4CodeGen_Private::EPropertyGenFlags::Int, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(FeedbackFunctions_eventGetNumEffectsPlaying_Parms, DeviceId), METADATA_PARAMS(nullptr, 0) };
	const UE4CodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UFeedbackFunctions_GetNumEffectsPlaying_Statics::PropPointers[] = {
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UFeedbackFunctions_GetNumEffectsPlaying_Statics::NewProp_ReturnValue,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UFeedbackFunctions_GetNumEffectsPlaying_Statics::NewProp_DeviceId,
	};
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UFunction_UFeedbackFunctions_GetNumEffectsPlaying_Statics::Function_MetaDataParams[] = {
		{ "Category", "Force Feedback" },
		{ "ModuleRelativePath", "Public/FeedbackFunctions.h" },
	};
#endif
	const UE4CodeGen_Private::FFunctionParams Z_Construct_UFunction_UFeedbackFunctions_GetNumEffectsPlaying_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_UFeedbackFunctions, nullptr, "GetNumEffectsPlaying", nullptr, nullptr, sizeof(FeedbackFunctions_eventGetNumEffectsPlaying_Parms), Z_Construct_UFunction_UFeedbackFunctions_GetNumEffectsPlaying_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_UFeedbackFunctions_GetNumEffectsPlaying_Statics::PropPointers), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x14022401, 0, 0, METADATA_PARAMS(Z_Construct_UFunction_UFeedbackFunctions_GetNumEffectsPlaying_Statics::Function_MetaDataParams, UE_ARRAY_COUNT(Z_Construct_UFunction_UFeedbackFunctions_GetNumEffectsPlaying_Statics::Function_MetaDataParams)) };
	UFunction* Z_Construct_UFunction_UFeedbackFunctions_GetNumEffectsPlaying()
	{
		static UFunction* ReturnFunction = nullptr;
		if (!ReturnFunction)
		{
			UE4CodeGen_Private::ConstructUFunction(ReturnFunction, Z_Construct_UFunction_UFeedbackFunctions_GetNumEffectsPlaying_Statics::FuncParams);
		}
		return ReturnFunction;
	}
	struct Z_Construct_UFunction_UFeedbackFunctions_PauseDevice_Statics
	{
		struct FeedbackFunctions_eventPauseDevice_Parms
		{
			int32 DeviceId;
		};
		static const UE4CodeGen_Private::FIntPropertyParams NewProp_DeviceId;
		static const UE4CodeGen_Private::FPropertyParamsBase* const PropPointers[];
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam Function_MetaDataParams[];
#endif
		static const UE4CodeGen_Private::FFunctionParams FuncParams;
	};
	const UE4CodeGen_Private::FIntPropertyParams Z_Construct_UFunction_UFeedbackFunctions_PauseDevice_Statics::NewProp_DeviceId = { "DeviceId", nullptr, (EPropertyFlags)0x0010000000000080, UE4CodeGen_Private::EPropertyGenFlags::Int, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(FeedbackFunctions_eventPauseDevice_Parms, DeviceId), METADATA_PARAMS(nullptr, 0) };
	const UE4CodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UFeedbackFunctions_PauseDevice_Statics::PropPointers[] = {
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UFeedbackFunctions_PauseDevice_Statics::NewProp_DeviceId,
	};
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UFunction_UFeedbackFunctions_PauseDevice_Statics::Function_MetaDataParams[] = {
		{ "Category", "Force Feedback" },
		{ "ModuleRelativePath", "Public/FeedbackFunctions.h" },
	};
#endif
	const UE4CodeGen_Private::FFunctionParams Z_Construct_UFunction_UFeedbackFunctions_PauseDevice_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_UFeedbackFunctions, nullptr, "PauseDevice", nullptr, nullptr, sizeof(FeedbackFunctions_eventPauseDevice_Parms), Z_Construct_UFunction_UFeedbackFunctions_PauseDevice_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_UFeedbackFunctions_PauseDevice_Statics::PropPointers), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x04022401, 0, 0, METADATA_PARAMS(Z_Construct_UFunction_UFeedbackFunctions_PauseDevice_Statics::Function_MetaDataParams, UE_ARRAY_COUNT(Z_Construct_UFunction_UFeedbackFunctions_PauseDevice_Statics::Function_MetaDataParams)) };
	UFunction* Z_Construct_UFunction_UFeedbackFunctions_PauseDevice()
	{
		static UFunction* ReturnFunction = nullptr;
		if (!ReturnFunction)
		{
			UE4CodeGen_Private::ConstructUFunction(ReturnFunction, Z_Construct_UFunction_UFeedbackFunctions_PauseDevice_Statics::FuncParams);
		}
		return ReturnFunction;
	}
	struct Z_Construct_UFunction_UFeedbackFunctions_SetAutocenter_Statics
	{
		struct FeedbackFunctions_eventSetAutocenter_Parms
		{
			int32 DeviceId;
			int32 Center;
			bool ReturnValue;
		};
		static void NewProp_ReturnValue_SetBit(void* Obj);
		static const UE4CodeGen_Private::FBoolPropertyParams NewProp_ReturnValue;
		static const UE4CodeGen_Private::FUnsizedIntPropertyParams NewProp_Center;
		static const UE4CodeGen_Private::FIntPropertyParams NewProp_DeviceId;
		static const UE4CodeGen_Private::FPropertyParamsBase* const PropPointers[];
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam Function_MetaDataParams[];
#endif
		static const UE4CodeGen_Private::FFunctionParams FuncParams;
	};
	void Z_Construct_UFunction_UFeedbackFunctions_SetAutocenter_Statics::NewProp_ReturnValue_SetBit(void* Obj)
	{
		((FeedbackFunctions_eventSetAutocenter_Parms*)Obj)->ReturnValue = 1;
	}
	const UE4CodeGen_Private::FBoolPropertyParams Z_Construct_UFunction_UFeedbackFunctions_SetAutocenter_Statics::NewProp_ReturnValue = { "ReturnValue", nullptr, (EPropertyFlags)0x0010000000000580, UE4CodeGen_Private::EPropertyGenFlags::Bool | UE4CodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, 1, sizeof(bool), sizeof(FeedbackFunctions_eventSetAutocenter_Parms), &Z_Construct_UFunction_UFeedbackFunctions_SetAutocenter_Statics::NewProp_ReturnValue_SetBit, METADATA_PARAMS(nullptr, 0) };
	const UE4CodeGen_Private::FUnsizedIntPropertyParams Z_Construct_UFunction_UFeedbackFunctions_SetAutocenter_Statics::NewProp_Center = { "Center", nullptr, (EPropertyFlags)0x0010000000000080, UE4CodeGen_Private::EPropertyGenFlags::Int, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(FeedbackFunctions_eventSetAutocenter_Parms, Center), METADATA_PARAMS(nullptr, 0) };
	const UE4CodeGen_Private::FIntPropertyParams Z_Construct_UFunction_UFeedbackFunctions_SetAutocenter_Statics::NewProp_DeviceId = { "DeviceId", nullptr, (EPropertyFlags)0x0010000000000080, UE4CodeGen_Private::EPropertyGenFlags::Int, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(FeedbackFunctions_eventSetAutocenter_Parms, DeviceId), METADATA_PARAMS(nullptr, 0) };
	const UE4CodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UFeedbackFunctions_SetAutocenter_Statics::PropPointers[] = {
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UFeedbackFunctions_SetAutocenter_Statics::NewProp_ReturnValue,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UFeedbackFunctions_SetAutocenter_Statics::NewProp_Center,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UFeedbackFunctions_SetAutocenter_Statics::NewProp_DeviceId,
	};
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UFunction_UFeedbackFunctions_SetAutocenter_Statics::Function_MetaDataParams[] = {
		{ "Category", "Force Feedback" },
		{ "ModuleRelativePath", "Public/FeedbackFunctions.h" },
	};
#endif
	const UE4CodeGen_Private::FFunctionParams Z_Construct_UFunction_UFeedbackFunctions_SetAutocenter_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_UFeedbackFunctions, nullptr, "SetAutocenter", nullptr, nullptr, sizeof(FeedbackFunctions_eventSetAutocenter_Parms), Z_Construct_UFunction_UFeedbackFunctions_SetAutocenter_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_UFeedbackFunctions_SetAutocenter_Statics::PropPointers), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x04022401, 0, 0, METADATA_PARAMS(Z_Construct_UFunction_UFeedbackFunctions_SetAutocenter_Statics::Function_MetaDataParams, UE_ARRAY_COUNT(Z_Construct_UFunction_UFeedbackFunctions_SetAutocenter_Statics::Function_MetaDataParams)) };
	UFunction* Z_Construct_UFunction_UFeedbackFunctions_SetAutocenter()
	{
		static UFunction* ReturnFunction = nullptr;
		if (!ReturnFunction)
		{
			UE4CodeGen_Private::ConstructUFunction(ReturnFunction, Z_Construct_UFunction_UFeedbackFunctions_SetAutocenter_Statics::FuncParams);
		}
		return ReturnFunction;
	}
	struct Z_Construct_UFunction_UFeedbackFunctions_SetGain_Statics
	{
		struct FeedbackFunctions_eventSetGain_Parms
		{
			int32 DeviceId;
			int32 Gain;
			bool ReturnValue;
		};
		static void NewProp_ReturnValue_SetBit(void* Obj);
		static const UE4CodeGen_Private::FBoolPropertyParams NewProp_ReturnValue;
		static const UE4CodeGen_Private::FUnsizedIntPropertyParams NewProp_Gain;
		static const UE4CodeGen_Private::FIntPropertyParams NewProp_DeviceId;
		static const UE4CodeGen_Private::FPropertyParamsBase* const PropPointers[];
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam Function_MetaDataParams[];
#endif
		static const UE4CodeGen_Private::FFunctionParams FuncParams;
	};
	void Z_Construct_UFunction_UFeedbackFunctions_SetGain_Statics::NewProp_ReturnValue_SetBit(void* Obj)
	{
		((FeedbackFunctions_eventSetGain_Parms*)Obj)->ReturnValue = 1;
	}
	const UE4CodeGen_Private::FBoolPropertyParams Z_Construct_UFunction_UFeedbackFunctions_SetGain_Statics::NewProp_ReturnValue = { "ReturnValue", nullptr, (EPropertyFlags)0x0010000000000580, UE4CodeGen_Private::EPropertyGenFlags::Bool | UE4CodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, 1, sizeof(bool), sizeof(FeedbackFunctions_eventSetGain_Parms), &Z_Construct_UFunction_UFeedbackFunctions_SetGain_Statics::NewProp_ReturnValue_SetBit, METADATA_PARAMS(nullptr, 0) };
	const UE4CodeGen_Private::FUnsizedIntPropertyParams Z_Construct_UFunction_UFeedbackFunctions_SetGain_Statics::NewProp_Gain = { "Gain", nullptr, (EPropertyFlags)0x0010000000000080, UE4CodeGen_Private::EPropertyGenFlags::Int, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(FeedbackFunctions_eventSetGain_Parms, Gain), METADATA_PARAMS(nullptr, 0) };
	const UE4CodeGen_Private::FIntPropertyParams Z_Construct_UFunction_UFeedbackFunctions_SetGain_Statics::NewProp_DeviceId = { "DeviceId", nullptr, (EPropertyFlags)0x0010000000000080, UE4CodeGen_Private::EPropertyGenFlags::Int, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(FeedbackFunctions_eventSetGain_Parms, DeviceId), METADATA_PARAMS(nullptr, 0) };
	const UE4CodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UFeedbackFunctions_SetGain_Statics::PropPointers[] = {
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UFeedbackFunctions_SetGain_Statics::NewProp_ReturnValue,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UFeedbackFunctions_SetGain_Statics::NewProp_Gain,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UFeedbackFunctions_SetGain_Statics::NewProp_DeviceId,
	};
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UFunction_UFeedbackFunctions_SetGain_Statics::Function_MetaDataParams[] = {
		{ "Category", "Force Feedback" },
		{ "ModuleRelativePath", "Public/FeedbackFunctions.h" },
	};
#endif
	const UE4CodeGen_Private::FFunctionParams Z_Construct_UFunction_UFeedbackFunctions_SetGain_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_UFeedbackFunctions, nullptr, "SetGain", nullptr, nullptr, sizeof(FeedbackFunctions_eventSetGain_Parms), Z_Construct_UFunction_UFeedbackFunctions_SetGain_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_UFeedbackFunctions_SetGain_Statics::PropPointers), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x04022401, 0, 0, METADATA_PARAMS(Z_Construct_UFunction_UFeedbackFunctions_SetGain_Statics::Function_MetaDataParams, UE_ARRAY_COUNT(Z_Construct_UFunction_UFeedbackFunctions_SetGain_Statics::Function_MetaDataParams)) };
	UFunction* Z_Construct_UFunction_UFeedbackFunctions_SetGain()
	{
		static UFunction* ReturnFunction = nullptr;
		if (!ReturnFunction)
		{
			UE4CodeGen_Private::ConstructUFunction(ReturnFunction, Z_Construct_UFunction_UFeedbackFunctions_SetGain_Statics::FuncParams);
		}
		return ReturnFunction;
	}
	struct Z_Construct_UFunction_UFeedbackFunctions_StopAllEffects_Statics
	{
		struct FeedbackFunctions_eventStopAllEffects_Parms
		{
			int32 DeviceId;
		};
		static const UE4CodeGen_Private::FIntPropertyParams NewProp_DeviceId;
		static const UE4CodeGen_Private::FPropertyParamsBase* const PropPointers[];
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam Function_MetaDataParams[];
#endif
		static const UE4CodeGen_Private::FFunctionParams FuncParams;
	};
	const UE4CodeGen_Private::FIntPropertyParams Z_Construct_UFunction_UFeedbackFunctions_StopAllEffects_Statics::NewProp_DeviceId = { "DeviceId", nullptr, (EPropertyFlags)0x0010000000000080, UE4CodeGen_Private::EPropertyGenFlags::Int, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(FeedbackFunctions_eventStopAllEffects_Parms, DeviceId), METADATA_PARAMS(nullptr, 0) };
	const UE4CodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UFeedbackFunctions_StopAllEffects_Statics::PropPointers[] = {
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UFeedbackFunctions_StopAllEffects_Statics::NewProp_DeviceId,
	};
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UFunction_UFeedbackFunctions_StopAllEffects_Statics::Function_MetaDataParams[] = {
		{ "Category", "Force Feedback" },
		{ "ModuleRelativePath", "Public/FeedbackFunctions.h" },
	};
#endif
	const UE4CodeGen_Private::FFunctionParams Z_Construct_UFunction_UFeedbackFunctions_StopAllEffects_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_UFeedbackFunctions, nullptr, "StopAllEffects", nullptr, nullptr, sizeof(FeedbackFunctions_eventStopAllEffects_Parms), Z_Construct_UFunction_UFeedbackFunctions_StopAllEffects_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_UFeedbackFunctions_StopAllEffects_Statics::PropPointers), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x04022401, 0, 0, METADATA_PARAMS(Z_Construct_UFunction_UFeedbackFunctions_StopAllEffects_Statics::Function_MetaDataParams, UE_ARRAY_COUNT(Z_Construct_UFunction_UFeedbackFunctions_StopAllEffects_Statics::Function_MetaDataParams)) };
	UFunction* Z_Construct_UFunction_UFeedbackFunctions_StopAllEffects()
	{
		static UFunction* ReturnFunction = nullptr;
		if (!ReturnFunction)
		{
			UE4CodeGen_Private::ConstructUFunction(ReturnFunction, Z_Construct_UFunction_UFeedbackFunctions_StopAllEffects_Statics::FuncParams);
		}
		return ReturnFunction;
	}
	struct Z_Construct_UFunction_UFeedbackFunctions_StopEffect_Statics
	{
		struct FeedbackFunctions_eventStopEffect_Parms
		{
			int32 DeviceId;
			int32 EffectId;
		};
		static const UE4CodeGen_Private::FIntPropertyParams NewProp_EffectId;
		static const UE4CodeGen_Private::FIntPropertyParams NewProp_DeviceId;
		static const UE4CodeGen_Private::FPropertyParamsBase* const PropPointers[];
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam Function_MetaDataParams[];
#endif
		static const UE4CodeGen_Private::FFunctionParams FuncParams;
	};
	const UE4CodeGen_Private::FIntPropertyParams Z_Construct_UFunction_UFeedbackFunctions_StopEffect_Statics::NewProp_EffectId = { "EffectId", nullptr, (EPropertyFlags)0x0010000000000080, UE4CodeGen_Private::EPropertyGenFlags::Int, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(FeedbackFunctions_eventStopEffect_Parms, EffectId), METADATA_PARAMS(nullptr, 0) };
	const UE4CodeGen_Private::FIntPropertyParams Z_Construct_UFunction_UFeedbackFunctions_StopEffect_Statics::NewProp_DeviceId = { "DeviceId", nullptr, (EPropertyFlags)0x0010000000000080, UE4CodeGen_Private::EPropertyGenFlags::Int, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(FeedbackFunctions_eventStopEffect_Parms, DeviceId), METADATA_PARAMS(nullptr, 0) };
	const UE4CodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UFeedbackFunctions_StopEffect_Statics::PropPointers[] = {
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UFeedbackFunctions_StopEffect_Statics::NewProp_EffectId,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UFeedbackFunctions_StopEffect_Statics::NewProp_DeviceId,
	};
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UFunction_UFeedbackFunctions_StopEffect_Statics::Function_MetaDataParams[] = {
		{ "Category", "Force Feedback" },
		{ "ModuleRelativePath", "Public/FeedbackFunctions.h" },
	};
#endif
	const UE4CodeGen_Private::FFunctionParams Z_Construct_UFunction_UFeedbackFunctions_StopEffect_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_UFeedbackFunctions, nullptr, "StopEffect", nullptr, nullptr, sizeof(FeedbackFunctions_eventStopEffect_Parms), Z_Construct_UFunction_UFeedbackFunctions_StopEffect_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_UFeedbackFunctions_StopEffect_Statics::PropPointers), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x04022401, 0, 0, METADATA_PARAMS(Z_Construct_UFunction_UFeedbackFunctions_StopEffect_Statics::Function_MetaDataParams, UE_ARRAY_COUNT(Z_Construct_UFunction_UFeedbackFunctions_StopEffect_Statics::Function_MetaDataParams)) };
	UFunction* Z_Construct_UFunction_UFeedbackFunctions_StopEffect()
	{
		static UFunction* ReturnFunction = nullptr;
		if (!ReturnFunction)
		{
			UE4CodeGen_Private::ConstructUFunction(ReturnFunction, Z_Construct_UFunction_UFeedbackFunctions_StopEffect_Statics::FuncParams);
		}
		return ReturnFunction;
	}
	struct Z_Construct_UFunction_UFeedbackFunctions_UnpauseDevice_Statics
	{
		struct FeedbackFunctions_eventUnpauseDevice_Parms
		{
			int32 DeviceId;
		};
		static const UE4CodeGen_Private::FIntPropertyParams NewProp_DeviceId;
		static const UE4CodeGen_Private::FPropertyParamsBase* const PropPointers[];
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam Function_MetaDataParams[];
#endif
		static const UE4CodeGen_Private::FFunctionParams FuncParams;
	};
	const UE4CodeGen_Private::FIntPropertyParams Z_Construct_UFunction_UFeedbackFunctions_UnpauseDevice_Statics::NewProp_DeviceId = { "DeviceId", nullptr, (EPropertyFlags)0x0010000000000080, UE4CodeGen_Private::EPropertyGenFlags::Int, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(FeedbackFunctions_eventUnpauseDevice_Parms, DeviceId), METADATA_PARAMS(nullptr, 0) };
	const UE4CodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UFeedbackFunctions_UnpauseDevice_Statics::PropPointers[] = {
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UFeedbackFunctions_UnpauseDevice_Statics::NewProp_DeviceId,
	};
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UFunction_UFeedbackFunctions_UnpauseDevice_Statics::Function_MetaDataParams[] = {
		{ "Category", "Force Feedback" },
		{ "ModuleRelativePath", "Public/FeedbackFunctions.h" },
	};
#endif
	const UE4CodeGen_Private::FFunctionParams Z_Construct_UFunction_UFeedbackFunctions_UnpauseDevice_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_UFeedbackFunctions, nullptr, "UnpauseDevice", nullptr, nullptr, sizeof(FeedbackFunctions_eventUnpauseDevice_Parms), Z_Construct_UFunction_UFeedbackFunctions_UnpauseDevice_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_UFeedbackFunctions_UnpauseDevice_Statics::PropPointers), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x04022401, 0, 0, METADATA_PARAMS(Z_Construct_UFunction_UFeedbackFunctions_UnpauseDevice_Statics::Function_MetaDataParams, UE_ARRAY_COUNT(Z_Construct_UFunction_UFeedbackFunctions_UnpauseDevice_Statics::Function_MetaDataParams)) };
	UFunction* Z_Construct_UFunction_UFeedbackFunctions_UnpauseDevice()
	{
		static UFunction* ReturnFunction = nullptr;
		if (!ReturnFunction)
		{
			UE4CodeGen_Private::ConstructUFunction(ReturnFunction, Z_Construct_UFunction_UFeedbackFunctions_UnpauseDevice_Statics::FuncParams);
		}
		return ReturnFunction;
	}
	struct Z_Construct_UFunction_UFeedbackFunctions_UpdateEffect_Statics
	{
		struct FeedbackFunctions_eventUpdateEffect_Parms
		{
			int32 DeviceId;
			int32 EffectId;
			FFeedbackData EffectData;
		};
		static const UE4CodeGen_Private::FStructPropertyParams NewProp_EffectData;
		static const UE4CodeGen_Private::FIntPropertyParams NewProp_EffectId;
		static const UE4CodeGen_Private::FIntPropertyParams NewProp_DeviceId;
		static const UE4CodeGen_Private::FPropertyParamsBase* const PropPointers[];
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam Function_MetaDataParams[];
#endif
		static const UE4CodeGen_Private::FFunctionParams FuncParams;
	};
	const UE4CodeGen_Private::FStructPropertyParams Z_Construct_UFunction_UFeedbackFunctions_UpdateEffect_Statics::NewProp_EffectData = { "EffectData", nullptr, (EPropertyFlags)0x0010000000000080, UE4CodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(FeedbackFunctions_eventUpdateEffect_Parms, EffectData), Z_Construct_UScriptStruct_FFeedbackData, METADATA_PARAMS(nullptr, 0) };
	const UE4CodeGen_Private::FIntPropertyParams Z_Construct_UFunction_UFeedbackFunctions_UpdateEffect_Statics::NewProp_EffectId = { "EffectId", nullptr, (EPropertyFlags)0x0010000000000080, UE4CodeGen_Private::EPropertyGenFlags::Int, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(FeedbackFunctions_eventUpdateEffect_Parms, EffectId), METADATA_PARAMS(nullptr, 0) };
	const UE4CodeGen_Private::FIntPropertyParams Z_Construct_UFunction_UFeedbackFunctions_UpdateEffect_Statics::NewProp_DeviceId = { "DeviceId", nullptr, (EPropertyFlags)0x0010000000000080, UE4CodeGen_Private::EPropertyGenFlags::Int, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(FeedbackFunctions_eventUpdateEffect_Parms, DeviceId), METADATA_PARAMS(nullptr, 0) };
	const UE4CodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UFeedbackFunctions_UpdateEffect_Statics::PropPointers[] = {
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UFeedbackFunctions_UpdateEffect_Statics::NewProp_EffectData,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UFeedbackFunctions_UpdateEffect_Statics::NewProp_EffectId,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UFeedbackFunctions_UpdateEffect_Statics::NewProp_DeviceId,
	};
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UFunction_UFeedbackFunctions_UpdateEffect_Statics::Function_MetaDataParams[] = {
		{ "Category", "Force Feedback" },
		{ "ModuleRelativePath", "Public/FeedbackFunctions.h" },
	};
#endif
	const UE4CodeGen_Private::FFunctionParams Z_Construct_UFunction_UFeedbackFunctions_UpdateEffect_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_UFeedbackFunctions, nullptr, "UpdateEffect", nullptr, nullptr, sizeof(FeedbackFunctions_eventUpdateEffect_Parms), Z_Construct_UFunction_UFeedbackFunctions_UpdateEffect_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_UFeedbackFunctions_UpdateEffect_Statics::PropPointers), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x04022401, 0, 0, METADATA_PARAMS(Z_Construct_UFunction_UFeedbackFunctions_UpdateEffect_Statics::Function_MetaDataParams, UE_ARRAY_COUNT(Z_Construct_UFunction_UFeedbackFunctions_UpdateEffect_Statics::Function_MetaDataParams)) };
	UFunction* Z_Construct_UFunction_UFeedbackFunctions_UpdateEffect()
	{
		static UFunction* ReturnFunction = nullptr;
		if (!ReturnFunction)
		{
			UE4CodeGen_Private::ConstructUFunction(ReturnFunction, Z_Construct_UFunction_UFeedbackFunctions_UpdateEffect_Statics::FuncParams);
		}
		return ReturnFunction;
	}
	UClass* Z_Construct_UClass_UFeedbackFunctions_NoRegister()
	{
		return UFeedbackFunctions::StaticClass();
	}
	struct Z_Construct_UClass_UFeedbackFunctions_Statics
	{
		static UObject* (*const DependentSingletons[])();
		static const FClassFunctionLinkInfo FuncInfo[];
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam Class_MetaDataParams[];
#endif
		static const FCppClassTypeInfoStatic StaticCppClassTypeInfo;
		static const UE4CodeGen_Private::FClassParams ClassParams;
	};
	UObject* (*const Z_Construct_UClass_UFeedbackFunctions_Statics::DependentSingletons[])() = {
		(UObject* (*)())Z_Construct_UClass_UObject,
		(UObject* (*)())Z_Construct_UPackage__Script_JoystickPlugin,
	};
	const FClassFunctionLinkInfo Z_Construct_UClass_UFeedbackFunctions_Statics::FuncInfo[] = {
		{ &Z_Construct_UFunction_UFeedbackFunctions_CreateEffect, "CreateEffect" }, // 497836634
		{ &Z_Construct_UFunction_UFeedbackFunctions_DestroyEffect, "DestroyEffect" }, // 3170246686
		{ &Z_Construct_UFunction_UFeedbackFunctions_GetEffectStatus, "GetEffectStatus" }, // 1190741258
		{ &Z_Construct_UFunction_UFeedbackFunctions_GetNumEffects, "GetNumEffects" }, // 1647136912
		{ &Z_Construct_UFunction_UFeedbackFunctions_GetNumEffectsPlaying, "GetNumEffectsPlaying" }, // 2864309597
		{ &Z_Construct_UFunction_UFeedbackFunctions_PauseDevice, "PauseDevice" }, // 3888920860
		{ &Z_Construct_UFunction_UFeedbackFunctions_SetAutocenter, "SetAutocenter" }, // 514526755
		{ &Z_Construct_UFunction_UFeedbackFunctions_SetGain, "SetGain" }, // 3863010649
		{ &Z_Construct_UFunction_UFeedbackFunctions_StopAllEffects, "StopAllEffects" }, // 1697446645
		{ &Z_Construct_UFunction_UFeedbackFunctions_StopEffect, "StopEffect" }, // 1310561978
		{ &Z_Construct_UFunction_UFeedbackFunctions_UnpauseDevice, "UnpauseDevice" }, // 745097543
		{ &Z_Construct_UFunction_UFeedbackFunctions_UpdateEffect, "UpdateEffect" }, // 3661481293
	};
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UFeedbackFunctions_Statics::Class_MetaDataParams[] = {
		{ "BlueprintType", "true" },
		{ "IncludePath", "FeedbackFunctions.h" },
		{ "ModuleRelativePath", "Public/FeedbackFunctions.h" },
	};
#endif
	const FCppClassTypeInfoStatic Z_Construct_UClass_UFeedbackFunctions_Statics::StaticCppClassTypeInfo = {
		TCppClassTypeTraits<UFeedbackFunctions>::IsAbstract,
	};
	const UE4CodeGen_Private::FClassParams Z_Construct_UClass_UFeedbackFunctions_Statics::ClassParams = {
		&UFeedbackFunctions::StaticClass,
		nullptr,
		&StaticCppClassTypeInfo,
		DependentSingletons,
		FuncInfo,
		nullptr,
		nullptr,
		UE_ARRAY_COUNT(DependentSingletons),
		UE_ARRAY_COUNT(FuncInfo),
		0,
		0,
		0x000000A0u,
		METADATA_PARAMS(Z_Construct_UClass_UFeedbackFunctions_Statics::Class_MetaDataParams, UE_ARRAY_COUNT(Z_Construct_UClass_UFeedbackFunctions_Statics::Class_MetaDataParams))
	};
	UClass* Z_Construct_UClass_UFeedbackFunctions()
	{
		static UClass* OuterClass = nullptr;
		if (!OuterClass)
		{
			UE4CodeGen_Private::ConstructUClass(OuterClass, Z_Construct_UClass_UFeedbackFunctions_Statics::ClassParams);
		}
		return OuterClass;
	}
	IMPLEMENT_CLASS(UFeedbackFunctions, 1406075284);
	template<> JOYSTICKPLUGIN_API UClass* StaticClass<UFeedbackFunctions>()
	{
		return UFeedbackFunctions::StaticClass();
	}
	static FCompiledInDefer Z_CompiledInDefer_UClass_UFeedbackFunctions(Z_Construct_UClass_UFeedbackFunctions, &UFeedbackFunctions::StaticClass, TEXT("/Script/JoystickPlugin"), TEXT("UFeedbackFunctions"), false, nullptr, nullptr, nullptr);
	DEFINE_VTABLE_PTR_HELPER_CTOR(UFeedbackFunctions);
PRAGMA_ENABLE_DEPRECATION_WARNINGS
#ifdef _MSC_VER
#pragma warning (pop)
#endif
