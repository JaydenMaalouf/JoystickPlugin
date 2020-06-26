// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "JoystickPlugin/Public/JoystickInterface.h"
#ifdef _MSC_VER
#pragma warning (push)
#pragma warning (disable : 4883)
#endif
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodeJoystickInterface() {}
// Cross Module References
	JOYSTICKPLUGIN_API UEnum* Z_Construct_UEnum_JoystickPlugin_EJoystickPOVDirection();
	UPackage* Z_Construct_UPackage__Script_JoystickPlugin();
	JOYSTICKPLUGIN_API UEnum* Z_Construct_UEnum_JoystickPlugin_EInputType();
	JOYSTICKPLUGIN_API UScriptStruct* Z_Construct_UScriptStruct_FJoystickInfo();
	COREUOBJECT_API UScriptStruct* Z_Construct_UScriptStruct_FGuid();
	JOYSTICKPLUGIN_API UScriptStruct* Z_Construct_UScriptStruct_FJoystickState();
	COREUOBJECT_API UScriptStruct* Z_Construct_UScriptStruct_FVector2D();
	JOYSTICKPLUGIN_API UClass* Z_Construct_UClass_UJoystickInterface_NoRegister();
	JOYSTICKPLUGIN_API UClass* Z_Construct_UClass_UJoystickInterface();
	COREUOBJECT_API UClass* Z_Construct_UClass_UInterface();
