// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "JoystickPlugin/Public/JoystickFunctions.h"
#ifdef _MSC_VER
#pragma warning (push)
#pragma warning (disable : 4883)
#endif
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodeJoystickFunctions() {}
// Cross Module References
	JOYSTICKPLUGIN_API UClass* Z_Construct_UClass_UJoystickFunctions_NoRegister();
	JOYSTICKPLUGIN_API UClass* Z_Construct_UClass_UJoystickFunctions();
	COREUOBJECT_API UClass* Z_Construct_UClass_UObject();
	UPackage* Z_Construct_UPackage__Script_JoystickPlugin();
	JOYSTICKPLUGIN_API UScriptStruct* Z_Construct_UScriptStruct_FJoystickInfo();
	JOYSTICKPLUGIN_API UScriptStruct* Z_Construct_UScriptStruct_FJoystickState();
	COREUOBJECT_API UScriptStruct* Z_Construct_UScriptStruct_FVector2D();
	JOYSTICKPLUGIN_API UEnum* Z_Construct_UEnum_JoystickPlugin_EJoystickPOVDirection();
	COREUOBJECT_API UClass* Z_Construct_UClass_UObject_NoRegister();
// End Cross Module References
	DEFINE_FUNCTION(UJoystickFunctions::execIgnoreGameControllers)
	{
		P_GET_UBOOL(Z_Param_bIgnore);
		P_FINISH;
		P_NATIVE_BEGIN;
		UJoystickFunctions::IgnoreGameControllers(Z_Param_bIgnore);
		P_NATIVE_END;
	}
	DEFINE_FUNCTION(UJoystickFunctions::execMapJoystickDeviceToPlayer)
	{
		P_GET_PROPERTY(FIntProperty,Z_Param_DeviceId);
		P_GET_PROPERTY(FIntProperty,Z_Param_Player);
		P_FINISH;
		P_NATIVE_BEGIN;
		UJoystickFunctions::MapJoystickDeviceToPlayer(Z_Param_DeviceId,Z_Param_Player);
		P_NATIVE_END;
	}
	DEFINE_FUNCTION(UJoystickFunctions::execRegisterForJoystickEvents)
	{
		P_GET_OBJECT(UObject,Z_Param_Listener);
		P_FINISH;
		P_NATIVE_BEGIN;
		UJoystickFunctions::RegisterForJoystickEvents(Z_Param_Listener);
		P_NATIVE_END;
	}
	DEFINE_FUNCTION(UJoystickFunctions::execJoystickCount)
	{
		P_FINISH;
		P_NATIVE_BEGIN;
		*(int32*)Z_Param__Result=UJoystickFunctions::JoystickCount();
		P_NATIVE_END;
	}
	DEFINE_FUNCTION(UJoystickFunctions::execGetPreviousJoystickState)
	{
		P_GET_PROPERTY(FIntProperty,Z_Param_DeviceId);
		P_FINISH;
		P_NATIVE_BEGIN;
		*(FJoystickState*)Z_Param__Result=UJoystickFunctions::GetPreviousJoystickState(Z_Param_DeviceId);
		P_NATIVE_END;
	}
	DEFINE_FUNCTION(UJoystickFunctions::execGetJoystickState)
	{
		P_GET_PROPERTY(FIntProperty,Z_Param_DeviceId);
		P_FINISH;
		P_NATIVE_BEGIN;
		*(FJoystickState*)Z_Param__Result=UJoystickFunctions::GetJoystickState(Z_Param_DeviceId);
		P_NATIVE_END;
	}
	DEFINE_FUNCTION(UJoystickFunctions::execGetJoystick)
	{
		P_GET_PROPERTY(FIntProperty,Z_Param_DeviceId);
		P_FINISH;
		P_NATIVE_BEGIN;
		*(FJoystickInfo*)Z_Param__Result=UJoystickFunctions::GetJoystick(Z_Param_DeviceId);
		P_NATIVE_END;
	}
	DEFINE_FUNCTION(UJoystickFunctions::execPOVAxis)
	{
		P_GET_ENUM(EJoystickPOVDirection,Z_Param_Direction);
		P_FINISH;
		P_NATIVE_BEGIN;
		*(FVector2D*)Z_Param__Result=UJoystickFunctions::POVAxis(EJoystickPOVDirection(Z_Param_Direction));
		P_NATIVE_END;
	}
	void UJoystickFunctions::StaticRegisterNativesUJoystickFunctions()
	{
		UClass* Class = UJoystickFunctions::StaticClass();
		static const FNameNativePtrPair Funcs[] = {
			{ "GetJoystick", &UJoystickFunctions::execGetJoystick },
			{ "GetJoystickState", &UJoystickFunctions::execGetJoystickState },
			{ "GetPreviousJoystickState", &UJoystickFunctions::execGetPreviousJoystickState },
			{ "IgnoreGameControllers", &UJoystickFunctions::execIgnoreGameControllers },
			{ "JoystickCount", &UJoystickFunctions::execJoystickCount },
			{ "MapJoystickDeviceToPlayer", &UJoystickFunctions::execMapJoystickDeviceToPlayer },
			{ "POVAxis", &UJoystickFunctions::execPOVAxis },
			{ "RegisterForJoystickEvents", &UJoystickFunctions::execRegisterForJoystickEvents },
		};
		FNativeFunctionRegistrar::RegisterFunctions(Class, Funcs, UE_ARRAY_COUNT(Funcs));
	}
	struct Z_Construct_UFunction_UJoystickFunctions_GetJoystick_Statics
	{
		struct JoystickFunctions_eventGetJoystick_Parms
		{
			int32 DeviceId;
			FJoystickInfo ReturnValue;
		};
		static const UE4CodeGen_Private::FStructPropertyParams NewProp_ReturnValue;
		static const UE4CodeGen_Private::FIntPropertyParams NewProp_DeviceId;
		static const UE4CodeGen_Private::FPropertyParamsBase* const PropPointers[];
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam Function_MetaDataParams[];
#endif
		static const UE4CodeGen_Private::FFunctionParams FuncParams;
	};
	const UE4CodeGen_Private::FStructPropertyParams Z_Construct_UFunction_UJoystickFunctions_GetJoystick_Statics::NewProp_ReturnValue = { "ReturnValue", nullptr, (EPropertyFlags)0x0010000000000580, UE4CodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(JoystickFunctions_eventGetJoystick_Parms, ReturnValue), Z_Construct_UScriptStruct_FJoystickInfo, METADATA_PARAMS(nullptr, 0) };
	const UE4CodeGen_Private::FIntPropertyParams Z_Construct_UFunction_UJoystickFunctions_GetJoystick_Statics::NewProp_DeviceId = { "DeviceId", nullptr, (EPropertyFlags)0x0010000000000080, UE4CodeGen_Private::EPropertyGenFlags::Int, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(JoystickFunctions_eventGetJoystick_Parms, DeviceId), METADATA_PARAMS(nullptr, 0) };
	const UE4CodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UJoystickFunctions_GetJoystick_Statics::PropPointers[] = {
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UJoystickFunctions_GetJoystick_Statics::NewProp_ReturnValue,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UJoystickFunctions_GetJoystick_Statics::NewProp_DeviceId,
	};
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UFunction_UJoystickFunctions_GetJoystick_Statics::Function_MetaDataParams[] = {
		{ "Category", "Input|Joystick input" },
		{ "ModuleRelativePath", "Public/JoystickFunctions.h" },
	};
