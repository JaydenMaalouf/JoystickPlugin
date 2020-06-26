// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/ObjectMacros.h"
#include "UObject/ScriptMacros.h"

PRAGMA_DISABLE_DEPRECATION_WARNINGS
class UObject;
struct FJoystickState;
struct FJoystickInfo;
enum class EJoystickPOVDirection : uint8;
struct FVector2D;
#ifdef JOYSTICKPLUGIN_JoystickFunctions_generated_h
#error "JoystickFunctions.generated.h already included, missing '#pragma once' in JoystickFunctions.h"
#endif
#define JOYSTICKPLUGIN_JoystickFunctions_generated_h

#define HostProject_Plugins_JoystickPlugin_Source_JoystickPlugin_Public_JoystickFunctions_h_20_SPARSE_DATA
#define HostProject_Plugins_JoystickPlugin_Source_JoystickPlugin_Public_JoystickFunctions_h_20_RPC_WRAPPERS \
 \
	DECLARE_FUNCTION(execIgnoreGameControllers); \
	DECLARE_FUNCTION(execMapJoystickDeviceToPlayer); \
	DECLARE_FUNCTION(execRegisterForJoystickEvents); \
	DECLARE_FUNCTION(execJoystickCount); \
	DECLARE_FUNCTION(execGetPreviousJoystickState); \
	DECLARE_FUNCTION(execGetJoystickState); \
	DECLARE_FUNCTION(execGetJoystick); \
	DECLARE_FUNCTION(execPOVAxis);


#define HostProject_Plugins_JoystickPlugin_Source_JoystickPlugin_Public_JoystickFunctions_h_20_RPC_WRAPPERS_NO_PURE_DECLS \
 \
	DECLARE_FUNCTION(execIgnoreGameControllers); \
	DECLARE_FUNCTION(execMapJoystickDeviceToPlayer); \
	DECLARE_FUNCTION(execRegisterForJoystickEvents); \
	DECLARE_FUNCTION(execJoystickCount); \
	DECLARE_FUNCTION(execGetPreviousJoystickState); \
	DECLARE_FUNCTION(execGetJoystickState); \
	DECLARE_FUNCTION(execGetJoystick); \
	DECLARE_FUNCTION(execPOVAxis);


#define HostProject_Plugins_JoystickPlugin_Source_JoystickPlugin_Public_JoystickFunctions_h_20_INCLASS_NO_PURE_DECLS \
private: \
	static void StaticRegisterNativesUJoystickFunctions(); \
	friend struct Z_Construct_UClass_UJoystickFunctions_Statics; \
public: \
	DECLARE_CLASS(UJoystickFunctions, UObject, COMPILED_IN_FLAGS(0), CASTCLASS_None, TEXT("/Script/JoystickPlugin"), NO_API) \
	DECLARE_SERIALIZER(UJoystickFunctions)


#define HostProject_Plugins_JoystickPlugin_Source_JoystickPlugin_Public_JoystickFunctions_h_20_INCLASS \
private: \
	static void StaticRegisterNativesUJoystickFunctions(); \
	friend struct Z_Construct_UClass_UJoystickFunctions_Statics; \
public: \
	DECLARE_CLASS(UJoystickFunctions, UObject, COMPILED_IN_FLAGS(0), CASTCLASS_None, TEXT("/Script/JoystickPlugin"), NO_API) \
	DECLARE_SERIALIZER(UJoystickFunctions)


#define HostProject_Plugins_JoystickPlugin_Source_JoystickPlugin_Public_JoystickFunctions_h_20_STANDARD_CONSTRUCTORS \
	/** Standard constructor, called after all reflected properties have been initialized */ \
	NO_API UJoystickFunctions(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get()); \
	DEFINE_DEFAULT_OBJECT_INITIALIZER_CONSTRUCTOR_CALL(UJoystickFunctions) \
	DECLARE_VTABLE_PTR_HELPER_CTOR(NO_API, UJoystickFunctions); \
DEFINE_VTABLE_PTR_HELPER_CTOR_CALLER(UJoystickFunctions); \
private: \
	/** Private move- and copy-constructors, should never be used */ \
	NO_API UJoystickFunctions(UJoystickFunctions&&); \
	NO_API UJoystickFunctions(const UJoystickFunctions&); \
