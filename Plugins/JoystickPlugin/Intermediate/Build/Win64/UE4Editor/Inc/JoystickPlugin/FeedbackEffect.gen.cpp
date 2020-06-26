// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "JoystickPlugin/Public/FeedbackEffect.h"
#ifdef _MSC_VER
#pragma warning (push)
#pragma warning (disable : 4883)
#endif
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodeFeedbackEffect() {}
// Cross Module References
	JOYSTICKPLUGIN_API UEnum* Z_Construct_UEnum_JoystickPlugin_EFeedbackType();
	UPackage* Z_Construct_UPackage__Script_JoystickPlugin();
	JOYSTICKPLUGIN_API UScriptStruct* Z_Construct_UScriptStruct_FFeedbackData();
	COREUOBJECT_API UScriptStruct* Z_Construct_UScriptStruct_FVector();
// End Cross Module References
	static UEnum* EFeedbackType_StaticEnum()
	{
		static UEnum* Singleton = nullptr;
		if (!Singleton)
		{
			Singleton = GetStaticEnum(Z_Construct_UEnum_JoystickPlugin_EFeedbackType, Z_Construct_UPackage__Script_JoystickPlugin(), TEXT("EFeedbackType"));
		}
		return Singleton;
	}
	template<> JOYSTICKPLUGIN_API UEnum* StaticEnum<EFeedbackType>()
	{
		return EFeedbackType_StaticEnum();
	}
	static FCompiledInDeferEnum Z_CompiledInDeferEnum_UEnum_EFeedbackType(EFeedbackType_StaticEnum, TEXT("/Script/JoystickPlugin"), TEXT("EFeedbackType"), false, nullptr, nullptr);
	uint32 Get_Z_Construct_UEnum_JoystickPlugin_EFeedbackType_Hash() { return 2203810892U; }
	UEnum* Z_Construct_UEnum_JoystickPlugin_EFeedbackType()
	{
#if WITH_HOT_RELOAD
		UPackage* Outer = Z_Construct_UPackage__Script_JoystickPlugin();
		static UEnum* ReturnEnum = FindExistingEnumIfHotReloadOrDynamic(Outer, TEXT("EFeedbackType"), 0, Get_Z_Construct_UEnum_JoystickPlugin_EFeedbackType_Hash(), false);
#else
		static UEnum* ReturnEnum = nullptr;
#endif // WITH_HOT_RELOAD
		if (!ReturnEnum)
		{
			static const UE4CodeGen_Private::FEnumeratorParam Enumerators[] = {
				{ "EFeedbackType::CONSTANT", (int64)EFeedbackType::CONSTANT },
				{ "EFeedbackType::SPRING", (int64)EFeedbackType::SPRING },
				{ "EFeedbackType::DAMPER", (int64)EFeedbackType::DAMPER },
				{ "EFeedbackType::INERTIA", (int64)EFeedbackType::INERTIA },
				{ "EFeedbackType::FRICTION", (int64)EFeedbackType::FRICTION },
				{ "EFeedbackType::SINE", (int64)EFeedbackType::SINE },
				{ "EFeedbackType::TRIANGLE", (int64)EFeedbackType::TRIANGLE },
				{ "EFeedbackType::SAWTOOTHUP", (int64)EFeedbackType::SAWTOOTHUP },
				{ "EFeedbackType::SAWTOOTHDOWN", (int64)EFeedbackType::SAWTOOTHDOWN },
				{ "EFeedbackType::RAMP", (int64)EFeedbackType::RAMP },
			};
#if WITH_METADATA
			const UE4CodeGen_Private::FMetaDataPairParam Enum_MetaDataParams[] = {
				{ "BlueprintType", "true" },
				{ "CONSTANT.Name", "EFeedbackType::CONSTANT" },
				{ "DAMPER.Name", "EFeedbackType::DAMPER" },
				{ "FRICTION.Name", "EFeedbackType::FRICTION" },
				{ "INERTIA.Name", "EFeedbackType::INERTIA" },
				{ "ModuleRelativePath", "Public/FeedbackEffect.h" },
				{ "RAMP.Name", "EFeedbackType::RAMP" },
				{ "SAWTOOTHDOWN.Name", "EFeedbackType::SAWTOOTHDOWN" },
				{ "SAWTOOTHUP.Name", "EFeedbackType::SAWTOOTHUP" },
				{ "SINE.Name", "EFeedbackType::SINE" },
				{ "SPRING.Name", "EFeedbackType::SPRING" },
				{ "TRIANGLE.Name", "EFeedbackType::TRIANGLE" },
			};
#endif
			static const UE4CodeGen_Private::FEnumParams EnumParams = {
				(UObject*(*)())Z_Construct_UPackage__Script_JoystickPlugin,
				nullptr,
				"EFeedbackType",
				"EFeedbackType",
				Enumerators,
				UE_ARRAY_COUNT(Enumerators),
				RF_Public|RF_Transient|RF_MarkAsNative,
				UE4CodeGen_Private::EDynamicType::NotDynamic,
				(uint8)UEnum::ECppForm::EnumClass,
				METADATA_PARAMS(Enum_MetaDataParams, UE_ARRAY_COUNT(Enum_MetaDataParams))
			};
			UE4CodeGen_Private::ConstructUEnum(ReturnEnum, EnumParams);
		}
		return ReturnEnum;
	}