#endif
	const UE4CodeGen_Private::FFunctionParams Z_Construct_UFunction_UJoystickFunctions_GetJoystick_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_UJoystickFunctions, nullptr, "GetJoystick", nullptr, nullptr, sizeof(JoystickFunctions_eventGetJoystick_Parms), Z_Construct_UFunction_UJoystickFunctions_GetJoystick_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_UJoystickFunctions_GetJoystick_Statics::PropPointers), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x14022401, 0, 0, METADATA_PARAMS(Z_Construct_UFunction_UJoystickFunctions_GetJoystick_Statics::Function_MetaDataParams, UE_ARRAY_COUNT(Z_Construct_UFunction_UJoystickFunctions_GetJoystick_Statics::Function_MetaDataParams)) };
	UFunction* Z_Construct_UFunction_UJoystickFunctions_GetJoystick()
	{
		static UFunction* ReturnFunction = nullptr;
		if (!ReturnFunction)
		{
			UE4CodeGen_Private::ConstructUFunction(ReturnFunction, Z_Construct_UFunction_UJoystickFunctions_GetJoystick_Statics::FuncParams);
		}
		return ReturnFunction;
	}
	struct Z_Construct_UFunction_UJoystickFunctions_GetJoystickState_Statics
	{
		struct JoystickFunctions_eventGetJoystickState_Parms
		{
			int32 DeviceId;
			FJoystickState ReturnValue;
		};
		static const UE4CodeGen_Private::FStructPropertyParams NewProp_ReturnValue;
		static const UE4CodeGen_Private::FIntPropertyParams NewProp_DeviceId;
		static const UE4CodeGen_Private::FPropertyParamsBase* const PropPointers[];
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam Function_MetaDataParams[];
#endif
		static const UE4CodeGen_Private::FFunctionParams FuncParams;
	};
	const UE4CodeGen_Private::FStructPropertyParams Z_Construct_UFunction_UJoystickFunctions_GetJoystickState_Statics::NewProp_ReturnValue = { "ReturnValue", nullptr, (EPropertyFlags)0x0010000000000580, UE4CodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(JoystickFunctions_eventGetJoystickState_Parms, ReturnValue), Z_Construct_UScriptStruct_FJoystickState, METADATA_PARAMS(nullptr, 0) };
	const UE4CodeGen_Private::FIntPropertyParams Z_Construct_UFunction_UJoystickFunctions_GetJoystickState_Statics::NewProp_DeviceId = { "DeviceId", nullptr, (EPropertyFlags)0x0010000000000080, UE4CodeGen_Private::EPropertyGenFlags::Int, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(JoystickFunctions_eventGetJoystickState_Parms, DeviceId), METADATA_PARAMS(nullptr, 0) };
	const UE4CodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UJoystickFunctions_GetJoystickState_Statics::PropPointers[] = {
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UJoystickFunctions_GetJoystickState_Statics::NewProp_ReturnValue,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UJoystickFunctions_GetJoystickState_Statics::NewProp_DeviceId,
	};
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UFunction_UJoystickFunctions_GetJoystickState_Statics::Function_MetaDataParams[] = {
		{ "Category", "Input|Joystick input" },
		{ "ModuleRelativePath", "Public/JoystickFunctions.h" },
	};