// End Cross Module References
	static UEnum* EJoystickPOVDirection_StaticEnum()
	{
		static UEnum* Singleton = nullptr;
		if (!Singleton)
		{
			Singleton = GetStaticEnum(Z_Construct_UEnum_JoystickPlugin_EJoystickPOVDirection, Z_Construct_UPackage__Script_JoystickPlugin(), TEXT("EJoystickPOVDirection"));
		}
		return Singleton;
	}
	template<> JOYSTICKPLUGIN_API UEnum* StaticEnum<EJoystickPOVDirection>()
	{
		return EJoystickPOVDirection_StaticEnum();
	}
	static FCompiledInDeferEnum Z_CompiledInDeferEnum_UEnum_EJoystickPOVDirection(EJoystickPOVDirection_StaticEnum, TEXT("/Script/JoystickPlugin"), TEXT("EJoystickPOVDirection"), false, nullptr, nullptr);
	uint32 Get_Z_Construct_UEnum_JoystickPlugin_EJoystickPOVDirection_Hash() { return 1322113115U; }
	UEnum* Z_Construct_UEnum_JoystickPlugin_EJoystickPOVDirection()
	{
#if WITH_HOT_RELOAD
		UPackage* Outer = Z_Construct_UPackage__Script_JoystickPlugin();
		static UEnum* ReturnEnum = FindExistingEnumIfHotReloadOrDynamic(Outer, TEXT("EJoystickPOVDirection"), 0, Get_Z_Construct_UEnum_JoystickPlugin_EJoystickPOVDirection_Hash(), false);
#else
		static UEnum* ReturnEnum = nullptr;
#endif // WITH_HOT_RELOAD
		if (!ReturnEnum)
		{
			static const UE4CodeGen_Private::FEnumeratorParam Enumerators[] = {
				{ "EJoystickPOVDirection::DIRECTION_NONE", (int64)EJoystickPOVDirection::DIRECTION_NONE },
				{ "EJoystickPOVDirection::DIRECTION_UP", (int64)EJoystickPOVDirection::DIRECTION_UP },
				{ "EJoystickPOVDirection::DIRECTION_UP_RIGHT", (int64)EJoystickPOVDirection::DIRECTION_UP_RIGHT },
				{ "EJoystickPOVDirection::DIRECTION_RIGHT", (int64)EJoystickPOVDirection::DIRECTION_RIGHT },
				{ "EJoystickPOVDirection::DIRECTION_DOWN_RIGHT", (int64)EJoystickPOVDirection::DIRECTION_DOWN_RIGHT },
				{ "EJoystickPOVDirection::DIRECTION_DOWN", (int64)EJoystickPOVDirection::DIRECTION_DOWN },
				{ "EJoystickPOVDirection::DIRECTION_DOWN_LEFT", (int64)EJoystickPOVDirection::DIRECTION_DOWN_LEFT },
				{ "EJoystickPOVDirection::DIRECTION_LEFT", (int64)EJoystickPOVDirection::DIRECTION_LEFT },
				{ "EJoystickPOVDirection::DIRECTION_UP_LEFT", (int64)EJoystickPOVDirection::DIRECTION_UP_LEFT },
			};
#if WITH_METADATA
			const UE4CodeGen_Private::FMetaDataPairParam Enum_MetaDataParams[] = {
				{ "BlueprintType", "true" },
				{ "DIRECTION_DOWN.Name", "EJoystickPOVDirection::DIRECTION_DOWN" },
				{ "DIRECTION_DOWN_LEFT.Name", "EJoystickPOVDirection::DIRECTION_DOWN_LEFT" },
				{ "DIRECTION_DOWN_RIGHT.Name", "EJoystickPOVDirection::DIRECTION_DOWN_RIGHT" },
				{ "DIRECTION_LEFT.Name", "EJoystickPOVDirection::DIRECTION_LEFT" },
				{ "DIRECTION_NONE.Name", "EJoystickPOVDirection::DIRECTION_NONE" },
				{ "DIRECTION_RIGHT.Name", "EJoystickPOVDirection::DIRECTION_RIGHT" },
				{ "DIRECTION_UP.Name", "EJoystickPOVDirection::DIRECTION_UP" },
				{ "DIRECTION_UP_LEFT.Name", "EJoystickPOVDirection::DIRECTION_UP_LEFT" },
				{ "DIRECTION_UP_RIGHT.Name", "EJoystickPOVDirection::DIRECTION_UP_RIGHT" },
				{ "ModuleRelativePath", "Public/JoystickInterface.h" },
			};
#endif
			static const UE4CodeGen_Private::FEnumParams EnumParams = {
				(UObject*(*)())Z_Construct_UPackage__Script_JoystickPlugin,
				nullptr,
				"EJoystickPOVDirection",
				"EJoystickPOVDirection",
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
	static UEnum* EInputType_StaticEnum()
	{
		static UEnum* Singleton = nullptr;
		if (!Singleton)
		{
			Singleton = GetStaticEnum(Z_Construct_UEnum_JoystickPlugin_EInputType, Z_Construct_UPackage__Script_JoystickPlugin(), TEXT("EInputType"));
		}
		return Singleton;
	}
	template<> JOYSTICKPLUGIN_API UEnum* StaticEnum<EInputType>()
	{
		return EInputType_StaticEnum();
	}
	static FCompiledInDeferEnum Z_CompiledInDeferEnum_UEnum_EInputType(EInputType_StaticEnum, TEXT("/Script/JoystickPlugin"), TEXT("EInputType"), false, nullptr, nullptr);
	uint32 Get_Z_Construct_UEnum_JoystickPlugin_EInputType_Hash() { return 3969021375U; }
	UEnum* Z_Construct_UEnum_JoystickPlugin_EInputType()
	{
#if WITH_HOT_RELOAD
		UPackage* Outer = Z_Construct_UPackage__Script_JoystickPlugin();
		static UEnum* ReturnEnum = FindExistingEnumIfHotReloadOrDynamic(Outer, TEXT("EInputType"), 0, Get_Z_Construct_UEnum_JoystickPlugin_EInputType_Hash(), false);
#else
		static UEnum* ReturnEnum = nullptr;
#endif // WITH_HOT_RELOAD
		if (!ReturnEnum)
		{
			static const UE4CodeGen_Private::FEnumeratorParam Enumerators[] = {
				{ "EInputType::INPUTTYPE_UNKNOWN", (int64)EInputType::INPUTTYPE_UNKNOWN },
				{ "EInputType::INPUTTYPE_JOYSTICK", (int64)EInputType::INPUTTYPE_JOYSTICK },
				{ "EInputType::INPUTTYPE_GAMECONTROLLER", (int64)EInputType::INPUTTYPE_GAMECONTROLLER },
			};
#if WITH_METADATA
			const UE4CodeGen_Private::FMetaDataPairParam Enum_MetaDataParams[] = {
				{ "BlueprintType", "true" },
				{ "INPUTTYPE_GAMECONTROLLER.Name", "EInputType::INPUTTYPE_GAMECONTROLLER" },
				{ "INPUTTYPE_JOYSTICK.Name", "EInputType::INPUTTYPE_JOYSTICK" },
				{ "INPUTTYPE_UNKNOWN.Name", "EInputType::INPUTTYPE_UNKNOWN" },
				{ "ModuleRelativePath", "Public/JoystickInterface.h" },
			};
#endif
			static const UE4CodeGen_Private::FEnumParams EnumParams = {
				(UObject*(*)())Z_Construct_UPackage__Script_JoystickPlugin,
				nullptr,
				"EInputType",
				"EInputType",
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
class UScriptStruct* FJoystickInfo::StaticStruct()
{
	static class UScriptStruct* Singleton = NULL;
	if (!Singleton)
	{
		extern JOYSTICKPLUGIN_API uint32 Get_Z_Construct_UScriptStruct_FJoystickInfo_Hash();
		Singleton = GetStaticStruct(Z_Construct_UScriptStruct_FJoystickInfo, Z_Construct_UPackage__Script_JoystickPlugin(), TEXT("JoystickInfo"), sizeof(FJoystickInfo), Get_Z_Construct_UScriptStruct_FJoystickInfo_Hash());
	}
	return Singleton;
}
template<> JOYSTICKPLUGIN_API UScriptStruct* StaticStruct<FJoystickInfo>()
{
	return FJoystickInfo::StaticStruct();
}
static FCompiledInDeferStruct Z_CompiledInDeferStruct_UScriptStruct_FJoystickInfo(FJoystickInfo::StaticStruct, TEXT("/Script/JoystickPlugin"), TEXT("JoystickInfo"), false, nullptr, nullptr);
static struct FScriptStruct_JoystickPlugin_StaticRegisterNativesFJoystickInfo
{
	FScriptStruct_JoystickPlugin_StaticRegisterNativesFJoystickInfo()
	{
		UScriptStruct::DeferCppStructOps(FName(TEXT("JoystickInfo")),new UScriptStruct::TCppStructOps<FJoystickInfo>);
	}
} ScriptStruct_JoystickPlugin_StaticRegisterNativesFJoystickInfo;
	struct Z_Construct_UScriptStruct_FJoystickInfo_Statics
	{
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam Struct_MetaDataParams[];
#endif
		static void* NewStructOps();
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam NewProp_InputType_MetaData[];
#endif
		static const UE4CodeGen_Private::FArrayPropertyParams NewProp_InputType;
		static const UE4CodeGen_Private::FEnumPropertyParams NewProp_InputType_Inner;
		static const UE4CodeGen_Private::FBytePropertyParams NewProp_InputType_Inner_Underlying;
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam NewProp_Connected_MetaData[];
#endif
		static void NewProp_Connected_SetBit(void* Obj);
		static const UE4CodeGen_Private::FBoolPropertyParams NewProp_Connected;
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam NewProp_DeviceName_MetaData[];
#endif
		static const UE4CodeGen_Private::FStrPropertyParams NewProp_DeviceName;
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam NewProp_ProductName_MetaData[];
#endif
		static const UE4CodeGen_Private::FStrPropertyParams NewProp_ProductName;
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam NewProp_ProductId_MetaData[];
#endif
		static const UE4CodeGen_Private::FStructPropertyParams NewProp_ProductId;
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam NewProp_IsRumbleDevice_MetaData[];
#endif
		static void NewProp_IsRumbleDevice_SetBit(void* Obj);
		static const UE4CodeGen_Private::FBoolPropertyParams NewProp_IsRumbleDevice;
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam NewProp_DeviceId_MetaData[];
#endif
		static const UE4CodeGen_Private::FIntPropertyParams NewProp_DeviceId;
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam NewProp_Player_MetaData[];
#endif
		static const UE4CodeGen_Private::FIntPropertyParams NewProp_Player;
		static const UE4CodeGen_Private::FPropertyParamsBase* const PropPointers[];
		static const UE4CodeGen_Private::FStructParams ReturnStructParams;
	};
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UScriptStruct_FJoystickInfo_Statics::Struct_MetaDataParams[] = {
		{ "BlueprintType", "true" },
		{ "ModuleRelativePath", "Public/JoystickInterface.h" },
	};
#endif
	void* Z_Construct_UScriptStruct_FJoystickInfo_Statics::NewStructOps()
	{
		return (UScriptStruct::ICppStructOps*)new UScriptStruct::TCppStructOps<FJoystickInfo>();
	}
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UScriptStruct_FJoystickInfo_Statics::NewProp_InputType_MetaData[] = {
		{ "Category", "JoystickInfo" },
		{ "ModuleRelativePath", "Public/JoystickInterface.h" },
	};
#endif
	const UE4CodeGen_Private::FArrayPropertyParams Z_Construct_UScriptStruct_FJoystickInfo_Statics::NewProp_InputType = { "InputType", nullptr, (EPropertyFlags)0x0010000000000015, UE4CodeGen_Private::EPropertyGenFlags::Array, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(FJoystickInfo, InputType), EArrayPropertyFlags::None, METADATA_PARAMS(Z_Construct_UScriptStruct_FJoystickInfo_Statics::NewProp_InputType_MetaData, UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FJoystickInfo_Statics::NewProp_InputType_MetaData)) };
	const UE4CodeGen_Private::FEnumPropertyParams Z_Construct_UScriptStruct_FJoystickInfo_Statics::NewProp_InputType_Inner = { "InputType", nullptr, (EPropertyFlags)0x0000000000000000, UE4CodeGen_Private::EPropertyGenFlags::Enum, RF_Public|RF_Transient|RF_MarkAsNative, 1, 0, Z_Construct_UEnum_JoystickPlugin_EInputType, METADATA_PARAMS(nullptr, 0) };
	const UE4CodeGen_Private::FBytePropertyParams Z_Construct_UScriptStruct_FJoystickInfo_Statics::NewProp_InputType_Inner_Underlying = { "UnderlyingType", nullptr, (EPropertyFlags)0x0000000000000000, UE4CodeGen_Private::EPropertyGenFlags::Byte, RF_Public|RF_Transient|RF_MarkAsNative, 1, 0, nullptr, METADATA_PARAMS(nullptr, 0) };
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UScriptStruct_FJoystickInfo_Statics::NewProp_Connected_MetaData[] = {
		{ "Category", "JoystickInfo" },
		{ "ModuleRelativePath", "Public/JoystickInterface.h" },
	};
#endif
	void Z_Construct_UScriptStruct_FJoystickInfo_Statics::NewProp_Connected_SetBit(void* Obj)
	{
		((FJoystickInfo*)Obj)->Connected = 1;
	}
	const UE4CodeGen_Private::FBoolPropertyParams Z_Construct_UScriptStruct_FJoystickInfo_Statics::NewProp_Connected = { "Connected", nullptr, (EPropertyFlags)0x0010000000000015, UE4CodeGen_Private::EPropertyGenFlags::Bool | UE4CodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, 1, sizeof(bool), sizeof(FJoystickInfo), &Z_Construct_UScriptStruct_FJoystickInfo_Statics::NewProp_Connected_SetBit, METADATA_PARAMS(Z_Construct_UScriptStruct_FJoystickInfo_Statics::NewProp_Connected_MetaData, UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FJoystickInfo_Statics::NewProp_Connected_MetaData)) };
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UScriptStruct_FJoystickInfo_Statics::NewProp_DeviceName_MetaData[] = {
		{ "Category", "JoystickInfo" },
		{ "ModuleRelativePath", "Public/JoystickInterface.h" },
	};
#endif
	const UE4CodeGen_Private::FStrPropertyParams Z_Construct_UScriptStruct_FJoystickInfo_Statics::NewProp_DeviceName = { "DeviceName", nullptr, (EPropertyFlags)0x0010000000000015, UE4CodeGen_Private::EPropertyGenFlags::Str, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(FJoystickInfo, DeviceName), METADATA_PARAMS(Z_Construct_UScriptStruct_FJoystickInfo_Statics::NewProp_DeviceName_MetaData, UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FJoystickInfo_Statics::NewProp_DeviceName_MetaData)) };
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UScriptStruct_FJoystickInfo_Statics::NewProp_ProductName_MetaData[] = {
		{ "Category", "JoystickInfo" },
		{ "ModuleRelativePath", "Public/JoystickInterface.h" },
	};
#endif
	const UE4CodeGen_Private::FStrPropertyParams Z_Construct_UScriptStruct_FJoystickInfo_Statics::NewProp_ProductName = { "ProductName", nullptr, (EPropertyFlags)0x0010000000000015, UE4CodeGen_Private::EPropertyGenFlags::Str, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(FJoystickInfo, ProductName), METADATA_PARAMS(Z_Construct_UScriptStruct_FJoystickInfo_Statics::NewProp_ProductName_MetaData, UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FJoystickInfo_Statics::NewProp_ProductName_MetaData)) };
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UScriptStruct_FJoystickInfo_Statics::NewProp_ProductId_MetaData[] = {
		{ "Category", "JoystickInfo" },
		{ "ModuleRelativePath", "Public/JoystickInterface.h" },
	};
#endif
	const UE4CodeGen_Private::FStructPropertyParams Z_Construct_UScriptStruct_FJoystickInfo_Statics::NewProp_ProductId = { "ProductId", nullptr, (EPropertyFlags)0x0010000000000015, UE4CodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(FJoystickInfo, ProductId), Z_Construct_UScriptStruct_FGuid, METADATA_PARAMS(Z_Construct_UScriptStruct_FJoystickInfo_Statics::NewProp_ProductId_MetaData, UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FJoystickInfo_Statics::NewProp_ProductId_MetaData)) };
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UScriptStruct_FJoystickInfo_Statics::NewProp_IsRumbleDevice_MetaData[] = {
		{ "Category", "JoystickInfo" },
		{ "ModuleRelativePath", "Public/JoystickInterface.h" },
	};
#endif
	void Z_Construct_UScriptStruct_FJoystickInfo_Statics::NewProp_IsRumbleDevice_SetBit(void* Obj)
	{
		((FJoystickInfo*)Obj)->IsRumbleDevice = 1;
	}
	const UE4CodeGen_Private::FBoolPropertyParams Z_Construct_UScriptStruct_FJoystickInfo_Statics::NewProp_IsRumbleDevice = { "IsRumbleDevice", nullptr, (EPropertyFlags)0x0010000000000015, UE4CodeGen_Private::EPropertyGenFlags::Bool | UE4CodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, 1, sizeof(bool), sizeof(FJoystickInfo), &Z_Construct_UScriptStruct_FJoystickInfo_Statics::NewProp_IsRumbleDevice_SetBit, METADATA_PARAMS(Z_Construct_UScriptStruct_FJoystickInfo_Statics::NewProp_IsRumbleDevice_MetaData, UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FJoystickInfo_Statics::NewProp_IsRumbleDevice_MetaData)) };
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UScriptStruct_FJoystickInfo_Statics::NewProp_DeviceId_MetaData[] = {
		{ "Category", "JoystickInfo" },
		{ "ModuleRelativePath", "Public/JoystickInterface.h" },
	};