class UScriptStruct* FFeedbackData::StaticStruct()
{
	static class UScriptStruct* Singleton = NULL;
	if (!Singleton)
	{
		extern JOYSTICKPLUGIN_API uint32 Get_Z_Construct_UScriptStruct_FFeedbackData_Hash();
		Singleton = GetStaticStruct(Z_Construct_UScriptStruct_FFeedbackData, Z_Construct_UPackage__Script_JoystickPlugin(), TEXT("FeedbackData"), sizeof(FFeedbackData), Get_Z_Construct_UScriptStruct_FFeedbackData_Hash());
	}
	return Singleton;
}
template<> JOYSTICKPLUGIN_API UScriptStruct* StaticStruct<FFeedbackData>()
{
	return FFeedbackData::StaticStruct();
}
static FCompiledInDeferStruct Z_CompiledInDeferStruct_UScriptStruct_FFeedbackData(FFeedbackData::StaticStruct, TEXT("/Script/JoystickPlugin"), TEXT("FeedbackData"), false, nullptr, nullptr);
static struct FScriptStruct_JoystickPlugin_StaticRegisterNativesFFeedbackData
{
	FScriptStruct_JoystickPlugin_StaticRegisterNativesFFeedbackData()
	{
		UScriptStruct::DeferCppStructOps(FName(TEXT("FeedbackData")),new UScriptStruct::TCppStructOps<FFeedbackData>);
	}
} ScriptStruct_JoystickPlugin_StaticRegisterNativesFFeedbackData;
	struct Z_Construct_UScriptStruct_FFeedbackData_Statics
	{
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam Struct_MetaDataParams[];
#endif
		static void* NewStructOps();
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam NewProp_RampEnd_MetaData[];
#endif
		static const UE4CodeGen_Private::FFloatPropertyParams NewProp_RampEnd;
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam NewProp_RampStart_MetaData[];
#endif
		static const UE4CodeGen_Private::FFloatPropertyParams NewProp_RampStart;
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam NewProp_RightSat_MetaData[];
#endif
		static const UE4CodeGen_Private::FStructPropertyParams NewProp_RightSat;
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam NewProp_RightCoeff_MetaData[];
#endif
		static const UE4CodeGen_Private::FStructPropertyParams NewProp_RightCoeff;
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam NewProp_LeftSat_MetaData[];
#endif
		static const UE4CodeGen_Private::FStructPropertyParams NewProp_LeftSat;
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam NewProp_LeftCoeff_MetaData[];
#endif
		static const UE4CodeGen_Private::FStructPropertyParams NewProp_LeftCoeff;
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam NewProp_Deadband_MetaData[];
#endif
		static const UE4CodeGen_Private::FStructPropertyParams NewProp_Deadband;
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam NewProp_Center_MetaData[];
#endif
		static const UE4CodeGen_Private::FStructPropertyParams NewProp_Center;
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam NewProp_FadeLevel_MetaData[];
#endif
		static const UE4CodeGen_Private::FFloatPropertyParams NewProp_FadeLevel;
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam NewProp_FadeLength_MetaData[];
#endif
		static const UE4CodeGen_Private::FFloatPropertyParams NewProp_FadeLength;
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam NewProp_AttackLevel_MetaData[];
#endif
		static const UE4CodeGen_Private::FFloatPropertyParams NewProp_AttackLevel;
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam NewProp_AttackLength_MetaData[];
#endif
		static const UE4CodeGen_Private::FFloatPropertyParams NewProp_AttackLength;
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam NewProp_InfiniteLength_MetaData[];
#endif
		static void NewProp_InfiniteLength_SetBit(void* Obj);
		static const UE4CodeGen_Private::FBoolPropertyParams NewProp_InfiniteLength;
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam NewProp_Length_MetaData[];
#endif
		static const UE4CodeGen_Private::FFloatPropertyParams NewProp_Length;
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam NewProp_Delay_MetaData[];
#endif
		static const UE4CodeGen_Private::FFloatPropertyParams NewProp_Delay;
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam NewProp_Phase_MetaData[];
#endif
		static const UE4CodeGen_Private::FFloatPropertyParams NewProp_Phase;
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam NewProp_Offset_MetaData[];
#endif
		static const UE4CodeGen_Private::FFloatPropertyParams NewProp_Offset;
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam NewProp_Magnitude_MetaData[];
#endif
		static const UE4CodeGen_Private::FFloatPropertyParams NewProp_Magnitude;
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam NewProp_Period_MetaData[];
#endif
		static const UE4CodeGen_Private::FFloatPropertyParams NewProp_Period;
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam NewProp_Level_MetaData[];
#endif
		static const UE4CodeGen_Private::FFloatPropertyParams NewProp_Level;
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam NewProp_Direction_MetaData[];
#endif
		static const UE4CodeGen_Private::FStructPropertyParams NewProp_Direction;
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam NewProp_Type_MetaData[];
#endif
		static const UE4CodeGen_Private::FEnumPropertyParams NewProp_Type;
		static const UE4CodeGen_Private::FBytePropertyParams NewProp_Type_Underlying;
		static const UE4CodeGen_Private::FPropertyParamsBase* const PropPointers[];
		static const UE4CodeGen_Private::FStructParams ReturnStructParams;
	};
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UScriptStruct_FFeedbackData_Statics::Struct_MetaDataParams[] = {
		{ "BlueprintType", "true" },
		{ "ModuleRelativePath", "Public/FeedbackEffect.h" },
	};
