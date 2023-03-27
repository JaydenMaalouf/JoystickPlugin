// JoystickPlugin is licensed under the MIT License.
// Copyright Jayden Maalouf. All Rights Reserved.

#include "JoystickLogManager.h"
#include "JoystickInputSettings.h"

DEFINE_LOG_CATEGORY(LogJoystickPlugin);

static TSharedPtr<FJoystickLogManager> LogManager;

FJoystickLogManager* FJoystickLogManager::Get()
{
	if (!LogManager.IsValid())
	{
		LogManager = MakeShareable(new FJoystickLogManager());
	}

	return LogManager.Get();
}

template <typename FmtType, typename... Types>
void FJoystickLogManager::Log(const ELogVerbosity::Type Level, const FmtType& Fmt, Types... Args)
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
void FJoystickLogManager::LogWarning(const FmtType& Fmt, Types... Args)
{
	if (!CanLog())
	{
		return;
	}

	UE_LOG(LogJoystickPlugin, Warning, TEXT("%s"), *FString::Printf(Fmt, Args...));
}

template <typename FmtType, typename... Types>
void FJoystickLogManager::LogError(const FmtType& Fmt, Types... Args)
{
	if (!CanLog())
	{
		return;
	}

	UE_LOG(LogJoystickPlugin, Error, TEXT("%s"), *FString::Printf(Fmt, Args...));
}

template <typename FmtType, typename... Types>
void FJoystickLogManager::LogDebug(const FmtType& Fmt, Types... Args)
{
	if (!CanLog())
	{
		return;
	}

	UE_LOG(LogJoystickPlugin, Log, TEXT("%s"), *FString::Printf(Fmt, Args...));
}

template <typename FmtType, typename... Types>
void FJoystickLogManager::LogInformation(const FmtType& Fmt, Types... Args)
{
	if (!CanLog())
	{
		return;
	}

	UE_LOG(LogJoystickPlugin, Display, TEXT("%s"), *FString::Printf(Fmt, Args...));
}

bool FJoystickLogManager::CanLog() const
{
	const UJoystickInputSettings* JoystickInputSettings = GetMutableDefault<UJoystickInputSettings>();
	if (!IsValid(JoystickInputSettings))
	{
		return false;
	}

	return JoystickInputSettings->EnableLogs;
}