#endif
	const UE4CodeGen_Private::FIntPropertyParams Z_Construct_UScriptStruct_FJoystickInfo_Statics::NewProp_DeviceId = { "DeviceId", nullptr, (EPropertyFlags)0x0010000000000015, UE4CodeGen_Private::EPropertyGenFlags::Int, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(FJoystickInfo, DeviceId), METADATA_PARAMS(Z_Construct_UScriptStruct_FJoystickInfo_Statics::NewProp_DeviceId_MetaData, UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FJoystickInfo_Statics::NewProp_DeviceId_MetaData)) };
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UScriptStruct_FJoystickInfo_Statics::NewProp_Player_MetaData[] = {
		{ "Category", "JoystickInfo" },
		{ "ModuleRelativePath", "Public/JoystickInterface.h" },
	};
#endif
	const UE4CodeGen_Private::FIntPropertyParams Z_Construct_UScriptStruct_FJoystickInfo_Statics::NewProp_Player = { "Player", nullptr, (EPropertyFlags)0x0010000000000015, UE4CodeGen_Private::EPropertyGenFlags::Int, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(FJoystickInfo, Player), METADATA_PARAMS(Z_Construct_UScriptStruct_FJoystickInfo_Statics::NewProp_Player_MetaData, UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FJoystickInfo_Statics::NewProp_Player_MetaData)) };
	const UE4CodeGen_Private::FPropertyParamsBase* const Z_Construct_UScriptStruct_FJoystickInfo_Statics::PropPointers[] = {
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FJoystickInfo_Statics::NewProp_InputType,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FJoystickInfo_Statics::NewProp_InputType_Inner,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FJoystickInfo_Statics::NewProp_InputType_Inner_Underlying,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FJoystickInfo_Statics::NewProp_Connected,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FJoystickInfo_Statics::NewProp_DeviceName,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FJoystickInfo_Statics::NewProp_ProductName,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FJoystickInfo_Statics::NewProp_ProductId,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FJoystickInfo_Statics::NewProp_IsRumbleDevice,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FJoystickInfo_Statics::NewProp_DeviceId,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FJoystickInfo_Statics::NewProp_Player,
	};
	const UE4CodeGen_Private::FStructParams Z_Construct_UScriptStruct_FJoystickInfo_Statics::ReturnStructParams = {
		(UObject* (*)())Z_Construct_UPackage__Script_JoystickPlugin,
		nullptr,
		&NewStructOps,
		"JoystickInfo",
		sizeof(FJoystickInfo),
		alignof(FJoystickInfo),
		Z_Construct_UScriptStruct_FJoystickInfo_Statics::PropPointers,
		UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FJoystickInfo_Statics::PropPointers),
		RF_Public|RF_Transient|RF_MarkAsNative,
		EStructFlags(0x00000001),
		METADATA_PARAMS(Z_Construct_UScriptStruct_FJoystickInfo_Statics::Struct_MetaDataParams, UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FJoystickInfo_Statics::Struct_MetaDataParams))
	};
	UScriptStruct* Z_Construct_UScriptStruct_FJoystickInfo()
	{
#if WITH_HOT_RELOAD
		extern uint32 Get_Z_Construct_UScriptStruct_FJoystickInfo_Hash();
		UPackage* Outer = Z_Construct_UPackage__Script_JoystickPlugin();
		static UScriptStruct* ReturnStruct = FindExistingStructIfHotReloadOrDynamic(Outer, TEXT("JoystickInfo"), sizeof(FJoystickInfo), Get_Z_Construct_UScriptStruct_FJoystickInfo_Hash(), false);
#else
		static UScriptStruct* ReturnStruct = nullptr;
#endif
		if (!ReturnStruct)
		{
			UE4CodeGen_Private::ConstructUScriptStruct(ReturnStruct, Z_Construct_UScriptStruct_FJoystickInfo_Statics::ReturnStructParams);
		}
		return ReturnStruct;
	}
	uint32 Get_Z_Construct_UScriptStruct_FJoystickInfo_Hash() { return 3557854694U; }
class UScriptStruct* FJoystickState::StaticStruct()
{
	static class UScriptStruct* Singleton = NULL;
	if (!Singleton)
	{
		extern JOYSTICKPLUGIN_API uint32 Get_Z_Construct_UScriptStruct_FJoystickState_Hash();
		Singleton = GetStaticStruct(Z_Construct_UScriptStruct_FJoystickState, Z_Construct_UPackage__Script_JoystickPlugin(), TEXT("JoystickState"), sizeof(FJoystickState), Get_Z_Construct_UScriptStruct_FJoystickState_Hash());
	}
	return Singleton;
}
template<> JOYSTICKPLUGIN_API UScriptStruct* StaticStruct<FJoystickState>()
{
	return FJoystickState::StaticStruct();
}
static FCompiledInDeferStruct Z_CompiledInDeferStruct_UScriptStruct_FJoystickState(FJoystickState::StaticStruct, TEXT("/Script/JoystickPlugin"), TEXT("JoystickState"), false, nullptr, nullptr);
static struct FScriptStruct_JoystickPlugin_StaticRegisterNativesFJoystickState
{
	FScriptStruct_JoystickPlugin_StaticRegisterNativesFJoystickState()
	{
		UScriptStruct::DeferCppStructOps(FName(TEXT("JoystickState")),new UScriptStruct::TCppStructOps<FJoystickState>);
	}
} ScriptStruct_JoystickPlugin_StaticRegisterNativesFJoystickState;
	struct Z_Construct_UScriptStruct_FJoystickState_Statics
	{
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam Struct_MetaDataParams[];
#endif
		static void* NewStructOps();
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam NewProp_Balls_MetaData[];
#endif
		static const UE4CodeGen_Private::FArrayPropertyParams NewProp_Balls;
		static const UE4CodeGen_Private::FStructPropertyParams NewProp_Balls_Inner;
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam NewProp_Hats_MetaData[];
#endif
		static const UE4CodeGen_Private::FArrayPropertyParams NewProp_Hats;
		static const UE4CodeGen_Private::FEnumPropertyParams NewProp_Hats_Inner;
		static const UE4CodeGen_Private::FBytePropertyParams NewProp_Hats_Inner_Underlying;
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam NewProp_Buttons_MetaData[];
#endif
		static const UE4CodeGen_Private::FArrayPropertyParams NewProp_Buttons;
		static const UE4CodeGen_Private::FBoolPropertyParams NewProp_Buttons_Inner;
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam NewProp_Axes_MetaData[];
#endif
		static const UE4CodeGen_Private::FArrayPropertyParams NewProp_Axes;
		static const UE4CodeGen_Private::FFloatPropertyParams NewProp_Axes_Inner;
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam NewProp_DeviceId_MetaData[];
#endif
		static const UE4CodeGen_Private::FIntPropertyParams NewProp_DeviceId;
		static const UE4CodeGen_Private::FPropertyParamsBase* const PropPointers[];
		static const UE4CodeGen_Private::FStructParams ReturnStructParams;
	};
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UScriptStruct_FJoystickState_Statics::Struct_MetaDataParams[] = {
		{ "BlueprintType", "true" },
		{ "ModuleRelativePath", "Public/JoystickInterface.h" },
	};