#endif
	void* Z_Construct_UScriptStruct_FFeedbackData_Statics::NewStructOps()
	{
		return (UScriptStruct::ICppStructOps*)new UScriptStruct::TCppStructOps<FFeedbackData>();
	}
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UScriptStruct_FFeedbackData_Statics::NewProp_RampEnd_MetaData[] = {
		{ "Category", "FeedbackData" },
		{ "ModuleRelativePath", "Public/FeedbackEffect.h" },
	};
#endif
	const UE4CodeGen_Private::FFloatPropertyParams Z_Construct_UScriptStruct_FFeedbackData_Statics::NewProp_RampEnd = { "RampEnd", nullptr, (EPropertyFlags)0x0010000000000005, UE4CodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(FFeedbackData, RampEnd), METADATA_PARAMS(Z_Construct_UScriptStruct_FFeedbackData_Statics::NewProp_RampEnd_MetaData, UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FFeedbackData_Statics::NewProp_RampEnd_MetaData)) };
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UScriptStruct_FFeedbackData_Statics::NewProp_RampStart_MetaData[] = {
		{ "Category", "FeedbackData" },
		{ "ModuleRelativePath", "Public/FeedbackEffect.h" },
	};
#endif
	const UE4CodeGen_Private::FFloatPropertyParams Z_Construct_UScriptStruct_FFeedbackData_Statics::NewProp_RampStart = { "RampStart", nullptr, (EPropertyFlags)0x0010000000000005, UE4CodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(FFeedbackData, RampStart), METADATA_PARAMS(Z_Construct_UScriptStruct_FFeedbackData_Statics::NewProp_RampStart_MetaData, UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FFeedbackData_Statics::NewProp_RampStart_MetaData)) };
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UScriptStruct_FFeedbackData_Statics::NewProp_RightSat_MetaData[] = {
		{ "Category", "FeedbackData" },
		{ "ModuleRelativePath", "Public/FeedbackEffect.h" },
	};
