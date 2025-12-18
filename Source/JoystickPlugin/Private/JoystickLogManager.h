// JoystickPlugin is licensed under the MIT License.
// Copyright Jayden Maalouf. All Rights Reserved.

#pragma once

#include "Data/ResultMessage.h"
#include "JoystickInputSettings.h"
#include "Misc/Build.h"
#include "Runtime/Launch/Resources/Version.h"

#if ENGINE_MAJOR_VERSION == 5 && ENGINE_MINOR_VERSION >= 6
#define FUNC_TEMPLATE_PARAMS typename... Types
#define FUNC_PARAMS UE::Core::TCheckedFormatString<FString::FmtCharType, Types...> Fmt, Types... Args
#else
#define FUNC_TEMPLATE_PARAMS typename FmtType, typename... Types
#define FUNC_PARAMS const FmtType& Fmt, Types... Args
#endif

#if UE_BUILD_SHIPPING
DECLARE_LOG_CATEGORY_EXTERN(LogJoystickPlugin, Display, All);
#else
DECLARE_LOG_CATEGORY_EXTERN(LogJoystickPlugin, Log, All);
#endif

#define JOYSTICK_LOG(Verbosity, Message) UE_LOG(LogJoystickPlugin, Verbosity, TEXT("%s"), Message)

class JOYSTICKPLUGIN_API FJoystickLogManager
{
public:
	static FJoystickLogManager* Get();

	template <FUNC_TEMPLATE_PARAMS>
	FORCEINLINE void Log(const ELogVerbosity::Type Level, FUNC_PARAMS)
	{
		if (!CanLog())
		{
			return;
		}

		const FString Message = FString::Printf(Fmt, Forward<Types>(Args)...);
		LogInternal(Level, *Message);
	}

	FORCEINLINE void Log(const ELogVerbosity::Type Level, const FInternalResultMessage& Message)
	{
		LogInternal(Level, *Message.ErrorMessage);
	}

	template <FUNC_TEMPLATE_PARAMS>
	FORCEINLINE void LogWarning(FUNC_PARAMS)
	{
		Log(ELogVerbosity::Type::Warning, Fmt, Forward<Types>(Args)...);
	}

	FORCEINLINE void LogWarning(const FInternalResultMessage& Message)
	{
		Log(ELogVerbosity::Type::Warning, Message);
	}

	template <FUNC_TEMPLATE_PARAMS>
	FORCEINLINE void LogError(FUNC_PARAMS)
	{
		Log(ELogVerbosity::Type::Error, Fmt, Forward<Types>(Args)...);
	}

	FORCEINLINE void LogError(const FInternalResultMessage& Message)
	{
		Log(ELogVerbosity::Type::Error, Message);
	}

	template <FUNC_TEMPLATE_PARAMS>
	FORCEINLINE void LogDebug(FUNC_PARAMS)
	{
		Log(ELogVerbosity::Type::Log, Fmt, Forward<Types>(Args)...);
	}

	FORCEINLINE void LogDebug(const FInternalResultMessage& Message)
	{
		Log(ELogVerbosity::Type::Log, Message);
	}

	template <FUNC_TEMPLATE_PARAMS>
	FORCEINLINE void LogInformation(FUNC_PARAMS)
	{
		Log(ELogVerbosity::Type::Display, Fmt, Forward<Types>(Args)...);
	}

	FORCEINLINE void LogInformation(const FInternalResultMessage& Message)
	{
		Log(ELogVerbosity::Type::Display, Message);
	}

	void LogSDLError(FStringView Message);

private:
	static bool CanLog()
	{
		const UJoystickInputSettings* JoystickInputSettings = GetDefault<UJoystickInputSettings>();
		if (!IsValid(JoystickInputSettings))
		{
			return false;
		}

		return JoystickInputSettings->EnableLogs;
	}

	static void LogInternal(const ELogVerbosity::Type Level, const TCHAR* Message)
	{
		switch (Level)
		{
		case ELogVerbosity::Error: JOYSTICK_LOG(Error, Message);
			break;
		case ELogVerbosity::Warning: JOYSTICK_LOG(Warning, Message);
			break;
		case ELogVerbosity::Display: JOYSTICK_LOG(Display, Message);
			break;
		case ELogVerbosity::Verbose: JOYSTICK_LOG(Verbose, Message);
			break;
		case ELogVerbosity::VeryVerbose: JOYSTICK_LOG(VeryVerbose, Message);
			break;
		case ELogVerbosity::Fatal: JOYSTICK_LOG(Fatal, Message);
			break;
		default: JOYSTICK_LOG(Log, Message);
			break;
		}
	}
};