#endif
	void* Z_Construct_UScriptStruct_FJoystickState_Statics::NewStructOps()
	{
		return (UScriptStruct::ICppStructOps*)new UScriptStruct::TCppStructOps<FJoystickState>();
	}
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UScriptStruct_FJoystickState_Statics::NewProp_Balls_MetaData[] = {
		{ "Category", "JoystickState" },
		{ "ModuleRelativePath", "Public/JoystickInterface.h" },
	};
#endif
	const UE4CodeGen_Private::FArrayPropertyParams Z_Construct_UScriptStruct_FJoystickState_Statics::NewProp_Balls = { "Balls", nullptr, (EPropertyFlags)0x0010000000000015, UE4CodeGen_Private::EPropertyGenFlags::Array, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(FJoystickState, Balls), EArrayPropertyFlags::None, METADATA_PARAMS(Z_Construct_UScriptStruct_FJoystickState_Statics::NewProp_Balls_MetaData, UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FJoystickState_Statics::NewProp_Balls_MetaData)) };
	const UE4CodeGen_Private::FStructPropertyParams Z_Construct_UScriptStruct_FJoystickState_Statics::NewProp_Balls_Inner = { "Balls", nullptr, (EPropertyFlags)0x0000000000000000, UE4CodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, 1, 0, Z_Construct_UScriptStruct_FVector2D, METADATA_PARAMS(nullptr, 0) };
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UScriptStruct_FJoystickState_Statics::NewProp_Hats_MetaData[] = {
		{ "Category", "JoystickState" },
		{ "ModuleRelativePath", "Public/JoystickInterface.h" },
	};
#endif
	const UE4CodeGen_Private::FArrayPropertyParams Z_Construct_UScriptStruct_FJoystickState_Statics::NewProp_Hats = { "Hats", nullptr, (EPropertyFlags)0x0010000000000015, UE4CodeGen_Private::EPropertyGenFlags::Array, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(FJoystickState, Hats), EArrayPropertyFlags::None, METADATA_PARAMS(Z_Construct_UScriptStruct_FJoystickState_Statics::NewProp_Hats_MetaData, UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FJoystickState_Statics::NewProp_Hats_MetaData)) };
	const UE4CodeGen_Private::FEnumPropertyParams Z_Construct_UScriptStruct_FJoystickState_Statics::NewProp_Hats_Inner = { "Hats", nullptr, (EPropertyFlags)0x0000000000000000, UE4CodeGen_Private::EPropertyGenFlags::Enum, RF_Public|RF_Transient|RF_MarkAsNative, 1, 0, Z_Construct_UEnum_JoystickPlugin_EJoystickPOVDirection, METADATA_PARAMS(nullptr, 0) };
	const UE4CodeGen_Private::FBytePropertyParams Z_Construct_UScriptStruct_FJoystickState_Statics::NewProp_Hats_Inner_Underlying = { "UnderlyingType", nullptr, (EPropertyFlags)0x0000000000000000, UE4CodeGen_Private::EPropertyGenFlags::Byte, RF_Public|RF_Transient|RF_MarkAsNative, 1, 0, nullptr, METADATA_PARAMS(nullptr, 0) };
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UScriptStruct_FJoystickState_Statics::NewProp_Buttons_MetaData[] = {
		{ "Category", "JoystickState" },
		{ "ModuleRelativePath", "Public/JoystickInterface.h" },
	};
#endif
	const UE4CodeGen_Private::FArrayPropertyParams Z_Construct_UScriptStruct_FJoystickState_Statics::NewProp_Buttons = { "Buttons", nullptr, (EPropertyFlags)0x0010000000000015, UE4CodeGen_Private::EPropertyGenFlags::Array, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(FJoystickState, Buttons), EArrayPropertyFlags::None, METADATA_PARAMS(Z_Construct_UScriptStruct_FJoystickState_Statics::NewProp_Buttons_MetaData, UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FJoystickState_Statics::NewProp_Buttons_MetaData)) };
	const UE4CodeGen_Private::FBoolPropertyParams Z_Construct_UScriptStruct_FJoystickState_Statics::NewProp_Buttons_Inner = { "Buttons", nullptr, (EPropertyFlags)0x0000000000000000, UE4CodeGen_Private::EPropertyGenFlags::Bool | UE4CodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, 1, sizeof(bool), 0, nullptr, METADATA_PARAMS(nullptr, 0) };
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UScriptStruct_FJoystickState_Statics::NewProp_Axes_MetaData[] = {
		{ "Category", "JoystickState" },
		{ "ModuleRelativePath", "Public/JoystickInterface.h" },
	};
#endif
	const UE4CodeGen_Private::FArrayPropertyParams Z_Construct_UScriptStruct_FJoystickState_Statics::NewProp_Axes = { "Axes", nullptr, (EPropertyFlags)0x0010000000000015, UE4CodeGen_Private::EPropertyGenFlags::Array, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(FJoystickState, Axes), EArrayPropertyFlags::None, METADATA_PARAMS(Z_Construct_UScriptStruct_FJoystickState_Statics::NewProp_Axes_MetaData, UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FJoystickState_Statics::NewProp_Axes_MetaData)) };
	const UE4CodeGen_Private::FFloatPropertyParams Z_Construct_UScriptStruct_FJoystickState_Statics::NewProp_Axes_Inner = { "Axes", nullptr, (EPropertyFlags)0x0000000000000000, UE4CodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, 1, 0, METADATA_PARAMS(nullptr, 0) };
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UScriptStruct_FJoystickState_Statics::NewProp_DeviceId_MetaData[] = {
		{ "Category", "JoystickState" },
		{ "ModuleRelativePath", "Public/JoystickInterface.h" },
	};