#endif
	const UE4CodeGen_Private::FStructPropertyParams Z_Construct_UScriptStruct_FFeedbackData_Statics::NewProp_RightSat = { "RightSat", nullptr, (EPropertyFlags)0x0010000000000005, UE4CodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(FFeedbackData, RightSat), Z_Construct_UScriptStruct_FVector, METADATA_PARAMS(Z_Construct_UScriptStruct_FFeedbackData_Statics::NewProp_RightSat_MetaData, UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FFeedbackData_Statics::NewProp_RightSat_MetaData)) };
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UScriptStruct_FFeedbackData_Statics::NewProp_RightCoeff_MetaData[] = {
		{ "Category", "FeedbackData" },
		{ "ModuleRelativePath", "Public/FeedbackEffect.h" },
	};
#endif
	const UE4CodeGen_Private::FStructPropertyParams Z_Construct_UScriptStruct_FFeedbackData_Statics::NewProp_RightCoeff = { "RightCoeff", nullptr, (EPropertyFlags)0x0010000000000005, UE4CodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(FFeedbackData, RightCoeff), Z_Construct_UScriptStruct_FVector, METADATA_PARAMS(Z_Construct_UScriptStruct_FFeedbackData_Statics::NewProp_RightCoeff_MetaData, UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FFeedbackData_Statics::NewProp_RightCoeff_MetaData)) };
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UScriptStruct_FFeedbackData_Statics::NewProp_LeftSat_MetaData[] = {
		{ "Category", "FeedbackData" },
		{ "ModuleRelativePath", "Public/FeedbackEffect.h" },
	};
#endif
	const UE4CodeGen_Private::FStructPropertyParams Z_Construct_UScriptStruct_FFeedbackData_Statics::NewProp_LeftSat = { "LeftSat", nullptr, (EPropertyFlags)0x0010000000000005, UE4CodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(FFeedbackData, LeftSat), Z_Construct_UScriptStruct_FVector, METADATA_PARAMS(Z_Construct_UScriptStruct_FFeedbackData_Statics::NewProp_LeftSat_MetaData, UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FFeedbackData_Statics::NewProp_LeftSat_MetaData)) };
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UScriptStruct_FFeedbackData_Statics::NewProp_LeftCoeff_MetaData[] = {
		{ "Category", "FeedbackData" },
		{ "ModuleRelativePath", "Public/FeedbackEffect.h" },
	};
#endif
	const UE4CodeGen_Private::FStructPropertyParams Z_Construct_UScriptStruct_FFeedbackData_Statics::NewProp_LeftCoeff = { "LeftCoeff", nullptr, (EPropertyFlags)0x0010000000000005, UE4CodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(FFeedbackData, LeftCoeff), Z_Construct_UScriptStruct_FVector, METADATA_PARAMS(Z_Construct_UScriptStruct_FFeedbackData_Statics::NewProp_LeftCoeff_MetaData, UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FFeedbackData_Statics::NewProp_LeftCoeff_MetaData)) };
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UScriptStruct_FFeedbackData_Statics::NewProp_Deadband_MetaData[] = {
		{ "Category", "FeedbackData" },
		{ "ModuleRelativePath", "Public/FeedbackEffect.h" },
	};