public:


#define HostProject_Plugins_JoystickPlugin_Source_JoystickPlugin_Public_JoystickFunctions_h_20_ENHANCED_CONSTRUCTORS \
	/** Standard constructor, called after all reflected properties have been initialized */ \
	NO_API UJoystickFunctions(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get()) : Super(ObjectInitializer) { }; \
private: \
	/** Private move- and copy-constructors, should never be used */ \
	NO_API UJoystickFunctions(UJoystickFunctions&&); \
	NO_API UJoystickFunctions(const UJoystickFunctions&); \
public: \
	DECLARE_VTABLE_PTR_HELPER_CTOR(NO_API, UJoystickFunctions); \
DEFINE_VTABLE_PTR_HELPER_CTOR_CALLER(UJoystickFunctions); \
	DEFINE_DEFAULT_OBJECT_INITIALIZER_CONSTRUCTOR_CALL(UJoystickFunctions)


#define HostProject_Plugins_JoystickPlugin_Source_JoystickPlugin_Public_JoystickFunctions_h_20_PRIVATE_PROPERTY_OFFSET
#define HostProject_Plugins_JoystickPlugin_Source_JoystickPlugin_Public_JoystickFunctions_h_16_PROLOG
#define HostProject_Plugins_JoystickPlugin_Source_JoystickPlugin_Public_JoystickFunctions_h_20_GENERATED_BODY_LEGACY \
PRAGMA_DISABLE_DEPRECATION_WARNINGS \
public: \
	HostProject_Plugins_JoystickPlugin_Source_JoystickPlugin_Public_JoystickFunctions_h_20_PRIVATE_PROPERTY_OFFSET \
	HostProject_Plugins_JoystickPlugin_Source_JoystickPlugin_Public_JoystickFunctions_h_20_SPARSE_DATA \
	HostProject_Plugins_JoystickPlugin_Source_JoystickPlugin_Public_JoystickFunctions_h_20_RPC_WRAPPERS \
	HostProject_Plugins_JoystickPlugin_Source_JoystickPlugin_Public_JoystickFunctions_h_20_INCLASS \
	HostProject_Plugins_JoystickPlugin_Source_JoystickPlugin_Public_JoystickFunctions_h_20_STANDARD_CONSTRUCTORS \
public: \
PRAGMA_ENABLE_DEPRECATION_WARNINGS


#define HostProject_Plugins_JoystickPlugin_Source_JoystickPlugin_Public_JoystickFunctions_h_20_GENERATED_BODY \
PRAGMA_DISABLE_DEPRECATION_WARNINGS \
public: \
	HostProject_Plugins_JoystickPlugin_Source_JoystickPlugin_Public_JoystickFunctions_h_20_PRIVATE_PROPERTY_OFFSET \
	HostProject_Plugins_JoystickPlugin_Source_JoystickPlugin_Public_JoystickFunctions_h_20_SPARSE_DATA \
	HostProject_Plugins_JoystickPlugin_Source_JoystickPlugin_Public_JoystickFunctions_h_20_RPC_WRAPPERS_NO_PURE_DECLS \
	HostProject_Plugins_JoystickPlugin_Source_JoystickPlugin_Public_JoystickFunctions_h_20_INCLASS_NO_PURE_DECLS \
	HostProject_Plugins_JoystickPlugin_Source_JoystickPlugin_Public_JoystickFunctions_h_20_ENHANCED_CONSTRUCTORS \
static_assert(false, "Unknown access specifier for GENERATED_BODY() macro in class JoystickFunctions."); \
PRAGMA_ENABLE_DEPRECATION_WARNINGS


template<> JOYSTICKPLUGIN_API UClass* StaticClass<class UJoystickFunctions>();

#undef CURRENT_FILE_ID
#define CURRENT_FILE_ID HostProject_Plugins_JoystickPlugin_Source_JoystickPlugin_Public_JoystickFunctions_h


PRAGMA_ENABLE_DEPRECATION_WARNINGS
