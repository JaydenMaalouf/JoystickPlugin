// JoystickPlugin is licensed under the MIT License.
// Copyright Jayden Maalouf. All Rights Reserved.

#pragma once

#include "Misc/Build.h"
#include "Data/ResultMessage.h"
#include "JoystickInputSettings.h"

#if UE_BUILD_SHIPPING
	DECLARE_LOG_CATEGORY_EXTERN(LogJoystickPlugin, Display, All);
#else
	DECLARE_LOG_CATEGORY_EXTERN(LogJoystickPlugin, Log, All);
#endif

class JOYSTICKPLUGIN_API FJoystickLogManager
{
public:
	static FJoystickLogManager* Get();

	template <typename FmtType, typename... Types>
	void Log(const ELogVerbosity::Type Level, const FmtType& Fmt, Types... Args)
	{
		InternalLog(Level, Fmt, Args...);
	}

	void Log(const ELogVerbosity::Type Level, const FInternalResultMessage& Message)
	{
		InternalLog(Level, TEXT("%s"), *Message.ErrorMessage);
	}

	template <typename FmtType, typename... Types>
	void LogWarning(const FmtType& Fmt, Types... Args)
	{
		InternalLog(ELogVerbosity::Warning, Fmt, Args...);
	}

	void LogWarning(const FInternalResultMessage& Message)
	{
		LogWarning(TEXT("%s"), *Message.ErrorMessage);
	}

	template <typename FmtType, typename... Types>
	void LogError(const FmtType& Fmt, Types... Args)
	{
		InternalLog(ELogVerbosity::Error, Fmt, Args...);
	}

	void LogError(const FInternalResultMessage& Message)
	{
		LogError(TEXT("%s"), *Message.ErrorMessage);
	}

	template <typename FmtType, typename... Types>
	void LogDebug(const FmtType& Fmt, Types... Args)
	{
		InternalLog(ELogVerbosity::Log, Fmt, Args...);
	}

	void LogDebug(const FInternalResultMessage& Message)
	{
		LogDebug(TEXT("%s"), *Message.ErrorMessage);
	}

	template <typename FmtType, typename... Types>
	void LogInformation(const FmtType& Fmt, Types... Args)
	{
		InternalLog(ELogVerbosity::Display, Fmt, Args...);
	}

	void LogInformation(const FInternalResultMessage& Message)
	{
		LogInformation(TEXT("%s"), *Message.ErrorMessage);
	}

	void LogSDLError(const FString& Message);

private:
	bool CanLog() const
	{
		const UJoystickInputSettings* JoystickInputSettings = GetDefault<UJoystickInputSettings>();
		if (!IsValid(JoystickInputSettings))
		{
			return false;
		}

		return JoystickInputSettings->EnableLogs;
	}

	template <typename FmtType, typename... Types>
	void InternalLog(const ELogVerbosity::Type Level, const FmtType& Fmt, Types... Args)
	{
		if (!CanLog())
		{
			return;
		}

		FMsg::Logf(__FILE__, __LINE__, LogJoystickPlugin.GetCategoryName(), Level, Fmt, Args...);
	}
};