#endif
	const UE4CodeGen_Private::FStructPropertyParams Z_Construct_UScriptStruct_FFeedbackData_Statics::NewProp_Deadband = { "Deadband", nullptr, (EPropertyFlags)0x0010000000000005, UE4CodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(FFeedbackData, Deadband), Z_Construct_UScriptStruct_FVector, METADATA_PARAMS(Z_Construct_UScriptStruct_FFeedbackData_Statics::NewProp_Deadband_MetaData, UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FFeedbackData_Statics::NewProp_Deadband_MetaData)) };
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UScriptStruct_FFeedbackData_Statics::NewProp_Center_MetaData[] = {
		{ "Category", "FeedbackData" },
		{ "ModuleRelativePath", "Public/FeedbackEffect.h" },
	};
#endif
	const UE4CodeGen_Private::FStructPropertyParams Z_Construct_UScriptStruct_FFeedbackData_Statics::NewProp_Center = { "Center", nullptr, (EPropertyFlags)0x0010000000000005, UE4CodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(FFeedbackData, Center), Z_Construct_UScriptStruct_FVector, METADATA_PARAMS(Z_Construct_UScriptStruct_FFeedbackData_Statics::NewProp_Center_MetaData, UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FFeedbackData_Statics::NewProp_Center_MetaData)) };
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UScriptStruct_FFeedbackData_Statics::NewProp_FadeLevel_MetaData[] = {
		{ "Category", "FeedbackData" },
		{ "ModuleRelativePath", "Public/FeedbackEffect.h" },
	};
#endif
	const UE4CodeGen_Private::FFloatPropertyParams Z_Construct_UScriptStruct_FFeedbackData_Statics::NewProp_FadeLevel = { "FadeLevel", nullptr, (EPropertyFlags)0x0010000000000005, UE4CodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(FFeedbackData, FadeLevel), METADATA_PARAMS(Z_Construct_UScriptStruct_FFeedbackData_Statics::NewProp_FadeLevel_MetaData, UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FFeedbackData_Statics::NewProp_FadeLevel_MetaData)) };
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UScriptStruct_FFeedbackData_Statics::NewProp_FadeLength_MetaData[] = {
		{ "Category", "FeedbackData" },
		{ "ModuleRelativePath", "Public/FeedbackEffect.h" },
	};
#endif
	const UE4CodeGen_Private::FFloatPropertyParams Z_Construct_UScriptStruct_FFeedbackData_Statics::NewProp_FadeLength = { "FadeLength", nullptr, (EPropertyFlags)0x0010000000000005, UE4CodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(FFeedbackData, FadeLength), METADATA_PARAMS(Z_Construct_UScriptStruct_FFeedbackData_Statics::NewProp_FadeLength_MetaData, UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FFeedbackData_Statics::NewProp_FadeLength_MetaData)) };
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UScriptStruct_FFeedbackData_Statics::NewProp_AttackLevel_MetaData[] = {
		{ "Category", "FeedbackData" },
		{ "ModuleRelativePath", "Public/FeedbackEffect.h" },
	};
#endif
	const UE4CodeGen_Private::FFloatPropertyParams Z_Construct_UScriptStruct_FFeedbackData_Statics::NewProp_AttackLevel = { "AttackLevel", nullptr, (EPropertyFlags)0x0010000000000005, UE4CodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(FFeedbackData, AttackLevel), METADATA_PARAMS(Z_Construct_UScriptStruct_FFeedbackData_Statics::NewProp_AttackLevel_MetaData, UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FFeedbackData_Statics::NewProp_AttackLevel_MetaData)) };
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UScriptStruct_FFeedbackData_Statics::NewProp_AttackLength_MetaData[] = {
		{ "Category", "FeedbackData" },
		{ "ModuleRelativePath", "Public/FeedbackEffect.h" },
	};