#endif
	const UE4CodeGen_Private::FFunctionParams Z_Construct_UFunction_UJoystickFunctions_GetJoystickState_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_UJoystickFunctions, nullptr, "GetJoystickState", nullptr, nullptr, sizeof(JoystickFunctions_eventGetJoystickState_Parms), Z_Construct_UFunction_UJoystickFunctions_GetJoystickState_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_UJoystickFunctions_GetJoystickState_Statics::PropPointers), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x14022401, 0, 0, METADATA_PARAMS(Z_Construct_UFunction_UJoystickFunctions_GetJoystickState_Statics::Function_MetaDataParams, UE_ARRAY_COUNT(Z_Construct_UFunction_UJoystickFunctions_GetJoystickState_Statics::Function_MetaDataParams)) };
	UFunction* Z_Construct_UFunction_UJoystickFunctions_GetJoystickState()
	{
		static UFunction* ReturnFunction = nullptr;
		if (!ReturnFunction)
		{
			UE4CodeGen_Private::ConstructUFunction(ReturnFunction, Z_Construct_UFunction_UJoystickFunctions_GetJoystickState_Statics::FuncParams);
		}
		return ReturnFunction;
	}
	struct Z_Construct_UFunction_UJoystickFunctions_GetPreviousJoystickState_Statics
	{
		struct JoystickFunctions_eventGetPreviousJoystickState_Parms
		{
			int32 DeviceId;
			FJoystickState ReturnValue;
		};
		static const UE4CodeGen_Private::FStructPropertyParams NewProp_ReturnValue;
		static const UE4CodeGen_Private::FIntPropertyParams NewProp_DeviceId;
		static const UE4CodeGen_Private::FPropertyParamsBase* const PropPointers[];
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam Function_MetaDataParams[];
#endif
		static const UE4CodeGen_Private::FFunctionParams FuncParams;
	};
	const UE4CodeGen_Private::FStructPropertyParams Z_Construct_UFunction_UJoystickFunctions_GetPreviousJoystickState_Statics::NewProp_ReturnValue = { "ReturnValue", nullptr, (EPropertyFlags)0x0010000000000580, UE4CodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(JoystickFunctions_eventGetPreviousJoystickState_Parms, ReturnValue), Z_Construct_UScriptStruct_FJoystickState, METADATA_PARAMS(nullptr, 0) };
	const UE4CodeGen_Private::FIntPropertyParams Z_Construct_UFunction_UJoystickFunctions_GetPreviousJoystickState_Statics::NewProp_DeviceId = { "DeviceId", nullptr, (EPropertyFlags)0x0010000000000080, UE4CodeGen_Private::EPropertyGenFlags::Int, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(JoystickFunctions_eventGetPreviousJoystickState_Parms, DeviceId), METADATA_PARAMS(nullptr, 0) };
	const UE4CodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UJoystickFunctions_GetPreviousJoystickState_Statics::PropPointers[] = {
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UJoystickFunctions_GetPreviousJoystickState_Statics::NewProp_ReturnValue,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UJoystickFunctions_GetPreviousJoystickState_Statics::NewProp_DeviceId,
	};
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UFunction_UJoystickFunctions_GetPreviousJoystickState_Statics::Function_MetaDataParams[] = {
		{ "Category", "Input|Joystick input" },
		{ "ModuleRelativePath", "Public/JoystickFunctions.h" },
	};
