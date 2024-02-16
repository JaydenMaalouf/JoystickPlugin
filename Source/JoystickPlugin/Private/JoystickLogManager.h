// JoystickPlugin is licensed under the MIT License.
// Copyright Jayden Maalouf. All Rights Reserved.

#pragma once

#include "JoystickInputSettings.h"

DECLARE_LOG_CATEGORY_EXTERN(LogJoystickPlugin, Log, All);

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

	template <typename FmtType, typename... Types>
	void LogWarning(const FmtType& Fmt, Types... Args)
	{
		if (!CanLog())
		{
			return;
		}

		UE_LOG(LogJoystickPlugin, Warning, TEXT("%s"), *FString::Printf(Fmt, Args...));
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

	template <typename FmtType, typename... Types>
	void LogDebug(const FmtType& Fmt, Types... Args)
	{
		if (!CanLog())
		{
			return;
		}

		UE_LOG(LogJoystickPlugin, Log, TEXT("%s"), *FString::Printf(Fmt, Args...));
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

private:
	bool CanLog() const
	{
		const UJoystickInputSettings* JoystickInputSettings = GetMutableDefault<UJoystickInputSettings>();
		if (!IsValid(JoystickInputSettings))
		{
			return false;
		}

		return JoystickInputSettings->EnableLogs;
	}
};