#endif
	const UE4CodeGen_Private::FFloatPropertyParams Z_Construct_UScriptStruct_FFeedbackData_Statics::NewProp_AttackLength = { "AttackLength", nullptr, (EPropertyFlags)0x0010000000000005, UE4CodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(FFeedbackData, AttackLength), METADATA_PARAMS(Z_Construct_UScriptStruct_FFeedbackData_Statics::NewProp_AttackLength_MetaData, UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FFeedbackData_Statics::NewProp_AttackLength_MetaData)) };
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UScriptStruct_FFeedbackData_Statics::NewProp_InfiniteLength_MetaData[] = {
		{ "Category", "FeedbackData" },
		{ "ModuleRelativePath", "Public/FeedbackEffect.h" },
	};
#endif
	void Z_Construct_UScriptStruct_FFeedbackData_Statics::NewProp_InfiniteLength_SetBit(void* Obj)
	{
		((FFeedbackData*)Obj)->InfiniteLength = 1;
	}
	const UE4CodeGen_Private::FBoolPropertyParams Z_Construct_UScriptStruct_FFeedbackData_Statics::NewProp_InfiniteLength = { "InfiniteLength", nullptr, (EPropertyFlags)0x0010000000000005, UE4CodeGen_Private::EPropertyGenFlags::Bool | UE4CodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, 1, sizeof(bool), sizeof(FFeedbackData), &Z_Construct_UScriptStruct_FFeedbackData_Statics::NewProp_InfiniteLength_SetBit, METADATA_PARAMS(Z_Construct_UScriptStruct_FFeedbackData_Statics::NewProp_InfiniteLength_MetaData, UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FFeedbackData_Statics::NewProp_InfiniteLength_MetaData)) };
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UScriptStruct_FFeedbackData_Statics::NewProp_Length_MetaData[] = {
		{ "Category", "FeedbackData" },
		{ "ModuleRelativePath", "Public/FeedbackEffect.h" },
	};
#endif
	const UE4CodeGen_Private::FFloatPropertyParams Z_Construct_UScriptStruct_FFeedbackData_Statics::NewProp_Length = { "Length", nullptr, (EPropertyFlags)0x0010000000000005, UE4CodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(FFeedbackData, Length), METADATA_PARAMS(Z_Construct_UScriptStruct_FFeedbackData_Statics::NewProp_Length_MetaData, UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FFeedbackData_Statics::NewProp_Length_MetaData)) };
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UScriptStruct_FFeedbackData_Statics::NewProp_Delay_MetaData[] = {
		{ "Category", "FeedbackData" },
		{ "ModuleRelativePath", "Public/FeedbackEffect.h" },
	};
#endif
	const UE4CodeGen_Private::FFloatPropertyParams Z_Construct_UScriptStruct_FFeedbackData_Statics::NewProp_Delay = { "Delay", nullptr, (EPropertyFlags)0x0010000000000005, UE4CodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(FFeedbackData, Delay), METADATA_PARAMS(Z_Construct_UScriptStruct_FFeedbackData_Statics::NewProp_Delay_MetaData, UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FFeedbackData_Statics::NewProp_Delay_MetaData)) };
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UScriptStruct_FFeedbackData_Statics::NewProp_Phase_MetaData[] = {
		{ "Category", "FeedbackData" },
		{ "ModuleRelativePath", "Public/FeedbackEffect.h" },
	};
#endif
	const UE4CodeGen_Private::FFloatPropertyParams Z_Construct_UScriptStruct_FFeedbackData_Statics::NewProp_Phase = { "Phase", nullptr, (EPropertyFlags)0x0010000000000005, UE4CodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(FFeedbackData, Phase), METADATA_PARAMS(Z_Construct_UScriptStruct_FFeedbackData_Statics::NewProp_Phase_MetaData, UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FFeedbackData_Statics::NewProp_Phase_MetaData)) };
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UScriptStruct_FFeedbackData_Statics::NewProp_Offset_MetaData[] = {
		{ "Category", "FeedbackData" },
		{ "ModuleRelativePath", "Public/FeedbackEffect.h" },
	};