#endif
	const UE4CodeGen_Private::FFunctionParams Z_Construct_UFunction_UJoystickFunctions_GetPreviousJoystickState_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_UJoystickFunctions, nullptr, "GetPreviousJoystickState", nullptr, nullptr, sizeof(JoystickFunctions_eventGetPreviousJoystickState_Parms), Z_Construct_UFunction_UJoystickFunctions_GetPreviousJoystickState_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_UJoystickFunctions_GetPreviousJoystickState_Statics::PropPointers), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x14022401, 0, 0, METADATA_PARAMS(Z_Construct_UFunction_UJoystickFunctions_GetPreviousJoystickState_Statics::Function_MetaDataParams, UE_ARRAY_COUNT(Z_Construct_UFunction_UJoystickFunctions_GetPreviousJoystickState_Statics::Function_MetaDataParams)) };
	UFunction* Z_Construct_UFunction_UJoystickFunctions_GetPreviousJoystickState()
	{
		static UFunction* ReturnFunction = nullptr;
		if (!ReturnFunction)
		{
			UE4CodeGen_Private::ConstructUFunction(ReturnFunction, Z_Construct_UFunction_UJoystickFunctions_GetPreviousJoystickState_Statics::FuncParams);
		}
		return ReturnFunction;
	}
	struct Z_Construct_UFunction_UJoystickFunctions_IgnoreGameControllers_Statics
	{
		struct JoystickFunctions_eventIgnoreGameControllers_Parms
		{
			bool bIgnore;
		};
		static void NewProp_bIgnore_SetBit(void* Obj);
		static const UE4CodeGen_Private::FBoolPropertyParams NewProp_bIgnore;
		static const UE4CodeGen_Private::FPropertyParamsBase* const PropPointers[];
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam Function_MetaDataParams[];
#endif
		static const UE4CodeGen_Private::FFunctionParams FuncParams;
	};
	void Z_Construct_UFunction_UJoystickFunctions_IgnoreGameControllers_Statics::NewProp_bIgnore_SetBit(void* Obj)
	{
		((JoystickFunctions_eventIgnoreGameControllers_Parms*)Obj)->bIgnore = 1;
	}
	const UE4CodeGen_Private::FBoolPropertyParams Z_Construct_UFunction_UJoystickFunctions_IgnoreGameControllers_Statics::NewProp_bIgnore = { "bIgnore", nullptr, (EPropertyFlags)0x0010000000000080, UE4CodeGen_Private::EPropertyGenFlags::Bool | UE4CodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, 1, sizeof(bool), sizeof(JoystickFunctions_eventIgnoreGameControllers_Parms), &Z_Construct_UFunction_UJoystickFunctions_IgnoreGameControllers_Statics::NewProp_bIgnore_SetBit, METADATA_PARAMS(nullptr, 0) };
	const UE4CodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UJoystickFunctions_IgnoreGameControllers_Statics::PropPointers[] = {
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UJoystickFunctions_IgnoreGameControllers_Statics::NewProp_bIgnore,
	};
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UFunction_UJoystickFunctions_IgnoreGameControllers_Statics::Function_MetaDataParams[] = {
		{ "Category", "Input|Joystick input" },
		{ "ModuleRelativePath", "Public/JoystickFunctions.h" },
	};
