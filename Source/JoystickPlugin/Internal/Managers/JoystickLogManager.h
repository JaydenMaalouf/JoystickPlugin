// JoystickPlugin is licensed under the MIT License.
// Copyright Jayden Maalouf 2026. All Rights Reserved.

#pragma once

#include "Data/ResultMessage.h"
#include "JoystickInputSettings.h"
#include "Misc/Build.h"
#include "Runtime/Launch/Resources/Version.h"

#if ENGINE_MAJOR_VERSION == 5 && ENGINE_MINOR_VERSION >= 6
#define JOYSTICK_LOG_TEMPLATE_PARAMS typename... Types
#define JOYSTICK_LOG_PARAMS UE::Core::TCheckedFormatString<FString::FmtCharType, Types...> Fmt, Types... Args
#else
#define JOYSTICK_LOG_TEMPLATE_PARAMS typename FmtType, typename... Types
#define JOYSTICK_LOG_PARAMS const FmtType& Fmt, Types... Args
#endif

#if UE_BUILD_SHIPPING
JOYSTICKPLUGIN_API DECLARE_LOG_CATEGORY_EXTERN(LogJoystickPlugin, Display, All);
#else
JOYSTICKPLUGIN_API DECLARE_LOG_CATEGORY_EXTERN(LogJoystickPlugin, Log, All);
#endif

#define JOYSTICK_LOG(Verbosity, Message) UE_LOG(LogJoystickPlugin, Verbosity, TEXT("%s"), Message)

class JOYSTICKPLUGIN_API FJoystickLogManager
{
public:
	static FJoystickLogManager* Get();

	template <JOYSTICK_LOG_TEMPLATE_PARAMS>
	FORCEINLINE void Log(const ELogVerbosity::Type Level, JOYSTICK_LOG_PARAMS)
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

	template <JOYSTICK_LOG_TEMPLATE_PARAMS>
	FORCEINLINE void LogWarning(JOYSTICK_LOG_PARAMS)
	{
		Log(ELogVerbosity::Type::Warning, Fmt, Forward<Types>(Args)...);
	}

	FORCEINLINE void LogWarning(const FInternalResultMessage& Message)
	{
		Log(ELogVerbosity::Type::Warning, Message);
	}

	template <JOYSTICK_LOG_TEMPLATE_PARAMS>
	FORCEINLINE void LogError(JOYSTICK_LOG_PARAMS)
	{
		Log(ELogVerbosity::Type::Error, Fmt, Forward<Types>(Args)...);
	}

	FORCEINLINE void LogError(const FInternalResultMessage& Message)
	{
		Log(ELogVerbosity::Type::Error, Message);
	}

	template <JOYSTICK_LOG_TEMPLATE_PARAMS>
	FORCEINLINE void LogDebug(JOYSTICK_LOG_PARAMS)
	{
		Log(ELogVerbosity::Type::Log, Fmt, Forward<Types>(Args)...);
	}

	FORCEINLINE void LogDebug(const FInternalResultMessage& Message)
	{
		Log(ELogVerbosity::Type::Log, Message);
	}

	template <JOYSTICK_LOG_TEMPLATE_PARAMS>
	FORCEINLINE void LogInformation(JOYSTICK_LOG_PARAMS)
	{
		Log(ELogVerbosity::Type::Display, Fmt, Forward<Types>(Args)...);
	}

	FORCEINLINE void LogInformation(const FInternalResultMessage& Message)
	{
		Log(ELogVerbosity::Type::Display, Message);
	}

	void LogSDLError(FStringView Message);
	void LogErrorWithDialog(const FText& Message, const FText& Title = FText::GetEmpty());

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