#endif
	const UE4CodeGen_Private::FFloatPropertyParams Z_Construct_UScriptStruct_FFeedbackData_Statics::NewProp_Offset = { "Offset", nullptr, (EPropertyFlags)0x0010000000000005, UE4CodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(FFeedbackData, Offset), METADATA_PARAMS(Z_Construct_UScriptStruct_FFeedbackData_Statics::NewProp_Offset_MetaData, UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FFeedbackData_Statics::NewProp_Offset_MetaData)) };
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UScriptStruct_FFeedbackData_Statics::NewProp_Magnitude_MetaData[] = {
		{ "Category", "FeedbackData" },
		{ "ModuleRelativePath", "Public/FeedbackEffect.h" },
	};
#endif
	const UE4CodeGen_Private::FFloatPropertyParams Z_Construct_UScriptStruct_FFeedbackData_Statics::NewProp_Magnitude = { "Magnitude", nullptr, (EPropertyFlags)0x0010000000000005, UE4CodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(FFeedbackData, Magnitude), METADATA_PARAMS(Z_Construct_UScriptStruct_FFeedbackData_Statics::NewProp_Magnitude_MetaData, UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FFeedbackData_Statics::NewProp_Magnitude_MetaData)) };
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UScriptStruct_FFeedbackData_Statics::NewProp_Period_MetaData[] = {
		{ "Category", "FeedbackData" },
		{ "ModuleRelativePath", "Public/FeedbackEffect.h" },
	};
#endif
	const UE4CodeGen_Private::FFloatPropertyParams Z_Construct_UScriptStruct_FFeedbackData_Statics::NewProp_Period = { "Period", nullptr, (EPropertyFlags)0x0010000000000005, UE4CodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(FFeedbackData, Period), METADATA_PARAMS(Z_Construct_UScriptStruct_FFeedbackData_Statics::NewProp_Period_MetaData, UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FFeedbackData_Statics::NewProp_Period_MetaData)) };
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UScriptStruct_FFeedbackData_Statics::NewProp_Level_MetaData[] = {
		{ "Category", "FeedbackData" },
		{ "ModuleRelativePath", "Public/FeedbackEffect.h" },
	};
#endif
	const UE4CodeGen_Private::FFloatPropertyParams Z_Construct_UScriptStruct_FFeedbackData_Statics::NewProp_Level = { "Level", nullptr, (EPropertyFlags)0x0010000000000005, UE4CodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(FFeedbackData, Level), METADATA_PARAMS(Z_Construct_UScriptStruct_FFeedbackData_Statics::NewProp_Level_MetaData, UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FFeedbackData_Statics::NewProp_Level_MetaData)) };
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UScriptStruct_FFeedbackData_Statics::NewProp_Direction_MetaData[] = {
		{ "Category", "FeedbackData" },
		{ "ModuleRelativePath", "Public/FeedbackEffect.h" },
	};
#endif
	const UE4CodeGen_Private::FStructPropertyParams Z_Construct_UScriptStruct_FFeedbackData_Statics::NewProp_Direction = { "Direction", nullptr, (EPropertyFlags)0x0010000000000005, UE4CodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(FFeedbackData, Direction), Z_Construct_UScriptStruct_FVector, METADATA_PARAMS(Z_Construct_UScriptStruct_FFeedbackData_Statics::NewProp_Direction_MetaData, UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FFeedbackData_Statics::NewProp_Direction_MetaData)) };
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UScriptStruct_FFeedbackData_Statics::NewProp_Type_MetaData[] = {
		{ "Category", "FeedbackData" },
		{ "ModuleRelativePath", "Public/FeedbackEffect.h" },
	};