#endif
	const UE4CodeGen_Private::FFunctionParams Z_Construct_UFunction_UJoystickFunctions_IgnoreGameControllers_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_UJoystickFunctions, nullptr, "IgnoreGameControllers", nullptr, nullptr, sizeof(JoystickFunctions_eventIgnoreGameControllers_Parms), Z_Construct_UFunction_UJoystickFunctions_IgnoreGameControllers_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_UJoystickFunctions_IgnoreGameControllers_Statics::PropPointers), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x04022401, 0, 0, METADATA_PARAMS(Z_Construct_UFunction_UJoystickFunctions_IgnoreGameControllers_Statics::Function_MetaDataParams, UE_ARRAY_COUNT(Z_Construct_UFunction_UJoystickFunctions_IgnoreGameControllers_Statics::Function_MetaDataParams)) };
	UFunction* Z_Construct_UFunction_UJoystickFunctions_IgnoreGameControllers()
	{
		static UFunction* ReturnFunction = nullptr;
		if (!ReturnFunction)
		{
			UE4CodeGen_Private::ConstructUFunction(ReturnFunction, Z_Construct_UFunction_UJoystickFunctions_IgnoreGameControllers_Statics::FuncParams);
		}
		return ReturnFunction;
	}
	struct Z_Construct_UFunction_UJoystickFunctions_JoystickCount_Statics
	{
		struct JoystickFunctions_eventJoystickCount_Parms
		{
			int32 ReturnValue;
		};
		static const UE4CodeGen_Private::FIntPropertyParams NewProp_ReturnValue;
		static const UE4CodeGen_Private::FPropertyParamsBase* const PropPointers[];
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam Function_MetaDataParams[];
#endif
		static const UE4CodeGen_Private::FFunctionParams FuncParams;
	};
	const UE4CodeGen_Private::FIntPropertyParams Z_Construct_UFunction_UJoystickFunctions_JoystickCount_Statics::NewProp_ReturnValue = { "ReturnValue", nullptr, (EPropertyFlags)0x0010000000000580, UE4CodeGen_Private::EPropertyGenFlags::Int, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(JoystickFunctions_eventJoystickCount_Parms, ReturnValue), METADATA_PARAMS(nullptr, 0) };
	const UE4CodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UJoystickFunctions_JoystickCount_Statics::PropPointers[] = {
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UJoystickFunctions_JoystickCount_Statics::NewProp_ReturnValue,
	};
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UFunction_UJoystickFunctions_JoystickCount_Statics::Function_MetaDataParams[] = {
		{ "Category", "Input|Joystick input" },
		{ "ModuleRelativePath", "Public/JoystickFunctions.h" },
	};
#endif
	const UE4CodeGen_Private::FFunctionParams Z_Construct_UFunction_UJoystickFunctions_JoystickCount_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_UJoystickFunctions, nullptr, "JoystickCount", nullptr, nullptr, sizeof(JoystickFunctions_eventJoystickCount_Parms), Z_Construct_UFunction_UJoystickFunctions_JoystickCount_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_UJoystickFunctions_JoystickCount_Statics::PropPointers), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x14022401, 0, 0, METADATA_PARAMS(Z_Construct_UFunction_UJoystickFunctions_JoystickCount_Statics::Function_MetaDataParams, UE_ARRAY_COUNT(Z_Construct_UFunction_UJoystickFunctions_JoystickCount_Statics::Function_MetaDataParams)) };
	UFunction* Z_Construct_UFunction_UJoystickFunctions_JoystickCount()
	{
		static UFunction* ReturnFunction = nullptr;
		if (!ReturnFunction)
		{
			UE4CodeGen_Private::ConstructUFunction(ReturnFunction, Z_Construct_UFunction_UJoystickFunctions_JoystickCount_Statics::FuncParams);
		}
		return ReturnFunction;
	}
	struct Z_Construct_UFunction_UJoystickFunctions_MapJoystickDeviceToPlayer_Statics
	{
		struct JoystickFunctions_eventMapJoystickDeviceToPlayer_Parms
		{
			int32 DeviceId;
			int32 Player;
		};
		static const UE4CodeGen_Private::FIntPropertyParams NewProp_Player;
		static const UE4CodeGen_Private::FIntPropertyParams NewProp_DeviceId;
		static const UE4CodeGen_Private::FPropertyParamsBase* const PropPointers[];
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam Function_MetaDataParams[];
#endif
		static const UE4CodeGen_Private::FFunctionParams FuncParams;
	};
	const UE4CodeGen_Private::FIntPropertyParams Z_Construct_UFunction_UJoystickFunctions_MapJoystickDeviceToPlayer_Statics::NewProp_Player = { "Player", nullptr, (EPropertyFlags)0x0010000000000080, UE4CodeGen_Private::EPropertyGenFlags::Int, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(JoystickFunctions_eventMapJoystickDeviceToPlayer_Parms, Player), METADATA_PARAMS(nullptr, 0) };
	const UE4CodeGen_Private::FIntPropertyParams Z_Construct_UFunction_UJoystickFunctions_MapJoystickDeviceToPlayer_Statics::NewProp_DeviceId = { "DeviceId", nullptr, (EPropertyFlags)0x0010000000000080, UE4CodeGen_Private::EPropertyGenFlags::Int, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(JoystickFunctions_eventMapJoystickDeviceToPlayer_Parms, DeviceId), METADATA_PARAMS(nullptr, 0) };
	const UE4CodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UJoystickFunctions_MapJoystickDeviceToPlayer_Statics::PropPointers[] = {
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UJoystickFunctions_MapJoystickDeviceToPlayer_Statics::NewProp_Player,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UJoystickFunctions_MapJoystickDeviceToPlayer_Statics::NewProp_DeviceId,
	};
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UFunction_UJoystickFunctions_MapJoystickDeviceToPlayer_Statics::Function_MetaDataParams[] = {
		{ "Category", "Input|Joystick input" },
		{ "ModuleRelativePath", "Public/JoystickFunctions.h" },
	};