#endif
	const UE4CodeGen_Private::FIntPropertyParams Z_Construct_UScriptStruct_FJoystickState_Statics::NewProp_DeviceId = { "DeviceId", nullptr, (EPropertyFlags)0x0010000000000015, UE4CodeGen_Private::EPropertyGenFlags::Int, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(FJoystickState, DeviceId), METADATA_PARAMS(Z_Construct_UScriptStruct_FJoystickState_Statics::NewProp_DeviceId_MetaData, UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FJoystickState_Statics::NewProp_DeviceId_MetaData)) };
	const UE4CodeGen_Private::FPropertyParamsBase* const Z_Construct_UScriptStruct_FJoystickState_Statics::PropPointers[] = {
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FJoystickState_Statics::NewProp_Balls,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FJoystickState_Statics::NewProp_Balls_Inner,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FJoystickState_Statics::NewProp_Hats,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FJoystickState_Statics::NewProp_Hats_Inner,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FJoystickState_Statics::NewProp_Hats_Inner_Underlying,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FJoystickState_Statics::NewProp_Buttons,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FJoystickState_Statics::NewProp_Buttons_Inner,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FJoystickState_Statics::NewProp_Axes,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FJoystickState_Statics::NewProp_Axes_Inner,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FJoystickState_Statics::NewProp_DeviceId,
	};
	const UE4CodeGen_Private::FStructParams Z_Construct_UScriptStruct_FJoystickState_Statics::ReturnStructParams = {
		(UObject* (*)())Z_Construct_UPackage__Script_JoystickPlugin,
		nullptr,
		&NewStructOps,
		"JoystickState",
		sizeof(FJoystickState),
		alignof(FJoystickState),
		Z_Construct_UScriptStruct_FJoystickState_Statics::PropPointers,
		UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FJoystickState_Statics::PropPointers),
		RF_Public|RF_Transient|RF_MarkAsNative,
		EStructFlags(0x00000001),
		METADATA_PARAMS(Z_Construct_UScriptStruct_FJoystickState_Statics::Struct_MetaDataParams, UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FJoystickState_Statics::Struct_MetaDataParams))
	};
	UScriptStruct* Z_Construct_UScriptStruct_FJoystickState()
	{
#if WITH_HOT_RELOAD
		extern uint32 Get_Z_Construct_UScriptStruct_FJoystickState_Hash();
		UPackage* Outer = Z_Construct_UPackage__Script_JoystickPlugin();
		static UScriptStruct* ReturnStruct = FindExistingStructIfHotReloadOrDynamic(Outer, TEXT("JoystickState"), sizeof(FJoystickState), Get_Z_Construct_UScriptStruct_FJoystickState_Hash(), false);
#else
		static UScriptStruct* ReturnStruct = nullptr;
#endif
		if (!ReturnStruct)
		{
			UE4CodeGen_Private::ConstructUScriptStruct(ReturnStruct, Z_Construct_UScriptStruct_FJoystickState_Statics::ReturnStructParams);
		}
		return ReturnStruct;
	}
	uint32 Get_Z_Construct_UScriptStruct_FJoystickState_Hash() { return 4224205833U; }
	void IJoystickInterface::JoystickAxisChanged(int32 Axis, float value, float valuePrev, FJoystickState state, FJoystickState prev)
	{
		check(0 && "Do not directly call Event functions in Interfaces. Call Execute_JoystickAxisChanged instead.");
	}
	void IJoystickInterface::JoystickBallMoved(int32 Ball, FVector2D Delta, FJoystickState State)
	{
		check(0 && "Do not directly call Event functions in Interfaces. Call Execute_JoystickBallMoved instead.");
	}
	void IJoystickInterface::JoystickButtonPressed(int32 Button, FJoystickState state)
	{
		check(0 && "Do not directly call Event functions in Interfaces. Call Execute_JoystickButtonPressed instead.");
	}
	void IJoystickInterface::JoystickButtonReleased(int32 Button, FJoystickState state)
	{
		check(0 && "Do not directly call Event functions in Interfaces. Call Execute_JoystickButtonReleased instead.");
	}
	void IJoystickInterface::JoystickHatChanged(int32 Hat, EJoystickPOVDirection Value, FJoystickState state)
	{
		check(0 && "Do not directly call Event functions in Interfaces. Call Execute_JoystickHatChanged instead.");
	}
	void IJoystickInterface::JoystickPluggedIn(int32 DeviceId)
	{
		check(0 && "Do not directly call Event functions in Interfaces. Call Execute_JoystickPluggedIn instead.");
	}
	void IJoystickInterface::JoystickUnplugged(int32 DeviceId)
	{
		check(0 && "Do not directly call Event functions in Interfaces. Call Execute_JoystickUnplugged instead.");
	}
	void UJoystickInterface::StaticRegisterNativesUJoystickInterface()
	{
	}
	struct Z_Construct_UFunction_UJoystickInterface_JoystickAxisChanged_Statics
	{
		static const UE4CodeGen_Private::FStructPropertyParams NewProp_prev;
		static const UE4CodeGen_Private::FStructPropertyParams NewProp_state;
		static const UE4CodeGen_Private::FFloatPropertyParams NewProp_valuePrev;
		static const UE4CodeGen_Private::FFloatPropertyParams NewProp_value;
		static const UE4CodeGen_Private::FIntPropertyParams NewProp_Axis;
		static const UE4CodeGen_Private::FPropertyParamsBase* const PropPointers[];
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam Function_MetaDataParams[];
#endif
		static const UE4CodeGen_Private::FFunctionParams FuncParams;
	};
	const UE4CodeGen_Private::FStructPropertyParams Z_Construct_UFunction_UJoystickInterface_JoystickAxisChanged_Statics::NewProp_prev = { "prev", nullptr, (EPropertyFlags)0x0010000000000080, UE4CodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(JoystickInterface_eventJoystickAxisChanged_Parms, prev), Z_Construct_UScriptStruct_FJoystickState, METADATA_PARAMS(nullptr, 0) };
	const UE4CodeGen_Private::FStructPropertyParams Z_Construct_UFunction_UJoystickInterface_JoystickAxisChanged_Statics::NewProp_state = { "state", nullptr, (EPropertyFlags)0x0010000000000080, UE4CodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(JoystickInterface_eventJoystickAxisChanged_Parms, state), Z_Construct_UScriptStruct_FJoystickState, METADATA_PARAMS(nullptr, 0) };
	const UE4CodeGen_Private::FFloatPropertyParams Z_Construct_UFunction_UJoystickInterface_JoystickAxisChanged_Statics::NewProp_valuePrev = { "valuePrev", nullptr, (EPropertyFlags)0x0010000000000080, UE4CodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(JoystickInterface_eventJoystickAxisChanged_Parms, valuePrev), METADATA_PARAMS(nullptr, 0) };
	const UE4CodeGen_Private::FFloatPropertyParams Z_Construct_UFunction_UJoystickInterface_JoystickAxisChanged_Statics::NewProp_value = { "value", nullptr, (EPropertyFlags)0x0010000000000080, UE4CodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(JoystickInterface_eventJoystickAxisChanged_Parms, value), METADATA_PARAMS(nullptr, 0) };
	const UE4CodeGen_Private::FIntPropertyParams Z_Construct_UFunction_UJoystickInterface_JoystickAxisChanged_Statics::NewProp_Axis = { "Axis", nullptr, (EPropertyFlags)0x0010000000000080, UE4CodeGen_Private::EPropertyGenFlags::Int, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(JoystickInterface_eventJoystickAxisChanged_Parms, Axis), METADATA_PARAMS(nullptr, 0) };
	const UE4CodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UJoystickInterface_JoystickAxisChanged_Statics::PropPointers[] = {
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UJoystickInterface_JoystickAxisChanged_Statics::NewProp_prev,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UJoystickInterface_JoystickAxisChanged_Statics::NewProp_state,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UJoystickInterface_JoystickAxisChanged_Statics::NewProp_valuePrev,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UJoystickInterface_JoystickAxisChanged_Statics::NewProp_value,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UJoystickInterface_JoystickAxisChanged_Statics::NewProp_Axis,
	};
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UFunction_UJoystickInterface_JoystickAxisChanged_Statics::Function_MetaDataParams[] = {
		{ "Category", "Joystick" },
		{ "ModuleRelativePath", "Public/JoystickInterface.h" },
	};
#endif
	const UE4CodeGen_Private::FFunctionParams Z_Construct_UFunction_UJoystickInterface_JoystickAxisChanged_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_UJoystickInterface, nullptr, "JoystickAxisChanged", nullptr, nullptr, sizeof(JoystickInterface_eventJoystickAxisChanged_Parms), Z_Construct_UFunction_UJoystickInterface_JoystickAxisChanged_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_UJoystickInterface_JoystickAxisChanged_Statics::PropPointers), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x08020800, 0, 0, METADATA_PARAMS(Z_Construct_UFunction_UJoystickInterface_JoystickAxisChanged_Statics::Function_MetaDataParams, UE_ARRAY_COUNT(Z_Construct_UFunction_UJoystickInterface_JoystickAxisChanged_Statics::Function_MetaDataParams)) };
	UFunction* Z_Construct_UFunction_UJoystickInterface_JoystickAxisChanged()
	{
		static UFunction* ReturnFunction = nullptr;
		if (!ReturnFunction)
		{
			UE4CodeGen_Private::ConstructUFunction(ReturnFunction, Z_Construct_UFunction_UJoystickInterface_JoystickAxisChanged_Statics::FuncParams);
		}
		return ReturnFunction;
	}
	struct Z_Construct_UFunction_UJoystickInterface_JoystickBallMoved_Statics
	{
		static const UE4CodeGen_Private::FStructPropertyParams NewProp_State;
		static const UE4CodeGen_Private::FStructPropertyParams NewProp_Delta;
		static const UE4CodeGen_Private::FIntPropertyParams NewProp_Ball;
		static const UE4CodeGen_Private::FPropertyParamsBase* const PropPointers[];
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam Function_MetaDataParams[];
#endif
		static const UE4CodeGen_Private::FFunctionParams FuncParams;
	};
	const UE4CodeGen_Private::FStructPropertyParams Z_Construct_UFunction_UJoystickInterface_JoystickBallMoved_Statics::NewProp_State = { "State", nullptr, (EPropertyFlags)0x0010000000000080, UE4CodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(JoystickInterface_eventJoystickBallMoved_Parms, State), Z_Construct_UScriptStruct_FJoystickState, METADATA_PARAMS(nullptr, 0) };
	const UE4CodeGen_Private::FStructPropertyParams Z_Construct_UFunction_UJoystickInterface_JoystickBallMoved_Statics::NewProp_Delta = { "Delta", nullptr, (EPropertyFlags)0x0010000000000080, UE4CodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(JoystickInterface_eventJoystickBallMoved_Parms, Delta), Z_Construct_UScriptStruct_FVector2D, METADATA_PARAMS(nullptr, 0) };
	const UE4CodeGen_Private::FIntPropertyParams Z_Construct_UFunction_UJoystickInterface_JoystickBallMoved_Statics::NewProp_Ball = { "Ball", nullptr, (EPropertyFlags)0x0010000000000080, UE4CodeGen_Private::EPropertyGenFlags::Int, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(JoystickInterface_eventJoystickBallMoved_Parms, Ball), METADATA_PARAMS(nullptr, 0) };
	const UE4CodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UJoystickInterface_JoystickBallMoved_Statics::PropPointers[] = {
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UJoystickInterface_JoystickBallMoved_Statics::NewProp_State,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UJoystickInterface_JoystickBallMoved_Statics::NewProp_Delta,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UJoystickInterface_JoystickBallMoved_Statics::NewProp_Ball,
	};
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UFunction_UJoystickInterface_JoystickBallMoved_Statics::Function_MetaDataParams[] = {
		{ "Category", "Joystick" },
		{ "ModuleRelativePath", "Public/JoystickInterface.h" },
	};
