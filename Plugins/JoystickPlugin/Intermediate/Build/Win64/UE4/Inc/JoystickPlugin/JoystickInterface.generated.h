// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/ObjectMacros.h"
#include "UObject/ScriptMacros.h"

PRAGMA_DISABLE_DEPRECATION_WARNINGS
struct FJoystickState;
struct FVector2D;
enum class EJoystickPOVDirection : uint8;
#ifdef JOYSTICKPLUGIN_JoystickInterface_generated_h
#error "JoystickInterface.generated.h already included, missing '#pragma once' in JoystickInterface.h"
#endif
#define JOYSTICKPLUGIN_JoystickInterface_generated_h

#define HostProject_Plugins_JoystickPlugin_Source_JoystickPlugin_Public_JoystickInterface_h_98_GENERATED_BODY \
	friend struct Z_Construct_UScriptStruct_FJoystickInfo_Statics; \
	JOYSTICKPLUGIN_API static class UScriptStruct* StaticStruct();


template<> JOYSTICKPLUGIN_API UScriptStruct* StaticStruct<struct FJoystickInfo>();

#define HostProject_Plugins_JoystickPlugin_Source_JoystickPlugin_Public_JoystickInterface_h_72_GENERATED_BODY \
	friend struct Z_Construct_UScriptStruct_FJoystickState_Statics; \
	JOYSTICKPLUGIN_API static class UScriptStruct* StaticStruct();


template<> JOYSTICKPLUGIN_API UScriptStruct* StaticStruct<struct FJoystickState>();

#define HostProject_Plugins_JoystickPlugin_Source_JoystickPlugin_Public_JoystickInterface_h_122_SPARSE_DATA
#define HostProject_Plugins_JoystickPlugin_Source_JoystickPlugin_Public_JoystickInterface_h_122_RPC_WRAPPERS
#define HostProject_Plugins_JoystickPlugin_Source_JoystickPlugin_Public_JoystickInterface_h_122_RPC_WRAPPERS_NO_PURE_DECLS
#define HostProject_Plugins_JoystickPlugin_Source_JoystickPlugin_Public_JoystickInterface_h_122_EVENT_PARMS \
	struct JoystickInterface_eventJoystickAxisChanged_Parms \
	{ \
		int32 Axis; \
		float value; \
		float valuePrev; \
		FJoystickState state; \
		FJoystickState prev; \
	}; \
	struct JoystickInterface_eventJoystickBallMoved_Parms \
	{ \
		int32 Ball; \
		FVector2D Delta; \
		FJoystickState State; \
	}; \
	struct JoystickInterface_eventJoystickButtonPressed_Parms \
	{ \
		int32 Button; \
		FJoystickState state; \
	}; \
	struct JoystickInterface_eventJoystickButtonReleased_Parms \
	{ \
		int32 Button; \
		FJoystickState state; \
	}; \
	struct JoystickInterface_eventJoystickHatChanged_Parms \
	{ \
		int32 Hat; \
		EJoystickPOVDirection Value; \
		FJoystickState state; \
	}; \
	struct JoystickInterface_eventJoystickPluggedIn_Parms \
	{ \
		int32 DeviceId; \
	}; \
	struct JoystickInterface_eventJoystickUnplugged_Parms \
	{ \
		int32 DeviceId; \
	};


#define HostProject_Plugins_JoystickPlugin_Source_JoystickPlugin_Public_JoystickInterface_h_122_CALLBACK_WRAPPERS
#define HostProject_Plugins_JoystickPlugin_Source_JoystickPlugin_Public_JoystickInterface_h_122_STANDARD_CONSTRUCTORS \
	/** Standard constructor, called after all reflected properties have been initialized */ \
	JOYSTICKPLUGIN_API UJoystickInterface(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get()); \
	DEFINE_DEFAULT_OBJECT_INITIALIZER_CONSTRUCTOR_CALL(UJoystickInterface) \
	DECLARE_VTABLE_PTR_HELPER_CTOR(JOYSTICKPLUGIN_API, UJoystickInterface); \
DEFINE_VTABLE_PTR_HELPER_CTOR_CALLER(UJoystickInterface); \
private: \
	/** Private move- and copy-constructors, should never be used */ \
	JOYSTICKPLUGIN_API UJoystickInterface(UJoystickInterface&&); \
	JOYSTICKPLUGIN_API UJoystickInterface(const UJoystickInterface&); \
public:


#define HostProject_Plugins_JoystickPlugin_Source_JoystickPlugin_Public_JoystickInterface_h_122_ENHANCED_CONSTRUCTORS \
	/** Standard constructor, called after all reflected properties have been initialized */ \
	JOYSTICKPLUGIN_API UJoystickInterface(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get()) : Super(ObjectInitializer) { }; \
private: \
	/** Private move- and copy-constructors, should never be used */ \
	JOYSTICKPLUGIN_API UJoystickInterface(UJoystickInterface&&); \
	JOYSTICKPLUGIN_API UJoystickInterface(const UJoystickInterface&); \
public: \
	DECLARE_VTABLE_PTR_HELPER_CTOR(JOYSTICKPLUGIN_API, UJoystickInterface); \
DEFINE_VTABLE_PTR_HELPER_CTOR_CALLER(UJoystickInterface); \
	DEFINE_DEFAULT_OBJECT_INITIALIZER_CONSTRUCTOR_CALL(UJoystickInterface)


#define HostProject_Plugins_JoystickPlugin_Source_JoystickPlugin_Public_JoystickInterface_h_122_GENERATED_UINTERFACE_BODY() \
private: \
	static void StaticRegisterNativesUJoystickInterface(); \
	friend struct Z_Construct_UClass_UJoystickInterface_Statics; \
public: \
	DECLARE_CLASS(UJoystickInterface, UInterface, COMPILED_IN_FLAGS(CLASS_Abstract | CLASS_Interface), CASTCLASS_None, TEXT("/Script/JoystickPlugin"), JOYSTICKPLUGIN_API) \
	DECLARE_SERIALIZER(UJoystickInterface)


#define HostProject_Plugins_JoystickPlugin_Source_JoystickPlugin_Public_JoystickInterface_h_122_GENERATED_BODY_LEGACY \
		PRAGMA_DISABLE_DEPRECATION_WARNINGS \
	HostProject_Plugins_JoystickPlugin_Source_JoystickPlugin_Public_JoystickInterface_h_122_GENERATED_UINTERFACE_BODY() \
	HostProject_Plugins_JoystickPlugin_Source_JoystickPlugin_Public_JoystickInterface_h_122_STANDARD_CONSTRUCTORS \
	PRAGMA_ENABLE_DEPRECATION_WARNINGS


#define HostProject_Plugins_JoystickPlugin_Source_JoystickPlugin_Public_JoystickInterface_h_122_GENERATED_BODY \
	PRAGMA_DISABLE_DEPRECATION_WARNINGS \
	HostProject_Plugins_JoystickPlugin_Source_JoystickPlugin_Public_JoystickInterface_h_122_GENERATED_UINTERFACE_BODY() \
	HostProject_Plugins_JoystickPlugin_Source_JoystickPlugin_Public_JoystickInterface_h_122_ENHANCED_CONSTRUCTORS \
private: \
	PRAGMA_ENABLE_DEPRECATION_WARNINGS


#define HostProject_Plugins_JoystickPlugin_Source_JoystickPlugin_Public_JoystickInterface_h_122_INCLASS_IINTERFACE_NO_PURE_DECLS \
protected: \
	virtual ~IJoystickInterface() {} \
public: \
	typedef UJoystickInterface UClassType; \
	typedef IJoystickInterface ThisClass; \
	static void Execute_JoystickAxisChanged(UObject* O, int32 Axis, float value, float valuePrev, FJoystickState state, FJoystickState prev); \
	static void Execute_JoystickBallMoved(UObject* O, int32 Ball, FVector2D Delta, FJoystickState State); \
	static void Execute_JoystickButtonPressed(UObject* O, int32 Button, FJoystickState state); \
	static void Execute_JoystickButtonReleased(UObject* O, int32 Button, FJoystickState state); \
	static void Execute_JoystickHatChanged(UObject* O, int32 Hat, EJoystickPOVDirection Value, FJoystickState state); \
	static void Execute_JoystickPluggedIn(UObject* O, int32 DeviceId); \
	static void Execute_JoystickUnplugged(UObject* O, int32 DeviceId); \
	virtual UObject* _getUObject() const { check(0 && "Missing required implementation."); return nullptr; }


#define HostProject_Plugins_JoystickPlugin_Source_JoystickPlugin_Public_JoystickInterface_h_122_INCLASS_IINTERFACE \
protected: \
	virtual ~IJoystickInterface() {} \