#endif
	const UE4CodeGen_Private::FFunctionParams Z_Construct_UFunction_UJoystickFunctions_MapJoystickDeviceToPlayer_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_UJoystickFunctions, nullptr, "MapJoystickDeviceToPlayer", nullptr, nullptr, sizeof(JoystickFunctions_eventMapJoystickDeviceToPlayer_Parms), Z_Construct_UFunction_UJoystickFunctions_MapJoystickDeviceToPlayer_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_UJoystickFunctions_MapJoystickDeviceToPlayer_Statics::PropPointers), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x04022401, 0, 0, METADATA_PARAMS(Z_Construct_UFunction_UJoystickFunctions_MapJoystickDeviceToPlayer_Statics::Function_MetaDataParams, UE_ARRAY_COUNT(Z_Construct_UFunction_UJoystickFunctions_MapJoystickDeviceToPlayer_Statics::Function_MetaDataParams)) };
	UFunction* Z_Construct_UFunction_UJoystickFunctions_MapJoystickDeviceToPlayer()
	{
		static UFunction* ReturnFunction = nullptr;
		if (!ReturnFunction)
		{
			UE4CodeGen_Private::ConstructUFunction(ReturnFunction, Z_Construct_UFunction_UJoystickFunctions_MapJoystickDeviceToPlayer_Statics::FuncParams);
		}
		return ReturnFunction;
	}
	struct Z_Construct_UFunction_UJoystickFunctions_POVAxis_Statics
	{
		struct JoystickFunctions_eventPOVAxis_Parms
		{
			EJoystickPOVDirection Direction;
			FVector2D ReturnValue;
		};
		static const UE4CodeGen_Private::FStructPropertyParams NewProp_ReturnValue;
		static const UE4CodeGen_Private::FEnumPropertyParams NewProp_Direction;
		static const UE4CodeGen_Private::FBytePropertyParams NewProp_Direction_Underlying;
		static const UE4CodeGen_Private::FPropertyParamsBase* const PropPointers[];
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam Function_MetaDataParams[];
#endif
		static const UE4CodeGen_Private::FFunctionParams FuncParams;
	};
	const UE4CodeGen_Private::FStructPropertyParams Z_Construct_UFunction_UJoystickFunctions_POVAxis_Statics::NewProp_ReturnValue = { "ReturnValue", nullptr, (EPropertyFlags)0x0010000000000580, UE4CodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(JoystickFunctions_eventPOVAxis_Parms, ReturnValue), Z_Construct_UScriptStruct_FVector2D, METADATA_PARAMS(nullptr, 0) };
	const UE4CodeGen_Private::FEnumPropertyParams Z_Construct_UFunction_UJoystickFunctions_POVAxis_Statics::NewProp_Direction = { "Direction", nullptr, (EPropertyFlags)0x0010000000000080, UE4CodeGen_Private::EPropertyGenFlags::Enum, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(JoystickFunctions_eventPOVAxis_Parms, Direction), Z_Construct_UEnum_JoystickPlugin_EJoystickPOVDirection, METADATA_PARAMS(nullptr, 0) };
	const UE4CodeGen_Private::FBytePropertyParams Z_Construct_UFunction_UJoystickFunctions_POVAxis_Statics::NewProp_Direction_Underlying = { "UnderlyingType", nullptr, (EPropertyFlags)0x0000000000000000, UE4CodeGen_Private::EPropertyGenFlags::Byte, RF_Public|RF_Transient|RF_MarkAsNative, 1, 0, nullptr, METADATA_PARAMS(nullptr, 0) };
	const UE4CodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UJoystickFunctions_POVAxis_Statics::PropPointers[] = {
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UJoystickFunctions_POVAxis_Statics::NewProp_ReturnValue,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UJoystickFunctions_POVAxis_Statics::NewProp_Direction,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UJoystickFunctions_POVAxis_Statics::NewProp_Direction_Underlying,
	};
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UFunction_UJoystickFunctions_POVAxis_Statics::Function_MetaDataParams[] = {
		{ "Category", "Input|Joystick input" },
		{ "ModuleRelativePath", "Public/JoystickFunctions.h" },
	};
