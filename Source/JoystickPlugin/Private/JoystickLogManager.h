// JoystickPlugin is licensed under the MIT License.
// Copyright Jayden Maalouf. All Rights Reserved.

#pragma once

#include "Misc/Build.h"
#include "Data/ResultMessage.h"
#include "JoystickInputSettings.h"

#if ENGINE_MAJOR_VERSION == 5 && ENGINE_MINOR_VERSION >= 6
	#include "Templates/CheckedFormatString.h"
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

class JOYSTICKPLUGIN_API FJoystickLogManager
{
public:
	static FJoystickLogManager* Get();

	template<FUNC_TEMPLATE_PARAMS>
	void Log(const ELogVerbosity::Type Level, FUNC_PARAMS)
	{
		if (!CanLog())
		{
			return;
		}

		const FString Message = FString::Printf(Fmt, Forward<Types>(Args)...);
		LogInternal(Level, *Message);
	}

	void Log(const ELogVerbosity::Type Level, const FInternalResultMessage& Message)
	{
		LogInternal(Level, *Message.ErrorMessage);
	}

	template<FUNC_TEMPLATE_PARAMS>
	void LogWarning(FUNC_PARAMS)
	{
		Log(ELogVerbosity::Warning, Fmt, Forward<Types>(Args)...);
	}

	void LogWarning(const FInternalResultMessage& Message)
	{
		Log(ELogVerbosity::Warning, Message);
	}

	template<FUNC_TEMPLATE_PARAMS>
	void LogError(FUNC_PARAMS)
	{
		Log(ELogVerbosity::Error, Fmt, Forward<Types>(Args)...);
	}

	void LogError(const FInternalResultMessage& Message)
	{
		Log(ELogVerbosity::Error, Message);
	}

	template<FUNC_TEMPLATE_PARAMS>
	void LogDebug(FUNC_PARAMS)
	{
		Log(ELogVerbosity::Log, Fmt, Forward<Types>(Args)...);
	}

	void LogDebug(const FInternalResultMessage& Message)
	{
		Log(ELogVerbosity::Log, Message);
	}

	template<FUNC_TEMPLATE_PARAMS>
	void LogInformation(FUNC_PARAMS)
	{
		Log(ELogVerbosity::Display, Fmt, Forward<Types>(Args)...);
	}

	void LogInformation(const FInternalResultMessage& Message)
	{
		Log(ELogVerbosity::Display, Message);
	}

	void LogSDLError(const FString& Message);

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
		case ELogVerbosity::Error: UE_LOG(LogJoystickPlugin, Error, TEXT("%s"), Message);
			break;
		case ELogVerbosity::Warning: UE_LOG(LogJoystickPlugin, Warning, TEXT("%s"), Message);
			break;
		case ELogVerbosity::Display: UE_LOG(LogJoystickPlugin, Display, TEXT("%s"), Message);
			break;
		case ELogVerbosity::Verbose: UE_LOG(LogJoystickPlugin, Verbose, TEXT("%s"), Message);
			break;
		case ELogVerbosity::VeryVerbose: UE_LOG(LogJoystickPlugin, VeryVerbose, TEXT("%s"), Message);
			break;
		default: UE_LOG(LogJoystickPlugin, Log, TEXT("%s"), Message);
			break;
		}
	}
};