public: \
	typedef UJoystickInterface UClassType; \
	typedef IJoystickInterface ThisClass; \
	static void Execute_JoystickAxisChanged(UObject* O, int32 Axis, float value, float valuePrev, FJoystickState state, FJoystickState prev); \
	static void Execute_JoystickBallMoved(UObject* O, int32 Ball, FVector2D Delta, FJoystickState State); \
	static void Execute_JoystickButtonPressed(UObject* O, int32 Button, FJoystickState state); \
	static void Execute_JoystickButtonReleased(UObject* O, int32 Button, FJoystickState state); \
	static void Execute_JoystickHatChanged(UObject* O, int32 Hat, EJoystickPOVDirection Value, FJoystickState state); \
	static void Execute_JoystickPluggedIn(UObject* O, int32 DeviceId); \
	static void Execute_JoystickUnplugged(UObject* O, int32 DeviceId); \
	virtual UObject* _getUObject() const { check(0 && "Missing required implementation."); return nullptr; }


#define HostProject_Plugins_JoystickPlugin_Source_JoystickPlugin_Public_JoystickInterface_h_119_PROLOG \
	HostProject_Plugins_JoystickPlugin_Source_JoystickPlugin_Public_JoystickInterface_h_122_EVENT_PARMS


#define HostProject_Plugins_JoystickPlugin_Source_JoystickPlugin_Public_JoystickInterface_h_127_GENERATED_BODY_LEGACY \
PRAGMA_DISABLE_DEPRECATION_WARNINGS \
public: \
	HostProject_Plugins_JoystickPlugin_Source_JoystickPlugin_Public_JoystickInterface_h_122_SPARSE_DATA \
	HostProject_Plugins_JoystickPlugin_Source_JoystickPlugin_Public_JoystickInterface_h_122_RPC_WRAPPERS \
	HostProject_Plugins_JoystickPlugin_Source_JoystickPlugin_Public_JoystickInterface_h_122_CALLBACK_WRAPPERS \
	HostProject_Plugins_JoystickPlugin_Source_JoystickPlugin_Public_JoystickInterface_h_122_INCLASS_IINTERFACE \
public: \
PRAGMA_ENABLE_DEPRECATION_WARNINGS


#define HostProject_Plugins_JoystickPlugin_Source_JoystickPlugin_Public_JoystickInterface_h_127_GENERATED_BODY \
PRAGMA_DISABLE_DEPRECATION_WARNINGS \
public: \
	HostProject_Plugins_JoystickPlugin_Source_JoystickPlugin_Public_JoystickInterface_h_122_SPARSE_DATA \
	HostProject_Plugins_JoystickPlugin_Source_JoystickPlugin_Public_JoystickInterface_h_122_RPC_WRAPPERS_NO_PURE_DECLS \
	HostProject_Plugins_JoystickPlugin_Source_JoystickPlugin_Public_JoystickInterface_h_122_CALLBACK_WRAPPERS \
	HostProject_Plugins_JoystickPlugin_Source_JoystickPlugin_Public_JoystickInterface_h_122_INCLASS_IINTERFACE_NO_PURE_DECLS \
private: \
PRAGMA_ENABLE_DEPRECATION_WARNINGS


template<> JOYSTICKPLUGIN_API UClass* StaticClass<class UJoystickInterface>();

#undef CURRENT_FILE_ID
#define CURRENT_FILE_ID HostProject_Plugins_JoystickPlugin_Source_JoystickPlugin_Public_JoystickInterface_h


#define FOREACH_ENUM_EJOYSTICKPOVDIRECTION(op) \
	op(EJoystickPOVDirection::DIRECTION_NONE) \
	op(EJoystickPOVDirection::DIRECTION_UP) \
	op(EJoystickPOVDirection::DIRECTION_UP_RIGHT) \
	op(EJoystickPOVDirection::DIRECTION_RIGHT) \
	op(EJoystickPOVDirection::DIRECTION_DOWN_RIGHT) \
	op(EJoystickPOVDirection::DIRECTION_DOWN) \
	op(EJoystickPOVDirection::DIRECTION_DOWN_LEFT) \
	op(EJoystickPOVDirection::DIRECTION_LEFT) \
	op(EJoystickPOVDirection::DIRECTION_UP_LEFT) 

enum class EJoystickPOVDirection : uint8;
template<> JOYSTICKPLUGIN_API UEnum* StaticEnum<EJoystickPOVDirection>();

#define FOREACH_ENUM_EINPUTTYPE(op) \
	op(EInputType::INPUTTYPE_UNKNOWN) \
	op(EInputType::INPUTTYPE_JOYSTICK) \
	op(EInputType::INPUTTYPE_GAMECONTROLLER) 

enum class EInputType : uint8;
template<> JOYSTICKPLUGIN_API UEnum* StaticEnum<EInputType>();

PRAGMA_ENABLE_DEPRECATION_WARNINGS