#endif
	const UE4CodeGen_Private::FFunctionParams Z_Construct_UFunction_UJoystickFunctions_POVAxis_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_UJoystickFunctions, nullptr, "POVAxis", nullptr, nullptr, sizeof(JoystickFunctions_eventPOVAxis_Parms), Z_Construct_UFunction_UJoystickFunctions_POVAxis_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_UJoystickFunctions_POVAxis_Statics::PropPointers), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x14822401, 0, 0, METADATA_PARAMS(Z_Construct_UFunction_UJoystickFunctions_POVAxis_Statics::Function_MetaDataParams, UE_ARRAY_COUNT(Z_Construct_UFunction_UJoystickFunctions_POVAxis_Statics::Function_MetaDataParams)) };
	UFunction* Z_Construct_UFunction_UJoystickFunctions_POVAxis()
	{
		static UFunction* ReturnFunction = nullptr;
		if (!ReturnFunction)
		{
			UE4CodeGen_Private::ConstructUFunction(ReturnFunction, Z_Construct_UFunction_UJoystickFunctions_POVAxis_Statics::FuncParams);
		}
		return ReturnFunction;
	}
	struct Z_Construct_UFunction_UJoystickFunctions_RegisterForJoystickEvents_Statics
	{
		struct JoystickFunctions_eventRegisterForJoystickEvents_Parms
		{
			UObject* Listener;
		};
		static const UE4CodeGen_Private::FObjectPropertyParams NewProp_Listener;
		static const UE4CodeGen_Private::FPropertyParamsBase* const PropPointers[];
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam Function_MetaDataParams[];
#endif
		static const UE4CodeGen_Private::FFunctionParams FuncParams;
	};
	const UE4CodeGen_Private::FObjectPropertyParams Z_Construct_UFunction_UJoystickFunctions_RegisterForJoystickEvents_Statics::NewProp_Listener = { "Listener", nullptr, (EPropertyFlags)0x0010000000000080, UE4CodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(JoystickFunctions_eventRegisterForJoystickEvents_Parms, Listener), Z_Construct_UClass_UObject_NoRegister, METADATA_PARAMS(nullptr, 0) };
	const UE4CodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UJoystickFunctions_RegisterForJoystickEvents_Statics::PropPointers[] = {
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UJoystickFunctions_RegisterForJoystickEvents_Statics::NewProp_Listener,
	};
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UFunction_UJoystickFunctions_RegisterForJoystickEvents_Statics::Function_MetaDataParams[] = {
		{ "Category", "Input|Joystick input" },
		{ "ModuleRelativePath", "Public/JoystickFunctions.h" },
	};