#endif
	const UE4CodeGen_Private::FEnumPropertyParams Z_Construct_UScriptStruct_FFeedbackData_Statics::NewProp_Type = { "Type", nullptr, (EPropertyFlags)0x0010000000000005, UE4CodeGen_Private::EPropertyGenFlags::Enum, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(FFeedbackData, Type), Z_Construct_UEnum_JoystickPlugin_EFeedbackType, METADATA_PARAMS(Z_Construct_UScriptStruct_FFeedbackData_Statics::NewProp_Type_MetaData, UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FFeedbackData_Statics::NewProp_Type_MetaData)) };
	const UE4CodeGen_Private::FBytePropertyParams Z_Construct_UScriptStruct_FFeedbackData_Statics::NewProp_Type_Underlying = { "UnderlyingType", nullptr, (EPropertyFlags)0x0000000000000000, UE4CodeGen_Private::EPropertyGenFlags::Byte, RF_Public|RF_Transient|RF_MarkAsNative, 1, 0, nullptr, METADATA_PARAMS(nullptr, 0) };
	const UE4CodeGen_Private::FPropertyParamsBase* const Z_Construct_UScriptStruct_FFeedbackData_Statics::PropPointers[] = {
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FFeedbackData_Statics::NewProp_RampEnd,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FFeedbackData_Statics::NewProp_RampStart,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FFeedbackData_Statics::NewProp_RightSat,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FFeedbackData_Statics::NewProp_RightCoeff,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FFeedbackData_Statics::NewProp_LeftSat,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FFeedbackData_Statics::NewProp_LeftCoeff,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FFeedbackData_Statics::NewProp_Deadband,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FFeedbackData_Statics::NewProp_Center,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FFeedbackData_Statics::NewProp_FadeLevel,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FFeedbackData_Statics::NewProp_FadeLength,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FFeedbackData_Statics::NewProp_AttackLevel,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FFeedbackData_Statics::NewProp_AttackLength,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FFeedbackData_Statics::NewProp_InfiniteLength,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FFeedbackData_Statics::NewProp_Length,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FFeedbackData_Statics::NewProp_Delay,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FFeedbackData_Statics::NewProp_Phase,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FFeedbackData_Statics::NewProp_Offset,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FFeedbackData_Statics::NewProp_Magnitude,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FFeedbackData_Statics::NewProp_Period,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FFeedbackData_Statics::NewProp_Level,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FFeedbackData_Statics::NewProp_Direction,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FFeedbackData_Statics::NewProp_Type,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FFeedbackData_Statics::NewProp_Type_Underlying,
	};
	const UE4CodeGen_Private::FStructParams Z_Construct_UScriptStruct_FFeedbackData_Statics::ReturnStructParams = {
		(UObject* (*)())Z_Construct_UPackage__Script_JoystickPlugin,
		nullptr,
		&NewStructOps,
		"FeedbackData",
		sizeof(FFeedbackData),
		alignof(FFeedbackData),
		Z_Construct_UScriptStruct_FFeedbackData_Statics::PropPointers,
		UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FFeedbackData_Statics::PropPointers),
		RF_Public|RF_Transient|RF_MarkAsNative,
		EStructFlags(0x00000001),
		METADATA_PARAMS(Z_Construct_UScriptStruct_FFeedbackData_Statics::Struct_MetaDataParams, UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FFeedbackData_Statics::Struct_MetaDataParams))
	};
	UScriptStruct* Z_Construct_UScriptStruct_FFeedbackData()
	{
#if WITH_HOT_RELOAD
		extern uint32 Get_Z_Construct_UScriptStruct_FFeedbackData_Hash();
		UPackage* Outer = Z_Construct_UPackage__Script_JoystickPlugin();
		static UScriptStruct* ReturnStruct = FindExistingStructIfHotReloadOrDynamic(Outer, TEXT("FeedbackData"), sizeof(FFeedbackData), Get_Z_Construct_UScriptStruct_FFeedbackData_Hash(), false);
#else
		static UScriptStruct* ReturnStruct = nullptr;
#endif
		if (!ReturnStruct)
		{
			UE4CodeGen_Private::ConstructUScriptStruct(ReturnStruct, Z_Construct_UScriptStruct_FFeedbackData_Statics::ReturnStructParams);
		}
		return ReturnStruct;
	}
	uint32 Get_Z_Construct_UScriptStruct_FFeedbackData_Hash() { return 3664157631U; }
PRAGMA_ENABLE_DEPRECATION_WARNINGS
#ifdef _MSC_VER
#pragma warning (pop)
#endif
