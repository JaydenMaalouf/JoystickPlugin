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
		switch (Level)
		{
		case ELogVerbosity::Error:
			LogError(Fmt, Args...);
			break;
		case ELogVerbosity::Log:
			LogDebug(Fmt, Args...);
			break;
		default:
			LogInformation(Fmt, Args...);
			break;
		}
	}

	void Log(const ELogVerbosity::Type Level, const FInternalResultMessage& Message)
	{
		Log(Level, TEXT("%s"), *Message.ErrorMessage);
	}

	template <typename FmtType, typename... Types>
	void LogWarning(const FmtType& Fmt, Types... Args)
	{
		if (!CanLog())
		{
			return;
		}

		UE_LOG(LogJoystickPlugin, Warning, TEXT("%s"), *FString::Printf(Fmt, Args...));
	}

	void LogWarning(const FInternalResultMessage& Message)
	{
		LogWarning(TEXT("%s"), *Message.ErrorMessage);
	}

	template <typename FmtType, typename... Types>
	void LogError(const FmtType& Fmt, Types... Args)
	{
		if (!CanLog())
		{
			return;
		}

		UE_LOG(LogJoystickPlugin, Error, TEXT("%s"), *FString::Printf(Fmt, Args...));
	}

	void LogError(const FInternalResultMessage& Message)
	{
		LogError(TEXT("%s"), *Message.ErrorMessage);
	}

	template <typename FmtType, typename... Types>
	void LogDebug(const FmtType& Fmt, Types... Args)
	{
		if (!CanLog())
		{
			return;
		}

		UE_LOG(LogJoystickPlugin, Log, TEXT("%s"), *FString::Printf(Fmt, Args...));
	}

	void LogDebug(const FInternalResultMessage& Message)
	{
		LogDebug(TEXT("%s"), *Message.ErrorMessage);
	}

	template <typename FmtType, typename... Types>
	void LogInformation(const FmtType& Fmt, Types... Args)
	{
		if (!CanLog())
		{
			return;
		}

		UE_LOG(LogJoystickPlugin, Display, TEXT("%s"), *FString::Printf(Fmt, Args...));
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
};