#endif
	const UE4CodeGen_Private::FFunctionParams Z_Construct_UFunction_UJoystickFunctions_RegisterForJoystickEvents_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_UJoystickFunctions, nullptr, "RegisterForJoystickEvents", nullptr, nullptr, sizeof(JoystickFunctions_eventRegisterForJoystickEvents_Parms), Z_Construct_UFunction_UJoystickFunctions_RegisterForJoystickEvents_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_UJoystickFunctions_RegisterForJoystickEvents_Statics::PropPointers), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x04022401, 0, 0, METADATA_PARAMS(Z_Construct_UFunction_UJoystickFunctions_RegisterForJoystickEvents_Statics::Function_MetaDataParams, UE_ARRAY_COUNT(Z_Construct_UFunction_UJoystickFunctions_RegisterForJoystickEvents_Statics::Function_MetaDataParams)) };
	UFunction* Z_Construct_UFunction_UJoystickFunctions_RegisterForJoystickEvents()
	{
		static UFunction* ReturnFunction = nullptr;
		if (!ReturnFunction)
		{
			UE4CodeGen_Private::ConstructUFunction(ReturnFunction, Z_Construct_UFunction_UJoystickFunctions_RegisterForJoystickEvents_Statics::FuncParams);
		}
		return ReturnFunction;
	}
	UClass* Z_Construct_UClass_UJoystickFunctions_NoRegister()
	{
		return UJoystickFunctions::StaticClass();
	}
	struct Z_Construct_UClass_UJoystickFunctions_Statics
	{
		static UObject* (*const DependentSingletons[])();
		static const FClassFunctionLinkInfo FuncInfo[];
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam Class_MetaDataParams[];
#endif
		static const FCppClassTypeInfoStatic StaticCppClassTypeInfo;
		static const UE4CodeGen_Private::FClassParams ClassParams;
	};
	UObject* (*const Z_Construct_UClass_UJoystickFunctions_Statics::DependentSingletons[])() = {
		(UObject* (*)())Z_Construct_UClass_UObject,
		(UObject* (*)())Z_Construct_UPackage__Script_JoystickPlugin,
	};
	const FClassFunctionLinkInfo Z_Construct_UClass_UJoystickFunctions_Statics::FuncInfo[] = {
		{ &Z_Construct_UFunction_UJoystickFunctions_GetJoystick, "GetJoystick" }, // 211823683
		{ &Z_Construct_UFunction_UJoystickFunctions_GetJoystickState, "GetJoystickState" }, // 4009559947
		{ &Z_Construct_UFunction_UJoystickFunctions_GetPreviousJoystickState, "GetPreviousJoystickState" }, // 742872203
		{ &Z_Construct_UFunction_UJoystickFunctions_IgnoreGameControllers, "IgnoreGameControllers" }, // 4200617909
		{ &Z_Construct_UFunction_UJoystickFunctions_JoystickCount, "JoystickCount" }, // 1174785774
		{ &Z_Construct_UFunction_UJoystickFunctions_MapJoystickDeviceToPlayer, "MapJoystickDeviceToPlayer" }, // 3428831314
		{ &Z_Construct_UFunction_UJoystickFunctions_POVAxis, "POVAxis" }, // 3735110547
		{ &Z_Construct_UFunction_UJoystickFunctions_RegisterForJoystickEvents, "RegisterForJoystickEvents" }, // 3360370064
	};
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UJoystickFunctions_Statics::Class_MetaDataParams[] = {
		{ "BlueprintType", "true" },
		{ "IncludePath", "JoystickFunctions.h" },
		{ "ModuleRelativePath", "Public/JoystickFunctions.h" },
	};
#endif
	const FCppClassTypeInfoStatic Z_Construct_UClass_UJoystickFunctions_Statics::StaticCppClassTypeInfo = {
		TCppClassTypeTraits<UJoystickFunctions>::IsAbstract,
	};
	const UE4CodeGen_Private::FClassParams Z_Construct_UClass_UJoystickFunctions_Statics::ClassParams = {
		&UJoystickFunctions::StaticClass,
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
		METADATA_PARAMS(Z_Construct_UClass_UJoystickFunctions_Statics::Class_MetaDataParams, UE_ARRAY_COUNT(Z_Construct_UClass_UJoystickFunctions_Statics::Class_MetaDataParams))
	};
	UClass* Z_Construct_UClass_UJoystickFunctions()
	{
		static UClass* OuterClass = nullptr;
		if (!OuterClass)
		{
			UE4CodeGen_Private::ConstructUClass(OuterClass, Z_Construct_UClass_UJoystickFunctions_Statics::ClassParams);
		}
		return OuterClass;
	}
	IMPLEMENT_CLASS(UJoystickFunctions, 406430641);
	template<> JOYSTICKPLUGIN_API UClass* StaticClass<UJoystickFunctions>()
	{
		return UJoystickFunctions::StaticClass();
	}
	static FCompiledInDefer Z_CompiledInDefer_UClass_UJoystickFunctions(Z_Construct_UClass_UJoystickFunctions, &UJoystickFunctions::StaticClass, TEXT("/Script/JoystickPlugin"), TEXT("UJoystickFunctions"), false, nullptr, nullptr, nullptr);
	DEFINE_VTABLE_PTR_HELPER_CTOR(UJoystickFunctions);
PRAGMA_ENABLE_DEPRECATION_WARNINGS
#ifdef _MSC_VER
#pragma warning (pop)
#endif