#endif
	const UE4CodeGen_Private::FFunctionParams Z_Construct_UFunction_UJoystickInterface_JoystickBallMoved_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_UJoystickInterface, nullptr, "JoystickBallMoved", nullptr, nullptr, sizeof(JoystickInterface_eventJoystickBallMoved_Parms), Z_Construct_UFunction_UJoystickInterface_JoystickBallMoved_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_UJoystickInterface_JoystickBallMoved_Statics::PropPointers), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x08820800, 0, 0, METADATA_PARAMS(Z_Construct_UFunction_UJoystickInterface_JoystickBallMoved_Statics::Function_MetaDataParams, UE_ARRAY_COUNT(Z_Construct_UFunction_UJoystickInterface_JoystickBallMoved_Statics::Function_MetaDataParams)) };
	UFunction* Z_Construct_UFunction_UJoystickInterface_JoystickBallMoved()
	{
		static UFunction* ReturnFunction = nullptr;
		if (!ReturnFunction)
		{
			UE4CodeGen_Private::ConstructUFunction(ReturnFunction, Z_Construct_UFunction_UJoystickInterface_JoystickBallMoved_Statics::FuncParams);
		}
		return ReturnFunction;
	}
	struct Z_Construct_UFunction_UJoystickInterface_JoystickButtonPressed_Statics
	{
		static const UE4CodeGen_Private::FStructPropertyParams NewProp_state;
		static const UE4CodeGen_Private::FIntPropertyParams NewProp_Button;
		static const UE4CodeGen_Private::FPropertyParamsBase* const PropPointers[];
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam Function_MetaDataParams[];
#endif
		static const UE4CodeGen_Private::FFunctionParams FuncParams;
	};
	const UE4CodeGen_Private::FStructPropertyParams Z_Construct_UFunction_UJoystickInterface_JoystickButtonPressed_Statics::NewProp_state = { "state", nullptr, (EPropertyFlags)0x0010000000000080, UE4CodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(JoystickInterface_eventJoystickButtonPressed_Parms, state), Z_Construct_UScriptStruct_FJoystickState, METADATA_PARAMS(nullptr, 0) };
	const UE4CodeGen_Private::FIntPropertyParams Z_Construct_UFunction_UJoystickInterface_JoystickButtonPressed_Statics::NewProp_Button = { "Button", nullptr, (EPropertyFlags)0x0010000000000080, UE4CodeGen_Private::EPropertyGenFlags::Int, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(JoystickInterface_eventJoystickButtonPressed_Parms, Button), METADATA_PARAMS(nullptr, 0) };
	const UE4CodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UJoystickInterface_JoystickButtonPressed_Statics::PropPointers[] = {
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UJoystickInterface_JoystickButtonPressed_Statics::NewProp_state,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UJoystickInterface_JoystickButtonPressed_Statics::NewProp_Button,
	};
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UFunction_UJoystickInterface_JoystickButtonPressed_Statics::Function_MetaDataParams[] = {
		{ "Category", "Joystick" },
		{ "Comment", "//Define blueprint events\n" },
		{ "ModuleRelativePath", "Public/JoystickInterface.h" },
		{ "ToolTip", "Define blueprint events" },
	};
#endif
	const UE4CodeGen_Private::FFunctionParams Z_Construct_UFunction_UJoystickInterface_JoystickButtonPressed_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_UJoystickInterface, nullptr, "JoystickButtonPressed", nullptr, nullptr, sizeof(JoystickInterface_eventJoystickButtonPressed_Parms), Z_Construct_UFunction_UJoystickInterface_JoystickButtonPressed_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_UJoystickInterface_JoystickButtonPressed_Statics::PropPointers), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x08020800, 0, 0, METADATA_PARAMS(Z_Construct_UFunction_UJoystickInterface_JoystickButtonPressed_Statics::Function_MetaDataParams, UE_ARRAY_COUNT(Z_Construct_UFunction_UJoystickInterface_JoystickButtonPressed_Statics::Function_MetaDataParams)) };
	UFunction* Z_Construct_UFunction_UJoystickInterface_JoystickButtonPressed()
	{
		static UFunction* ReturnFunction = nullptr;
		if (!ReturnFunction)
		{
			UE4CodeGen_Private::ConstructUFunction(ReturnFunction, Z_Construct_UFunction_UJoystickInterface_JoystickButtonPressed_Statics::FuncParams);
		}
		return ReturnFunction;
	}
	struct Z_Construct_UFunction_UJoystickInterface_JoystickButtonReleased_Statics
	{
		static const UE4CodeGen_Private::FStructPropertyParams NewProp_state;
		static const UE4CodeGen_Private::FIntPropertyParams NewProp_Button;
		static const UE4CodeGen_Private::FPropertyParamsBase* const PropPointers[];
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam Function_MetaDataParams[];
#endif
		static const UE4CodeGen_Private::FFunctionParams FuncParams;
	};
	const UE4CodeGen_Private::FStructPropertyParams Z_Construct_UFunction_UJoystickInterface_JoystickButtonReleased_Statics::NewProp_state = { "state", nullptr, (EPropertyFlags)0x0010000000000080, UE4CodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(JoystickInterface_eventJoystickButtonReleased_Parms, state), Z_Construct_UScriptStruct_FJoystickState, METADATA_PARAMS(nullptr, 0) };
	const UE4CodeGen_Private::FIntPropertyParams Z_Construct_UFunction_UJoystickInterface_JoystickButtonReleased_Statics::NewProp_Button = { "Button", nullptr, (EPropertyFlags)0x0010000000000080, UE4CodeGen_Private::EPropertyGenFlags::Int, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(JoystickInterface_eventJoystickButtonReleased_Parms, Button), METADATA_PARAMS(nullptr, 0) };
	const UE4CodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UJoystickInterface_JoystickButtonReleased_Statics::PropPointers[] = {
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UJoystickInterface_JoystickButtonReleased_Statics::NewProp_state,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UJoystickInterface_JoystickButtonReleased_Statics::NewProp_Button,
	};
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UFunction_UJoystickInterface_JoystickButtonReleased_Statics::Function_MetaDataParams[] = {
		{ "Category", "Joystick" },
		{ "ModuleRelativePath", "Public/JoystickInterface.h" },
	};
#endif
	const UE4CodeGen_Private::FFunctionParams Z_Construct_UFunction_UJoystickInterface_JoystickButtonReleased_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_UJoystickInterface, nullptr, "JoystickButtonReleased", nullptr, nullptr, sizeof(JoystickInterface_eventJoystickButtonReleased_Parms), Z_Construct_UFunction_UJoystickInterface_JoystickButtonReleased_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_UJoystickInterface_JoystickButtonReleased_Statics::PropPointers), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x08020800, 0, 0, METADATA_PARAMS(Z_Construct_UFunction_UJoystickInterface_JoystickButtonReleased_Statics::Function_MetaDataParams, UE_ARRAY_COUNT(Z_Construct_UFunction_UJoystickInterface_JoystickButtonReleased_Statics::Function_MetaDataParams)) };
	UFunction* Z_Construct_UFunction_UJoystickInterface_JoystickButtonReleased()
	{
		static UFunction* ReturnFunction = nullptr;
		if (!ReturnFunction)
		{
			UE4CodeGen_Private::ConstructUFunction(ReturnFunction, Z_Construct_UFunction_UJoystickInterface_JoystickButtonReleased_Statics::FuncParams);
		}
		return ReturnFunction;
	}
	struct Z_Construct_UFunction_UJoystickInterface_JoystickHatChanged_Statics
	{
		static const UE4CodeGen_Private::FStructPropertyParams NewProp_state;
		static const UE4CodeGen_Private::FEnumPropertyParams NewProp_Value;
		static const UE4CodeGen_Private::FBytePropertyParams NewProp_Value_Underlying;
		static const UE4CodeGen_Private::FIntPropertyParams NewProp_Hat;
		static const UE4CodeGen_Private::FPropertyParamsBase* const PropPointers[];
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam Function_MetaDataParams[];
#endif
		static const UE4CodeGen_Private::FFunctionParams FuncParams;
	};
	const UE4CodeGen_Private::FStructPropertyParams Z_Construct_UFunction_UJoystickInterface_JoystickHatChanged_Statics::NewProp_state = { "state", nullptr, (EPropertyFlags)0x0010000000000080, UE4CodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(JoystickInterface_eventJoystickHatChanged_Parms, state), Z_Construct_UScriptStruct_FJoystickState, METADATA_PARAMS(nullptr, 0) };
	const UE4CodeGen_Private::FEnumPropertyParams Z_Construct_UFunction_UJoystickInterface_JoystickHatChanged_Statics::NewProp_Value = { "Value", nullptr, (EPropertyFlags)0x0010000000000080, UE4CodeGen_Private::EPropertyGenFlags::Enum, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(JoystickInterface_eventJoystickHatChanged_Parms, Value), Z_Construct_UEnum_JoystickPlugin_EJoystickPOVDirection, METADATA_PARAMS(nullptr, 0) };
	const UE4CodeGen_Private::FBytePropertyParams Z_Construct_UFunction_UJoystickInterface_JoystickHatChanged_Statics::NewProp_Value_Underlying = { "UnderlyingType", nullptr, (EPropertyFlags)0x0000000000000000, UE4CodeGen_Private::EPropertyGenFlags::Byte, RF_Public|RF_Transient|RF_MarkAsNative, 1, 0, nullptr, METADATA_PARAMS(nullptr, 0) };
	const UE4CodeGen_Private::FIntPropertyParams Z_Construct_UFunction_UJoystickInterface_JoystickHatChanged_Statics::NewProp_Hat = { "Hat", nullptr, (EPropertyFlags)0x0010000000000080, UE4CodeGen_Private::EPropertyGenFlags::Int, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(JoystickInterface_eventJoystickHatChanged_Parms, Hat), METADATA_PARAMS(nullptr, 0) };
	const UE4CodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UJoystickInterface_JoystickHatChanged_Statics::PropPointers[] = {
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UJoystickInterface_JoystickHatChanged_Statics::NewProp_state,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UJoystickInterface_JoystickHatChanged_Statics::NewProp_Value,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UJoystickInterface_JoystickHatChanged_Statics::NewProp_Value_Underlying,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UJoystickInterface_JoystickHatChanged_Statics::NewProp_Hat,
	};
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UFunction_UJoystickInterface_JoystickHatChanged_Statics::Function_MetaDataParams[] = {
		{ "Category", "Joystick" },
		{ "ModuleRelativePath", "Public/JoystickInterface.h" },
	};
#endif
	const UE4CodeGen_Private::FFunctionParams Z_Construct_UFunction_UJoystickInterface_JoystickHatChanged_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_UJoystickInterface, nullptr, "JoystickHatChanged", nullptr, nullptr, sizeof(JoystickInterface_eventJoystickHatChanged_Parms), Z_Construct_UFunction_UJoystickInterface_JoystickHatChanged_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_UJoystickInterface_JoystickHatChanged_Statics::PropPointers), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x08020800, 0, 0, METADATA_PARAMS(Z_Construct_UFunction_UJoystickInterface_JoystickHatChanged_Statics::Function_MetaDataParams, UE_ARRAY_COUNT(Z_Construct_UFunction_UJoystickInterface_JoystickHatChanged_Statics::Function_MetaDataParams)) };
	UFunction* Z_Construct_UFunction_UJoystickInterface_JoystickHatChanged()
	{
		static UFunction* ReturnFunction = nullptr;
		if (!ReturnFunction)
		{
			UE4CodeGen_Private::ConstructUFunction(ReturnFunction, Z_Construct_UFunction_UJoystickInterface_JoystickHatChanged_Statics::FuncParams);
		}
		return ReturnFunction;
	}
	struct Z_Construct_UFunction_UJoystickInterface_JoystickPluggedIn_Statics
	{
		static const UE4CodeGen_Private::FIntPropertyParams NewProp_DeviceId;
		static const UE4CodeGen_Private::FPropertyParamsBase* const PropPointers[];
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam Function_MetaDataParams[];
#endif
		static const UE4CodeGen_Private::FFunctionParams FuncParams;
	};
	const UE4CodeGen_Private::FIntPropertyParams Z_Construct_UFunction_UJoystickInterface_JoystickPluggedIn_Statics::NewProp_DeviceId = { "DeviceId", nullptr, (EPropertyFlags)0x0010000000000080, UE4CodeGen_Private::EPropertyGenFlags::Int, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(JoystickInterface_eventJoystickPluggedIn_Parms, DeviceId), METADATA_PARAMS(nullptr, 0) };
	const UE4CodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UJoystickInterface_JoystickPluggedIn_Statics::PropPointers[] = {
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UJoystickInterface_JoystickPluggedIn_Statics::NewProp_DeviceId,
	};
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UFunction_UJoystickInterface_JoystickPluggedIn_Statics::Function_MetaDataParams[] = {
		{ "Category", "Joystick" },
		{ "ModuleRelativePath", "Public/JoystickInterface.h" },
	};
#endif
	const UE4CodeGen_Private::FFunctionParams Z_Construct_UFunction_UJoystickInterface_JoystickPluggedIn_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_UJoystickInterface, nullptr, "JoystickPluggedIn", nullptr, nullptr, sizeof(JoystickInterface_eventJoystickPluggedIn_Parms), Z_Construct_UFunction_UJoystickInterface_JoystickPluggedIn_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_UJoystickInterface_JoystickPluggedIn_Statics::PropPointers), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x08020800, 0, 0, METADATA_PARAMS(Z_Construct_UFunction_UJoystickInterface_JoystickPluggedIn_Statics::Function_MetaDataParams, UE_ARRAY_COUNT(Z_Construct_UFunction_UJoystickInterface_JoystickPluggedIn_Statics::Function_MetaDataParams)) };
	UFunction* Z_Construct_UFunction_UJoystickInterface_JoystickPluggedIn()
	{
		static UFunction* ReturnFunction = nullptr;
		if (!ReturnFunction)
		{
			UE4CodeGen_Private::ConstructUFunction(ReturnFunction, Z_Construct_UFunction_UJoystickInterface_JoystickPluggedIn_Statics::FuncParams);
		}
		return ReturnFunction;
	}
	struct Z_Construct_UFunction_UJoystickInterface_JoystickUnplugged_Statics
	{
		static const UE4CodeGen_Private::FIntPropertyParams NewProp_DeviceId;
		static const UE4CodeGen_Private::FPropertyParamsBase* const PropPointers[];
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam Function_MetaDataParams[];
#endif
		static const UE4CodeGen_Private::FFunctionParams FuncParams;
	};
	const UE4CodeGen_Private::FIntPropertyParams Z_Construct_UFunction_UJoystickInterface_JoystickUnplugged_Statics::NewProp_DeviceId = { "DeviceId", nullptr, (EPropertyFlags)0x0010000000000080, UE4CodeGen_Private::EPropertyGenFlags::Int, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(JoystickInterface_eventJoystickUnplugged_Parms, DeviceId), METADATA_PARAMS(nullptr, 0) };
	const UE4CodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UJoystickInterface_JoystickUnplugged_Statics::PropPointers[] = {
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UJoystickInterface_JoystickUnplugged_Statics::NewProp_DeviceId,
	};
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UFunction_UJoystickInterface_JoystickUnplugged_Statics::Function_MetaDataParams[] = {
		{ "Category", "Joystick" },
		{ "ModuleRelativePath", "Public/JoystickInterface.h" },
	};
#endif
	const UE4CodeGen_Private::FFunctionParams Z_Construct_UFunction_UJoystickInterface_JoystickUnplugged_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_UJoystickInterface, nullptr, "JoystickUnplugged", nullptr, nullptr, sizeof(JoystickInterface_eventJoystickUnplugged_Parms), Z_Construct_UFunction_UJoystickInterface_JoystickUnplugged_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_UJoystickInterface_JoystickUnplugged_Statics::PropPointers), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x08020800, 0, 0, METADATA_PARAMS(Z_Construct_UFunction_UJoystickInterface_JoystickUnplugged_Statics::Function_MetaDataParams, UE_ARRAY_COUNT(Z_Construct_UFunction_UJoystickInterface_JoystickUnplugged_Statics::Function_MetaDataParams)) };
	UFunction* Z_Construct_UFunction_UJoystickInterface_JoystickUnplugged()
	{
		static UFunction* ReturnFunction = nullptr;
		if (!ReturnFunction)
		{
			UE4CodeGen_Private::ConstructUFunction(ReturnFunction, Z_Construct_UFunction_UJoystickInterface_JoystickUnplugged_Statics::FuncParams);
		}
		return ReturnFunction;
	}
	UClass* Z_Construct_UClass_UJoystickInterface_NoRegister()
	{
		return UJoystickInterface::StaticClass();
	}
	struct Z_Construct_UClass_UJoystickInterface_Statics
	{
		static UObject* (*const DependentSingletons[])();
		static const FClassFunctionLinkInfo FuncInfo[];
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam Class_MetaDataParams[];
#endif
		static const FCppClassTypeInfoStatic StaticCppClassTypeInfo;
		static const UE4CodeGen_Private::FClassParams ClassParams;
	};
	UObject* (*const Z_Construct_UClass_UJoystickInterface_Statics::DependentSingletons[])() = {
		(UObject* (*)())Z_Construct_UClass_UInterface,
		(UObject* (*)())Z_Construct_UPackage__Script_JoystickPlugin,
	};
	const FClassFunctionLinkInfo Z_Construct_UClass_UJoystickInterface_Statics::FuncInfo[] = {
		{ &Z_Construct_UFunction_UJoystickInterface_JoystickAxisChanged, "JoystickAxisChanged" }, // 153523196
		{ &Z_Construct_UFunction_UJoystickInterface_JoystickBallMoved, "JoystickBallMoved" }, // 3278626726
		{ &Z_Construct_UFunction_UJoystickInterface_JoystickButtonPressed, "JoystickButtonPressed" }, // 1187857429
		{ &Z_Construct_UFunction_UJoystickInterface_JoystickButtonReleased, "JoystickButtonReleased" }, // 381166946
		{ &Z_Construct_UFunction_UJoystickInterface_JoystickHatChanged, "JoystickHatChanged" }, // 3263292493
		{ &Z_Construct_UFunction_UJoystickInterface_JoystickPluggedIn, "JoystickPluggedIn" }, // 3745455586
		{ &Z_Construct_UFunction_UJoystickInterface_JoystickUnplugged, "JoystickUnplugged" }, // 508790596
	};
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UJoystickInterface_Statics::Class_MetaDataParams[] = {
		{ "ModuleRelativePath", "Public/JoystickInterface.h" },
	};
#endif
	const FCppClassTypeInfoStatic Z_Construct_UClass_UJoystickInterface_Statics::StaticCppClassTypeInfo = {
		TCppClassTypeTraits<IJoystickInterface>::IsAbstract,
	};
	const UE4CodeGen_Private::FClassParams Z_Construct_UClass_UJoystickInterface_Statics::ClassParams = {
		&UJoystickInterface::StaticClass,
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
		0x000840A1u,
		METADATA_PARAMS(Z_Construct_UClass_UJoystickInterface_Statics::Class_MetaDataParams, UE_ARRAY_COUNT(Z_Construct_UClass_UJoystickInterface_Statics::Class_MetaDataParams))
	};
	UClass* Z_Construct_UClass_UJoystickInterface()
	{
		static UClass* OuterClass = nullptr;
		if (!OuterClass)
		{
			UE4CodeGen_Private::ConstructUClass(OuterClass, Z_Construct_UClass_UJoystickInterface_Statics::ClassParams);
		}
		return OuterClass;
	}
	IMPLEMENT_CLASS(UJoystickInterface, 3467091861);
	template<> JOYSTICKPLUGIN_API UClass* StaticClass<UJoystickInterface>()
	{
		return UJoystickInterface::StaticClass();
	}
	static FCompiledInDefer Z_CompiledInDefer_UClass_UJoystickInterface(Z_Construct_UClass_UJoystickInterface, &UJoystickInterface::StaticClass, TEXT("/Script/JoystickPlugin"), TEXT("UJoystickInterface"), false, nullptr, nullptr, nullptr);
	DEFINE_VTABLE_PTR_HELPER_CTOR(UJoystickInterface);
	static FName NAME_UJoystickInterface_JoystickAxisChanged = FName(TEXT("JoystickAxisChanged"));
	void IJoystickInterface::Execute_JoystickAxisChanged(UObject* O, int32 Axis, float value, float valuePrev, FJoystickState state, FJoystickState prev)
	{
		check(O != NULL);
		check(O->GetClass()->ImplementsInterface(UJoystickInterface::StaticClass()));
		JoystickInterface_eventJoystickAxisChanged_Parms Parms;
		UFunction* const Func = O->FindFunction(NAME_UJoystickInterface_JoystickAxisChanged);
		if (Func)
		{
			Parms.Axis=Axis;
			Parms.value=value;
			Parms.valuePrev=valuePrev;
			Parms.state=state;
			Parms.prev=prev;
			O->ProcessEvent(Func, &Parms);
		}
	}
	static FName NAME_UJoystickInterface_JoystickBallMoved = FName(TEXT("JoystickBallMoved"));
	void IJoystickInterface::Execute_JoystickBallMoved(UObject* O, int32 Ball, FVector2D Delta, FJoystickState State)
	{
		check(O != NULL);
		check(O->GetClass()->ImplementsInterface(UJoystickInterface::StaticClass()));
		JoystickInterface_eventJoystickBallMoved_Parms Parms;
		UFunction* const Func = O->FindFunction(NAME_UJoystickInterface_JoystickBallMoved);
		if (Func)
		{
			Parms.Ball=Ball;
			Parms.Delta=Delta;
			Parms.State=State;
			O->ProcessEvent(Func, &Parms);
		}
	}
	static FName NAME_UJoystickInterface_JoystickButtonPressed = FName(TEXT("JoystickButtonPressed"));
	void IJoystickInterface::Execute_JoystickButtonPressed(UObject* O, int32 Button, FJoystickState state)
	{
		check(O != NULL);
		check(O->GetClass()->ImplementsInterface(UJoystickInterface::StaticClass()));
		JoystickInterface_eventJoystickButtonPressed_Parms Parms;
		UFunction* const Func = O->FindFunction(NAME_UJoystickInterface_JoystickButtonPressed);
		if (Func)
		{
			Parms.Button=Button;
			Parms.state=state;
			O->ProcessEvent(Func, &Parms);
		}
	}
	static FName NAME_UJoystickInterface_JoystickButtonReleased = FName(TEXT("JoystickButtonReleased"));
	void IJoystickInterface::Execute_JoystickButtonReleased(UObject* O, int32 Button, FJoystickState state)
	{
		check(O != NULL);
		check(O->GetClass()->ImplementsInterface(UJoystickInterface::StaticClass()));
		JoystickInterface_eventJoystickButtonReleased_Parms Parms;
		UFunction* const Func = O->FindFunction(NAME_UJoystickInterface_JoystickButtonReleased);
		if (Func)
		{
			Parms.Button=Button;
			Parms.state=state;
			O->ProcessEvent(Func, &Parms);
		}
	}
	static FName NAME_UJoystickInterface_JoystickHatChanged = FName(TEXT("JoystickHatChanged"));
	void IJoystickInterface::Execute_JoystickHatChanged(UObject* O, int32 Hat, EJoystickPOVDirection Value, FJoystickState state)
	{
		check(O != NULL);
		check(O->GetClass()->ImplementsInterface(UJoystickInterface::StaticClass()));
		JoystickInterface_eventJoystickHatChanged_Parms Parms;
		UFunction* const Func = O->FindFunction(NAME_UJoystickInterface_JoystickHatChanged);
		if (Func)
		{
			Parms.Hat=Hat;
			Parms.Value=Value;
			Parms.state=state;
			O->ProcessEvent(Func, &Parms);
		}
	}
	static FName NAME_UJoystickInterface_JoystickPluggedIn = FName(TEXT("JoystickPluggedIn"));
	void IJoystickInterface::Execute_JoystickPluggedIn(UObject* O, int32 DeviceId)
	{
		check(O != NULL);
		check(O->GetClass()->ImplementsInterface(UJoystickInterface::StaticClass()));
		JoystickInterface_eventJoystickPluggedIn_Parms Parms;
		UFunction* const Func = O->FindFunction(NAME_UJoystickInterface_JoystickPluggedIn);
		if (Func)
		{
			Parms.DeviceId=DeviceId;
			O->ProcessEvent(Func, &Parms);
		}
	}
	static FName NAME_UJoystickInterface_JoystickUnplugged = FName(TEXT("JoystickUnplugged"));
	void IJoystickInterface::Execute_JoystickUnplugged(UObject* O, int32 DeviceId)
	{
		check(O != NULL);
		check(O->GetClass()->ImplementsInterface(UJoystickInterface::StaticClass()));
		JoystickInterface_eventJoystickUnplugged_Parms Parms;
		UFunction* const Func = O->FindFunction(NAME_UJoystickInterface_JoystickUnplugged);
		if (Func)
		{
			Parms.DeviceId=DeviceId;
			O->ProcessEvent(Func, &Parms);
		}
	}
PRAGMA_ENABLE_DEPRECATION_WARNINGS
#ifdef _MSC_VER
#pragma warning (pop)
#endif
